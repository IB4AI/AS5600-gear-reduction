#include <Wire.h>
#define AS5600_ADDRESS 0x36
#define RAW_ANGLE 0x0C
float lastAngle = -1.0;
float totalAngle = 0.0;

// change to whatever your motor is, ie. 24:1 -> 24
int ratio = 24;

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  // Read and process angle data
  float currentAngle = readShaftAngle();

  // Calculate the change in angle
  float deltaAngle = currentAngle - lastAngle;

  if (deltaAngle < -180) {
    deltaAngle += 360;
  } else if (deltaAngle > 180) {
    deltaAngle -= 360;
  }

    // Update the total angle
    totalAngle += deltaAngle;
    lastAngle = currentAngle;

    // Calculate the shaft angle
    float shaftAngle = totalAngle / ratio;

    // Ensure the shaft angle is within 0-360 degrees
    shaftAngle = normalizeAngle(shaftAngle);
  
    Serial.print("Shaft Angle: ");
    Serial.println(shaftAngle);
}

// Function to read shaft angle
float readShaftAngle() {
  Wire.beginTransmission(AS5600_ADDRESS);
  Wire.write(RAW_ANGLE);
  Wire.endTransmission();
  Wire.requestFrom(AS5600_ADDRESS, 2);

  if (Wire.available() == 2) {
    int highByte = Wire.read();
    int lowByte = Wire.read();
    int rawAngle = (highByte << 8) | lowByte;
    return rawAngle * 0.0879; // Convert to degrees
  }
}
