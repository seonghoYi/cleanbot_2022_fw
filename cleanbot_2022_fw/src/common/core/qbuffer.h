#ifndef QBUFFER_H_
#define QBUFFER_H_

#include "def.h"

/*
	사용법
	
	qbuffer_t 구조체 변수를 선언한 뒤 이를 함수에 넘기는 것이 기본적인 이용 방법이다.
	
	qbufferCreate -> qbuffer_t 구조체 변수와 실제로 데이터가 저장될 배열 선언 후 그의 길이와 함께 전달하여 초기화. 결과는 true.
	qbufferWrite -> qbuffer_t 구조체와 쓰기를 수행할 데이터의 시작 주소와 그의 길이를 전달하여 쓰기 수행. 결과는 true.
	qbufferRead -> qbuffer_t 구조체와 읽기의 결과인 데이터를 저장할 변수의 시작 주소와 읽을 바이트의 길이를 전달하여 읽기 수행. 결과는 true.
	qbufferAvailable -> qbuffer_t 구조체를 전달하여 읽을 수 있는 데이터의 수를 반환. 결과는 데이터의 개수.
	qbufferFlush -> qbuffer_t 구조체를 전달하여 버퍼를 초기화. 결과는 void
*/


typedef struct
{
	uint32_t in;
	uint32_t out;
	uint32_t len;

	uint8_t *p_buf;
} qbuffer_t;



void      qbufferInit(void);
bool      qbufferCreate(qbuffer_t *p_node, uint8_t *p_buf, uint32_t length);
bool      qbufferWrite(qbuffer_t *p_node, uint8_t *p_data, uint32_t length);
bool      qbufferRead(qbuffer_t *p_node, uint8_t *p_data, uint32_t length);
uint32_t  qbufferAvailable(qbuffer_t *p_node);
void      qbufferFlush(qbuffer_t *p_node);


#endif /* QBUFFER_H_ */