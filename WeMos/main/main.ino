#include <Servo.h>
Servo myServo;

const int ServoNum = 1;
const int ServoPIN[] = {D4};

void setup() {
  myServo.attach(ServoPIN[0]);
}

void loop() {

  for (int angle = 0; angle <= 180; angle += 5) {
    myServo.write(angle);
    delay(20);
  }

  delay(1000);

  for (int angle = 180; angle >= 0; angle -= 5) {
    myServo.write(angle);
    delay(20);
  }

  delay(1000);
}
