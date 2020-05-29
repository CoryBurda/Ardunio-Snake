#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RGBLED.h"
#include "Point.h"
#include "Rectangle.h"

#include <QueueArray.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define XPin A2
#define YPin A3
#define SCREENWIDTH 128
#define SCREENHEIGHT 64

Rectangle<int> g_PlayField(0, SCREENWIDTH - 1, 8, SCREENHEIGHT - 1);

Point<float> g_JoyStickOrigin;

Point<int> MakeRandPixel();
void DrawSnake();
void MoveSnake(const Point<int>& point, bool bHit);
bool IsGameOver();
void bresenham(const Point<int>& p1, const Point<int>& p2);

Point<int> g_RandPixel;
Point<float> g_LastPixelPos(SCREENWIDTH/2, SCREENHEIGHT/2);

QueueArray <Point<int>> g_Snake(50);//used to be 75

Point<int> MakeRandPixel(){
  Point<int> RandPixel(random(g_PlayField.left(), g_PlayField.right()), random(g_PlayField.top(), g_PlayField.bottom()));
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
  g_myLED.SetColor(255,0,0); //Red On
  delay(200);
  
  g_myLED.SetColor(255,255,0); //Yellow On
  delay(200);  
  
  g_myLED.SetColor(0,255,0);//Green On
  delay(200);
  
  g_myLED.SetColor(0,0,0);//turn off LED
  Point<int> startPixelPos(round(g_LastPixelPos.GetX()), round(g_LastPixelPos.GetY()));
  //g_Snake.push(startPixelPos);
  //g_Snake.push(startPixelPos+Point<int>(10,0));
  Point<float> p1(10,10);
  Point<float> p2(10,17);
  Point<float> p3 = p1.GetaMidPoint(p2,2.0);
  display.print(p3.GetX());
  display.print("/");
  display.println(p3.GetY());
  display.display();
  return ;
//  for(int i=0;i<10;i++){
//    g_Snake.push(startPixelPos+Point<int>(1,0));
//  }
  
  //Point<int> tail_pixel(g_Snake.peeki(0));
  //ShowSnake();
  display.println(" ");
  //bresenham(tail_pixel,Point<int>(80,50));
  DrawSnake();
  display.setCursor(0,8);
  //ShowSnake();
  display.display();
}

#define MAX_SPEED 3
Point<float> g_JoyStickScalar(512/MAX_SPEED, 512/MAX_SPEED);

void ShowSnake()
{
//  display.println(g_Snake.Size());
//  display.println(g_Snake.count());
  //display.setCursor(0,0);
  for(unsigned int i = 0; i < g_Snake.count(); i++){
    const Point<int>& point = g_Snake.peeki(i);
    display.print(point.GetX());
    display.print("/");
    display.print(point.GetY());
    display.print(" ");
  }
}

void loop() {
  return ;
  Point<float> JoyStickVal(analogRead(YPin), analogRead(XPin));
  JoyStickVal*=Point<float> (-1,-1);
  JoyStickVal+=g_JoyStickOrigin;
  Point<float> CurrentSpeed = JoyStickVal/g_JoyStickScalar;
  Point<float> NextPixelPos = g_LastPixelPos + CurrentSpeed;
  NextPixelPos.Constrain(Point<float>(g_PlayField.left(), g_PlayField.top()), Point<float>(g_PlayField.right(), g_PlayField.bottom()));
  Point<int> NextPixelPosInt(round(NextPixelPos.GetX()), round(NextPixelPos.GetY()));
  Point<int> tail_pixel(g_Snake.peeki(0)); // the tail of the snake is the head of the queue
  display.clearDisplay();
  bool bHit = (NextPixelPosInt == g_RandPixel);
  
  //MoveSnake(NextPixelPosInt, bHit);
  // now NextPixelPosInt is the head of the snake and the tail of the queue
  // and the tail_pixel (that used to be the tail of the queue) is no longer the part of the queue

  bresenham(tail_pixel, NextPixelPosInt);
  
  if(bHit)
    g_RandPixel = MakeRandPixel();  

  display.drawPixel(g_RandPixel.GetX(), g_RandPixel.GetY(), WHITE);

  DrawSnake();

  g_LastPixelPos = NextPixelPos;

//  ShowFPS();
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
//    display.print("Press Reset!");
    display.display();
    while(true); // hang
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

void MoveSnake(const Point<int>& point, bool bHit)
{
  //tail_pixel is tail of the queue but head of the snake
  const Point<int>& tail_pixel = g_Snake.peeki(g_Snake.count() - 1);
 
  if(tail_pixel == point)
     return;
 
  g_Snake.push(point);
  if(!bHit)
    g_Snake.pop();
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

//void CalcLine(Point<int> p1, Point<int> p2, QueueArray <Point<int>>& output){
//  
//}

//// function for line generation 
//void bresenham(const Point<int>& p1, const Point<int>& p2) 
//{ 
//   Point<int> delta = p1.GetManhattanDist(p2);
//   int twoDy = 2 * delta.GetY(); 
//   int p = twoDy - delta.GetX(); 
//   int twoDx = 2 * delta.GetX();
//   int xinc = delta.GetX()
//   
//   for (int x = p1.GetX(), y = p1.GetY(); x <= p2.GetX(); x++) 
//   {     
//      // Slope error reached limit, time to 
//      // increment y and update slope error. 
//      if (p >= 0) 
//      { 
//         y++;
//         p += twoDy - twoDx; 
//      }
//      else{
//        // Add slope to increment angle formed 
//        p += twoDy; 
//      }
//      Point<int> Pt(x,y);
//      MoveSnake(Pt,false); 
//   } 
//} 
