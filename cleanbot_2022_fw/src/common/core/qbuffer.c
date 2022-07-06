﻿#include "qbuffer.h"


void qbufferInit(void)
{
	//사용하지 않음. 일관성을 위한 구조.
}

//qbuffer 에 대한 동작방식은 qbuffer 또는 ring buffer 등의 키워드로 검색하여 공부하길 바람. 

bool qbufferCreate(qbuffer_t *p_node, uint8_t *p_buf, uint32_t length)
{
	bool ret = true;

	p_node->in    = 0;
	p_node->out   = 0;
	p_node->len   = length;
	p_node->size 	= 1;
	p_node->p_buf = p_buf;

	return ret;
}

bool qbufferCreateBySize(qbuffer_t *p_node, uint8_t *p_buf, uint32_t size, uint32_t length)
{
	bool ret = true;

	p_node->in    = 0;
	p_node->out   = 0;
	p_node->len   = length;
	p_node->size	= size;
	p_node->p_buf = p_buf;

	return ret;
}

bool qbufferWrite(qbuffer_t *p_node, uint8_t *p_data, uint32_t length)
{
	bool ret = 0;
	uint32_t next_in;

	for (int i=0; i<length; i++)
	{
		next_in = (p_node->in + 1) %p_node->len;

		if (next_in != p_node->out)
		{
			if (p_node->p_buf != NULL && p_data != NULL)
			{
				uint8_t *p_buf = &p_node->p_buf[p_node->in*p_node->size];
				for (int i = 0; i < p_node->size; i++)
				{
					p_buf[i] = p_data[i];
				}
			}
			p_node->in = next_in;
		}
		else
		{
			ret = false;
			break;
		}
	}
	return ret;
}


bool qbufferRead(qbuffer_t *p_node, uint8_t *p_data, uint32_t length)
{
	bool ret = true;

	for (int i=0; i<length; i++)
	{
		if (p_node->p_buf != NULL && p_data != NULL)
		{
			uint8_t *p_buf;

			p_buf = &p_node->p_buf[p_node->out*p_node->size];

			for (int j = 0; j < p_node->size; j++)
			{
				p_data[i*p_node->size+j] = p_buf[j];
			}
		}

		if (p_node->out != p_node->in)
		{
			p_node->out = (p_node->out + 1) % p_node->len;
		}
		else
		{
			ret = false;
			break;
		}
	}
	return ret;
}

uint32_t qbufferAvailable(qbuffer_t *p_node)
{
	uint32_t ret;

	ret = (p_node->in - p_node->out) %p_node->len;

	return ret;
}


void qbufferFlush(qbuffer_t *p_node)
{
	p_node->in  = 0;
	p_node->out = 0;
}
