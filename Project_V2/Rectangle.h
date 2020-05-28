#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "Point.h"

template <class T>
bool IsInRange(const T& x, const T& l, const T& r)
{
  T temp = l;
  l = min(l, r);
  r = max(temp, r);

  return l <= x && x <= r;
}

template <class T>
class Rectangle
{
  Point<T> m_TL;
  Point<T> m_BR;

  public:
  Rectangle(){}
  Rectangle(T l, T r, T t, T b) : m_TL(l, t), m_BR(r, b) { }
  T left() const { return m_TL.GetX(); }
  T right() const { return m_BR.GetX(); }
  T top() const { return m_TL.GetY(); }
  T bottom() const { return m_BR.GetY(); }
  bool IsInside(const Point<T>& p) const
  {
    return IsInRange(p.GetX(), left(), right()) && IsInRange(p.GetY(), top(), bottom());
  }
};

#endif // RECTANGLE_H_INCLUDED
