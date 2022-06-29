#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "hw_def.h"

#ifdef _USE_HW_BT
#define BT_MSG_BUF_MAX		HW_BT_MSG_BUF_MAX

typedef struct
{
	bool is_init;
	
	bool		(*open)(uint8_t bt_ch_, uint32_t baud_);
	bool		(*close)(uint8_t bt_ch_);
	uint32_t	(*available)(uint8_t bt_ch_);
	uint32_t	(*write)(uint8_t bt_ch_, uint8_t *p_data, uint32_t length);
	uint8_t		(*read)(uint8_t bt_ch_);
	bool		(*flush)(uint8_t bt_ch_);
} bt_driver_t;



#endif
#endif /* BLUETOOTH_H_ */