#include <Arduino.h>
#include <ESP32Servo.h>
#include <Ticker.h>

#define PIN_SERVO_A 21
#define PIN_SERVO_B 22
#define PIN_SERVO_C 23
#define PIN_SERVO_G 19

struct SERVO
{
    Servo engine[4];
    uint8_t now[4];
    uint8_t set[4];
};

SERVO servo_engine;
Ticker timer;

void Servo_Init()
{
    uint8_t x;
    for(x = 0;x < 4;x ++)
    {
        servo_engine.engine[x].setPeriodHertz(50);
    }
    servo_engine.engine[0].attach(PIN_SERVO_A,500,2500);
    servo_engine.engine[1].attach(PIN_SERVO_B,500,2500);
    servo_engine.engine[2].attach(PIN_SERVO_C,500,2500);
    servo_engine.engine[3].attach(PIN_SERVO_G,500,2500);
    Serial.printf("Servos are ready for 500,2500 us with 50Hz\n");

}

void Servo_back()               //定义舵机归位函数
{
    servo_engine.now[0] = 90;
    servo_engine.set[0] = 90;
    servo_engine.engine[0].write(90);

    servo_engine.now[1] = 0;
    servo_engine.set[1] = 0;
    servo_engine.engine[1].write(0);

    servo_engine.now[2] = 150;
    servo_engine.set[2] = 150;
    servo_engine.engine[2].write(150);

    servo_engine.now[3] = 0;
    servo_engine.set[3] = 0;
    servo_engine.engine[3].write(0);

    Serial.printf("Servos back\n");
}

void timer_speed(uint8_t x)
{
    void timer_back();

    timer.attach_ms(x,timer_back);
    Serial.printf("The speed delay is %d\n",x);
}

void timer_back()
{
    uint8_t Servo_run(uint8_t x,uint8_t set_angle,uint8_t now_angle);

    uint8_t x;
    for(x = 0;x < 4;x ++)
    {
        servo_engine.now[x] = Servo_run(x,servo_engine.set[x],servo_engine.now[x]);
    }
}

uint8_t Servo_run(uint8_t x,uint8_t set_angle,uint8_t now_angle)
{
    if(set_angle > now_angle)
    {
        servo_engine.engine[x].write(now_angle);
        now_angle++;
    }
    else if(set_angle < now_angle)
    {
        servo_engine.engine[x].write(now_angle);
        now_angle--;
    }
    return now_angle;
}

void Servo_motor_run(uint8_t a,uint8_t b,uint8_t c,uint8_t d)
{
    servo_engine.set[0] = a;
    servo_engine.set[1] = b;
    servo_engine.set[2] = c;
    servo_engine.set[3] = d;
    Serial.printf("Order:ServoA %d ServoB %d ServoC %d ServoG %d\n",a,b,c,d);

}

void Servo_motor_to(uint8_t x,uint8_t y)
{
    servo_engine.engine[x].write(y);
    servo_engine.now[x] = y;
    servo_engine.set[x] = y;
    Serial.printf("Servo%d has been %d\n",x,y);
}

void BLE_motor_run(uint8_t *angle)        //定义BLE蓝牙驱动函数
{
    uint8_t x;
    for(x = 0;x < 4;x ++)
    {
        servo_engine.set[x] = angle[x];
    }
}
