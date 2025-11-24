

#include <Streaming.h>
// -- OLED -------
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// OLED i2c
#define OLED_RESET -1
#define OLED_SCREEN_I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);
void setup()
{
Serial.begin(115200);
Serial << endl << "Hello World" << endl;
// -- OLED --------------
display.begin(SSD1306_SWITCHCAPVCC, OLED_SCREEN_I2C_ADDRESS);
display.display();
delay(2000);
display.clearDisplay();
display.setCursor(0,0);
display.setTextSize(1); // - a line is 21 chars in this size
display.setTextColor(WHITE);
}


void loop()
{
display.clearDisplay();
display.setCursor(0,0);
//clock

//display.drawCircle(centerX, centerY, radius, color)
display.drawCircle(60, 30, 30, WHITE);

//display.drawLine(startX, startY, endX, endY, color)
display.drawLine(60,30,60,5, WHITE); // 12 o'clock
display.drawLine(60,30,75,9,WHITE); // 1 o'clock
display.drawLine(60,30,83,17,WHITE); // 2 o'clock
display.drawLine(60,30,85,30,WHITE); // 3 o'clock
//display.drawLine(60,30,77,37,WHITE); // 4 o'clock
//display.drawLine(60,30,67,47,WHITE); // 5 o'clock
display.drawLine(60,30,60,55,WHITE); // 6 o'clock
//display.drawLine(60,30,55,47,WHITE);// 7 o'clock
//display.drawLine(60,30,45,37,WHITE);// 8 o'clock
display.drawLine(60,30,35,30,WHITE); // 9 o'clock
//display.drawLine(60,30,25,27,WHITE); // 10 o'clock
//display.drawLine(60,30,15,17,WHITE); // 11 o'clock



display.display();
delay(300);
display.clearDisplay();
display.display();

}