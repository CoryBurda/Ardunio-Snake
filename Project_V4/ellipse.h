#ifndef ELLIPSE_H_INCLUDED
#define ELLIPSE_H_INCLUDED
#include "Point.h"

class ellipse{
  Point<int> m_center;
  Point<int> m_rad;
  public:
  ellipse(){}
  ellipse(const Point<int>& c, const Point<int>& r){
    m_center = c;
    m_rad = r;
  }
  
  bool IsInside(const Point<int>& p){
    Point<int> Manhattandist = p - m_center; //m_center.GetManhattanDist(p);
    int xdiffsqr = Manhattandist.GetX()*Manhattandist.GetX();
    int ydiffsqr = Manhattandist.GetY()*Manhattandist.GetY();
    float xradsqr = m_rad.GetX()*m_rad.GetX();
    float yradsqr = m_rad.GetY()*m_rad.GetY();
    return ((xdiffsqr/xradsqr + ydiffsqr/yradsqr) <= 1);
  }

};
#endif // ELLIPSE_H_INCLUDED
