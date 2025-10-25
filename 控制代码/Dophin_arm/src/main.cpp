#include <Arduino.h>
#include "Dophin_run.h"
#include "BLE_app.h"

void setup() {
    Serial.begin(115200);
    Serial.printf("Start\n");



    Servo_Init();
    delay(1500);        //留给串口一些时间
    Servo_back(); 
    delay(1500);
    timer_speed(20);
    




}
void loop() {
    
    
}
