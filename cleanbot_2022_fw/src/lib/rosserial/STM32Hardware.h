/* 
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2018, Kenta Yonekura (a.k.a. yoneken)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of Willow Garage, Inc. nor the names of its
 *    contributors may be used to endorse or promote prducts derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ROS_STM32_HARDWARE_H_
#define ROS_STM32_HARDWARE_H_


#include "hw_def.h"
#include "uart.h"


/*
 * rosserial의 하드웨어 드라이버 부분.
 * rosserial의 다른 부분은 건들지 말 것!
 * 프로젝트와 하드웨어의 상황에 따라 아래 코드를 적절히 수정하여 사용할 것.
 */

extern UART_HandleTypeDef huart2;

class STM32Hardware {
  protected:
		uint8_t uart_ch = _DEF_UART1;
    const static uint16_t rbuflen = 1024; //수신 버퍼 최대 크기
    const static uint16_t tbuflen = 1024; //송신 버퍼 최대 크기

    uint8_t tbuf[tbuflen];
    uint32_t twind, tfind;

  public:
    STM32Hardware(): //실제로 작동되는 생성자
      uart_ch(_DEF_UART1), twind(0), tfind(0) //기본값. 적절히 수정하여 사용할 것.
    {
    }

    STM32Hardware(uint8_t uart_ch_):
      uart_ch(uart_ch_), twind(0), tfind(0)
    {
    }
  
    void init()
    {
    	uartOpen(uart_ch, 57600);
    }

    void setHardware(uint8_t uart_ch_, uint32_t baud)
    {
    	uart_ch = uart_ch_;
    	uartOpen(uart_ch, baud);
    }


    int read()
    {
      int c = -1;
      if(uartAvailable(uart_ch) > 0)
      {
        c = uartRead(_DEF_UART1);
        //uartWrite(_DEF_UART2, (uint8_t *)&c, 1);
      }
      return c;

    }

    void flush(void){
      static bool mutex = false;

      if(!mutex){
        mutex = true;

        if(twind != tfind){
          uint16_t len = tfind < twind ? twind - tfind : tbuflen - tfind;
          if (uartWrite(_DEF_UART1, &(tbuf[tfind]), len) > 0)
          {
          	tfind = (tfind + len) & (tbuflen - 1);
          }
        }
        mutex = false;
      }
    }

    void write(uint8_t* data, int length) //tbuflen을 초과하는 경우 결과를 보장할 수 없음
    {
      int n = length;
      n = n <= tbuflen ? n : tbuflen;

      int n_tail = n <= tbuflen - twind ? n : tbuflen - twind;
      memcpy(&(tbuf[twind]), data, n_tail);
      twind = (twind + n) & (tbuflen - 1);

      if(n != n_tail){
        memcpy(tbuf, &(data[n_tail]), n - n_tail);
      }

      flush();
    }

    unsigned long time()
    {
    	return millis();
    }

  protected:
};

#endif
