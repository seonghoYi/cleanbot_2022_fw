#ifndef ROS_UART_H_
#define ROS_UART_H_

#include "hw_def.h"

#ifdef _USE_HW_ROS
#include "ros.h"

bool rosUartDriver(ros_driver_t *p_driver);

#endif
#endif /* ROS_UART_H_ */