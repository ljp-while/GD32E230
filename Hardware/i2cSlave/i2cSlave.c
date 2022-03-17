#include "i2cSlave.h"

uint16_t temp = 0;

uint16_t testValue1 = 0;
uint16_t testValue2 = 0;

i2cSlaveMessage i2cSlaveDevice;

void i2c_slave_init(void)
{
	i2c_slave_gpio_config();
	i2c_slave_mode_config();
	i2c_slave_nvic_config();
	memset(&i2cSlaveDevice,0,sizeof(i2cSlaveMessage));
}

static void i2c_slave_gpio_config(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(I2C_SLAVE_GPIO_CLOCK);
	/* enable I2C clock */
    rcu_periph_clock_enable(I2C_SLAVE_CLOCK);

    /* config I2C0_SCL */
    gpio_af_set(I2C_SLAVE_GPIO_PORT, GPIO_AF_1, I2C_SLAVE_SCL_PIN);
    /* config I2C0_SDA */
    gpio_af_set(I2C_SLAVE_GPIO_PORT, GPIO_AF_1, I2C_SLAVE_SDA_PIN);
    /* configure GPIO pins of I2C0 */
    gpio_mode_set(I2C_SLAVE_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,I2C_SLAVE_SCL_PIN);
    gpio_output_options_set(I2C_SLAVE_GPIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,I2C_SLAVE_SCL_PIN);
    gpio_mode_set(I2C_SLAVE_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,I2C_SLAVE_SDA_PIN);
    gpio_output_options_set(I2C_SLAVE_GPIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,I2C_SLAVE_SDA_PIN);	
}

static void i2c_slave_mode_config(void)
{
	/* enable I2C clock */
    rcu_periph_clock_enable(I2C_SLAVE_CLOCK);
	/* configure I2C clock */
    i2c_clock_config(I2C_SLAVE_PORT,I2C_SLAVE_SPEED,I2C_DTCY_2);
    /* configure I2C address */
    i2c_mode_addr_config(I2C_SLAVE_PORT,I2C_I2CMODE_ENABLE,I2C_ADDFORMAT_7BITS,I2C_SLAVE_ADDRESS << 1);
    /* enable I2C0 */
    i2c_enable(I2C_SLAVE_PORT);
    /* enable acknowledge */
    i2c_ack_config(I2C_SLAVE_PORT,I2C_ACK_ENABLE);
	i2c_interrupt_enable(I2C_SLAVE_PORT,I2C_INT_EV | I2C_INT_ERR);
}

static void i2c_slave_nvic_config(void)
{
    /* enable and set key EXTI interrupt priority */
    nvic_irq_enable(I2C_SLAVE_EV_IRQ,1U);	
	nvic_irq_enable(I2C_SLAVE_ER_IRQ,0U);
}

uint8_t i2c_test_function(void)
{
	if(i2cSlaveDevice.receiveCount != REGISTER_VALUE_CHAR_LENGTH)
	{
		return 0;
	}
	switch(i2cSlaveDevice.registerAdd)
	{
		case TEST_REGISTER1:
			testValue1 = (i2cSlaveDevice.dataBuff[0] << 8) | (i2cSlaveDevice.dataBuff[1]);
			printf("testValue1 = %#X\r\n",testValue1);
			break;
		case TEST_REGISTER2:
			testValue2 = (i2cSlaveDevice.dataBuff[0] << 8) | (i2cSlaveDevice.dataBuff[1]);
			printf("testValue2 = %#X\r\n",testValue2);
			break;
		default:
			break;
	}
	return 1;
}

void I2C_SLAVE_EV_FUNCTION(void)
{

	if(i2c_flag_get(I2C_SLAVE_PORT,I2C_FLAG_ADDSEND))  //地址匹配
	{
		temp = I2C_STAT0(I2C_SLAVE_PORT);
		temp = I2C_STAT1(I2C_SLAVE_PORT);
		i2cSlaveDevice.firstByteFlag = 1;
	}
	else if(i2c_flag_get(I2C_SLAVE_PORT,I2C_FLAG_RBNE))	//从机接收
	{
		if(i2cSlaveDevice.firstByteFlag == 1)
		{
			i2cSlaveDevice.registerAdd = i2c_data_receive(I2C_SLAVE_PORT);
			i2cSlaveDevice.receiveCount = 0;
			i2cSlaveDevice.firstByteFlag = 0;
			i2cSlaveDevice.sendCount = 0;
		}
		else
		{
			i2cSlaveDevice.dataBuff[i2cSlaveDevice.receiveCount++] = i2c_data_receive(I2C_SLAVE_PORT);
		}
		if(i2cSlaveDevice.registerAdd == TEST_REGISTER1)
		{
			i2cSlaveDevice.sendBuff[0] = (uint8_t)(testValue1 >> 8);
			i2cSlaveDevice.sendBuff[1] = testValue1 & 0xff;
		}
		else if(i2cSlaveDevice.registerAdd == TEST_REGISTER2)
		{
			i2cSlaveDevice.sendBuff[0] = (uint8_t)(testValue2 >> 8);
			i2cSlaveDevice.sendBuff[1] = testValue2 & 0xff;			
		}
	}
	else if(i2c_flag_get(I2C_SLAVE_PORT,I2C_FLAG_TBE))		//从机发送
	{
		i2c_data_transmit(I2C_SLAVE_PORT,i2cSlaveDevice.sendBuff[i2cSlaveDevice.sendCount++]);
	}
	else if(i2c_flag_get(I2C_SLAVE_PORT,I2C_FLAG_AERR))		//
	{
		i2c_flag_clear(I2C_SLAVE_PORT,I2C_FLAG_AERR);
	}
	else if(i2c_flag_get(I2C_SLAVE_PORT,I2C_FLAG_STPDET))   //停止信号
	{
		i2cSlaveDevice.stopFlag = 1;
		temp = I2C_STAT0(I2C_SLAVE_PORT);
		I2C_CTL0(I2C_SLAVE_PORT) |= I2C_CTL0_STOP;
	}
}
void I2C_SLAVE_ER_FUNCTION(void)
{
	if(i2c_flag_get(I2C_SLAVE_PORT,I2C_FLAG_BERR))  
	{
		i2c_flag_clear(I2C_SLAVE_PORT,I2C_FLAG_BERR);
	}	
	else if(i2c_flag_get(I2C_SLAVE_PORT,I2C_FLAG_LOSTARB))		//
	{
		i2c_flag_clear(I2C_SLAVE_PORT,I2C_FLAG_LOSTARB);
	}	
	else if(i2c_flag_get(I2C_SLAVE_PORT,I2C_FLAG_AERR))		//
	{
		i2c_flag_clear(I2C_SLAVE_PORT,I2C_FLAG_AERR);
	}
	else if(i2c_flag_get(I2C_SLAVE_PORT,I2C_FLAG_OUERR))		//
	{
		i2c_flag_clear(I2C_SLAVE_PORT,I2C_FLAG_OUERR);
	}
	else if(i2c_flag_get(I2C_SLAVE_PORT,I2C_FLAG_PECERR))		//
	{
		i2c_flag_clear(I2C_SLAVE_PORT,I2C_FLAG_PECERR);
	}
	else if(i2c_flag_get(I2C_SLAVE_PORT,I2C_FLAG_SMBTO))		//
	{
		i2c_flag_clear(I2C_SLAVE_PORT,I2C_FLAG_SMBTO);
	}
	else if(i2c_flag_get(I2C_SLAVE_PORT,I2C_FLAG_SMBALT))		//
	{
		i2c_flag_clear(I2C_SLAVE_PORT,I2C_FLAG_SMBALT);
	}
}

