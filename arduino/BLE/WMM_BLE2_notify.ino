/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara
   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8
   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.
   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <iostream>
#include <sstream> 
#include <stdio.h>
#include <string.h>


/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (10)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno1 = Adafruit_BNO055(1, 0x28);
Adafruit_BNO055 bno2 = Adafruit_BNO055(2, 0x29);

/*void displaySensorDetails(bno)
{
  sensor_t bno;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}*/


BLEServer* pServer = NULL;
BLECharacteristic* quat1_c = NULL;
BLECharacteristic* calib1_c = NULL;
BLECharacteristic* linear1_c = NULL;
BLECharacteristic* gyro1_c = NULL;
BLECharacteristic* quat2_c = NULL;
BLECharacteristic* calib2_c = NULL;
BLECharacteristic* linear2_c = NULL;
BLECharacteristic* gyro2_c = NULL;
BLECharacteristic* passive_c = NULL;


bool deviceConnected = false;
bool oldDeviceConnected = false;
String passive_array[400];
uint8_t count = 0;
// Set mode selection variable 0 = passive; 1 = active; (passive by default)
uint8_t device_mode = 0;

std::ostringstream os;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define IMU_UUID          "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define IMU1_QUAT_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define IMU1_CALIB_UUID   "1773bab4-4d83-4355-ba80-7bab8d2e776e"
#define IMU1_LINEAR_UUID  "9d0a663e-35a2-4649-a4e6-abb3dd5fcf59"
#define IMU1_GYRO_UUID "a94b0060-62dc-4f06-9f52-be1dfa5b36f5"
#define IMU2_QUAT_UUID    "a860ec8e-b2f8-48ec-9ecf-9d20c7b50738"
#define IMU2_CALIB_UUID   "776f298c-6843-4915-8d11-cd0d58123972"
#define IMU2_LINEAR_UUID  "cd356156-f015-4c8a-bc03-d081e2cfeb39"
#define IMU2_GYRO_UUID "1cb3d4f1-5adc-4f44-8cd6-382673b2473e"
#define PASSIVE_UUID      "c2460c7a-dece-445f-8326-5ce21d100f42"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};



void setup() {

  /* Initialise the sensor */
  if(!bno1.begin() || !bno2.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
   
  delay(1000);

  /* Use external crystal for better accuracy */
  bno1.setExtCrystalUse(true);
  bno2.setExtCrystalUse(true);
   
  /* Display some basic information on this sensor */
  int8_t temp1 = bno1.getTemp();
  int8_t temp2 = bno2.getTemp();
  Serial.print("Current Temperature: ");
  Serial.print(temp1);
  Serial.println(" C");
  Serial.println("");
  
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("WMM");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service for the imu's
  BLEService* imu = pServer->createService(IMU_UUID);

  // Create BLE Characteristics for quaternions, eulers, and calibration
  quat1_c = imu->createCharacteristic(
                      IMU1_QUAT_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  calib1_c = imu->createCharacteristic(
                      IMU1_CALIB_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  linear1_c = imu->createCharacteristic(
                      IMU1_LINEAR_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  gyro1_c = imu->createCharacteristic(
                      IMU1_GYRO_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  quat2_c = imu->createCharacteristic(
                      IMU2_QUAT_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  calib2_c = imu->createCharacteristic(
                      IMU2_CALIB_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  linear2_c = imu->createCharacteristic(
                      IMU2_LINEAR_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  gyro2_c = imu->createCharacteristic(
                      IMU2_GYRO_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  passive_c = imu->createCharacteristic(
                      PASSIVE_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create BLE Descriptors for characteristics
  quat1_c->addDescriptor(new BLE2902());
  calib1_c->addDescriptor(new BLE2902());
  quat2_c->addDescriptor(new BLE2902());
  calib2_c->addDescriptor(new BLE2902());
  passive_c->addDescriptor(new BLE2902());

  // Start the service
  imu->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(IMU_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value
    sensors_event_t event;
    bno1.getEvent(&event);
    bno2.getEvent(&event);
    imu::Quaternion quat1 = bno1.getQuat();
    imu::Quaternion quat2 = bno2.getQuat();
    imu::Vector<3> linear1 = bno1.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    imu::Vector<3> linear2 = bno2.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    imu::Vector<3> gyro1 = bno1.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> gyro2 = bno2.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    uint8_t sys, gyro_cal, accel, mag = 0;
    uint8_t sys2, gyro_cal2, accel2, mag2 = 0;
    bno1.getCalibration(&sys, &gyro_cal, &accel, &mag);
    bno2.getCalibration(&sys2, &gyro_cal2, &accel2, &mag2);
    String quat1_str = "";
    String quat2_str = "";
    String calib1_str = "";
    String calib2_str = "";
    String linear1_str = "";
    String linear2_str = "";
    String gyro1_str = "";
    String gyro2_str = "";
    
    
    if (deviceConnected) {
      switch(device_mode) {
        case 0:  
          int i;
        
          quat1_str = (String)quat1.w() + "," + (String)quat1.x() + "," + (String)quat1.y() + "," + (String)quat1.z() + ";" ;
          calib1_str = (String)sys + "," + (String)gyro_cal + "," + (String)accel + "," + (String)mag + ";" ;
          linear1_str = (String)linear1.x() + "," + (String)linear1.y() + "," + (String)linear1.z() + ";" ;
          gyro1_str = (String)gyro1.x() + "," + (String)gyro1.y() + "," + (String)gyro1.z() + ";" ;
          quat2_str = (String)quat2.w() + "," + (String)quat2.x() + "," + (String)quat2.y() + "," + (String)quat2.z() + ";" ;
          calib2_str = (String)sys2 + "," + (String)gyro_cal2 + "," + (String)accel2 + "," + (String)mag2 + ";" ;
          linear2_str = (String)linear2.x() + "," + (String)linear2.y() + "," + (String)linear2.z() + ";" ;
          gyro2_str = (String)gyro2.x() + "," + (String)gyro2.y() + "," + (String)gyro2.z() + ";" ;
          passive_array[count] = quat1_str;
          count++;
          passive_array[count] = calib1_str;
          count++;
          passive_array[count] = linear1_str;
          count++;
          passive_array[count] = gyro1_str;
          count++;
          passive_array[count] = quat2_str;
          count++;
          passive_array[count] = calib2_str;
          count++;
          passive_array[count] = linear2_str;
          count++;
          passive_array[count] = gyro2_str;
          count++;
          if(count > 400) {
            for(i = 0; i < 400; i = i + 1) {
              os << passive_array[i].c_str();
              passive_c->setValue(os.str());
              passive_c->notify();
            }
            count = 0;
          }
          
          break;
        case 1:  
          
          
          //Convert vectors into strings
          quat1_str = (String)quat1.w() + "," + (String)quat1.x() + "," + (String)quat1.y() + "," + (String)quat1.z() + ";" ;
          calib1_str = (String)sys + "," + (String)gyro_cal + "," + (String)accel + "," + (String)mag + ";" ;
          linear1_str = (String)linear1.x() + "," + (String)linear1.y() + "," + (String)linear1.z() + ";" ;
          gyro1_str = (String)gyro1.x() + "," + (String)gyro1.y() + "," + (String)gyro1.z() + ";" ;
          quat2_str = (String)quat2.w() + "," + (String)quat2.x() + "," + (String)quat2.y() + "," + (String)quat2.z() + ";" ;
          calib2_str = (String)sys2 + "," + (String)gyro_cal2 + "," + (String)accel2 + "," + (String)mag2 + ";" ;
          linear2_str = (String)linear2.x() + "," + (String)linear2.y() + "," + (String)linear2.z() + ";" ;
          gyro2_str = (String)gyro2.x() + "," + (String)gyro2.y() + "," + (String)gyro2.z() + ";" ;

          //Update values of characteristics
          os << quat1_str.c_str();
          quat1_c->setValue(os.str());
          quat1_c->notify();

          os << calib1_str.c_str();
          calib1_c->setValue(os.str());
          calib1_c->notify();

          os << linear1_str.c_str();
          linear1_c->setValue(os.str());
          linear1_c->notify();

          os << gyro1_str.c_str();
          gyro1_c->setValue(os.str());
          gyro1_c->notify();
          
          os << quat2_str.c_str();
          quat2_c->setValue(os.str());
          quat2_c->notify();

          os << calib2_str.c_str();
          calib2_c->setValue(os.str());
          calib2_c->notify();

          os << linear2_str.c_str();
          linear2_c->setValue(os.str());
          linear2_c->notify();

          os << gyro2_str.c_str();
          gyro2_c->setValue(os.str());
          gyro2_c->notify();
          break;
      }
        delay(BNO055_SAMPLERATE_DELAY_MS); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
