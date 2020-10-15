#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp32_digital_led_lib.h"
#include "hsv2rgb.h"

#define BLINK_GPIO 5
#define MATRIX_GPIO 4

#define MATRIX_SIZE 256


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

    strand_t pStrand;
    pStrand.rmtChannel = 0;  
    pStrand.gpioNum = MATRIX_GPIO;
    pStrand.ledType = LED_WS2812B_V2;
    pStrand.brightLimit = 256;
    pStrand.numPixels = MATRIX_SIZE;
    pStrand.pixels = NULL;
    if (digitalLeds_initStrands(&pStrand, 1)) {
        while (1) {
            printf("init failure (digital led lib)\n");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
    digitalLeds_resetPixels(&pStrand);
    digitalLeds_updatePixels(&pStrand);

    int i;

    while(1) {
        for (i=0; i<MATRIX_SIZE; i++) {
            pStrand.pixels[i].r = 255;
            pStrand.pixels[i].g = 0;
            pStrand.pixels[i].b = 0;
        }
        digitalLeds_updatePixels(&pStrand);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        for (i=0; i<MATRIX_SIZE; i++) {
            pStrand.pixels[i].r = 0;
            pStrand.pixels[i].g = 255;
            pStrand.pixels[i].b = 0;
        }
        digitalLeds_updatePixels(&pStrand);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        for (i=0; i<MATRIX_SIZE; i++) {
            pStrand.pixels[i].r = 0;
            pStrand.pixels[i].g = 0;
            pStrand.pixels[i].b = 255;
        }
        digitalLeds_updatePixels(&pStrand);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
    //sled_task(NULL, -1);
}
