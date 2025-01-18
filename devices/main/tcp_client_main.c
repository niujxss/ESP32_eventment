/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include <stdio.h>
#include "driver/gpio.h"
#include "MyI2C.h"
#include <string.h>


extern void tcp_client(char * buffer,uint8_t size);



#define GPIO_OUTPUT_IO_0    12
#define GPIO_OUTPUT_IO_1    13

static void configure_led(void)
{
    printf("Example configured to GPIO LED!");
    gpio_reset_pin(GPIO_OUTPUT_IO_0);
    gpio_reset_pin(GPIO_OUTPUT_IO_1);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(GPIO_OUTPUT_IO_0, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_OUTPUT_IO_1, GPIO_MODE_OUTPUT);
}


void vTaskled( void * pvParameters )
{
    configure_led();
    while (1)
    {
        gpio_set_level(GPIO_OUTPUT_IO_0,0);
        gpio_set_level(GPIO_OUTPUT_IO_1,1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        gpio_set_level(GPIO_OUTPUT_IO_0,1);
        gpio_set_level(GPIO_OUTPUT_IO_1,0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void vTaskData( void * pvParameters )
{
    uint8_t id_data[6];
    uint8_t buffer[6];
    char tcp_buffer[20];
    my_I2C_config();
    my_read_id(id_data);
    printf("sensor ID is 0x%x%x 0x%x%x\n",id_data[0],id_data[1],id_data[3],id_data[4]);

    while (1)
    {
        uint16_t recovery_temper = 0u;
        uint16_t recovery_hum = 0u;
        float temp = 0.f;
        float hum = 0.f;

        my_read_data(buffer);
        recovery_temper = ((uint16_t)buffer[0]<<8)|buffer[1];
        temp = -45 + 175*((float)recovery_temper/65535);

        if(temp < -40.f)
        {
            continue;
        }
 
        /* 转换湿度数据 */
        recovery_hum = ((uint16_t)buffer[3]<<8)|buffer[4];
        hum = -6 + 125 * ((float)recovery_hum / 65535);
		if(hum >= 100)   //根据数据手册编写
		{		
            hum = 100;
        }
		else if(hum <= 0)
        {
            hum = 0;
        }
				
		memset(tcp_buffer,0x0,sizeof(tcp_buffer));
		sprintf(tcp_buffer,"%.2f %.2f end",temp,hum);
        printf("send buffer:%s\n",tcp_buffer);

        tcp_client(tcp_buffer,sizeof(tcp_buffer));

        vTaskDelay(600 * 1000 / portTICK_PERIOD_MS);
    }
    
}


void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    xTaskCreate( vTaskled, "myled", 4096, NULL, 1, NULL );

    xTaskCreate( vTaskData, "mydata", 4096, NULL, 1, NULL );

}
