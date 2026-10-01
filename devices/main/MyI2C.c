#include "MyI2C.h"
#include "driver/i2c.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "sdkconfig.h"
#include "esp_err.h"


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


esp_err_t my_read_data(uint8_t * buffer)
{
    //写入指令
    uint8_t controlid = 0xFD;
    esp_err_t err = i2c_master_write_to_device(I2C_NUMBER,I2C_DEVICE_ADDR,&controlid,1,I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    if(err != ESP_OK)
    {
        return err;  // 测量命令没发出去，传感器不会重新测量，此时读回来的必然是旧数据
    }

    //等待数据计算
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    //读取数据

    return i2c_master_read_from_device(I2C_NUMBER,I2C_DEVICE_ADDR,buffer,6,I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

}

//SHT4x 软复位，命令 0x94，数据手册规定 1ms 内完成
esp_err_t my_reset(void)
{
    uint8_t controlid = 0x94;
    esp_err_t err = i2c_master_write_to_device(I2C_NUMBER,I2C_DEVICE_ADDR,&controlid,1,I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    vTaskDelay(pdMS_TO_TICKS(10));
    return err;
}

//SHT4x CRC-8：多项式 0x31，初值 0xFF，MSB first，不反转
uint8_t my_crc8(const uint8_t * data, int len)
{
    uint8_t crc = 0xFF;
    int i;
    int b;
    for(i = 0; i < len; i++)
    {
        crc ^= data[i];
        for(b = 0; b < 8; b++)
        {
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
        }
    }
    return crc;
}