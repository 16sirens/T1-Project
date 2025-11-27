/////////////////////////////////////// this 
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

/////////////////////////////////////// 


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
  
  bool paused;
  int power;
  // TM1638 BUTTON
  int buttons;
  // potentiomometer
  int sensorValue;
  // pos
  int pos;

  public:

    Clock();
    ~Clock();

    // if it breaks, remove deconstructor thingy
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
      pos = sensorValue/6;
    }
    
    void setMinute()
    {
      // increments or decrements minute
      int minute = rtc.getMinute();
      if (buttons == 32)
      {
        rtc.setMinute(rtc.getMinute() + 1);
      }
      else if (buttons == 64)
      {
        rtc.setMinute(minute -= 1);
      }
    }
     
    void setHour()
    {
      // increments or decrements hour
      int hour =rtc.getHour(h12Flag, pmFlag);
      if (buttons == 32)
      {
        rtc.setHour(hour += 1);
      }
      else if (buttons == 64)
      {
        rtc.setHour(hour -= 1);
      }
    }
     
    void setDate()
    {
      // increments or decrements date
      int day = rtc.getDate();
      if (buttons == 32)
      {
        rtc.setDate(day += 1);
      }
      else if (buttons == 64)
      {
        rtc.setDate(day-= 1);
      }
    }
     
    void setMonth()
    {
      // increments or decrements month
      int month = rtc.getMonth(century);
      if (buttons == 32)
      {
        rtc.setMonth(month += 1);
      }
      else if (buttons == 64)
      {
        rtc.setMonth(month -= 1);
      }
    }
     
    void setYear()
    {
      // increments or decrements year
      int year = rtc.getYear();
      if (buttons == 32)
      {
        rtc.setYear(year += 1);
      }
      else if (buttons == 64)
      {
        rtc.setYear(year -= 1);
      }
    }

    void setDateAndTime()
    {
      // set date and time ONCE
      rtc.setClockMode(false); // false = 24hr clock mode
      rtc.setSecond(0);
      rtc.setMinute(0);
      rtc.setHour(0);
      rtc.setDate(1);
      rtc.setMonth(1);
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
    }

    // getters
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
    void powerReserve()
    {
      // code for power reserve complication
      if (power >= 1)
      {
        setPaused(false);
        setPower((power-1));

      }
      else if (power = 0)
      {
        setPaused(true);
      }

    }

    void displayOutput()
    {
      // everything the display outputs normally
      display << rtc.getDate() << "/" << rtc.getMonth(century) << "/" << rtc.getYear() << endl;
      display << rtc.getHour(h12Flag, pmFlag) << ":" << rtc.getMinute() << ":" << rtc.getSecond() << endl;
      display << sensorValue << endl;
      display << pos << " P: " << power << endl;

      hourlyCheck();
    }

    void serialOutput()
    {
      // some serial stuff 
      Serial << rtc.getDate() << "/" << rtc.getMonth(century) << "/" << rtc.getYear() << " " ;
      Serial << rtc.getHour(h12Flag, pmFlag) << ":" << rtc.getMinute() << ":" << rtc.getSecond() << endl;
      Serial << "Servo Pos: " << pos << endl;
      Serial << "Button pressed: " << buttons << endl;

    }

    void buttonPressCheck()
    {
      // buttons = tm.readButtons();
      if (buttons <= 16 && buttons >= 1)
      {
        setPaused(true);
        switch(buttons)
        {
          case 1:setMinute(); break;
          case 2:setHour(); break;
          case 4:setDate(); break;
          case 8:setMonth(); break;
          case 16:setYear(); break;
        }
      }
      else if (buttons >= 128)
      {
        setPaused(false);
      }
    }



    void run7SegDisplay()
    {
      //display thingy
      tm.reset();
      int h1,h2,m1,m2,s1,s2;
      int hours = rtc.getHour(h12Flag, pmFlag);
      int mins = rtc.getMinute();
      int seconds = rtc.getSecond();

      h1 = hours /10;    // https://stackoverflow.com/questions/4261589/how-do-i-split-an-int-into-its-digits
      h2 = hours % 10;
      m1 = mins / 10;
      m2 = mins % 10;
      s1 = seconds /10;
      s2 = seconds % 10;

      //i literally tried everything from text,ascii,7seg and intnum display but this works so yay
      tm.displayHex(0,h1);
      tm.displayHex(1,h2);
      tm.displayHex(3,m1);
      tm.displayHex(4,m2);
      tm.displayHex(6,s1);
      tm.displayHex(7,s2);

      // LED LED LED
      tm.setLEDs(getButtons());
    }


    void runServo()
    {
      //Servo
      myservo.write(getPos());
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
  MainClock.setDateAndTime(); // Only need to do this once ever.

  //Servo
  myservo.attach(D5, 500, 2400);  // attaches the servo on GIO2 to the servo object

  MainClock.setPaused(false);
  MainClock.setPower(100);
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

  
  Serial << MainClock.getPaused() << endl;
  display.display();

  delay(200); // do nothing

}



