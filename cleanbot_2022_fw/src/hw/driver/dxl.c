#include "dxl.h"
#include "uart.h"


static uint8_t dxl_ch[DXL_MAX_CH] = {_DEF_UART2, }; //_DEF_DXL1




enum
{
	PACKET_HEADER_1 = 0x00U,
	PACKET_HEADER_2,
	PACKET_HEADER_3,
	PACKET_HEADER_4,
	PACKET_ID,
	PACKET_LENGTH_1,
	PACKET_LENGTH_2,
	PACKET_INSTRUCTION,
	PACKET_ERR,
	PACKET_PARAM,
	PACKET_CRC_1,
	PACKET_CRC_2,
};



uint16_t dxlCrcUpdate(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size);

bool dxlInit(void)
{
	bool ret = true;


	return ret;
}

bool dxlOpen(dxl_t *p_dxl, uint8_t ch, uint32_t baud)
{
	bool ret = true;

	p_dxl->uart_ch = dxl_ch[ch];

	ret &= uartOpen(p_dxl->uart_ch, baud);

	p_dxl->is_open = ret;
	return ret;
}


/*
 * https://emanual.robotis.com/docs/en/dxl/crc/
 */
uint16_t dxlCrcUpdate(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size)
{
	uint16_t i, j;
  uint16_t crc_table[256] = {
        0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
        0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
        0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
        0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
        0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
        0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
        0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
        0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
        0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
        0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
        0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
        0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
        0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
        0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
        0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
        0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
        0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
        0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
        0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
        0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
        0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
        0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
        0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
        0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
        0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
        0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
        0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
        0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
        0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
        0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
        0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
        0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
  };

	for(j = 0; j < data_blk_size; j++)
	{
			i = ((uint16_t)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
			crc_accum = (crc_accum << 8) ^ crc_table[i];
	}

  return crc_accum;
}





bool dxlTransmitPacket(dxl_t *p_dxl, uint8_t id, uint8_t instr, uint8_t *params, uint32_t param_len)
{
	bool ret = true;
	uint32_t index = 0;
	uint32_t length = param_len + 3;
	uint16_t crc = 0;

	p_dxl->transmit_packet_buf[PACKET_HEADER_1] = 0xFF;
	p_dxl->transmit_packet_buf[PACKET_HEADER_2] = 0xFF;
	p_dxl->transmit_packet_buf[PACKET_HEADER_3] = 0xFD;
	p_dxl->transmit_packet_buf[PACKET_HEADER_4] = 0x00;
	p_dxl->transmit_packet_buf[PACKET_ID] 			= id;
	p_dxl->transmit_packet_buf[PACKET_LENGTH_1] = (uint8_t)(length & 0xFF);
	p_dxl->transmit_packet_buf[PACKET_LENGTH_2] = (uint8_t)((length >> 8) & 0xFF);
	p_dxl->transmit_packet_buf[PACKET_INSTRUCTION] = instr;


	index = PACKET_INSTRUCTION+1;

	for (uint32_t i = 0; i < param_len; i++)
	{
		p_dxl->transmit_packet_buf[index++] = params[i];
	}


	crc = dxlCrcUpdate(0, &p_dxl->transmit_packet_buf[0], index);

	p_dxl->transmit_packet_buf[index++] = (uint8_t)(crc & 0xFF);
	p_dxl->transmit_packet_buf[index++] = (uint8_t)((crc >> 8) & 0xFF);

	uartWrite(p_dxl->uart_ch, &p_dxl->transmit_packet_buf[0], index);

	return ret;
}

bool dxlReceivePacket(dxl_t *p_dxl)
{
	bool ret = false;
	uint8_t rx_data;
	uint16_t crc;
	uint32_t i;


	if (millis()-p_dxl->loop_time >= 100)
	{
		p_dxl->state = PACKET_HEADER_1;
	}

	if (uartAvailable(p_dxl->uart_ch) > 0)
	{
		rx_data = uartRead(p_dxl->uart_ch);
		p_dxl->loop_time = millis();
	}
	else
	{
		return false;
	}



	switch(p_dxl->state)
	{
	case PACKET_HEADER_1:
		if (rx_data == 0xFF)
		{
			p_dxl->receive_packet_buf[PACKET_HEADER_1] = rx_data;
			p_dxl->state = PACKET_HEADER_2;
		}
		break;
	case PACKET_HEADER_2:
		if (rx_data == 0xFF)
		{
			p_dxl->receive_packet_buf[PACKET_HEADER_2] = rx_data;
			p_dxl->state = PACKET_HEADER_3;
		}
		else
		{
			p_dxl->state = PACKET_HEADER_1;
		}
		break;
	case PACKET_HEADER_3:
		if (rx_data == 0xFD)
		{
			p_dxl->receive_packet_buf[PACKET_HEADER_3] = rx_data;
			p_dxl->state = PACKET_HEADER_4;
		}
		else
		{
			p_dxl->state = PACKET_HEADER_1;
		}
		break;
	case PACKET_HEADER_4:
		if (rx_data == 0x00)
		{
			p_dxl->receive_packet_buf[PACKET_HEADER_4] = rx_data;
			p_dxl->state = PACKET_ID;
		}
		else
		{
			p_dxl->state = PACKET_HEADER_1;
		}
		break;
	case PACKET_ID:
		p_dxl->receive_packet_buf[PACKET_ID] = rx_data;
		p_dxl->state = PACKET_LENGTH_1;
		break;
	case PACKET_LENGTH_1:
		p_dxl->receive_packet_buf[PACKET_LENGTH_1] = rx_data;
		p_dxl->packet.length = rx_data;
		p_dxl->state = PACKET_LENGTH_2;
		break;
	case PACKET_LENGTH_2:
		p_dxl->receive_packet_buf[PACKET_LENGTH_2] = rx_data;
		p_dxl->packet.length |= (uint16_t)(rx_data << 8);
		p_dxl->state = PACKET_INSTRUCTION;
		break;
	case PACKET_INSTRUCTION:
		p_dxl->receive_packet_buf[PACKET_INSTRUCTION] = rx_data;

		if (rx_data == 0x55)
		{
			p_dxl->packet_ix = PACKET_ERR+1;
			p_dxl->param_len = p_dxl->packet.length - 3;
			p_dxl->state = PACKET_ERR;
		}
		else
		{
			p_dxl->packet_ix = PACKET_INSTRUCTION+1;
			p_dxl->param_len = p_dxl->packet.length - 4;
			p_dxl->state = PACKET_PARAM;
		}

		p_dxl->param_ix = 0;
		break;
	case PACKET_ERR:
		p_dxl->receive_packet_buf[PACKET_ERR] = rx_data;
		p_dxl->state = PACKET_PARAM;
		break;
	case PACKET_PARAM:
		i = p_dxl->packet_ix + p_dxl->param_ix;
		p_dxl->param_ix++;

		if (p_dxl->param_ix >= p_dxl->param_len)
		{
			p_dxl->packet.param = &p_dxl->receive_packet_buf[i-p_dxl->param_len-1];
			p_dxl->packet_ix += p_dxl->param_len;
			p_dxl->state = PACKET_CRC_1;
		}
		else
		{
			p_dxl->receive_packet_buf[i] = rx_data;
		}
		break;
	case PACKET_CRC_1:
		p_dxl->receive_packet_buf[PACKET_CRC_1] = rx_data;
		p_dxl->packet.crc = rx_data;
		p_dxl->state = PACKET_CRC_2;
		break;
	case PACKET_CRC_2:
		p_dxl->receive_packet_buf[PACKET_CRC_2] = rx_data;
		p_dxl->packet.crc |= (uint16_t)(rx_data << 8);


		crc = dxlCrcUpdate(0, &p_dxl->receive_packet_buf[0], p_dxl->packet_ix);

		if (crc == p_dxl->packet.crc)
		{
			ret = true;
		}

		p_dxl->state = PACKET_HEADER_1;
		break;
	}


	if (ret == true)
	{
		p_dxl->packet.packet_id = p_dxl->receive_packet_buf[PACKET_ID];
		p_dxl->packet.instr = p_dxl->receive_packet_buf[PACKET_INSTRUCTION];
		p_dxl->packet.err = p_dxl->receive_packet_buf[PACKET_ERR];
	}

	return ret;
}
