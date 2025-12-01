// 
//  Author: Sawlayh
//  First Year Ethical Hacking Student
//  2503483@uad.ac.uk sawlayhh@gmail.com
//  CMP 101/ CMP 104 Project
//  Copyright (c) Sawlayh
// 



//  File history, and by extention my thought process and problem solving can be viewed on github:
//  https://github.com/16sirens/T1-Project/blob/main/ughhh/ughhh.ino
//  Please note my project randomly decided to delete itself a few times, no idea why!
//  For refrences please see below:
//
//  Line 330: https://www.instructables.com/Fun-With-OLED-Display-and-Arduino/
//
//  Line 439: https://stackoverflow.com/questions/4261589/how-do-i-split-an-int-into-its-digits
//  Also watched this for some direction but it was not much help: https://www.youtube.com/watch?v=m6VfrWVUtEk       



/////////////////////////////////////// this code was given in CMP 101
#include <Streaming.h>
#include <Wire.h>
#include <TM1638plus.h>       // 7SEG LED&KEY
#include <Adafruit_GFX.h>     //  OLED 
#include <Adafruit_SSD1306.h> //  OLED 
#include <Servo.h>            //  Servo 
#include <DS3231.h>           //  RTC 
#include <string.h>
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

///////////////////////////////////////  this code was given in CMP 101


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
  // boolean for pausing the clock
  bool paused;
  // power for the clock
  int power;
  // TM1638 BUTTON
  int buttons;
  // potentiomometer
  int sensorValue;
  // pos
  int pos;

  // time and date variables
  int sec;
  int min;
  int hour;
  int date;
  int month;
  int year;


  public:

    Clock();
    ~Clock();

    // setters
    void setPaused(bool input)
    {
      paused = input;
    }

    void setPower(int input)
    {
      power = input;
    }

    void setButtons()
    {
      buttons = tm.readButtons();
    }

    void setSensorValue()
    {
      sensorValue = analogRead(A0);
    }

    void setPos()
    {
      pos = getPower();
    }
  
    void setMinute(int* value)
    {
      // increments or decrements minutes
      // makes sure minute doesnt overflow
      if (min <= 58)
      {
        if (buttons == 33)
        {
          rtc.setMinute(*value += 1);
        }
      }
      if (min >= 1)
      {
        if (buttons == 65)
        {
          rtc.setMinute(*value -= 1);
        }
      }
    }
    
    void setHour(int* value)
    {
      // increments or decrements hour
      // makes sure hour doesnt overflow
      if (hour <= 22)
      {
        if (buttons == 34)
        {
          rtc.setHour(*value += 1);
        }
      }
      if (hour >= 1)
      {
        if (buttons == 66)
        {
          rtc.setHour(*value -= 1);
        }
      }
    }
     
    void setDate(int* value)
    {
      // increments or decrements date
      // makes sure date doesnt overflow
      if (date <= 30)
      {
        if (buttons == 36)
        {
          rtc.setDate(*value += 1);
        }
      }
      if (date >= 2)
      {
        if (buttons == 68)
        {
          rtc.setDate(*value-= 1);
        }
      }
    }
     
    void setMonth(int* value)
    {
      // increments or decrements month
      // makes sure month doesnt overflow
      if (month <= 11)
      {
        if (buttons == 40)
        {
          rtc.setMonth(*value += 1);
        }
      }
      if (month >= 2)
      {
        if (buttons == 72)
        {
          rtc.setMonth(*value -= 1);
        }
      }
    }
     
    void setYear(int* value)
    {
      // increments or decrements year
      // makes sure year doesnt overflow
      if (year <= 98)
      {
        if (buttons == 48)
        {
          rtc.setYear(*value += 1);
        }
      }
      if (year >= 1)
      {
        if (buttons == 80)
        {
          rtc.setYear(*value -= 1);
        }
      }
    }

    void setInitialTime()
    {
      // sets the variables to the current time
      sec = rtc.getSecond();
      min = rtc.getMinute();
      hour = rtc.getHour(h12Flag, pmFlag);
      date = rtc.getDate();
      month = rtc.getMonth(century);
      year = rtc.getYear();
    }

    void setDateAndTime()
    {
      // set date and time ONCE
      rtc.setClockMode(false); // false = 24hr clock mode
      rtc.setSecond(0);
      rtc.setMinute(0);
      rtc.setHour(0);
      rtc.setDate(1);
      rtc.setMonth(12);
      rtc.setYear(25);
    }
    
    void ifPaused() 
    {
      // effectively pauses the time
      if (paused == true)
      {
        rtc.setSecond(rtc.getSecond());
        rtc.setMinute(rtc.getMinute());
        rtc.setHour(rtc.getHour(h12Flag, pmFlag));
        rtc.setDate(rtc.getDate());
        rtc.setMonth(rtc.getMonth(century));
        rtc.setYear(rtc.getYear());
      }
      else if (paused == false)
      {
        // continues to update time variables
        setInitialTime();
      }
    }

    // getters
    // The getters in this program are not nessecary for functionality however,
    // they are here for the added modularity.
    // They have been used in some places as to not hurt their feelings
    // and give them some purpose.

    bool getPaused()
    {
      return paused;
    }

    int getPower()
    {
      return power;
    }

    int getButtons()
    {
      return buttons;
    }

    int getSensorValue()
    {
      return sensorValue;
    }
    
    int getPos()
    {
      return pos;
    }

    // funtion used to check if it is the first minute of the hour, then calls function to display the hour hand
    void hourlyCheck()
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
          }
          // call the function to display hour hand
          displayHour(twelveHour);
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
          // Read this for some direction on how to fulfil the "display graphics" requirement:
          // https://www.instructables.com/Fun-With-OLED-Display-and-Arduino/
          // The co-ordinates were all done by me.

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
    void powerReserve()
    {
      // code for power reserve complication
      if (power >= 1 && getPaused() == false)
      {
        setPaused(false);
        setPower((power-1));
        // brightens 7seg
        tm.brightness(7);
      }
      else if (power == 0)
      {
        setPaused(true);
        // dims 7seg
        tm.brightness(0);
      }

    }

    void displayOutput()
    {
      // everything the display outputs normally
      display << rtc.getDate() << "/" << rtc.getMonth(century) << "/" << rtc.getYear() << endl;
      display << rtc.getHour(h12Flag, pmFlag) << ":" << rtc.getMinute() << ":" << rtc.getSecond() << endl;
      display << "Power:" << power << endl;

      hourlyCheck();
    }

    void serialOutput()
    {
      // some serial stuff 
      Serial << rtc.getDate() << "/" << rtc.getMonth(century) << "/" << rtc.getYear() << " " ;
      Serial << rtc.getHour(h12Flag, pmFlag) << ":" << rtc.getMinute() << ":" << rtc.getSecond() << endl;
      Serial << "SensorVal: " << sensorValue << endl;
      Serial << "Servo Pos: " << pos << endl;
      Serial << "Button pressed: " << buttons << endl;
      Serial << "Paused status: " << getPaused() << endl;

    }

    void buttonPressCheck()
    {
      // Specific IF loop makes it so random buttons pressed do not trigger the function.
      if (buttons <= 80 && buttons >= 1)
      {
        // Depending on the combination of buttons pressed, the value is incremented or decremented. 
        // This part also uses POINTERS!!!
        setPaused(true);
        if (buttons == 33 || buttons == 65)
        {
          setMinute(&min);
        }
        else if (buttons == 34 || buttons == 66)
        {
          setHour(&hour);
        }
        else if (buttons == 36 || buttons == 68)
        {
          setDate(&date);
        }
        else if (buttons == 40 || buttons == 72)
        {
          setMonth(&month);
        }
        else if (buttons == 48 || buttons == 80)
        {
          setYear(&year);
        }
     
      }
      else if (buttons >= 128)
      {
        // If the last button is pressed, the clock resumes function.
        setPaused(false);
      }
    }

    void run7SegDisplay()
    {
      // Function used to display the 7seg Display
      tm.reset();
      int h1,h2,m1,m2,s1,s2;
      int hours = rtc.getHour(h12Flag, pmFlag);
      int mins = rtc.getMinute();
      int seconds = rtc.getSecond();
      
      // disects each digit into a variable
      h1 = hours /10;    // https://stackoverflow.com/questions/4261589/how-do-i-split-an-int-into-its-digits
      h2 = hours % 10;
      m1 = mins / 10;
      m2 = mins % 10;
      s1 = seconds /10;
      s2 = seconds % 10;

      // I literally tried everything from text,ascii,7seg and intnum display but this works so yay
      // displays each variable in the appropriate location
      tm.displayHex(0,h1);
      tm.displayHex(1,h2);
      tm.displayHex(3,m1);
      tm.displayHex(4,m2);
      tm.displayHex(6,s1);
      tm.displayHex(7,s2);

      // LED LED LED
      tm.setLEDs(buttons);
    }

    void runServo()
    {
      //Servo
      myservo.write(pos);
    }

    void addPower()
    {
      // checks power level and adds power back
      if (power == 0)
      {
        display << "NO POWER!";
        
        if (sensorValue >= 1020)
        { 
          setPower(50);
          setPaused(false);
        }
      }
    }
};

Clock::Clock() {};
Clock::~Clock() {};


Clock MainClock;


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
  /////////////////////////////////////// this code was given in CMP 101 ///////////////////////////////////////
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
  
  // setDateAndTime setDateAndTime setDateAndTime setDateAndTime setDateAndTime setDateAndTime 
  //MainClock.setDateAndTime(); //only needed once per ever

  //Servo
  myservo.attach(D5, 500, 2400);  // attaches the servo on GIO2 to the servo object
  /////////////////////////////////////// this code was given in CMP 101 ///////////////////////////////////////

  MainClock.setInitialTime();
  MainClock.setPaused(false);
  MainClock.setPower(50);
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
  // clears display
  display.clearDisplay();
  display.setCursor(0,0);

  // calls functions for program to magically run
  MainClock.ifPaused();
  
  MainClock.setButtons();
  MainClock.setSensorValue();
  MainClock.setPos();

  MainClock.powerReserve();
  MainClock.buttonPressCheck();

  MainClock.displayOutput();
  MainClock.serialOutput();

  MainClock.run7SegDisplay();
  MainClock.runServo();
  MainClock.addPower();
  
  // displays display
  display.display();

  delay(200); // do nothing

}