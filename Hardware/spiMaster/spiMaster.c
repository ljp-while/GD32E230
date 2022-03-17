#include "spiMaster.h"


void spi_master_init(void)
{
	spi_gpio_config();
	spi_master_mode_config();
}

static void spi_gpio_config(void)
{
	 /* enable  clock */
    rcu_periph_clock_enable(SPI_MASTER_GPIO_CLOCK);
   
    gpio_af_set(SPI_MASTER_GPIO_PORT, GPIO_AF_0, SPI_MASTER_SCK_PIN | SPI_MASTER_MISO_PIN | SPI_MASTER_MOSI_PIN);
    gpio_mode_set(SPI_MASTER_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_MASTER_SCK_PIN | SPI_MASTER_MISO_PIN | SPI_MASTER_MOSI_PIN);
    gpio_output_options_set(SPI_MASTER_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI_MASTER_SCK_PIN | SPI_MASTER_MISO_PIN | SPI_MASTER_MOSI_PIN);

    rcu_periph_clock_enable(SPI_CS_GPIO_CLOCK);   
    gpio_mode_set(SPI_CS_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SPI_CS_PIN);
    gpio_output_options_set(SPI_CS_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI_CS_PIN);		
}

static void spi_master_mode_config(void)
{
	spi_parameter_struct spi_init_struct;
	
	rcu_periph_clock_enable(SPI_MASTER_CLOCK);
    /* SPI parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_16;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI_MASTER_PORT, &spi_init_struct);

    /* set crc polynomial */
    spi_enable(SPI_MASTER_PORT);
}

void spi_cs_low(void)
{
	gpio_bit_reset(SPI_CS_GPIO_PORT, SPI_CS_PIN);
}

void spi_cs_high(void)
{
	gpio_bit_set(SPI_CS_GPIO_PORT, SPI_CS_PIN);
}

uint8_t spi_master_transfer(uint8_t data)
{
	
    SPI_CTL1(SPI_MASTER_PORT) |= SPI_CTL1_BYTEN;
    while(RESET == (SPI_STAT(SPI_MASTER_PORT)&SPI_FLAG_TBE));
    SPI_DATA(SPI_MASTER_PORT) = data;

    while(RESET == (SPI_STAT(SPI_MASTER_PORT)&SPI_FLAG_RBNE));
    return(SPI_DATA(SPI_MASTER_PORT));
}