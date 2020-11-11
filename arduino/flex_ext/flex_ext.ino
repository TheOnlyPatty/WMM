#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#define _USE_MATH_DEFINES
#include <cmath>

/* Returns the IMU data as both a euler angles and quaternions as the WebSerial
   3D Model viewer at https://adafruit-3dmodel-viewer.glitch.me/ expects.
 
   This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.
   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.
   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).
   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3-5V DC
   Connect GROUND to common ground
   History
   =======
   2020/JUN/01  - First release (Melissa LeBlanc-Williams)
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno1 = Adafruit_BNO055(1, 0x28);
Adafruit_BNO055 bno2 = Adafruit_BNO055(2, 0x29);


/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/

struct Quaternion {
    double w, x, y, z;
};

struct EulerAngles {
    double roll, pitch, yaw;
};

EulerAngles ToEulerAngles(Quaternion q) {
    EulerAngles angles;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.roll = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (q.w * q.y - q.z * q.x);
    if (std::abs(sinp) >= 1)
        angles.pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        angles.pitch = std::asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.yaw = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}

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

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/

void setup(void)
{
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
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  sensors_event_t event1;
  sensors_event_t event2;
  bno1.getEvent(&event1);  
  imu::Quaternion quat1 = bno1.getQuat();

  bno2.getEvent(&event2);
  imu::Quaternion quat2 = bno2.getQuat();

  
  EulerAngles em3;
  Quaternion qm3; 
  
  
  
  imu::Quaternion quat3, quat4;
  quat3 = quat1.conjugate();
  quat4 = quat2*quat3;
  
  qm3.w = quat4.w();
  qm3.x = quat4.x();
  qm3.y = quat4.y();
  qm3.z = quat4.z();

  em3 = ToEulerAngles(qm3);
  
  String result = ""; 

  result = (String)quat1.w() + "," + (String)quat1.x() + "," + (String)quat1.y() + "," + (String)quat1.z() + "," + 
           (String)quat2.w() + "," + (String)quat2.x() + "," + (String)quat2.y() + "," + (String)quat2.z() + ";" ; 
  Serial.println(result);

  result = "Ulnar/Radial Deviation(quat): " + (String)((em3.yaw*)*180/M_PI) + ", Flexion/Extension: " + (String)((em3.pitch)*180/M_PI) + ";";
  Serial.println(result);

  
  delay(500);
}
