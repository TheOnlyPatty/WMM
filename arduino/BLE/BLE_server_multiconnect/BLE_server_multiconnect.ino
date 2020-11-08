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
#define BNO055_SAMPLERATE_DELAY_MS (100)

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
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


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

  Serial.begin(9600);


  /* Initialise the sensor */
  if(!bno1.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  if(!bno2.begin())
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
  displaySensorDetails();

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value
    
    if (deviceConnected) {
      
        sensors_event_t event1;
        sensors_event_t event2;
        bno1.getEvent(&event1);  
        imu::Quaternion quat1 = bno1.getQuat();

        bno2.getEvent(&event2);
        imu::Quaternion quat2 = bno2.getQuat();

        String result = ""; 

        result = (String)quat1.w() + "," + (String)quat1.x() + "," + (String)quat1.y() + "," + (String)quat1.z() + "," + 
           (String)quat2.w() + "," + (String)quat2.x() + "," + (String)quat2.y() + "," + (String)quat2.z() + ";" ;
        Serial.println(result);
        std::ostringstream os;
        os << result.c_str();
        
        pCharacteristic->setValue(os.str());
        pCharacteristic->notify();
        value++;
        delay(33); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
        
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
