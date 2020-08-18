// Adapted from Adafruit's Arduino Sensor Example: https://learn.adafruit.com/adafruit-feather-sense/arduino-sensor-example
// and owennewo's roll pitch yaw example: // https://github.com/arduino-libraries/Arduino_LSM6DS3/blob/5eac7f5e6145c4747da27698faf3a548d2893a2b/examples/RollPitchYaw/RollPitchYaw.ino
// Can be used with this processing sketch to visualize roll pitch yaw: https://github.com/arduino-libraries/Arduino_LSM6DS3/blob/5eac7f5e6145c4747da27698faf3a548d2893a2b/examples/RollPitchYaw/data/processing/RollPitchYaw3d.pde

//#include <Adafruit_BMP280.h>
//#include <Adafruit_LIS3MDL.h>
#include <Adafruit_LSM6DS33.h>
//#include <Adafruit_SHT31.h>
//#include <Adafruit_Sensor.h>

//#include <PDM.h>

//Adafruit_BMP280 bmp280;     // temperautre, barometric pressure
//Adafruit_LIS3MDL lis3mdl;   // magnetometer
Adafruit_LSM6DS33 lsm6ds33; // accelerometer, gyroscope
//Adafruit_SHT31 sht30;       // humidity

float temperature, pressure, altitude;
float magnetic_x, magnetic_y, magnetic_z;
float humidity;
//int32_t mic;
//
//extern PDMClass PDM;
//short sampleBuffer[256];  // buffer to read samples into, each sample is 16-bits
//volatile int samplesRead; // number of samples read

float accelX,            accelY,             accelZ,            // units m/s/s i.e. accelZ if often 9.8 (gravity)
      gyroX,             gyroY,              gyroZ,             // units dps (degrees per second)
      gyroDriftX,        gyroDriftY,         gyroDriftZ,        // units dps
      gyroRoll,          gyroPitch,          gyroYaw,           // units degrees (expect major drift)
      gyroCorrectedRoll, gyroCorrectedPitch, gyroCorrectedYaw,  // units degrees (expect minor drift)
      accRoll,           accPitch,           accYaw,            // units degrees (roll and pitch noisy, yaw not possible)
      complementaryRoll, complementaryPitch, complementaryYaw;  // units degrees (excellent roll, pitch, yaw minor drift)

long lastTime;
long lastInterval;

sensors_event_t accel;
sensors_event_t gyro;
sensors_event_t temp;

bool readIMU() {
  lsm6ds33.getEvent(&accel, &gyro, &temp);
  accelX = accel.acceleration.x;
  accelY = accel.acceleration.y;
  accelZ = accel.acceleration.z;
  gyroX = gyro.gyro.x;
  gyroY = gyro.gyro.y;
  gyroZ = gyro.gyro.z;
  return true;
}

/*
  the gyro's x,y,z values drift by a steady amount. if we measure this when arduino is still
  we can correct the drift when doing real measurements later
*/
void calibrateIMU(int delayMillis, int calibrationMillis) {
  int calibrationCount = 0;

  delay(delayMillis); // to avoid shakes after pressing reset button

  float sumX, sumY, sumZ;
  int startTime = millis();
  while (millis() < startTime + calibrationMillis) {
    if (readIMU()) {
      // in an ideal world gyroX/Y/Z == 0, anything higher or lower represents drift
      sumX += gyroX;
      sumY += gyroY;
      sumZ += gyroZ;

      calibrationCount++;
    }
  }

  if (calibrationCount == 0) {
    Serial.println("Failed to calibrate");
  }

  gyroDriftX = sumX / calibrationCount;
  gyroDriftY = sumY / calibrationCount;
  gyroDriftZ = sumZ / calibrationCount;

}

void doImuCalculations() {
  accRoll = atan2(accelY, accelZ) * 180 / M_PI;
  accPitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180 / M_PI;

  float lastFrequency = (float) 1000000.0 / lastInterval;
  gyroRoll = gyroRoll + (gyroX / lastFrequency);
  gyroPitch = gyroPitch + (gyroY / lastFrequency);
  gyroYaw = gyroYaw + (gyroZ / lastFrequency);

  gyroCorrectedRoll = gyroCorrectedRoll + ((gyroX - gyroDriftX) / lastFrequency);
  gyroCorrectedPitch = gyroCorrectedPitch + ((gyroY - gyroDriftY) / lastFrequency);
  gyroCorrectedYaw = gyroCorrectedYaw + ((gyroZ - gyroDriftZ) / lastFrequency);

  complementaryRoll = complementaryRoll + ((gyroX - gyroDriftX) / lastFrequency);
  complementaryPitch = complementaryPitch + ((gyroY - gyroDriftY) / lastFrequency);
  complementaryYaw = complementaryYaw + ((gyroZ - gyroDriftZ) / lastFrequency);

  complementaryRoll = 0.98 * complementaryRoll + 0.02 * accRoll;
  complementaryPitch = 0.98 * complementaryPitch + 0.02 * accPitch;
}

/**
   This comma separated format is best 'viewed' using 'serial plotter' or processing.org client (see https://github.com/arduino-libraries/Arduino_LSM6DS3/blob/5eac7f5e6145c4747da27698faf3a548d2893a2b/examples/RollPitchYaw/data/processing/RollPitchYaw3d.pde)
*/
void printCalculations() {
  Serial.print(gyroRoll);
  Serial.print(',');
  Serial.print(gyroPitch);
  Serial.print(',');
  Serial.print(gyroYaw);
  Serial.print(',');
  Serial.print(gyroCorrectedRoll);
  Serial.print(',');
  Serial.print(gyroCorrectedPitch);
  Serial.print(',');
  Serial.print(gyroCorrectedYaw);
  Serial.print(',');
  Serial.print(accRoll);
  Serial.print(',');
  Serial.print(accPitch);
  Serial.print(',');
  Serial.print(accYaw);
  Serial.print(',');
  Serial.print(complementaryRoll);
  Serial.print(',');
  Serial.print(complementaryPitch);
  Serial.print(',');
  Serial.print(complementaryYaw);
  Serial.println("");
}
