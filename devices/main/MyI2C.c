#include "MyI2C.h"
#include "driver/i2c.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "sdkconfig.h"


#define I2C_MASTER_SCL_IO           CONFIG_I2C_MASTER_SCL      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           CONFIG_I2C_MASTER_SDA      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_FREQ_HZ          300000u


#define I2C_MASTER_RX_BUF_DISABLE 0u
#define I2C_MASTER_TX_BUF_DISABLE 0u

#define I2C_DEVICE_ADDR 0x44
#define I2C_NUMBER 0

#define I2C_MASTER_TIMEOUT_MS       1000

void my_I2C_config(void)
{
    int i2c_master_port = I2C_NUMBER;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 4,         // 配置 SDA 的 GPIO
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = 5,         // 配置 SCL 的 GPIO
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,  // 为项目选择频率
        .clk_flags = 0,          // 可选项，可以使用 I2C_SCLK_SRC_FLAG_* 标志来选择 I2C 源时钟
    };

    i2c_param_config(i2c_master_port, &conf);

    i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);

}

void my_read_id(uint8_t * buffer)
{
    //写入指令
    uint8_t controlid = 0x89;
    i2c_master_write_to_device(I2C_NUMBER,I2C_DEVICE_ADDR,&controlid,1,I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    //等待数据计算
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    //读取数据

    i2c_master_read_from_device(I2C_NUMBER,I2C_DEVICE_ADDR,buffer,6,I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

}


void my_read_data(uint8_t * buffer)
{
    //写入指令
    uint8_t controlid = 0xFD;
    i2c_master_write_to_device(I2C_NUMBER,I2C_DEVICE_ADDR,&controlid,1,I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    //等待数据计算
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    //读取数据

    i2c_master_read_from_device(I2C_NUMBER,I2C_DEVICE_ADDR,buffer,6,I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

}