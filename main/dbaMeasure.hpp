#ifndef DBAMEASURE
#define DBAMEASURE 1

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "iir-filter.hpp"

class DbaMeasure
{
public:
    QueueHandle_t dBaQueue;

    DbaMeasure();

    static void audioProcessWrapper(void *);

private:
    void audioProcessTask(void *);
};

#endif