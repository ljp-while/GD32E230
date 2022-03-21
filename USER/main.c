

#include "main.h"

extern i2cSlaveMessage i2cSlaveDevice;


int main(void)
{
	uint16_t val = 0;
    systick_config();
    led_init();
	i2c_slave_init();
	spi_master_init();
	i2c_master_init();
	uart_init(115200);
    while(1)
	{

		val = i2c_master_read_reg8_data16(0x00);
		printf("val = %#X\r\n",val);
		i2c_master_send_reg8_data16(0x00,0x1234);
		delay_1ms(5);		
		if(i2cSlaveDevice.stopFlag == 1)
		{
			i2c_test_function();
			i2cSlaveDevice.stopFlag = 0;
			i2cSlaveDevice.receiveCount = 0;
		}
		delay_1ms(1000);
    }
}
