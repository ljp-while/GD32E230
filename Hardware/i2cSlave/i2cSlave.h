#ifndef  __I2CSLAVE_H
#define  __I2CSLAVE_H

#include "main.h"

#define I2C_SLAVE_GPIO_CLOCK RCU_GPIOB
#define I2C_SLAVE_GPIO_PORT  GPIOB
#define I2C_SLAVE_SDA_PIN    GPIO_PIN_7
#define I2C_SLAVE_SCL_PIN    GPIO_PIN_6
#define I2C_SLAVE_CLOCK      RCU_I2C0
#define I2C_SLAVE_PORT  	 I2C0

#define I2C_SLAVE_EV_IRQ     I2C0_EV_IRQn
#define I2C_SLAVE_ER_IRQ     I2C0_ER_IRQn

#define I2C_SLAVE_EV_FUNCTION I2C0_EV_IRQHandler
#define I2C_SLAVE_ER_FUNCTION I2C0_ER_IRQHandler
 
#define I2C_SLAVE_ADDRESS    0x18
#define I2C_SLAVE_SPEED      400000

#define REGISTER_VALUE_CHAR_LENGTH  2

#define TEST_REGISTER1 0X00
#define TEST_REGISTER2 0x01

typedef struct{
	uint8_t receiveCount;
	uint8_t sendCount;
	uint8_t stopFlag;
	uint8_t registerAdd;
	uint16_t dataValue;
	uint8_t firstByteFlag;
	uint8_t sendBuff[REGISTER_VALUE_CHAR_LENGTH];
	uint8_t dataBuff[REGISTER_VALUE_CHAR_LENGTH];
}i2cSlaveMessage;


void i2c_slave_init(void);
uint8_t i2c_test_function(void);
static void i2c_slave_gpio_config(void);
static void i2c_slave_mode_config(void);
static void i2c_slave_nvic_config(void);
static uint32_t strcat_register(uint8_t *buff);

#endif /*__I2CSLAVE_H*/