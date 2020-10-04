#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* This driver reads raw data from the BNO055

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno1 = Adafruit_BNO055(1, 0x28);
Adafruit_BNO055 bno2 = Adafruit_BNO055(2, 0x29);



/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(115200);
  Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno1.begin() || !bno2.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  /* Display the current temperature */
  int8_t temp1 = bno1.getTemp();
  int8_t temp2 = bno2.getTemp();
  Serial.print("Current Temperature: ");
  Serial.print(temp1);
  Serial.println(" C");
  Serial.println("");

  bno1.setExtCrystalUse(true);
  bno2.setExtCrystalUse(true); 

  Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2
  imu::Vector<3> euler1 = bno1.getVector(Adafruit_BNO055::VECTOR_EULER);
  imu::Vector<3> euler2 = bno2.getVector(Adafruit_BNO055::VECTOR_EULER);

  /* Display the floating point data */
  Serial.print("BNO1:");
  Serial.print("X: ");
  Serial.print(euler1.x());
  Serial.print(" Y: ");
  Serial.print(euler1.y());
  Serial.print(" Z: ");
  Serial.print(euler1.z());
  Serial.print("\t\t");

  Serial.print("BNO2:");
  Serial.print("X: ");
  Serial.print(euler2.x());
  Serial.print(" Y: ");
  Serial.print(euler2.y());
  Serial.print(" Z: ");
  Serial.print(euler2.z());
  Serial.print("\t\t");
  

  
  // Quaternion data
  imu::Quaternion quat1 = bno1.getQuat();
  imu::Quaternion quat2 = bno2.getQuat();

  Serial.print("BNO1 "); 
  Serial.print("qW: ");
  Serial.print(quat1.w(), 4);
  Serial.print(" qX: ");
  Serial.print(quat1.x(), 4);
  Serial.print(" qY: ");
  Serial.print(quat1.y(), 4);
  Serial.print(" qZ: ");
  Serial.print(quat1.z(), 4);
  Serial.print("\t\t");

  Serial.print("BNO2 "); 
  Serial.print("qW: ");
  Serial.print(quat2.w(), 4);
  Serial.print(" qX: ");
  Serial.print(quat2.x(), 4);
  Serial.print(" qY: ");
  Serial.print(quat2.y(), 4);
  Serial.print(" qZ: ");
  Serial.print(quat2.z(), 4);
  Serial.print("\t\t");

  
  

  /* Display calibration status for each sensor. */
  uint8_t system, gyro, accel, mag = 0;
  bno1.getCalibration(&system, &gyro, &accel, &mag);
  Serial.print("CALIBRATION: Sys=");
  Serial.print(system, DEC);
  Serial.print(" Gyro=");
  Serial.print(gyro, DEC);
  Serial.print(" Accel=");
  Serial.print(accel, DEC);
  Serial.print(" Mag=");
  Serial.println(mag, DEC);

  bno2.getCalibration(&system, &gyro, &accel, &mag);
  Serial.print("CALIBRATION: Sys=");
  Serial.print(system, DEC);
  Serial.print(" Gyro=");
  Serial.print(gyro, DEC);
  Serial.print(" Accel=");
  Serial.print(accel, DEC);
  Serial.print(" Mag=");
  Serial.println(mag, DEC);

  delay(BNO055_SAMPLERATE_DELAY_MS);
}
