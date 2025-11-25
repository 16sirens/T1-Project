
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards


void setup() {
  myservo.attach(D5, 500, 2400);  // attaches the servo on GIO2 to the servo object
}

void loop() {
  int pos = 180;

  for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
   // delay(0);                           // waits 15ms for the servo to reach the position
  }
  delay(200);
  myservo.write(180);
  delay(400);


}
