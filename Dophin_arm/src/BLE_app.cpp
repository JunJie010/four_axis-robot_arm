#include <Arduino.h>
#include "Dophin_run.h"

#include <BLEDevice.h>
#include <BLE2902.h>              //引进相关蓝牙通讯协议

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"       //完整的128位UUID码

BLECharacteristic *pCharacteristic = NULL;            //创建特征Characteristic
bool bleConnected = false;                            //定义布尔型变量用于判断蓝牙是否连接

class bleServerCallbacks : public BLEServerCallbacks                     //Server回调函数声明
{
    void onConnect(BLEServer *pServer)
    {
        bleConnected = true;
        Serial.printf("现在有设备接入~");
    }

    void onDisconnect(BLEServer *pServer)
    {
        bleConnected = false;
        Serial.printf("现在有设备断开连接~");
        
        pServer->startAdvertising(); 
    }   // 在有设备接入后Advertising广播会被停止，所以要在设备断开连接时重新开启广播,不然的话只有重启ESP32后才能重新搜索到
};

class bleCharacteristicCallbacks : public BLECharacteristicCallbacks      //客户端回调函数声明
{
    void onRead(BLECharacteristic *pCharacteristic)            // 客户端读取事件回调函数
    { 
        Serial.printf("触发读取事件");
    }

    void onWrite(BLECharacteristic *pCharacteristic)           // 客户端写入事件回调函数
    { 
        size_t length = pCharacteristic->getLength();
        uint8_t *pdata = pCharacteristic->getData();
        int x;
        if(length == 8){
            if(pdata[0] == 0xA5 && pdata[1] == 0xA5 && pdata[2] == 0x01 ){
                //自定义接受数据协议0xA5 0xA5 0x01 angle1 angle2 angle3 angle4        
                BLE_motor_run(pdata+3);
            }
        }
        for (x = 0; x < length; x++)
        {
            Serial.printf(" %d\n", pdata[x]);
        }
    }
};

void BLE_Init()                   //定义初始化蓝牙函数
{
    BLEDevice :: init("Mini-Arm");                 //创建BLE蓝牙设备，初始化蓝牙功能
    BLEServer *pServer = BLEDevice::createServer();      //创建服务器，设置设备的模式为BLE Server
    pServer->setCallbacks(new bleServerCallbacks());       //绑定回调函数

    BLEService *pService = pServer->createService(SERVICE_UUID);         //创建服务，给Server添加一个Service
    pCharacteristic = pService->createCharacteristic(            
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE);     //为Serveice创建Characteristic，并且声明该特征的属性为可读可写可申报
    
    pCharacteristic->setCallbacks(new bleCharacteristicCallbacks());    //如有客户端发送数据，自动执行回调函数
    pCharacteristic->addDescriptor(new BLE2902());  // 添加描述  

    pService->start(); // 启动服务
    BLEDevice :: startAdvertising();               //开启advertising广播                 

}
