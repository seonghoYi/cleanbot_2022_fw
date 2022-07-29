#ifndef SRC_COMMON_HW_INCLUDE_DXL_H_
#define SRC_COMMON_HW_INCLUDE_DXL_H_

#include "hw_def.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef _USE_HW_DXL
#define DXL_MAX_CH	HW_DXL_MAX_CH



typedef struct
{
	uint8_t header[3];
	uint8_t reserved;
	uint8_t packet_id;
	uint16_t length;
	uint8_t instr;
	uint8_t err;
	uint8_t *param;
	uint16_t crc;
} dxl_packet_t;


typedef struct
{
	bool is_open;
	uint8_t uart_ch;
	uint8_t state;
	uint32_t packet_ix;
	uint32_t param_ix;
	uint32_t param_len;
	uint32_t loop_time;

	dxl_packet_t packet;

	uint8_t receive_packet_buf[HW_DXL_BUF_MAX];
	uint8_t transmit_packet_buf[HW_DXL_BUF_MAX];
} dxl_t;


bool dxlInit(void);
bool dxlOpen(dxl_t *p_dxl, uint8_t ch, uint32_t baud);
bool dxlTransmitPacket(dxl_t *p_dxl, uint8_t id, uint8_t instr, uint8_t *params, uint32_t param_len);
bool dxlReceivePacket(dxl_t *p_dxl);


#endif

#ifdef __cplusplus
}

#endif

#endif /* SRC_COMMON_HW_INCLUDE_DXL_H_ */
