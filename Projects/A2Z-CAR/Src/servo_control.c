#include "servo_control.h"
#include "adc_driver.h"
#include "pwm_driver.h"
#include "cmsis_os.h"

void set_servo_angle(int8_t angle)
{
	// 5% duty cycle is the leftmost position of the steering, 10% is the rightmost,
	// for now leftmost is -45 degrees, rightmost is 45,
	// so 1 degree equals to (5 / 90) % in duty cycle.
	// 7.5 % is 0 degrees
	float duty = 7.5 + ((5.0 / 90.0) * (float)angle);
	servo_pwm_set_duty(duty);
}

void stop()
{
	BSP_LED_On(LED2);
}

void set_servo()
{
	int8_t bias = get_bias();
	printf("bias:%d\n",bias);

	if (bias <= 12) {
		set_servo_angle(bias * 3);
		printf("servo: %d\n", bias * 3);
		BSP_LED_Off(LED2);
	} else {
		printf("no line, stop\n");
		stop();
	}
}

void servo_control_thread(void const * argument)
{
	while(1) {
		set_servo();
		osDelay(10);
		//BSP_LED_Toggle(LED2);
	}
	while (1) {
		/* Delete the thread */
		osThreadTerminate(NULL);
	}
}




