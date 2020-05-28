#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED
template <class T>
class Point{
  T m_nX;
  T m_nY;
  public:
    Point(){
      m_nX=0;
      m_nY=0;
    }
    Point(T x, T y){
      m_nX=x;
      m_nY=y;
    }
    void Get(T& x, T& y){
      x=m_nX;
      y=m_nY;
    }
    T GetX() const{
      return m_nX;
      }
    T GetY() const{
      return m_nY;
    }
    T GetDistX(const Point& p) const{
      return p.m_nX-m_nX;
    }
    T GetDistY(const Point& p) const{
      return p.m_nY-m_nY;
    }
    Point GetManhattanDist(const Point& p) const{
      Point pret(GetDistX(p),GetDistY(p));
      return pret;
    }
    
    void Set(T x, T y){
      m_nX=x;
      m_nY=y;
    }
    Point& operator=(const Point& p){
      Set(p.m_nX, p.m_nY);
      return *this;
    }
    bool operator==(const Point& p) const{
      return m_nX==p.m_nX && m_nY==p.m_nY;
    }
    Point operator*(const Point& p) const{
      Point pret(m_nX*p.m_nX, m_nY*p.m_nY);
      return pret;
    }
    Point& operator*=(const Point& p){
      m_nX*=p.m_nX;
      m_nY*=p.m_nY;
      return *this;
    }
    Point operator/(const Point& p) const{
      Point pret(m_nX/p.m_nX, m_nY/p.m_nY);
      return pret;
    }
    Point& operator/=(const Point& p){
      m_nX/=p.m_nX;
      m_nY/=p.m_nY;
      return *this;
    } 
    Point operator+(const Point& p) const{
      Point pret(m_nX+p.m_nX, m_nY+p.m_nY);
      return pret;
    }
    Point& operator+=(const Point& p){
      m_nX+=p.m_nX;
      m_nY+=p.m_nY;
      return *this;
    }
    Point operator-(const Point& p) const{
      Point pret(m_nX-p.m_nX, m_nY-p.m_nY);
      return pret;
    }
    Point& operator-=(const Point& p){
      m_nX-=p.m_nX;
      m_nY-=p.m_nY;
      return *this;
    }
    void Constrain(const Point& TopLeft, const Point& BotRight){
      m_nX = constrain(m_nX, TopLeft.m_nX, BotRight.m_nX);
      m_nY = constrain(m_nY, TopLeft.m_nY, BotRight.m_nY);
    }
};
#endif // POINT_H_INCLUDED
