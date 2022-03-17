

#include "main.h"

extern i2cSlaveMessage i2cSlaveDevice;


int main(void)
{
	uint16_t count = 0;
    systick_config();
    led_init();
	i2c_slave_init();
	spi_master_init();
	uart_init(115200);
    while(1)
	{
		for(count = 0; count < 10; count++)
		{
			spi_cs_low();
			spi_master_transfer(count);
			spi_cs_high();
			delay_1ms(10);
		}
		delay_1ms(1000);
//		if(i2cSlaveDevice.stopFlag == 1)
//		{
//			i2c_test_function();
//			i2cSlaveDevice.stopFlag = 0;
//			i2cSlaveDevice.receiveCount = 0;
//		}
    }
}
