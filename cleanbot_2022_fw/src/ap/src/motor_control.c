#include "motor_control.h"
#include "math.h"


#ifdef _USE_AP_MOTOR_CONTROL


#define P_GAIN  16.0f
#define I_GAIN	10.0f

uint32_t update_ms = 0;

bool motorControlInit(uint32_t update_hz)
{
	bool ret = true;

	update_ms = (uint32_t)(1.0 / (float)update_hz * 1000.0);

	motorSetLeftSpeed(0.0f);
	motorSetRightSpeed(0.0f);
	motorStop();

	return ret;
}


motor_speed_t *motorControlUpdate(motor_speed_t set_speed)
{
	static motor_speed_t ret_speed = {0.0f, 0.0f};
	static motor_speed_t cur_speed = {0.0f, 0.0f};

	static float ep_left_term = 0, ei_left_term = 0;//, prev_left_error = 0;
	static float ep_right_term = 0, ei_right_term = 0;//, prev_right_error = 0;

	static uint32_t prev_time = 0;//millis();



	motor_speed_t set_point;
	bool left_dir = true, right_dir = true;

	uint32_t delta_ms = millis() - prev_time;
	if (delta_ms >= update_ms)
	{
		if (set_speed.left_speed != 0.0f || set_speed.right_speed != 0.0f)
		{
			motorRun();
		}
		else
		{
			motorStop();
		}


		float dt = (float)delta_ms / 1000.0f;


		cur_speed.left_speed = motorGetLeftSpeed();
		cur_speed.right_speed = motorGetRightSpeed();


		ret_speed.left_speed = motorGetLeftDirection() ? -cur_speed.left_speed : cur_speed.left_speed;
		ret_speed.right_speed = motorGetRightDirection() ? -cur_speed.right_speed : cur_speed.right_speed;

		ep_left_term = set_speed.left_speed - cur_speed.left_speed;
		ep_right_term = set_speed.right_speed - cur_speed.right_speed;

		ei_left_term += ep_left_term * dt;
		ei_right_term += ep_right_term * dt;

		//uartPrintf(_DEF_UART1, "%f\n", ep_right_term);

		//prev_left_error = ep_left_term;
		//prev_right_error = ep_right_term;

		set_point.left_speed = P_GAIN*ep_left_term + I_GAIN*ei_left_term;
		set_point.right_speed = P_GAIN*ep_right_term + I_GAIN*ei_right_term;
		//uartPrintf(_DEF_UART1, "left: %f, right: %f\n", set_point.left_speed, set_point.right_speed);

		//left_duty = (uint16_t)(P_GAIN*ep_left_term + I_GAIN*ei_left_term + 0.5f);
		//right_duty = (uint16_t)(P_GAIN*ep_right_term + I_GAIN*ei_right_term + 0.5f);

		//uartPrintf(_DEF_UART1, "left: %d, right: %d\n", left_duty, right_duty);

		left_dir = set_point.left_speed > 0 ? _DEF_FOR : _DEF_BACK;
		right_dir = set_point.right_speed > 0 ? _DEF_FOR : _DEF_BACK;

		motorSetLeftDirection(left_dir);
		motorSetRightDirection(right_dir);


		set_point.left_speed = fabs(set_point.left_speed);
		set_point.right_speed = fabs(set_point.right_speed);
		//set_point.left_speed = constrain(fabs(set_point.left_speed), 0, 100);
		//set_point.right_speed = constrain(fabs(set_point.right_speed), 0, 100);

		motorSetLeftSpeed(set_point.left_speed);
		motorSetRightSpeed(set_point.right_speed);



		prev_time = millis();
	}

	return &ret_speed;
}

#endif
