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
//void sled_task(void *pvParameters, int core);
//void sled_task(void *pvParameters);
void user_main(void *pvParameters, int core);
int signal(int sign, void *hnd)
{
    return 0;
}

void blink_task(void *pvParameter)
{
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while (1)
    {
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void sled_task(void *pvParameter)
{
    gpio_pad_select_gpio(MATRIX_GPIO);
    gpio_set_direction(MATRIX_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(MATRIX_GPIO, 0);
    user_main(pvParameter, -1);
}

void app_main()
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("welcome to the matrix\n");

    xTaskCreate(&blink_task,              // task function
                "blink_task",             // descriptive name of task
                configMINIMAL_STACK_SIZE, // stack depth
                NULL,                     // arguments passed to task function's pvParameter argument
                0,                        // priority of the task
                NULL                      // created task handle
    );

    xTaskCreate(&sled_task,                    // task function
                "sled_task",                   // descriptive name of task
                configMINIMAL_STACK_SIZE * 12, // stack depth
                NULL,                          // arguments passed to task function's pvParameter argument
                configMAX_PRIORITIES - 1,      // priority of the task
                NULL                           // created task handle
    );
}
