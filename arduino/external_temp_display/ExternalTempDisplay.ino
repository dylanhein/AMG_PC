/*********************************************************************
Written by Dylan Hein for the AMG PC.
*********************************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

/* DHT11 temperature and humidity sensor
 */
#define dht_dpin A0
byte bGlobalErr;
byte dht_dat[5];

void setup()   {
  InitDHT(); //Initialize DHT11 Temp Sensor
  
  Serial.begin(9600);
  delay(300);
  
  Serial.println("Humidity and temperature\n\n");
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  //display.display(); // show splashscreen
  //delay(2000);
  display.clearDisplay();   // clears the screen and buffer
}


void loop() {
  // draw mercedes logo
  drawMercedesBitmap();
  delay(5000);
  display.clearDisplay();
  
  // draw 1986 logo
  draw1986Bitmap();
  delay(5000);
  display.clearDisplay();

  // draw 560sec logo
  drawSecBitmap();
  delay(5000);
  display.clearDisplay();

  // draw old amg logo
  drawOldAmgBitmap();
  delay(5000);
  display.clearDisplay();
  
  // draw amg logo
  drawAmgBitmap();
  delay(5000);
  display.clearDisplay();

  // get temp and humidity reading
  ReadDHT();
  
  // draw temp and humidity display
  drawTempDisplay(dht_dat[0], convertToFahrenheit(dht_dat[2]));
  delay(5000);
  display.clearDisplay();
}

int convertToFahrenheit(int celsius) {
  return celsius * 1.8 + 32;
}

void InitDHT(){
  pinMode(dht_dpin,OUTPUT);
  digitalWrite(dht_dpin,HIGH);
}

void ReadDHT(){
  bGlobalErr=0;
  byte dht_in;
  byte i;
  digitalWrite(dht_dpin,LOW);
  delay(20);
  
  digitalWrite(dht_dpin,HIGH);
  delayMicroseconds(40);
  pinMode(dht_dpin,INPUT);
  //delayMicroseconds(40);
  dht_in=digitalRead(dht_dpin);
  
  if (dht_in){
    bGlobalErr=1;
    return;
  }
  
  delayMicroseconds(80);
  dht_in=digitalRead(dht_dpin);
  
  if (!dht_in){
    bGlobalErr=2;
    return;
  }
  delayMicroseconds(80);
  for (i=0; i<5; i++) {
     dht_dat[i] = read_dht_dat();
  }
  
  pinMode(dht_dpin,OUTPUT);
  digitalWrite(dht_dpin,HIGH);
  byte dht_check_sum = dht_dat[0]+dht_dat[1]+dht_dat[2]+dht_dat[3];
  
  if (dht_dat[4]!= dht_check_sum) {
     bGlobalErr=3;
  }
}

byte read_dht_dat(){
  byte i = 0;
  byte result=0;
  for(i=0; i< 8; i++){
      while(digitalRead(dht_dpin)==LOW);
      delayMicroseconds(30);
      if (digitalRead(dht_dpin)==HIGH)
     result |=(1<<(7-i));
    while (digitalRead(dht_dpin)==HIGH);
    }
  return result;
}

void drawAmgBitmap() {
  uint8_t w = 128;
  uint8_t h = 12;
  static unsigned char PROGMEM const amglogo_128x12_bmp[] = {
    0x00, 0x0F, 0xFC, 0x78, 0xF1, 0xC6, 0x1F, 0xFF, 0xF8, 0xF0, 0x00, 0x07, 0x8F, 0xFF, 0xFF, 0xF0,
    0x00, 0x1F, 0xFC, 0xF9, 0xF3, 0x8E, 0x3F, 0xFF, 0xFC, 0xFC, 0x00, 0x0F, 0xCF, 0xFF, 0xFF, 0xF0,
    0x00, 0x3F, 0xF9, 0xF3, 0xE7, 0x9C, 0x7F, 0xFF, 0xFC, 0xFF, 0x00, 0x3F, 0xCF, 0xFF, 0xFF, 0xF0,
    0x00, 0x3F, 0xF3, 0xF3, 0xCF, 0x18, 0xF8, 0x00, 0x7C, 0xFF, 0x80, 0xFF, 0xCF, 0x80, 0x00, 0x00,
    0x00, 0x7F, 0xF7, 0xE7, 0x8E, 0x39, 0xF8, 0x00, 0x7C, 0xFF, 0xE1, 0xFF, 0xCF, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xEF, 0xCF, 0x1C, 0x71, 0xF0, 0x00, 0x7C, 0xF3, 0xF7, 0xF3, 0xCF, 0x00, 0xFF, 0xF0,
    0x01, 0xFF, 0xCF, 0x9F, 0x3C, 0xE3, 0xFF, 0xFF, 0xFC, 0xF0, 0xFF, 0xC3, 0xCF, 0x00, 0xFF, 0xF0,
    0x01, 0xFF, 0x9F, 0xBE, 0x78, 0xC7, 0xFF, 0xFF, 0xFC, 0xF0, 0x7F, 0x03, 0xCF, 0x00, 0x00, 0xF0,
    0x03, 0xFF, 0xBF, 0x3C, 0x71, 0xCF, 0xFF, 0xFF, 0xFC, 0xF0, 0x1E, 0x03, 0xCF, 0x80, 0x00, 0xF0,
    0x07, 0xFF, 0x7E, 0x78, 0xE3, 0x8F, 0x80, 0x00, 0x7C, 0xF0, 0x00, 0x03, 0xCF, 0xFF, 0xFF, 0xF0,
    0x0F, 0xFE, 0x7C, 0xF9, 0xC7, 0x1F, 0x00, 0x00, 0x7C, 0xF0, 0x00, 0x03, 0xCF, 0xFF, 0xFF, 0xF0,
    0x0F, 0xFC, 0xF8, 0xF1, 0x86, 0x3E, 0x00, 0x00, 0x38, 0xF0, 0x00, 0x03, 0x8F, 0xFF, 0xFF, 0xF0
  };

  display.drawBitmap(0, 26, amglogo_128x12_bmp, w, h, WHITE);
  display.display();
}

void drawOldAmgBitmap() {
  uint8_t w = 80;
  uint8_t h = 16;
  static unsigned char PROGMEM const oldamglogo_80x16_bmp[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFC, 0x01, 0xF0, 0x03,
    0xE1, 0xFF, 0xFF, 0xE0, 0x00, 0x7F, 0xFE, 0x01, 0xF8, 0x07, 0xE1, 0xFF, 0xFF, 0xE0, 0x00, 0xFF,
    0xFF, 0x01, 0xFE, 0x1F, 0xE1, 0xFF, 0xFF, 0xE0, 0x00, 0xE0, 0x07, 0x01, 0xDF, 0x3E, 0xE1, 0xC0,
    0x00, 0x00, 0x01, 0xC0, 0x03, 0x81, 0xCF, 0xFC, 0xE1, 0xC1, 0xFF, 0xE0, 0x01, 0xFF, 0xFF, 0x81,
    0xC3, 0xF0, 0xE1, 0xC1, 0xFF, 0xE0, 0x03, 0xFF, 0xFF, 0xC1, 0xC1, 0xE0, 0xE1, 0xC0, 0x00, 0xE0,
    0x03, 0xFF, 0xFF, 0xC1, 0xC0, 0x00, 0xE1, 0xFF, 0xFF, 0xE0, 0x07, 0x80, 0x01, 0xE1, 0xC0, 0x00,
    0xE1, 0xFF, 0xFF, 0xE0, 0x07, 0x80, 0x01, 0xE1, 0xC0, 0x00, 0xE1, 0xFF, 0xFF, 0xE0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFC, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x3F, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  display.drawBitmap(24, 25, oldamglogo_80x16_bmp, w, h, WHITE);
  display.display();
}

void drawMercedesBitmap() {
  uint8_t w = 48;
  uint8_t h = 48;
  static unsigned char PROGMEM const mercedeslog_64x64_bmp[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x0F, 0xE1, 0x87, 0xF0, 0x00, 0x00, 0x3C,
    0x01, 0x80, 0x3C, 0x00, 0x00, 0x3C, 0x01, 0x80, 0x3C, 0x00, 0x00, 0xF0, 0x01, 0xC0, 0x0F, 0x00,
    0x01, 0xE0, 0x03, 0xC0, 0x07, 0x80, 0x01, 0xC0, 0x03, 0xC0, 0x03, 0x80, 0x07, 0x80, 0x03, 0xC0,
    0x01, 0xE0, 0x07, 0x00, 0x03, 0xC0, 0x00, 0xE0, 0x0E, 0x00, 0x03, 0xC0, 0x00, 0x70, 0x0E, 0x00,
    0x03, 0xC0, 0x00, 0x70, 0x18, 0x00, 0x03, 0xC0, 0x00, 0x18, 0x18, 0x00, 0x03, 0xC0, 0x00, 0x18,
    0x38, 0x00, 0x03, 0xC0, 0x00, 0x1C, 0x38, 0x00, 0x03, 0xC0, 0x00, 0x1C, 0x38, 0x00, 0x03, 0xC0,
    0x00, 0x1C, 0x70, 0x00, 0x07, 0xE0, 0x00, 0x0E, 0x70, 0x00, 0x07, 0xE0, 0x00, 0x0E, 0x70, 0x00,
    0x07, 0xE0, 0x00, 0x0E, 0x70, 0x00, 0x0F, 0xF0, 0x00, 0x0E, 0x60, 0x00, 0x1F, 0xF8, 0x00, 0x06,
    0x60, 0x00, 0x3F, 0xFC, 0x00, 0x06, 0x70, 0x00, 0xFF, 0xFF, 0x00, 0x0E, 0x70, 0x01, 0xFF, 0xFF,
    0x80, 0x0E, 0x70, 0x03, 0xFF, 0xFF, 0xC0, 0x0E, 0x70, 0x0F, 0xFE, 0x7F, 0xF0, 0x0E, 0x38, 0x1F,
    0xF0, 0x0F, 0xF8, 0x1C, 0x38, 0x3F, 0x80, 0x01, 0xFC, 0x1C, 0x38, 0xFF, 0x00, 0x00, 0xFF, 0x1C,
    0x19, 0xF0, 0x00, 0x00, 0x0F, 0x98, 0x19, 0xC0, 0x00, 0x00, 0x03, 0x98, 0x0F, 0x00, 0x00, 0x00,
    0x00, 0xF0, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x07, 0x80,
    0x00, 0x00, 0x01, 0xE0, 0x01, 0xC0, 0x00, 0x00, 0x03, 0x80, 0x01, 0xE0, 0x00, 0x00, 0x07, 0x80,
    0x00, 0xF0, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x3E, 0x00, 0x00,
    0x7C, 0x00, 0x00, 0x0F, 0xE0, 0x07, 0xF0, 0x00, 0x00, 0x03, 0xFE, 0x7F, 0xC0, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(26,0);
  display.print("Mercedes-Benz");
  display.drawBitmap(40, 16, mercedeslog_64x64_bmp, w, h, WHITE);
  display.display();
}

void drawSecBitmap() {
  uint8_t w = 128;
  uint8_t h = 16;
  static unsigned char PROGMEM const seclogo_128x16_bmp[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1F, 0xFF, 0xE0, 0xFF, 0xFC, 0x0F, 0xFF, 0xE0, 0x03, 0xFF, 0xF0, 0x3F, 0xFF, 0x03, 0xFF, 0xF0,
    0x1F, 0xFF, 0xE1, 0xFF, 0xFE, 0x1F, 0xFF, 0xF0, 0x07, 0xFF, 0xF8, 0x7F, 0xFF, 0x87, 0xFF, 0xF8,
    0x1F, 0x00, 0x01, 0xF0, 0x1E, 0x1F, 0x83, 0xF0, 0x07, 0xC0, 0x78, 0x7C, 0x07, 0x87, 0xE0, 0x78,
    0x1F, 0x00, 0x01, 0xF0, 0x00, 0x1F, 0x01, 0xF0, 0x07, 0xF8, 0x00, 0x7C, 0x00, 0x07, 0xC0, 0x00,
    0x1F, 0xFF, 0xC1, 0xFF, 0xFC, 0x1F, 0x01, 0xF0, 0x03, 0xFF, 0x80, 0x7F, 0xF8, 0x07, 0xC0, 0x00,
    0x1F, 0xFF, 0xE1, 0xFF, 0xFE, 0x1F, 0x01, 0xF0, 0x00, 0x7F, 0xF0, 0x7F, 0xF8, 0x07, 0xC0, 0x00,
    0x00, 0x03, 0xE1, 0xF0, 0x3E, 0x1F, 0x01, 0xF0, 0x00, 0x07, 0xF8, 0x7C, 0x00, 0x07, 0xC0, 0x00,
    0x1E, 0x03, 0xE1, 0xF0, 0x3E, 0x1F, 0x83, 0xF0, 0x07, 0x80, 0xF8, 0x7C, 0x07, 0x87, 0xE0, 0x78,
    0x1F, 0xFF, 0xE1, 0xFF, 0xFE, 0x1F, 0xFF, 0xF0, 0x07, 0xFF, 0xF8, 0x7F, 0xFF, 0x87, 0xFF, 0xF8,
    0x0F, 0xFF, 0xC0, 0xFF, 0xFC, 0x0F, 0xFF, 0xE0, 0x03, 0xFF, 0xF0, 0x3F, 0xFF, 0x03, 0xFF, 0xF0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  display.drawBitmap(0, 24, seclogo_128x16_bmp, w, h, WHITE);
  display.display();
}

void draw1986Bitmap() {
  uint8_t w = 80;
  uint8_t h = 16;
  static unsigned char PROGMEM const logo1986_80x16_bmp[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x3F, 0xFF, 0x03, 0xFF,
    0xF0, 0x3F, 0xFF, 0x00, 0x01, 0xF8, 0x7F, 0xFF, 0x87, 0xFF, 0xF8, 0x7F, 0xFF, 0x80, 0x00, 0xF8,
    0x7C, 0x0F, 0x87, 0xC0, 0xF8, 0x7C, 0x07, 0x80, 0x00, 0xF8, 0x7C, 0x0F, 0x87, 0xC0, 0xF8, 0x7C,
    0x00, 0x00, 0x00, 0xF8, 0x7F, 0xFF, 0x87, 0xFF, 0xF8, 0x7F, 0xFF, 0x00, 0x00, 0xF8, 0x3F, 0xFF,
    0x87, 0xFF, 0xF8, 0x7F, 0xFF, 0x80, 0x00, 0xF8, 0x00, 0x0F, 0x87, 0xC0, 0xF8, 0x7C, 0x0F, 0x80,
    0x00, 0xF8, 0x78, 0x0F, 0x87, 0xC0, 0xF8, 0x7C, 0x0F, 0x80, 0x01, 0xFC, 0x7F, 0xFF, 0x87, 0xFF,
    0xF8, 0x7F, 0xFF, 0x80, 0x01, 0xFC, 0x3F, 0xFF, 0x03, 0xFF, 0xF0, 0x3F, 0xFF, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  display.drawBitmap(24, 25, logo1986_80x16_bmp, w, h, WHITE);
  display.display();
}

void drawTempDisplay(int humidity, int temp) {
  int tempXcoord = 20;
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.print("Ext. Humidity:");
  display.setTextSize(2);
  display.setCursor(92, 0);
  display.print(humidity);
  display.print("%");

  display.setTextSize(1);
  display.setCursor(0,tempXcoord);
  display.print("Ext.");
  display.setCursor(0,tempXcoord + 9);
  display.print("Temp:");
  
  display.setTextSize(6);
  display.setCursor(34,tempXcoord);
  display.print(temp);
  
  display.setTextSize(1);
  display.setCursor(108,tempXcoord - 2);
  display.print("o");
  
  display.setTextSize(2);
  display.setCursor(116, tempXcoord);
  display.print("F");
  
  display.display();
}
