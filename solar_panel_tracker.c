#include <Servo.h>

Servo servoX, servoY;
const int LDR1 = A0, LDR2 = A1, LDR3 = A2, LDR4 = A3;
unsigned int rRDL1 = 0, rRDL2 = 0, rRDL3 = 0, rRDL4 = 0;
unsigned int max1 = 0, max2 = 0, max3 = 0;
int servoXPos = 0, servoYPos = 60;
unsigned int prevMax3 = 0;
bool sunMovementDetected = false;

void setup() {
  servoX.attach(8);
  servoY.attach(9);
  Serial.begin(9600);
  servoX.write(servoXPos);
  servoY.write(servoYPos);
}

void loop() {
  // Read the light sensor values
  rRDL1 = analogRead(LDR1);
  rRDL2 = analogRead(LDR2);
  rRDL3 = analogRead(LDR3);
  rRDL4 = analogRead(LDR4);

  // Calculate the maximum light readings
  max1 = max(rRDL1, rRDL2);
  max2 = max(rRDL3, rRDL4);
  max3 = max(max1, max2);

  Serial.println(max3);
  Serial.println(String(rRDL1) + ", " + String(rRDL2) + ", " + String(rRDL3) + ", " + String(rRDL4));

  // Control for servoX (X-axis)
  if (rRDL1 == max3 || rRDL2 == max3) {
    if (servoXPos < 140) servoXPos += 1;
  } else if (servoXPos > 0) {
    servoXPos -= 1;
  }
  servoX.write(servoXPos);

  // Control for servoY (Y-axis)
  if (rRDL3 == max3 || rRDL4 == max3) {
    if (servoYPos < 120) servoYPos += 1;
  } else if (servoYPos > 0) {
    servoYPos -= 1;
  }
  servoY.write(servoYPos);

  // Check for sunlight movement
  if (max3 != prevMax3) {
    sunMovementDetected = true;
  } else {
    sunMovementDetected = false;
  }
  prevMax3 = max3;

  // Slow down servo movement if sunlight movement detected
  if (sunMovementDetected) {
    delay(500); // Adjust this delay for slower movement
  } else {
    delay(100);
  }
}
