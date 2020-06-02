#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RGBLED.h"
#include "Point.h"
#include "Rectangle.h"
#include <QueueArray.h>
#include "joystick.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define XPin A2
#define YPin A3
#define SCREENWIDTH 128
#define SCREENHEIGHT 64
#define REDPIN 6
#define GREENPIN 9
#define BLUEPIN 11
#define MAX_SPEED 3                                      //<---------------- its this line
#define MAX_ACCEL 2

//Function declarations
Point<int> MakeRandPixel();
void DrawSnake();
void ShowFPS();
void ShowPoint(const Point<int>& point);
void ShowSnake();
void MoveSnakeOnePt(const Point<int>& point, bool bHit);
void MoveSnake(const Point<int>& point, bool bHit);
bool IsGameOver();
Point<float> NextPosUsingPos();
Point<float> NextPosUsingSpeed();
Point<float>NextPosUsingAccel();

//Global Variables
RGBLED g_myLED(REDPIN, GREENPIN, BLUEPIN, COMMON_ANNODE);
Point<float> g_JoyStickScalar(512/MAX_SPEED, 512/MAX_SPEED);
Rectangle<int> g_PlayField(0, SCREENWIDTH - 1, 8, SCREENHEIGHT - 1);
Point<float> g_JoyStickOrigin;
Point<int> g_RandPixel;
Point<float> g_LastPixelPos(SCREENWIDTH/2, SCREENHEIGHT/2);
QueueArray <Point<int>> g_Snake(60);//used to be 75
Joystick<float> g_Joystick(YPin, XPin);

void setup() {
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
  g_myLED.SetColor(255,0,0); //Red On
  delay(200);
  g_myLED.SetColor(255,255,0); //Yellow On
  delay(200);
  g_myLED.SetColor(0,255,0);//Green On
  delay(200);
  g_myLED.SetColor(0,0,0);//turn off LED

  Point<int> startPixelPos(round(g_LastPixelPos.GetX()), round(g_LastPixelPos.GetY()));
  g_Snake.push(startPixelPos);
  for(int i = 1; i < 20; i++)
    g_Snake.push(startPixelPos + Point<int>(-i,0));
}

void loop() {
  display.clearDisplay();
  //Point<float> NextPixelPos = NextPosUsingAccel();
  Point<float> NextPixelPos = NextPosUsingSpeed();  
  NextPixelPos.Constrain(Point<float>(g_PlayField.left(), g_PlayField.top()), Point<float>(g_PlayField.right(), g_PlayField.bottom()));
  Point<int> NextPixelPosInt(round(NextPixelPos.GetX()), round(NextPixelPos.GetY()));
  Point<int> tail_pixel(g_Snake.peeki(0)); // the tail of the snake is the head of the queue
  
  
  bool bHit = (NextPixelPosInt == g_RandPixel);
  
  MoveSnake(NextPixelPosInt, bHit);
  // now NextPixelPosInt is the head of the snake and the tail of the queue
  // and the tail_pixel (that used to be the tail of the queue) is no longer the part of the queue

  if(bHit)
    g_RandPixel = MakeRandPixel();  

  display.drawPixel(g_RandPixel.GetX(), g_RandPixel.GetY(), WHITE);
  DrawSnake();

  g_LastPixelPos = NextPixelPos;

  ShowFPS();
  ShowSnakeLength();
  
  if(IsGameOver()){
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("Score: ");
    display.print(g_Snake.count());
    display.setCursor(0, 32);
    display.setTextSize(1);
    display.println("Game Over!\nReset to continue");
    display.display();
    while(true); // hang
  }
  display.display();
}

Point<int> MakeRandPixel(){
  Point<int> RandPixel(random(g_PlayField.left(), g_PlayField.right()), random(g_PlayField.top(), g_PlayField.bottom()));
  return RandPixel;
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
  display.setCursor(70, 0);
  display.print("Score: ");
  display.print(g_Snake.count());
}

void ShowFPS()
{
  static unsigned long last_time = micros();
  unsigned long now = micros();
  unsigned long duration = now - last_time;

  float fps = 1000000.0 /duration;

  display.setCursor(0, 0);
  display.print("FPS: ");
  display.print(fps);

  last_time = now;
}

void ShowPoint(const Point<int>& point)
{
    display.print(point.GetX());
    display.print("/");
    display.print(point.GetY());
    display.print(" ");
}

void ShowSnake()
{
//  display.println(g_Snake.Size());
//  display.println(g_Snake.count());
  //display.setCursor(0,0);
  for(unsigned int i = 0; i < g_Snake.count(); i++){
    const Point<int>& point = g_Snake.peeki(i);
    ShowPoint(point);
  }
}

void MoveSnakeOnePt(const Point<int>& point, bool bHit)
{
  //tail_pixel is tail of the queue but head of the snake
  const Point<int>& tail_pixel = g_Snake.peeki(g_Snake.count() - 1);
 
  if(tail_pixel == point)
     return;
 
  g_Snake.push(point);
  if(!bHit)
    g_Snake.pop();
}

void MoveSnake(const Point<int>& point, bool bHit)
{
  //tail_pixel is tail of the queue but head of the snake
  const Point<int>& tail_pixel = g_Snake.peeki(g_Snake.count() - 1);
  const Point<int> pdiff = point - tail_pixel;
  unsigned int nSteps = max(abs(pdiff.GetX()), abs(pdiff.GetY()));

  Point<float> tpf(tail_pixel.GetX(), tail_pixel.GetY());
  Point<float> pf(point.GetX(), point.GetY());
  
  Point<float> diff(pdiff.GetX(), pdiff.GetY());
  float dist = diff.GetDist();
  float fstep = dist / nSteps;

  for(unsigned int i = 0; i < nSteps; i++)
  {
    float d = (i + 1) * fstep;
    Point<float> pMid = tpf.GetaMidPoint(pf, d);
    Point<int> pMidInt(round(pMid.GetX()), round(pMid.GetY()));
    bool bLast = i == nSteps - 1;
    MoveSnakeOnePt(pMidInt, bLast ? bHit : false);
  }
}

bool IsGameOver(){
  if(g_Snake.count()<2){
    return false;
  }
  const Point<int>& head = g_Snake.peeki(0);
  for(unsigned int i=1; i <=g_Snake.count()-1; i++){
    const Point<int>& pixel = g_Snake.peeki(i);
    if(pixel == head){
      return true;
    }
  }
  return false;
}

Point<float> NextPosUsingPos(){
  Point<float> NextPixelPos = g_Joystick.JoystickUnitCircleOrigin();
  NextPixelPos *= Point<float> (SCREENWIDTH/2, SCREENHEIGHT/2);
  NextPixelPos += Point<float> (SCREENWIDTH/2, SCREENHEIGHT/2);
  return NextPixelPos;
}

Point<float> NextPosUsingSpeed(){
  Point<float> JoyStickVal = g_Joystick.JoystickRead();
  JoyStickVal*=Point<float> (-1,-1);
  JoyStickVal+=g_JoyStickOrigin;
  Point<float> CurrentSpeed = JoyStickVal/g_JoyStickScalar;
  Point<float> NextPixelPos = g_LastPixelPos + CurrentSpeed;
  return NextPixelPos;
}

Point<float>NextPosUsingAccel(){
  Point<float> JoyStickVal = g_Joystick.JoystickRead();
  JoyStickVal*=Point<float> (-1,-1);
  JoyStickVal+=g_JoyStickOrigin;
  Point<float> CurrentAccel = JoyStickVal/g_JoyStickScalar;
  Point<float> CurrentSpeed = CurrentSpeed + CurrentAccel;
  Point<float> NextPixelPos = g_LastPixelPos + CurrentSpeed;
  return NextPixelPos;
}
