// https://github.com/ainjayla/rae2026r.git

#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>
#include <qmc5883p.h>
#include <Wire.h>

#define PRESSURE_SEA_LEVEL 1013.5

float pitchf = 0.0;
float rollf = 0.0;

Adafruit_BMP085 bmp;
Adafruit_MPU6050 mpu;
QMC5883P compass;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  delay(10);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050");
    while (true) {
      delay(10);
    }
  }
  Serial.println("MPU6050 is ready to go");
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
  Wire.beginTransmission(0x68); // bypass mode for QMC5883P
  Wire.write(0x37);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x6A);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x68); // disable sleep mode
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  if (!compass.begin()) {
    // Serial.println("Failed to find QMC5883P");
    while (true) {
      delay(10);
    }
  }
  // Serial.println("QMC5883P is ready to go");
  
  if (!bmp.begin()) {
    // Serial.println("Failed to find BMP180");
    while (true) {
      delay(10);
    }
  }
  // Serial.println("BMP180 is ready to go");
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float x = a.acceleration.x;
  float y = a.acceleration.y;
  float z = a.acceleration.z;

  float pitch = atan(x / sqrt(pow(y, 2) + pow(z, 2))) * 180 / PI;
  float roll = atan(y / sqrt(pow(x, 2) + pow(z, 2))) * 180 / PI;
  pitchf = 0.94 * pitchf + 0.06 * pitch;
  rollf = 0.94 * rollf + 0.06 * roll;

  float xyz[3];
  float heading = 0;
  if (compass.readXYZ(xyz)) {
    heading = compass.getHeadingDeg();
  }

/*
  Serial.print("Pitch: ");
  Serial.print(pitchf);
  Serial.print(", roll: ");
  Serial.print(rollf);
  Serial.print(", heading: ");
  Serial.println(heading);
*/

  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100;
  float altitude = bmp.readAltitude(PRESSURE_SEA_LEVEL * 100);

  /*
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");
  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");
  */
  // delay(250);
    Serial.print(rollf);
    Serial.print("/");
    Serial.print(pitchf);
    Serial.print("/");
    Serial.println(heading);
}
