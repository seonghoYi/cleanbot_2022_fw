#include "ros/ros_uart.h"
#include "uart.h"

#ifdef _USE_HW_ROS


static const uint8_t ros_ch_tbl[] = {_DEF_UART1, }; //_DEF_ROS0



bool		open(uint8_t ros_ch_, uint32_t baud_);
bool		close(uint8_t ros_ch_);
uint32_t	available(uint8_t ros_ch_);
uint32_t	write(uint8_t ros_ch_, uint8_t *p_data, uint32_t length);
uint8_t		read(uint8_t ros_ch_);
bool		flush(uint8_t ros_ch_);


bool rosUartDriver(ros_driver_t *p_driver)
{
	bool ret = true;
	p_driver->is_init	= true;
	p_driver->open		= open;
	p_driver->close		= close;
	p_driver->available = available;
	p_driver->write		= write;
	p_driver->read		= read;
	p_driver->flush		= flush;
	
	return ret;
}


bool open(uint8_t ros_ch_, uint32_t baud_)
{
	bool ret = false;
	switch(ros_ch_)
	{
		case _DEF_ROS0:
		ret = uartOpen(ros_ch_tbl[ros_ch_], baud_);
		break;
		default:
		break;
	}
	
	return ret;
}

bool close(uint8_t ros_ch_)
{
	bool ret = true;
	return ret;
}

uint32_t available(uint8_t ros_ch_)
{
	uint32_t ret = 0;
	switch(ros_ch_)
	{
		case _DEF_ROS0:
		ret = uartAvailable(ros_ch_tbl[ros_ch_]);
		break;
		default:
		break;
	}
	return ret;
}

uint32_t write(uint8_t ros_ch_, uint8_t *p_data, uint32_t length)
{
	uint32_t ret = 0;
	switch(ros_ch_)
	{
		case _DEF_ROS0:
		ret = uartWrite(ros_ch_tbl[ros_ch_], p_data, length);
		break;
		default:
		break;
	}
	return ret;
}

uint8_t read(uint8_t ros_ch_)
{
	uint8_t ret = 0;
	switch(ros_ch_)
	{
		case _DEF_ROS0:
		ret = uartRead(ros_ch_tbl[ros_ch_]);
		break;
		default:
		break;
	}	
	
	return ret;
}

bool flush(uint8_t ros_ch_)
{
	bool ret = true;
	switch(ros_ch_)
	{
		case _DEF_ROS0:
		ret = uartFlush(ros_ch_tbl[ros_ch_]);
		break;
		default:
		break;
	}
	return ret;
}






#endif