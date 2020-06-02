#ifndef JOYSTICK_H_INCLUDED
#define JOYSTICK_H_INCLUDED

#include "Point.h"

template <class T>
class Joystick{
  unsigned int XPin;
  unsigned int YPin;
  enum en_orientation{JO_NORTH, JO_SOUTH, JO_WEST, JO_EAST}; //direction of text on joystick relative to user
  public:
  Joystick(unsigned int X, unsigned int Y, en_orientation orientation){
    XPin = X;
    YPin = Y;
    m_en_orientation = orientation;
  }
  Point<T> JoystickRead(){
    Point<T> point(analogRead(XPin),analogRead(YPin));
    return  point;
  }
   Point<T> SetJoystickReadtoOrigin(){
    Point<T> pret = JoystickRead() * Point<T> (-1,1) + Point<T> (512, 512);
    return  pret;
  }
  Point<float> JoystickUnitCircle(){
    Point<float> UnitJoystick = JoystickRead() * Point<T> (-1,1);
    return UnitJoystick / Point<float> (1024,1024);
  }
  Point<float> JoystickUnitCircleOrigin(){
    Point<float> UnitJoystick = SetJoystickReadtoOrigin();
    return UnitJoystick / Point<float> (512,512);
  }
};

#endif // JOYSTICK_H_INCLUDED
