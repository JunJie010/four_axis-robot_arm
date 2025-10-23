#ifndef __DOPHIN_RUN_H__
#define __DOPHIN_RUN_H__

void Servo_Init();
void Servo_back();
void timer_speed(uint8_t x);
void Servo_motor_run(uint8_t a,uint8_t b,uint8_t c,uint8_t d);
void Servo_motor_to(uint8_t x,uint8_t y);
void BLE_motor_run(uint8_t *angle);

#endif
