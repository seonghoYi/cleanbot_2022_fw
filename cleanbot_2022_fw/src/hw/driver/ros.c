#include "ros.h"
#include "ros/ros_uart.h"
#include "uart.h"

#ifdef _USE_HW_ROS


#define MID	0x01

enum
{
	ROS_PKT_SYNC1 = 0,
	ROS_PKT_SYNC2,
	ROS_PKT_LEN1,
	ROS_PKT_LEN2,
	ROS_PKT_LENCHECK,
	ROS_PKT_ID1,
	ROS_PKT_ID2
};

enum
{
	ROS_STATE_SYNC1 = 0,
	ROS_STATE_SYNC2,
	ROS_STATE_LEN1,
	ROS_STATE_LEN2,
	ROS_STATE_LENCHECK,
	ROS_STATE_ID1,
	ROS_STATE_ID2,
	ROS_STATE_MSGS,
	ROS_STATE_CS
};





bool rosInit(void)
{
	bool ret = true;
	
	return ret;
}

bool rosLoadDriver(ros_t *p_ros)
{
	bool ret = true;
	
	rosUartDriver(&p_ros->driver);
	
	return ret;
}

bool rosOpen(ros_t *p_ros, uint8_t ros_ch_, uint32_t baud_)
{
	bool ret = true;
	
	if (p_ros->driver.is_init == false)
	{
		return false;
	}

	
	p_ros->ch = ros_ch_;
	p_ros->baud = baud_;
	p_ros->index = 0;
	p_ros->state = ROS_STATE_SYNC1;
	p_ros->packet.msgs = NULL;
	p_ros->is_open = p_ros->driver.open(ros_ch_, baud_);
	
	for (int i = 0; i < ROS_MAX_SERVICE; i++)
	{
		p_ros->func[i] = NULL;
	}
	p_ros->service_index = 0;
	
	ret = p_ros->is_open;
	return ret;
}

bool rosIsOpen(ros_t *p_ros)
{
	return p_ros->is_open;
}

bool rosClose(ros_t *p_ros)
{
	bool ret = true;
	
	return ret;
}

bool rosSendInst(ros_t *p_ros, uint8_t id_, uint8_t inst_, uint8_t *p_msg_, uint8_t msg_len_)
{
	bool ret = true;
	//uint8_t packet_len;
	uint8_t index;
	uint32_t checksum = 0;
	uint8_t packet_buf[ROS_PACKET_BUF_MAX];

	if (p_ros->is_open != true)
	{
		return false;
	}
	
	//packet_len = msg_len_ + 7;
	
	packet_buf[ROS_PKT_SYNC1]		= 0xFF;
	packet_buf[ROS_PKT_SYNC2]		= 0xFF;
	packet_buf[ROS_PKT_LEN1]		= (msg_len_ >> 0) & 0xFF;
	packet_buf[ROS_PKT_LEN2]		= (msg_len_ >> 8) & 0xFF;
	packet_buf[ROS_PKT_LENCHECK]	= (uint8_t)(255 - (packet_buf[ROS_PKT_LEN1] + packet_buf[ROS_PKT_LEN2]) % 256);
	packet_buf[ROS_PKT_ID1]			= inst_;
	packet_buf[ROS_PKT_ID2]			= id_;
	
	index = 7;
	
	for (int i = 0; i < msg_len_; i++)
	{
		packet_buf[index++] = p_msg_[i];
		checksum += p_msg_[i];
	}
	
	packet_buf[index++] = 255 - (uint8_t)(checksum % 256);
	//uartPrintf(_DEF_UART0, "%X\n", packet_buf[index]);
	p_ros->driver.write(p_ros->ch, &packet_buf[0], index);
	//uartWrite(_DEF_UART0, packet_buf, index);
	return ret;
}


bool rosReceivePacket(ros_t *p_ros)
{
	bool ret = false;
	uint8_t rx_data;
	uint8_t index;
	uint32_t buf;
	
	if (p_ros->is_open != true)
	{
		return false;
	}
	
	if (p_ros->driver.available(p_ros->ch) > 0)
	{
		//rx_data = p_ros->driver.available(p_ros->ch);
		//p_ros->driver.write(p_ros->ch, &rx_data, 1);
		rx_data = p_ros->driver.read(p_ros->ch);
		//p_ros->driver.write(p_ros->ch, &rx_data, 1);
		//uartPrintf(_DEF_UART0, "%X\n", rx_data);
	}
	else
	{
		return false;
	}
	
	if (millis() - p_ros->pre_time >= 500)
	{
		p_ros->state = ROS_STATE_SYNC1;
	}
	p_ros->pre_time = millis();
	
	switch(p_ros->state)
	{
		case ROS_STATE_SYNC1:
			if (rx_data == 0xFF)
			{
				p_ros->packet_buf[ROS_PKT_SYNC1] = rx_data;
				p_ros->state = ROS_STATE_SYNC2;
			}
			else
			{
				p_ros->state = ROS_STATE_SYNC1;
			}
			break;
		case ROS_STATE_SYNC2:
			if (rx_data == 0xFF)
			{
				p_ros->packet_buf[ROS_PKT_SYNC2] = rx_data;
				p_ros->state = ROS_STATE_LEN1;
			}
			else
			{
				p_ros->state = ROS_STATE_SYNC1;
			}
			break;
		case ROS_STATE_LEN1:
			p_ros->packet_buf[ROS_PKT_LEN1] = rx_data;
			p_ros->state = ROS_STATE_LEN2;
			break;
		case ROS_STATE_LEN2:
			p_ros->packet_buf[ROS_PKT_LEN2] = rx_data;
			p_ros->state = ROS_STATE_LENCHECK;
			break;
		case ROS_STATE_LENCHECK:
			p_ros->packet_buf[ROS_STATE_LENCHECK] = rx_data;
			
			buf = p_ros->packet_buf[ROS_PKT_LEN2] + p_ros->packet_buf[ROS_PKT_LEN1];
			p_ros->packet.msg_len_checksum = 255 - (uint8_t)(buf % 256);
			
			if (p_ros->packet.msg_len_checksum == p_ros->packet_buf[ROS_PKT_LENCHECK])
			{
				p_ros->state = ROS_STATE_ID1;
			}
			else
			{
				p_ros->state = ROS_STATE_SYNC1;
			}
			
			p_ros->packet.msg_len =	(p_ros->packet_buf[ROS_PKT_LEN1] << 0) & 0x00FF;
			
			p_ros->packet.msg_len |= (p_ros->packet_buf[ROS_PKT_LEN2] << 8) & 0xFF00;
			break;
		case ROS_STATE_ID1:
			p_ros->packet_buf[ROS_STATE_ID1] = rx_data;
			p_ros->state = ROS_STATE_ID2;
			break;
		case ROS_STATE_ID2:
			p_ros->packet_buf[ROS_STATE_ID2] = rx_data;
			p_ros->index = 7;
			p_ros->packet.msgs = &p_ros->packet_buf[7];
			if (p_ros->packet.msg_len > 0)
			{
				p_ros->state = ROS_STATE_MSGS;
			}
			else
			{
				p_ros->state = ROS_STATE_SYNC1;
				ret = true;
			}
			break;
		case ROS_STATE_MSGS:
			index = p_ros->index;
			p_ros->index++;
			p_ros->packet_buf[index] = rx_data;
			if (p_ros->index >= p_ros->packet.msg_len + 7)
			{
				p_ros->state = ROS_STATE_CS;
				/*
				p_ros->state = ROS_STATE_SYNC1;
				p_ros->index = 0;
				ret = true;
				*/
				//p_ros->driver.write(p_ros->ch, p_ros->packet_buf, p_ros->packet.msg_len + 7);
			}
			break;
		case ROS_STATE_CS:
			index = p_ros->index;
			p_ros->packet_buf[index] = rx_data;
			buf = 0;
			for (int i = 0; i < p_ros->packet.msg_len; i++)
			{
				buf += p_ros->packet_buf[7 + i];
			}
			p_ros->packet.checksum = 255 - (uint8_t)(buf % 256);
			if (p_ros->packet_buf[index] == p_ros->packet.checksum)
			{
				ret = true;
				p_ros->state = ROS_STATE_SYNC1;
			}
			else
			{
				ret = false;
				p_ros->state = ROS_STATE_SYNC1;
			}
			
			p_ros->index = 0;
		break;
		default:
			break;
	}
	if (ret == true)
	{
		p_ros->packet.id = p_ros->packet_buf[ROS_PKT_ID2];
		p_ros->packet.inst = p_ros->packet_buf[ROS_PKT_ID1];
	}
	
	return ret;
}


void rosAddService(ros_t *p_ros, void (*func)(uint8_t *params))
{
	p_ros->func[p_ros->service_index] = func;
	p_ros->service_index++;
}

void rosCallService(ros_t *p_ros, uint8_t service_ch_, uint8_t *params_)
{
	if (service_ch_ < 0 || service_ch_ >= ROS_MAX_SERVICE)
	{
		return;
	}
	
	p_ros->func[service_ch_](params_);
}




#endif