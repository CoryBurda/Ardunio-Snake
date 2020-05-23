#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <QueueArray.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define XPin A2
#define YPin A3

#define REDPIN 6
#define GREENPIN 9
#define BLUEPIN 11
#define LED_ON LOW
#define LED_OFF HIGH
int RandPixel[2];
int StateChange=0;
int Qlen=1;

//class Snake{
//  public:
//    Snake();
//}


//QueueArray <Snake> queue;

bool OutofBounds(int Xrand, int Yrand){
  float result = (Xrand-64)*(Xrand-64)/4096.+(Yrand-32)*(Yrand-32)/1024.;
    if(result>1.0){
    return true;
  }
  return false;
}

int *DrawRandPixel(){
  int Xrand = random(128);
  int Yrand = random(64);
  while(OutofBounds(Xrand,Yrand)){
    Xrand = random(128);
    Yrand = random(64);
  }
  display.drawPixel(Xrand,Yrand,WHITE);
  RandPixel[0]=Xrand;
  RandPixel[1]=Yrand;
  return RandPixel;
}


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  pinMode(XPin,INPUT);
  pinMode(YPin,INPUT);

  //Serial.begin(9600);                     //<------------- its this line

  pinMode(REDPIN,OUTPUT);
  pinMode(GREENPIN,OUTPUT);
  pinMode(BLUEPIN,OUTPUT);
  digitalWrite(REDPIN,LED_OFF); /* Turn off LED */
  digitalWrite(GREENPIN,LED_OFF); /* Turn off LED */
  digitalWrite(BLUEPIN,LED_OFF); /* Turn off LED */
  display.println("Collect the dots!");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.setCursor(0,0);
  
  //Red On
  analogWrite(REDPIN,0);
  analogWrite(GREENPIN,255);
  analogWrite(BLUEPIN,255);
  display.println("Ready");
  display.display();
  delay(1000);
  //Yellow On
  analogWrite(REDPIN,0);
  analogWrite(GREENPIN,0);
  analogWrite(BLUEPIN,255);
  display.setTextSize(2);
  display.println("Set");
  display.display();
  delay(1000);  
  //Green On
  analogWrite(REDPIN,255);
  analogWrite(GREENPIN,0);
  analogWrite(BLUEPIN,255);
  display.setTextSize(3);
  display.println("Go!");
  display.display();
  delay(1000);
  //turn off LED
  analogWrite(REDPIN,255);
  analogWrite(GREENPIN,255);
  analogWrite(BLUEPIN,255); 
  display.clearDisplay();
  display.display();
}

void loop() {
  int Xval;
  int Yval;
  int XLED;
  int YLED;
  int Xrand;
  int Yrand;
  

  Xval=analogRead(XPin);
  Yval=analogRead(YPin);
  XLED=64./512.*(1024-Yval);
  YLED=32./512.*(1024-Xval);

  //Random Pixel
  if(!StateChange){
    RandPixel[2]=DrawRandPixel();
    Xrand = RandPixel[0];
    Yrand = RandPixel[1];
    StateChange = 1;
  }

  //Joystick touches random pixel
  if(XLED == Xrand && YLED == Yrand){
    StateChange = 0;
    Qlen++;
  }

  display.clearDisplay();
  display.drawPixel(Xrand,Yrand,WHITE);
  display.drawPixel(XLED,YLED,WHITE);
  
  display.display();
}
