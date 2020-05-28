#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RGBLED.h"
#include "Point.h"
#include "ellipse.h"
#include "Rectangle.h"

// #define ARDUINO
#include <QueueArray.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define XPin A2
#define YPin A3
#define SCREENWIDTH 128
#define SCREENHEIGHT 64

Rectangle<int> g_PlayField(0, SCREENWIDTH - 1, 8, SCREENHEIGHT - 1);

Point<float> g_JoyStickOrigin;

ellipse g_ellipse(Point<int>(SCREENWIDTH/2, SCREENHEIGHT/2), Point<int>(SCREENWIDTH/2, SCREENHEIGHT/2));

Point<int> MakeRandPixel();
void DrawSnake();
void MoveSnake(const Point<int>& point);

Point<int> g_RandPixel;
Point<float> g_LastPixelPos(SCREENWIDTH/2, SCREENHEIGHT/2);

QueueArray <Point<int>> g_Snake(52);

Point<int> MakeRandPixel(){
  Point<int> RandPixel(random(g_PlayField.left(), g_PlayField.right()), random(g_PlayField.top(), g_PlayField.bottom()));
//  while(!g_ellipse.IsInside(RandPixel)){
//  while(!g_PlayField.IsInside(RandPixel))
//    RandPixel.Set(random(SCREENWIDTH),random(8 + SCREENHEIGHT));
//  }
  return RandPixel;
}

#define REDPIN 6
#define GREENPIN 9
#define BLUEPIN 11
RGBLED g_myLED(REDPIN, GREENPIN, BLUEPIN, COMMON_ANNODE);

void setup() {
//  Serial.begin(115200);                     //<------------- its this line
  
  randomSeed(analogRead(A1));
  g_RandPixel= MakeRandPixel();
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  pinMode(XPin,INPUT);
  pinMode(YPin,INPUT);

  g_JoyStickOrigin.Set(analogRead(YPin),analogRead(XPin));
  
  display.println("Collect the dots!");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.setCursor(0,0);
  
  g_myLED.SetColor(255,0,0); //Red On
  display.println("Ready");
  display.display();
  delay(1000);
  
  g_myLED.SetColor(255,255,0); //Yellow On
  display.setTextSize(2);
  display.println("Set");
  display.display();
  delay(1000);  
  
  g_myLED.SetColor(0,255,0);//Green On
  display.setTextSize(3);
  display.println("Go!");
  display.display();
  delay(1000);
  
  g_myLED.SetColor(0,0,0);//turn off LED
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  Point<int> startPixelPos(round(g_LastPixelPos.GetX()), round(g_LastPixelPos.GetY()));
  g_Snake.push(startPixelPos);
}

#define MAX_SPEED 1
Point<float> g_JoyStickScalar(512/MAX_SPEED, 512/MAX_SPEED);

void ShowSnake()
{
//  display.println(g_Snake.Size());
//  display.println(g_Snake.count());
  display.setCursor(0,32);
  for(unsigned int i=0; i<g_Snake.count(); i++){
    const Point<int>& point = g_Snake.peeki(i);
    display.print(point.GetX());
    display.print("/");
    display.print(point.GetY());
    display.print(" ");
  }
}

void loop() {
  Point<float> JoyStickVal(analogRead(YPin), analogRead(XPin));
  JoyStickVal*=Point<float> (-1,-1);
  JoyStickVal+=g_JoyStickOrigin;
  Point<float> CurrentSpeed = JoyStickVal/g_JoyStickScalar;
  Point<float> NextPixelPos = g_LastPixelPos + CurrentSpeed;
  
  NextPixelPos.Constrain(Point<float>(g_PlayField.left(), g_PlayField.top()), Point<float>(g_PlayField.right(), g_PlayField.bottom()));
  
  //Joystick touches random pixel
  Point<int> NextPixelPosInt(round(NextPixelPos.GetX()), round(NextPixelPos.GetY()));

  Point<int> tail_pixel(g_Snake.peeki(g_Snake.count() - 1));

  display.clearDisplay();
  
  MoveSnake(NextPixelPosInt);
  
  if(NextPixelPosInt == g_RandPixel){
    g_RandPixel = MakeRandPixel();  
    g_Snake.push(tail_pixel);
  }

  display.drawPixel(g_RandPixel.GetX(), g_RandPixel.GetY(), WHITE);

  DrawSnake();

  g_LastPixelPos = NextPixelPos;

  ShowFPS();
  ShowSnakeLength();
  ShowSnake();
  
  if(IsGameOver()){
    ShowSnake();
    display.setCursor(0, 32);
    display.setTextSize(1);
    //display.println("Game Over!");
    //display.print("Press Reset!");
    display.display();
    while(true){
      
    }
  }
  
  display.display();
}

void DrawSnake()
{
  for(unsigned int i = 0; i < g_Snake.count(); i++)
  {
    const Point<int>& pixel = g_Snake.peeki(i);
    display.drawPixel(pixel.GetX(), pixel.GetY(), WHITE);
  }
}

void ShowSnakeLength()
{
  display.setCursor(115, 0);
  display.print(g_Snake.count());
}

void ShowFPS()
{
  static unsigned long last_time = micros();
  unsigned long now = micros();
  unsigned long duration = now - last_time;

  float fps = 1000000.0 /duration;

  display.setCursor(0, 0);
  display.print(fps);

  last_time = now;
}

void MoveSnake(const Point<int>& point)
{
  const Point<int>& tail_pixel = g_Snake.peeki(g_Snake.count() - 1);
 
  if(tail_pixel == point)
     return;
 
  g_Snake.push(point);
  g_Snake.pop();
}

bool IsGameOver(){
  if(g_Snake.count()<2){
    return false;
  }
  //const Point<int>& tail_pixel = g_Snake.peeki(g_Snake.count()-1);
  const Point<int>& head = g_Snake.peeki(0);
  for(unsigned int i=1; i <=g_Snake.count()-2 ; i++){
    const Point<int>& pixel = g_Snake.peeki(i);
    if(pixel == head){
      display.setCursor(0, 50);
      display.print(i);
      return true;
    }
  }
  return false;
}
