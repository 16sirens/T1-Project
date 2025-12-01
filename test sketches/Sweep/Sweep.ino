#include <Streaming.h>
#include <Servo.h>

// -- OLED -------
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// OLED i2c
#define OLED_RESET -1
#define OLED_SCREEN_I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards


void setup() {
  myservo.attach(D5, 500, 2400);  // attaches the servo on GIO2 to the servo object

  Serial.begin(115200);
  Serial << endl << "Hello World" << endl;
  
  // -- OLED --------------
  display.begin(SSD1306_SWITCHCAPVCC, OLED_SCREEN_I2C_ADDRESS);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(3); // - a line is 21 chars in this size
  display.setTextColor(WHITE);


}

void loop() {


  int sensorValue = analogRead(A0);

  int pos = 180;

myservo.write(sensorValue/6);
  ///for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
  ///  myservo.write(pos);                  // tell servo to go to position in variable 'pos'
  ///  delay(500);                           // waits 15ms for the servo to reach the position
  ///}
  ///delay(200);
  ///myservo.write(180);
  ///delay(400);
display.clearDisplay();
display.setCursor(0,0);

display << sensorValue << endl;
display << sensorValue/6 << endl;

display.display();

delay(100);

}
