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

void displaySensorDetails(void)
{
  sensor_t sensor1;
  bno1.getSensor(&sensor1);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor1.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor1.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor1.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor1.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor1.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor1.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");

  sensor_t sensor2;
  bno2.getSensor(&sensor2);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor2.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor2.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor2.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor2.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor2.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor2.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  
  delay(500);
}


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

#define IMU_UUID          "d495e683-a794-4d0e-9464-821bfa06acd7"
#define IMU1_QUAT_UUID    "70a00029-1d2a-4068-baa1-dac520c6fd4d"
#define IMU1_CALIB_UUID   "558fed32-5a97-4866-8a02-a7d7729b88b7"
#define IMU1_LINEAR_UUID  "aec118d4-ba81-4945-93ad-d77f52146584"
#define IMU1_GYRO_UUID    "ba7b779a-1fa3-4fb8-b28d-790163b4668c"
#define IMU2_QUAT_UUID    "50d7a2cd-0dda-40cc-be74-f9003f72a0bd"
#define IMU2_CALIB_UUID   "b551857a-e2cf-4688-95e3-835f71f94045"
#define IMU2_LINEAR_UUID  "2d80be96-da97-46f0-96dc-76989b6b4e2e"
#define IMU2_GYRO_UUID    "5a2ee6ad-105e-4db1-98c2-a5467c158862"
#define PASSIVE_UUID      "f3383094-3fc2-4a9e-aa3c-9a9f808b789d"


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
  
  Serial.begin(9600);

  // Create the BLE Device
  BLEDevice::init("WMM");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service for the imu's
  BLEService *imu = pServer->createService(IMU_UUID);

  // Create BLE Characteristics for quaternions, eulers, and calibration
  BLECharacteristic *quat1_c = imu->createCharacteristic(
                      IMU1_QUAT_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  BLECharacteristic *calib1_c = imu->createCharacteristic(
                      IMU1_CALIB_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  BLECharacteristic *linear1_c = imu->createCharacteristic(
                      IMU1_LINEAR_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  BLECharacteristic *gyro1_c = imu->createCharacteristic(
                      IMU1_GYRO_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  BLECharacteristic *quat2_c = imu->createCharacteristic(
                      IMU2_QUAT_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  BLECharacteristic *calib2_c = imu->createCharacteristic(
                      IMU2_CALIB_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  BLECharacteristic *linear2_c = imu->createCharacteristic(
                      IMU2_LINEAR_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  BLECharacteristic *gyro2_c = imu->createCharacteristic(
                      IMU2_GYRO_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  BLECharacteristic *passive_c = imu->createCharacteristic(
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
  gyro1_c->addDescriptor(new BLE2902());
  quat2_c->addDescriptor(new BLE2902());
  calib2_c->addDescriptor(new BLE2902());
  gyro2_c->addDescriptor(new BLE2902());
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
    sensors_event_t event1;
    bno1.getEvent(&event1);
    imu::Quaternion quat1 = bno1.getQuat();
    imu::Vector<3> linear1 = bno1.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    imu::Vector<3> gyro1 = bno1.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    
    sensors_event_t event2;
    bno2.getEvent(&event2);
    imu::Quaternion quat2 = bno2.getQuat();
    imu::Vector<3> linear2 = bno2.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
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
      //switch(device_mode) {
        //case 0:  
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
              Serial.println(passive_array[i]);
              os << passive_array[i].c_str();
              passive_c->setValue(os.str());
              passive_c->notify();
              delay(3);
            }
            count = 0;
          }
          
          /*break;
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
          Serial.println("Quaternion1: " + quat1_str);
          os << quat1_str.c_str();
          quat1_c->setValue(os.str());
          quat1_c->notify();
          
          Serial.println("Calibration1: " + calib1_str);
          os << calib1_str.c_str();
          calib1_c->setValue(os.str());
          calib1_c->notify();

          Serial.println("Linear1: " + linear1_str);
          os << linear1_str.c_str();
          linear1_c->setValue(os.str());
          linear1_c->notify();
          
          Serial.println("Gyro1: " + gyro1_str);
          os << gyro1_str.c_str();
          gyro1_c->setValue(os.str());
          gyro1_c->notify();

          Serial.println("Quaternion2: " + quat2_str);
          os << quat2_str.c_str();
          quat2_c->setValue(os.str());
          quat2_c->notify();
          
          Serial.println("Calibration2: " + calib2_str);
          os << calib2_str.c_str();
          calib2_c->setValue(os.str());
          calib2_c->notify();

          Serial.println("Linear2: " + linear2_str);
          os << linear2_str.c_str();
          linear2_c->setValue(os.str());
          linear2_c->notify();

          Serial.println("Gyro2: " + gyro2_str);
          os << gyro2_str.c_str();
          gyro2_c->setValue(os.str());
          gyro2_c->notify();
          break;
      }*/
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
