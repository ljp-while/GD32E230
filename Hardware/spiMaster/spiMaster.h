#ifndef  __SPIMASTER_H
#define  __SPIMASTER_H

#include "main.h"

#define SPI_MASTER_GPIO_CLOCK RCU_GPIOB
#define SPI_MASTER_GPIO_PORT  GPIOB
#define SPI_MASTER_SCK_PIN    GPIO_PIN_13 
#define SPI_MASTER_MISO_PIN   GPIO_PIN_14 
#define SPI_MASTER_MOSI_PIN   GPIO_PIN_15 
#define SPI_CS_GPIO_CLOCK     RCU_GPIOB
#define SPI_CS_GPIO_PORT      GPIOB
#define SPI_CS_PIN            GPIO_PIN_12 

#define SPI_MASTER_PORT       SPI1
#define SPI_MASTER_CLOCK      RCU_SPI1
void spi_master_init(void);
uint8_t spi_master_transfer(uint8_t data);
void spi_cs_low(void);
void spi_cs_high(void);
static void spi_gpio_config(void);
static void spi_master_mode_config(void);

#endif /*__SPIMASTER_H*/