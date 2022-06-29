#ifndef ROS_H_
#define ROS_H_

#include "hw_def.h"

#ifdef _USE_HW_ROS
#define ROS_PACKET_BUF_MAX	HW_ROS_PACKET_BUF_MAX
#define ROS_MAX_SERVICE		128

typedef struct
{
	bool is_init;
	
	bool		(*open)(uint8_t ros_ch_, uint32_t baud_);
	bool		(*close)(uint8_t ros_ch_);
	uint32_t	(*available)(uint8_t ros_ch_);
	uint32_t	(*write)(uint8_t ros_ch_, uint8_t *p_data, uint32_t length);
	uint8_t		(*read)(uint8_t ros_ch_);
	bool		(*flush)(uint8_t ros_ch_);
} ros_driver_t;



enum
{
	ROS_INST_PING	= 0x01,
	ROS_INST_RQSVC	= 0x02,
	ROS_INST_WAITRQ	= 0x03
};

enum
{
	ROS_ID_PUBLISHER			= 0x00,
	ROS_ID_SUBSCRIBER			= 0x01,
	ROS_ID_SERVICE_SERVER		= 0x02,
	ROS_ID_SERVICE_CLIENT		= 0x04,
	ROS_ID_PARAMETER_REQUEST	= 0x06,
	ROS_ID_LOG					= 0x07,
	ROS_ID_TIME					= 0x0A,
	ROS_ID_TX_STOP				= 0x0B
};



typedef struct
{
	uint8_t		sync;
	uint8_t		protocol_version;
	uint16_t	msg_len;
	uint8_t		msg_len_checksum;
	uint8_t		id;
	uint8_t		inst;
	uint8_t		*msgs;
	uint8_t		checksum;
} ros_packet_t;


typedef struct
{
	bool			is_open;
	uint8_t			ch;
	uint32_t		baud;
	uint8_t			state;
	uint8_t			index;
	uint32_t		pre_time;
	
	ros_packet_t	packet;
	uint8_t			packet_buf[ROS_PACKET_BUF_MAX];
	
	ros_driver_t	driver;
	
	void (*func[ROS_MAX_SERVICE])(uint8_t *params);
	uint8_t			service_index;
} ros_t;




bool rosInit(void);
bool rosLoadDriver(ros_t *p_ros);
bool rosOpen(ros_t *p_ros, uint8_t ros_ch_, uint32_t baud_);
bool rosIsOpen(ros_t *p_ros);
bool rosClose(ros_t *p_ros);

bool rosSendInst(ros_t *p_ros, uint8_t id_, uint8_t inst_, uint8_t *p_msg_, uint8_t msg_len_);
bool rosReceivePacket(ros_t *p_ros);

void rosAddService(ros_t *p_ros, void (*func)(uint8_t *params));
void rosCallService(ros_t *p_ros, uint8_t service_ch_, uint8_t *params_);

#endif
#endif /* ROS_H_ */