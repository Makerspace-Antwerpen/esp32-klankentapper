// save some bytes
const gel = (e) => document.getElementById(e);

const wifi_div = gel("wifi");
const connect_div = gel("connect");
const connect_manual_div = gel("connect_manual");
const connect_wait_div = gel("connect-wait");
const connect_details_div = gel("connect-details");

function docReady(fn) {
  // see if DOM is already available
  if (
    document.readyState === "complete" ||
    document.readyState === "interactive"
  ) {
    // call on next available tick
    setTimeout(fn, 1);
  } else {
    document.addEventListener("DOMContentLoaded", fn);
  }
}

var selectedSSID = "";
var refreshAPInterval = null;
var refreshMQTTInterval = null;
var checkStatusInterval = null;

function stopCheckStatusInterval() {
  if (checkStatusInterval != null) {
    clearInterval(checkStatusInterval);
    checkStatusInterval = null;
  }
}

function stopRefreshAPInterval() {
  if (refreshAPInterval != null) {
    clearInterval(refreshAPInterval);
    refreshAPInterval = null;
  }
}

function stopRefreshMQTTInterval() {
  if (refreshMQTTInterval != null) {
    clearInterval(refreshMQTTInterval);
    refreshMQTTInterval = null;
  }
}

function startCheckStatusInterval() {
  checkStatusInterval = setInterval(checkStatus, 950);
}

function startRefreshAPInterval() {
  refreshAPInterval = setInterval(refreshAP, 3800);
}

function startRefreshMQTTInterval() {
  refreshMQTTInterval = setInterval(refreshMQTT, 1000);
}

docReady(async function () {
  gel("wifi-status").addEventListener(
    "click",
    () => {
      wifi_div.style.display = "none";
      document.getElementById("connect-details").style.display = "block";
    },
    false
  );

  gel("manual_add").addEventListener(
    "click",
    (e) => {
      selectedSSID = e.target.innerText;

      gel("ssid-pwd").textContent = selectedSSID;
      wifi_div.style.display = "none";
      connect_manual_div.style.display = "block";
      connect_div.style.display = "none";

      gel("connect-success").display = "none";
      gel("connect-fail").display = "none";
    },
    false
  );

  gel("wifi-list").addEventListener(
    "click",
    (e) => {
      selectedSSID = e.target.innerText;
      gel("ssid-pwd").textContent = selectedSSID;
      connect_div.style.display = "block";
      wifi_div.style.display = "none";
      // init_cancel();
    },
    false
  );

  function cancel() {
    selectedSSID = "";
    connect_div.style.display = "none";
    connect_manual_div.style.display = "none";
    wifi_div.style.display = "block";
  }

  gel("cancel").addEventListener("click", cancel, false);

  gel("manual_cancel").addEventListener("click", cancel, false);

  gel("join").addEventListener("click", performConnect, false);

  gel("mqtt-save").addEventListener("click", performMqtt, false);

  gel("manual_join").addEventListener(
    "click",
    (e) => {
      performConnect("manual");
    },
    false
  );

  gel("ok-details").addEventListener(
    "click",
    () => {
      connect_details_div.style.display = "none";
      wifi_div.style.display = "block";
    },
    false
  );

  gel("ok-credits").addEventListener(
    "click",
    () => {
      gel("credits").style.display = "none";
      gel("app").style.display = "block";
    },
    false
  );

  gel("acredits").addEventListener(
    "click",
    () => {
      event.preventDefault();
      gel("app").style.display = "none";
      gel("credits").style.display = "block";
    },
    false
  );

  gel("ok-connect").addEventListener(
    "click",
    () => {
      connect_wait_div.style.display = "none";
      wifi_div.style.display = "block";
    },
    false
  );

  gel("disconnect").addEventListener(
    "click",
    () => {
      gel("diag-disconnect").style.display = "block";
      gel("connect-details-wrap").classList.add("blur");
    },
    false
  );

  gel("no-disconnect").addEventListener(
    "click",
    () => {
      gel("diag-disconnect").style.display = "none";
      gel("connect-details-wrap").classList.remove("blur");
    },
    false
  );

  gel("yes-disconnect").addEventListener("click", async () => {
    stopCheckStatusInterval();
    selectedSSID = "";

    document.getElementById("diag-disconnect").style.display = "none";
    gel("connect-details-wrap").classList.remove("blur");

    await fetch("connect.json", {
      method: "DELETE",
      headers: {
        "Content-Type": "application/json",
      },
      body: { timestamp: Date.now() },
    });

    startCheckStatusInterval();

    connect_details_div.style.display = "none";
    wifi_div.style.display = "block";
  });

  //first time the page loads: attempt get the connection status and start the wifi scan
  await loadMQTT();
  await refreshAP();
  await refreshMQTT();
  startCheckStatusInterval();
  startRefreshAPInterval();
  startRefreshMQTTInterval();
});

async function performMqtt(conntype) {
  stopRefreshMQTTInterval();
  gel("mqtt-status").innerHTML = `<span>Waiting</span>`;
  gel("mqtt-status").style.backgroundColor = "red";
  let selected_server = gel("mqtt-server").value;
  let selected_topic = gel("mqtt-topic").value;
  let selected_user = gel("mqtt-user").value;
  let selected_pass = gel("mqtt-pass").value;
  await fetch("mqtt.json", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
      "X-Custom-server": selected_server,
      "X-Custom-topic": selected_topic,
      "X-Custom-user": selected_user,
      "X-Custom-pass": selected_pass,
    },
    body: { timestamp: Date.now() },
  });
  startRefreshMQTTInterval();
}

async function performConnect(conntype) {
  //stop the status refresh. This prevents a race condition where a status
  //request would be refreshed with wrong ip info from a previous connection
  //and the request would automatically shows as succesful.
  stopCheckStatusInterval();

  //stop refreshing wifi list
  stopRefreshAPInterval();

  var pwd;
  if (conntype == "manual") {
    //Grab the manual SSID and PWD
    selectedSSID = gel("manual_ssid").value;
    pwd = gel("manual_pwd").value;
  } else {
    pwd = gel("pwd").value;
  }
  //reset connection
  gel("loading").style.display = "block";
  gel("connect-success").style.display = "none";
  gel("connect-fail").style.display = "none";

  gel("ok-connect").disabled = true;
  gel("ssid-wait").textContent = selectedSSID;
  connect_div.style.display = "none";
  connect_manual_div.style.display = "none";
  connect_wait_div.style.display = "block";

  await fetch("connect.json", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
      "X-Custom-ssid": selectedSSID,
      "X-Custom-pwd": pwd,
    },
    body: { timestamp: Date.now() },
  });

  //now we can re-set the intervals regardless of result
  startCheckStatusInterval();
  startRefreshAPInterval();
}

function rssiToIcon(rssi) {
  if (rssi >= -60) {
    return "w0";
  } else if (rssi >= -67) {
    return "w1";
  } else if (rssi >= -75) {
    return "w2";
  } else {
    return "w3";
  }
}

async function refreshAP(url = "ap.json") {
  try {
    var res = await fetch(url);
    var access_points = await res.json();
    if (access_points.length > 0) {
      //sort by signal strength
      access_points.sort((a, b) => {
        var x = a["rssi"];
        var y = b["rssi"];
        return x < y ? 1 : x > y ? -1 : 0;
      });
      refreshAPHTML(access_points);
    }
  } catch (e) {
    console.info("Access points returned empty from /ap.json!");
  }
}

function refreshAPHTML(data) {
  var h = "";
  data.forEach(function (e, idx, array) {
    let ap_class = idx === array.length - 1 ? "" : " brdb";
    let rssicon = rssiToIcon(e.rssi);
    let auth = e.auth == 0 ? "" : "pw";
    h += `<div class="ape${ap_class}"><div class="${rssicon}"><div class="${auth}">${e.ssid}</div></div></div>\n`;
  });

  gel("wifi-list").innerHTML = h;
}

async function refreshMQTT(url = "mqtt_status.json") {
  try {
    var res = await fetch(url);
    var mqtt_status = await res.json();
    refreshMQTTHTML(mqtt_status);
  } catch (e) {
    console.info("Access points returned empty from /ap.json!");
  }
}


function refreshMQTTHTML(data) {
  let content = `
  <ul>
    <li>Server: ${data.server}</li>
    <li>Topic: ${data.topic}</li>
    <li>User: ${data.user}</li>
    <li>Pass: ${data.pass}</li>
  </ul>\n`;
  gel("mqtt-status").innerHTML = content;
  let color = "red";
  if (data.status == "connected"){
    color = "green";
  }
  gel("mqtt-status").style.backgroundColor = color;
}

async function loadMQTT(url = "mqtt_status.json") {
  try {
    var res = await fetch(url);
    var mqtt_status = await res.json();
    loadMQTTHTML(mqtt_status);
  } catch (e) {
    console.info("Access points returned empty from /ap.json!");
  }
}

function loadMQTTHTML(data){
  gel("mqtt-server").value = data.server;
  gel("mqtt-topic").value = data.topic;
  gel("mqt-user").value = data.user;
  gel("mqtt-pass").value = data.pass;
}

async function checkStatus(url = "status.json") {
  try {
    var response = await fetch(url);
    var data = await response.json();
    if (data && data.hasOwnProperty("ssid") && data["ssid"] != "") {
      if (data["ssid"] === selectedSSID) {
        // Attempting connection
        switch (data["urc"]) {
          case 0:
            console.info("Got connection!");
            document.querySelector(
              "#connected-to div div div span"
            ).textContent = data["ssid"];
            document.querySelector("#connect-details h1").textContent =
              data["ssid"];
            gel("ip").textContent = data["ip"];
            gel("netmask").textContent = data["netmask"];
            gel("gw").textContent = data["gw"];
            gel("wifi-status").style.display = "block";

            //unlock the wait screen if needed
            gel("ok-connect").disabled = false;

            //update wait screen
            gel("loading").style.display = "none";
            gel("connect-success").style.display = "block";
            gel("connect-fail").style.display = "none";
            break;
          case 1:
            console.info("Connection attempt failed!");
            document.querySelector(
              "#connected-to div div div span"
            ).textContent = data["ssid"];
            document.querySelector("#connect-details h1").textContent =
              data["ssid"];
            gel("ip").textContent = "0.0.0.0";
            gel("netmask").textContent = "0.0.0.0";
            gel("gw").textContent = "0.0.0.0";

            //don't show any connection
            gel("wifi-status").display = "none";

            //unlock the wait screen
            gel("ok-connect").disabled = false;

            //update wait screen
            gel("loading").display = "none";
            gel("connect-fail").style.display = "block";
            gel("connect-success").style.display = "none";
            break;
        }
      } else if (data.hasOwnProperty("urc") && data["urc"] === 0) {
        console.info("Connection established");
        //ESP32 is already connected to a wifi without having the user do anything
        if (
          gel("wifi-status").style.display == "" ||
          gel("wifi-status").style.display == "none"
        ) {
          document.querySelector("#connected-to div div div span").textContent =
            data["ssid"];
          document.querySelector("#connect-details h1").textContent =
            data["ssid"];
          gel("ip").textContent = data["ip"];
          gel("netmask").textContent = data["netmask"];
          gel("gw").textContent = data["gw"];
          gel("wifi-status").style.display = "block";
        }
      }
    } else if (data.hasOwnProperty("urc") && data["urc"] === 2) {
      console.log("Manual disconnect requested...");
      if (gel("wifi-status").style.display == "block") {
        gel("wifi-status").style.display = "none";
      }
    }
  } catch (e) {
    console.info("Was not able to fetch /status.json");
  }
}
