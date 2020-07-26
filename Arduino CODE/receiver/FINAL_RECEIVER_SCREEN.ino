#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>     // Touch library

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#define SENSIBILITY 300
#define MINPRESSURE 10
#define MAXPRESSURE 1000
//These are the pins for the shield!
#define YP A1 
#define XM A2 
#define YM 7  
#define XP 6 
// Calibrate values
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905
// Init TouchScreen:
TouchScreen ts = TouchScreen(XP, YP, XM, YM, SENSIBILITY);
RF24 radio(53, 48); // CE, CSN
const byte address[6] = "00001";
int i, j;
int piezoPin = 28;
void setup() {
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(1);
  drawBorder();
  tft.setCursor (55, 50);
  tft.setTextSize (3);
  tft.setTextColor(GREEN);
  tft.println("DESIGN");
  tft.setCursor (65, 85);
  tft.println("PROJECT"); 
  tft.setCursor (25, 150);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println(" SIGN BOARD");
  waitOneTouch(true);
  drawBorder();
  tft.setTextSize (2);
  tft.setTextColor(BLUE);
  tft.setCursor (20, 60);
  //tft.println("READY");
  tft.setCursor (20, 100);
  tft.println("SEATBELT PLEASE");
  tft.setCursor (20, 160);
  tft.println("HAVE A SAFE JOURNEY");
  // Wait touch
  waitOneTouch(true);
  drawBorder();
}

void loop() {
  digitalWrite(30, LOW);
  //digitalWrite(25, HIGH);
  //digitalWrite(24, LOW);
  while (radio.available()>0)
  {
    int i=1;
    char text[32] = "";
    radio.read(&text, sizeof(text));
    // Serial.println(text);
    // tft.reset();
    //drawBorder();
    tft.setCursor (20, 100);
    tft.setTextSize (4);
    tft.setTextColor(MAGENTA);
    tft.println(text);
    
    for (int j=0; j <= i; j++){
      tone(piezoPin, 3000, 500);
      //tone(piezoPin, 3000, 500);
      delay(500);
      noTone( piezoPin);
      delay(100);
    }
   delay(500);
  }
  
    drawBorder();
   // tft.clrScr();
   // tft.setCursor (20, 100);
   //tft.setTextSize (3);
   //tft.setTextColor(BLACK);
   //tft.println("   ");
   // tft.reset();
   delay(1000);
}

TSPoint waitOneTouch(boolean showMessage)
{
   // wait 1 touch to exit function
   uint8_t save = 0;
    if (showMessage) {
    save = tft.getRotation(); // Save it
    tft.setRotation(0); // Show in normal  
    tft.setCursor (80, 250);
    tft.setTextSize (1);
    tft.setTextColor(BLACK);
    tft.println("Touch to proceed");
    }
  // Wait a touch
   TSPoint p;
  
  do {
    p= ts.getPoint(); 
    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);
     } while((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE));

    if (showMessage) {
      tft.setRotation(save);
    }
  return p;
}

void drawBorder () {
  // Draw a border
  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 10;
  tft.fillScreen(WHITE);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE);  
}
