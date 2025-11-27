#include <Streaming.h>
#include <Wire.h>
#include <TM1638plus.h>       // 7SEG LED&KEY
#include <Adafruit_GFX.h>     //  OLED 
#include <Adafruit_SSD1306.h> //  OLED 
#include <Servo.h>            //  Servo 
#include <DS3231.h>           //  RTC 

Servo myservo;  // create servo object to control a servo

// OLED i2c
#define OLED_RESET -1
#define OLED_SCREEN_I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// 7SEG display 
#define STROBE_TM D5 // strobe = GPIO connected to strobe line of module
#define CLOCK_TM D6 // clock = GPIO connected to clock line of module
#define DIO_TM D7 // data = GPIO connected to data line of module
bool high_freq = false; //default false, If using a high freq CPU > ~100 MHZ set to true.

// constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);

// Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, high_freq);

// DS3231 uses I2C bus - device ID 0x68
DS3231 rtc;
bool century = false;
bool h12Flag;
bool pmFlag;


//           __                               
//          |  \                              
//  _______ | $$  ______    _______   _______ 
// /       \| $$ |      \  /       \ /       \
//|  $$$$$$$| $$  \$$$$$$\|  $$$$$$$|  $$$$$$$
//| $$      | $$ /      $$ \$$    \  \$$    \ 
//| $$_____ | $$|  $$$$$$$ _\$$$$$$\ _\$$$$$$\
// \$$     \| $$ \$$    $$|       $$|       $$
//  \$$$$$$$ \$$  \$$$$$$$ \$$$$$$$  \$$$$$$$ 
//                        
                                 
class Clock
{

  bool paused = false;
  int power = 0;
  // TM1638 BUTTON
  int buttons = tm.readButtons();
  //potentiomometer
  int sensorValue = analogRead(A0);
  
  public:

    int getButtons()
    {
      return buttons;
    }

    int getSensorValue()
    {
      return sensorValue;
    }


    void setDateAndTime()
    {
      rtc.setClockMode(false); // false = 24hr clock mode
      rtc.setYear(25);
      rtc.setMonth(11);
      rtc.setDate(25);
      rtc.setHour(20);
      rtc.setMinute(59);
      rtc.setSecond(55);
    }

    // funtion used to check if it is the first minute of the hour, then calls function to display the hour hand
    void hourlyCheck(bool paused)
    {
        if (paused == false)
      {
        // check if it is the first minute of the hour
        if (rtc.getMinute()== 0x0)
        {
          int twelveHour = rtc.getHour(h12Flag, pmFlag);
          
          // if it is past or equal to the 12th hour, minus twelve so the switch statement works for 24hours
          if (twelveHour >= 12)
          {
            twelveHour -= 12;
            // call the function to display hour hand
            displayHour(twelveHour);
          }
        }
      }
    }

    // function to display the hour hand
    void displayHour(int input)
    {
      // clear display
        display.clearDisplay();
        display.setCursor(0,0);
        // draw circle
        // display.drawCircle(centerX, centerY, radius, color)
        display.drawCircle(60, 30, 30, WHITE);

        // switch case to display the correct hour hand
        switch(input)
        {
          case 0:display.drawLine(60,30,60,5, WHITE); break;  // 12  o'clock
          case 1:display.drawLine(60,30,75,9,WHITE); break;   // 1  o'clock
          case 2:display.drawLine(60,30,83,17,WHITE); break;  // 2  o'clock
          case 3:display.drawLine(60,30,85,30,WHITE); break;  // 3  o'clock
          case 4:display.drawLine(60,30,83,43,WHITE); break;  // 4  o'clock
          case 5:display.drawLine(60,30,75,51,WHITE); break;  // 5  o'clock
          case 6:display.drawLine(60,30,60,55,WHITE); break;  // 6  o'clock
          case 7:display.drawLine(60,30,45,51,WHITE); break;  // 7  o'clock
          case 8:display.drawLine(60,30,37,43,WHITE); break;  // 8  o'clock
          case 9:display.drawLine(60,30,35,30,WHITE); break;  // 9  o'clock
          case 10:display.drawLine(60,30,37,17,WHITE); break; // 10 o'clock
          case 11:display.drawLine(60,30,45,9,WHITE); break;  // 11 o'clock
        }

        // serial communication
        Serial << "It's "  << input << " o' clock!" << endl;
        display.display();
    }

    // Power Reserve Function
    void powerReserve(int power)
    {
      

    }

    void displayOutput(bool paused,int sensorValue,int pos)
    {
      // everything the display outputs normally
      display << rtc.getDate() << "/" << rtc.getMonth(century) << "/" << rtc.getYear() << endl;
      display << rtc.getHour(h12Flag, pmFlag) << ":" << rtc.getMinute() << ":" << rtc.getSecond() << endl;
      display << getSensorValue() << endl;
      display << pos << endl;
      hourlyCheck(paused);
    }

    void serialOutput(int pos, byte buttons)
    {
      // some serial stuff 
      Serial << rtc.getDate() << "/" << rtc.getMonth(century) << "/" << rtc.getYear() << " " ;
      Serial << rtc.getHour(h12Flag, pmFlag) << ":" << rtc.getMinute() << ":" << rtc.getSecond() << endl;
      Serial << "Servo Pos: " << pos << endl;
      Serial << "Button pressed: " << buttons << endl;

    }



    void buttonPressCheck(bool& paused)
    {
      buttons = tm.readButtons();
      if (buttons == 1)
      {
        

        pauseTime(paused);
      } 
    }

    void pauseTime(bool& paused)
    {
      // effectively pauses the time
      rtc.setSecond(rtc.getSecond());
      rtc.setMinute(rtc.getMinute());
      rtc.setHour(rtc.getHour(h12Flag, pmFlag));
      rtc.setDate(rtc.getDate());
      rtc.setMonth(rtc.getMonth(century));
      rtc.setYear(rtc.getYear());
      paused = true;
    }

    void run7SegDisplay()
    {
      //display thingy
      tm.reset();
      tm.displayIntNum(rtc.getSecond(), false);
      // LED LED LED
      tm.setLEDs(getButtons());
    }


    void runServo()
    {

      //Servo
      int pos = getSensorValue()/6;
      myservo.write(pos);


    }
};

//                           /$$                        
//                          | $$                        
//      /$$$$$$$  /$$$$$$  /$$$$$$   /$$   /$$  /$$$$$$ 
//     /$$_____/ /$$__  $$|_  $$_/  | $$  | $$ /$$__  $$
//    |  $$$$$$ | $$$$$$$$  | $$    | $$  | $$| $$  \ $$
//     \____  $$| $$_____/  | $$ /$$| $$  | $$| $$  | $$
//     /$$$$$$$/|  $$$$$$$  |  $$$$/|  $$$$$$/| $$$$$$$/
//    |_______/  \_______/   \___/   \______/ | $$____/ 
//                                            | $$      
//                                            | $$      
//                                            |__/      

void setup()
{

  Clock clock;
  
  // OLED
  Serial.begin(115200);
  Serial << endl << "Hello OLED World" << endl;

  // -- OLED --------------
  display.begin(SSD1306_SWITCHCAPVCC, OLED_SCREEN_I2C_ADDRESS);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2); // - a line is 21 chars in this size
  display.setTextColor(WHITE);

  //TM1638
  tm.displayBegin();  

  //RTC
  Wire.begin();
  Serial << (F("\nDS3231 Hi Precision Real Time Clock")) << endl;

  // You should comment this out after you've successfully set the RTC // You should comment this out after you've successfully set the RTC
  clock.setDateAndTime(); // Only need to do this once ever.

  //Servo
  myservo.attach(D5, 500, 2400);  // attaches the servo on GIO2 to the servo object

}

//     /$$                                    
//    | $$                                    
//    | $$        /$$$$$$   /$$$$$$   /$$$$$$ 
//    | $$       /$$__  $$ /$$__  $$ /$$__  $$
//    | $$      | $$  \ $$| $$  \ $$| $$  \ $$
//    | $$      | $$  | $$| $$  | $$| $$  | $$
//    | $$$$$$$$|  $$$$$$/|  $$$$$$/| $$$$$$$/
//    |________/ \______/  \______/ | $$____/ 
//                                  | $$      
//                                  | $$      
//                                  |__/ (main one)     

void loop()
{
  display.clearDisplay();
  display.setCursor(0,0);



  clock.displayOutput(paused, sensorValue, pos);
  clock.serialOutput(pos,buttons);
  

  display.display();
  delay(200); // do nothing

}



