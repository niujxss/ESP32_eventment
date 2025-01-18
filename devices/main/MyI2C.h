#ifndef MYI2C_H__
#define MYI2C_H__
#include "stdint.h"

void my_I2C_config(void);
void my_read_id(uint8_t * buffer);
void my_read_data(uint8_t * buffer);
#endif
