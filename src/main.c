#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp32_digital_led_lib.h"

#define BLINK_GPIO 5
#define MATRIX_GPIO 21


// needed for sled
void sled_task(void* pvParameters, int core);
int signal(int sign, void * hnd) {
  return 0;
}

void blink_task(void *pvParameter)
{
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    xTaskCreate(&blink_task,        // task function
        "blink_task",               // descriptive name of task
        configMINIMAL_STACK_SIZE,   // stack depth
        NULL,                       // arguments passed to task function's pvParameter argument
        5,                          // priority of the task
        NULL                        // created task handle
    );

    gpio_pad_select_gpio(MATRIX_GPIO);
    gpio_set_direction(MATRIX_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(MATRIX_GPIO, 0);

    strand_t strand;
    strand.rmtChannel = 0;strand.gpioNum = MATRIX_GPIO;
    strand.ledType = LED_WS2812B_V3;
    strand.brightLimit = 100;
    strand.numPixels = 500;
    strand.pixels = NULL;
    if (!digitalLeds_initStrands(&strand, 1)) {
        printf("init failure (digital led lib)");
        //while (1) {}
    }
    digitalLeds_resetPixels(&strand);
    digitalLeds_updatePixels(&strand);
    sled_task(NULL, -1);
}
