# Klankentappers hardware

## Attach the O-ring onto the microphone cover

![O-ring](/documentation/imgs/hardware_oring.png)

*We start with this step to make sure the glue has hardened before using the cover.*

Supplies

- 3D printed Microphone cover
- O-ring
- Superglue
- Tweezers

Process

1. Carefully put 2 to 3 drops of superglue onto the o-ring.
2. Use the tweezers to attach the o-ring onto the microphone cover.

⚠️ **Attention!** Superglue sticks immediately and cannot be repaired, so make sure that the o-ring is centered immediately.

⚠️ **Attention!** Make sure that the hole in the cover stays open. Use a needle to reopen the hole, if necessary.

## Preperation of the junction box

![Junction box](/documentation/imgs/hardware_junction_box.png)

Supplies

- Junction box
- Screwdriver/hammer

Process

1. Make place for both cable glands by opening up one of the openings at either side of the junction box. **Attention!** Make sure the openings are both across from eachother and on the small sides of the junction box.
2. Plug the holes ate the bottom of the junction box with the included rubber stops.

## Preperation of the base plate

![Baseplate](/documentation/imgs/hardware_baseplate.png)

Supplies

- Base plate
- 4 M3x8 screws
- 3 M2.5 standoffs

Process

1. Secure the 4 standoffs onto the base plate (the 3 smaller holes).
2. Secure the base plate into the junction box.

## Make the USB cable thicker

![Cable](/documentation/imgs/hardware_cable.png)

*We are making the USB cable thicker with insulating tape because the USB cable is too thin to be properly closed with the cable gland.*

Supplies

- USB-C cable
- Insulating tape

Process

1. Use the insulating tape to make the USB cable thicker, starting 3cm from the plug, about 9 - 10mm thick.

## Mount the ESP32

![ESP32](/documentation/imgs/hardware_ESP32.png)

Supplies

- USB-C cable
- Cable gland
- ESP32
- 3 M2.5x10 screws

Process

1. Secure the ESP32 with the screws onto the standoffs.
2. Screw the cable gland onto the junction box.
3. Remove the sealing nut of the cable gland and slide it over the USB cable.
4. Pull the USB cable trough the cable gland and secure it into the ESP32.
5. Screw the sealing nut onto the cable gland.

![Cable insert](/documentation/imgs/hardware_cable_insert.png)

## Wire the microphone

Supplies

- Microphone PCB
- 5 F-F jumper cables (5 different colours)
- Pen and paper

Process

1. Attach the cables to the following pins: VDD, GND, SDATA, BCLK and LRCLK on the PCB.
2. Make notes of which colour you connected to which pin.

![Microphone](/documentation/imgs/hardware_microphone.png)

## Connect the microphone to the ESP32

![GPIO connected](/documentation/imgs/hardware_gpio_connected.png)

Supplies

- 3D printed cable gland

Process

1. Screw the 3D printed cable gland onto the junction box. **Attention!** Make sure the edge inside the cable gland, where the PCB will rest is pointed outwards.
2. Pull the cables through the cable gland.
3. Attach the 5 cables using your notes to the ESP32, as shown in the image below.

![GPIO](/documentation/imgs/hardware_gpio.png)

⚠️ **Attention!** The 3D printed cable gland is brittle. Don't screw it on too tight or it will break.

⚠️ **Attention!** The PCB can break if the connections are incorrect! Make sure the connections are properly connected.

## Cover up the microphone

![ESP32](/documentation/imgs/hardware_cover.png)

Supplies

- 3D printed microphone cover with o-ring

Process

1. Use the microphone cover to secure the PCB into the 3D printed cable gland by screwing it on.

⚠️ **Attention!** The 3D printed cable gland is brittle. Don't screw the cover on too tight or it will break.

## Mount the windshield

![ESP32](/documentation/imgs/hardware_windshield.png)

Supplies

- Windshield
- Cable tie

Process

1. Secure the windshield onto the Microphone cover using a cable tie.

## Done!
