#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>
#include <qmc5883p.h>
#include <Wire.h>

#define PRESSURE_SEA_LEVEL 1013.5

Adafruit_BMP085 bmp;
Adafruit_MPU6050 mpu;
QMC5883P compass;

void setup() {
  Serial.begin(9600);
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
    Serial.println("Failed to find QMC5883P");
    while (true) {
      delay(10);
    }
  }
  Serial.println("QMC5883P is ready to go");
  
  if (!bmp.begin()) {
    Serial.println("Failed to find BMP180");
    while (true) {
      delay(10);
    }
  }
  Serial.println("BMP180 is ready to go");
  delay(100);
}

void loop() {
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100;
  float altitude = bmp.readAltitude(PRESSURE_SEA_LEVEL * 100);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");
  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");
  delay(1000);
}
