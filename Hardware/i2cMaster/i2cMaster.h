#ifndef  __I2CMASTER_H
#define  __I2CMASTER_H

#include "main.h"

#define I2C_SLAVE_ADDRESS7   0x18
#define I2C_MASTER_SPEED     100000
#define I2C_MASTER_PORT      I2C1
#define I2C_MASTER_CLOCK     RCU_I2C1
void i2c_master_init(void);
uint8_t i2c_master_send_reg8_data8(uint8_t addr, uint8_t val);
uint8_t i2c_master_send_reg8_data16(uint8_t addr, uint16_t val);

uint8_t i2c_master_read_reg8_data8(uint8_t addr);
uint16_t i2c_master_read_reg8_data16(uint8_t addr);

static void i2c_master_gpio_config(void);
static void i2c_master_mode_config(void);


#endif /*__I2CMASTER_H*/