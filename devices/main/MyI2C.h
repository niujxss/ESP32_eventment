#ifndef MYI2C_H__
#define MYI2C_H__
#include "stdint.h"
#include "esp_err.h"

void my_I2C_config(void);
void my_read_id(uint8_t * buffer);
esp_err_t my_read_data(uint8_t * buffer);
esp_err_t my_reset(void);
uint8_t my_crc8(const uint8_t * data, int len);
#endif
