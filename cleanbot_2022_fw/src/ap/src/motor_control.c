#include "motor_control.h"
#include "math.h"
#include "uart.h"

#ifdef _USE_AP_MOTOR_CONTROL


#define P_GAIN  300.0f
#define I_GAIN	150.0f
#define D_GAIN	100.0f
#define A_GAIN	15.0f

uint32_t update_ms = 0;

typedef struct
{
	uint8_t filter_type;
	float prev_input;
	float prev_output;
} filter_t;

filter_t filter[4];


float lowPassFilter(filter_t *filter, float input, float sampling_time, float cutOffFreq)
{
	float output;
	float wc = 2*M_PI*cutOffFreq; // 차단 각주파수
	float tau = 1/wc;


	output = (tau*filter->prev_output + sampling_time*input)/(tau+sampling_time);
	filter->prev_output = output; //나중을 위한 출력값 저장
	return output;
}


float highPassFilter(filter_t *filter, float input, float sampling_time, float cutOffFreq)
{
	float output;
	float wc = 2*M_PI*cutOffFreq; // 차단 각주파수
	float tau = 1/wc;


	output = (tau*filter->prev_output + tau*(input-filter->prev_input))/(tau+sampling_time);
	filter->prev_output = output;
	filter->prev_input = input;
	return output;
}


bool motorControlInit(uint32_t update_hz)
{
	bool ret = true;

	update_ms = (uint32_t)(1.0 / (float)update_hz * 1000.0);

	motorSetLeftSpeedByDuty(0);
	motorSetRightSpeedByDuty(0);
	motorStop();

	return ret;
}


motor_speed_t *motorControlUpdate(motor_speed_t set_speed)
{
	static motor_speed_t old_set_speed;
	static motor_speed_t ret_speed = {0.0f, 0.0f};
	motor_speed_t cur_speed = {0.0f, 0.0f};

	static float ep_left_term = 0, ei_left_term = 0, ed_left_term = 0, ea_left_term = 0;
	static float ep_right_term = 0, ei_right_term = 0, ed_right_term = 0, ea_right_term = 0;

	static uint32_t prev_time = 0;

	static filter_t filter[4];

	int16_t left_duty, right_duty;
	bool left_dir, right_dir;

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

		if ((int16_t)(old_set_speed.left_speed*100) != (int16_t)(set_speed.left_speed*100) || (int16_t)(old_set_speed.right_speed*100) != (int16_t)(set_speed.right_speed*100))
		{
			ei_left_term = 0;
			ei_right_term = 0;
		}


		float dt = (float)delta_ms / 1000.0f;

		left_dir = set_speed.left_speed > 0 ? _DEF_FOR : _DEF_BACK;
		right_dir = set_speed.right_speed > 0 ? _DEF_FOR : _DEF_BACK;

		cur_speed.left_speed = motorGetLeftSpeed();
		cur_speed.right_speed = motorGetRightSpeed();

		cur_speed.left_speed = motorGetLeftDirection() ? -cur_speed.left_speed : cur_speed.left_speed;
		cur_speed.right_speed = motorGetRightDirection() ? -cur_speed.right_speed : cur_speed.right_speed;

		ret_speed.left_speed = cur_speed.left_speed;
		ret_speed.right_speed = cur_speed.right_speed;

		cur_speed.left_speed = lowPassFilter(&filter[0], cur_speed.left_speed, dt, 0.1);
		cur_speed.right_speed = lowPassFilter(&filter[1], cur_speed.right_speed, dt, 0.1);

		ep_left_term = set_speed.left_speed - cur_speed.left_speed;
		ep_right_term = set_speed.right_speed - cur_speed.right_speed;

		ei_left_term += (ep_left_term - A_GAIN*ea_left_term)  * dt;
		ei_right_term += (ep_right_term - A_GAIN*ea_right_term) * dt; //anti windup


		ed_left_term = ep_left_term;
		ed_right_term = ep_right_term;
		ed_left_term = -highPassFilter(&filter[2], ed_left_term, dt, 0.1);
		ed_right_term = -highPassFilter(&filter[3], ed_right_term, dt, 0.1);

		left_duty = (int16_t)(P_GAIN*ep_left_term + I_GAIN*ei_left_term + D_GAIN*ed_left_term + 0.5f);
		right_duty = (int16_t)(P_GAIN*ep_right_term + I_GAIN*ei_right_term + D_GAIN*ed_right_term + 0.5f);


		ea_left_term = (float)left_duty;
		ea_right_term = (float)right_duty; //saturation 이전

		left_duty = constrain(left_duty, -100, 100);
		right_duty = constrain(right_duty, -100, 100);

		ea_left_term -= (float)left_duty;
		ea_right_term -= (float)right_duty; //saturation 이후

		left_dir = left_duty > 0 ? _DEF_FOR : _DEF_BACK;
		right_dir = right_duty > 0 ? _DEF_FOR : _DEF_BACK;

		motorSetLeftDirection(left_dir);
		motorSetRightDirection(right_dir);

		left_duty = abs(left_duty);
		right_duty = abs(right_duty);

		motorSetLeftSpeedByDuty(left_duty);
		motorSetRightSpeedByDuty(right_duty);



		prev_time = millis();
	}

	return &ret_speed;
}






#endif
