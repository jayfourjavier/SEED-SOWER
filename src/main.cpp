#include <Arduino.h>
#include <Servo.h>
#include "Sonar.h"

#define SERVO_PIN 4
#define SONAR_ECHO_PIN 8
#define SONAR_TRIG_PIN 9

Servo servoDispenser;
Sonar sonar(SONAR_TRIG_PIN, SONAR_ECHO_PIN);

#define drop_rice_seed_angle 5
#define get_rice_seed_angle 35

int currentServoAngle = 0;

#define SHAKE_AMPLITUDE 10
#define SHAKE_COUNT 5
#define SHAKE_DELAY_MS 100

/*@brief Shakes the servo around the given angle to help seeds settle/fall */
void shakeServo(int centerAngle)
{
  for (int i = 0; i < SHAKE_COUNT; i++)
  {
    servoDispenser.write(centerAngle + SHAKE_AMPLITUDE);
    delay(SHAKE_DELAY_MS);
    servoDispenser.write(max(0, centerAngle - SHAKE_AMPLITUDE));
    delay(SHAKE_DELAY_MS);
  }
  servoDispenser.write(centerAngle); // Return to the target angle
  delay(SHAKE_DELAY_MS);
}

/*@brief Opens the servo to the open angle */
void getRiceSeeds()
{
  Serial.println("Getting rice seeds...");
  shakeServo(currentServoAngle); // Shake before pulling
  for (int angle = currentServoAngle; angle <= get_rice_seed_angle; angle++)
  {
    servoDispenser.write(angle);
    delay(15); // Adjust the delay for smoother movement
  }
  currentServoAngle = get_rice_seed_angle;
}

void dropRiceSeeds()
{
  Serial.println("Dropping rice seeds...");
  shakeServo(currentServoAngle); // Shake before pushing
  for (int angle = currentServoAngle; angle >= drop_rice_seed_angle; angle--)
  {
    servoDispenser.write(angle);
    delay(15); // Adjust the delay for smoother movement
  }
  currentServoAngle = drop_rice_seed_angle;
}

void setup()
{
  Serial.begin(9600);
  sonar.begin();
  servoDispenser.attach(SERVO_PIN);
  servoDispenser.write(drop_rice_seed_angle); // Start with the servo in the closed position
  currentServoAngle = drop_rice_seed_angle;

  delay(3000); // Allow time for the servo to reach the initial position
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  delay(5000);
  getRiceSeeds();
  delay(5000);
  dropRiceSeeds();

  // Serial.print("Distance: ");
  // float distance = sonar.getMedianDistance();
  // Serial.println(distance);
  // delay(1000);
}
