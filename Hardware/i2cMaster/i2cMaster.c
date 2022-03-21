#include "i2cMaster.h"

void i2c_master_init(void)
{
	i2c_master_gpio_config();
	i2c_master_mode_config();
}


static void i2c_master_gpio_config(void)
{
	
//	    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);

    /* connect PB6 to I2C0_SCL */
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_10);
    /* connect PB7 to I2C0_SDA */
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_11);
    /* configure GPIO pins of I2C0 */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_10);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_10);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_11);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_11);
}


static void i2c_master_mode_config(void)
{
    /* enable I2C clock */
    rcu_periph_clock_enable(I2C_MASTER_CLOCK);
    /* configure I2C clock */
    i2c_clock_config(I2C_MASTER_PORT,I2C_MASTER_SPEED,I2C_DTCY_2);
    /* configure I2C address */
    i2c_mode_addr_config(I2C_MASTER_PORT,I2C_I2CMODE_ENABLE,I2C_ADDFORMAT_7BITS,I2C_SLAVE_ADDRESS7 << 1);
    /* enable I2C */
    i2c_enable(I2C_MASTER_PORT);
    /* enable acknowledge */
    i2c_ack_config(I2C_MASTER_PORT,I2C_ACK_ENABLE);
}

uint8_t i2c_master_send_reg8_data16(uint8_t addr, uint16_t val)
{
	uint8_t low = val & 0xff;
	uint8_t high = (val >> 8) & 0xff;
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_I2CBSY));
    
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_MASTER_PORT);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C_MASTER_PORT, I2C_SLAVE_ADDRESS7 << 1, I2C_TRANSMITTER);
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C_MASTER_PORT,I2C_FLAG_ADDSEND);
    
    /* wait until the transmit data buffer is empty */
    while( SET != i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_TBE));
    
    /* send the EEPROM's internal address to write to : only one byte address */
    i2c_data_transmit(I2C_MASTER_PORT, addr);
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_BTC));
    
    /* send the byte to be written */
    i2c_data_transmit(I2C_MASTER_PORT, high); 
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_BTC));
    /* send the byte to be written */
    i2c_data_transmit(I2C_MASTER_PORT, low); 
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_BTC));
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C_MASTER_PORT);
    
    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C_MASTER_PORT)&0x0200);	
	return 0;
}

uint8_t i2c_master_send_reg8_data8(uint8_t addr, uint8_t val)
{
	    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_MASTER_PORT);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C_MASTER_PORT, I2C_SLAVE_ADDRESS7, I2C_TRANSMITTER);
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C_MASTER_PORT,I2C_FLAG_ADDSEND);
    
    /* wait until the transmit data buffer is empty */
    while(SET != i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_TBE));
    
    /* send the EEPROM's internal address to write to : only one byte address */
    i2c_data_transmit(I2C_MASTER_PORT, addr);
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_BTC));
    
    /* send the byte to be written */
    i2c_data_transmit(I2C_MASTER_PORT, val); 
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_BTC));

    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C_MASTER_PORT);
    
    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C_MASTER_PORT)&0x0200);
	return 0;
}

uint16_t i2c_master_read_reg8_data16(uint8_t addr)
{
	uint8_t count = 0;
	uint8_t p_buffer[5]={0};
	uint8_t number_of_byte = 2;
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_I2CBSY));

    if(2 == number_of_byte)
	{
        i2c_ackpos_config(I2C_MASTER_PORT,I2C_ACKPOS_NEXT);
    }
    
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_MASTER_PORT);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C_MASTER_PORT, I2C_SLAVE_ADDRESS7 << 1, I2C_TRANSMITTER);
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C_MASTER_PORT,I2C_FLAG_ADDSEND);
    
    /* wait until the transmit data buffer is empty */
    while(SET != i2c_flag_get( I2C_MASTER_PORT , I2C_FLAG_TBE));

    /* enable I2C0*/
    i2c_enable(I2C_MASTER_PORT);
    
    /* send the EEPROM's internal address to write to */
    i2c_data_transmit(I2C_MASTER_PORT, addr);  
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_BTC));
    
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_MASTER_PORT);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C_MASTER_PORT, I2C_SLAVE_ADDRESS7 << 1, I2C_RECEIVER);
  
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_ADDSEND));
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C_MASTER_PORT,I2C_FLAG_ADDSEND);

    if(number_of_byte < 3)
	{
        /* disable acknowledge */
        i2c_ack_config(I2C_MASTER_PORT,I2C_ACK_DISABLE);
    }
	
    if(1 == number_of_byte)
	{
        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(I2C_MASTER_PORT);
    }
    
    /* while there is data to be read */
    while(number_of_byte)
	{
        if(3 == number_of_byte)
		{
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_BTC));

            /* disable acknowledge */
            i2c_ack_config(I2C_MASTER_PORT,I2C_ACK_DISABLE);
        }
        if(2 == number_of_byte)
		{
			i2c_ack_config(I2C_MASTER_PORT,I2C_ACK_DISABLE);
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_BTC));
            
            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2C_MASTER_PORT);
        }
        
        /* wait until the RBNE bit is set and clear it */
        if(i2c_flag_get(I2C_MASTER_PORT, I2C_FLAG_RBNE))
		{
            /* read a byte from the EEPROM */
            p_buffer[count] = i2c_data_receive(I2C_MASTER_PORT);
            
            /* point to the next location where the byte read will be saved */
            count++; 
            
            /* decrement the read bytes counter */
            number_of_byte--;
        } 
    }
	

    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C_MASTER_PORT)&0x0200);
    
    /* enable acknowledge */
    i2c_ack_config(I2C_MASTER_PORT,I2C_ACK_ENABLE);

    i2c_ackpos_config(I2C_MASTER_PORT,I2C_ACKPOS_CURRENT);
	return (p_buffer[0] << 8 | p_buffer[1]);
}