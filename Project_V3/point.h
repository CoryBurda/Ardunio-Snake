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
    T GetDistX(const Point<T>& p) const{
      return p.m_nX-m_nX;
    }
    T GetDistY(const Point<T>& p) const{
      return p.m_nY-m_nY;
    }
    Point<T> GetManhattanDist(const Point<T>& p) const{
      Point<T> pret(GetDistX(p),GetDistY(p));
      return pret;
    }
    
    void Set(T x, T y){
      m_nX=x;
      m_nY=y;
    }
        
    Point<T>& operator=(const Point<T>& p){
      Set(p.m_nX, p.m_nY);
      return *this;
    }
    bool operator==(const Point<T>& p) const{
      return m_nX==p.m_nX && m_nY==p.m_nY;
    }
    Point<T> operator*(const Point<T>& p) const{
      Point pret(m_nX*p.m_nX, m_nY*p.m_nY);
      return pret;
    }
    Point<T> operator*(const T& t) const{
      Point pret(m_nX*t, m_nY*t);
      return pret;
    }
    Point<T>& operator*=(const T& t){
      m_nX*=t;
      m_nY*=t;
      return *this;
    }
    Point<T>& operator*=(const Point<T>& p){
      m_nX*=p.m_nX;
      m_nY*=p.m_nY;
      return *this;
    }
    Point<T> operator/(const Point<T>& p) const{
      Point pret(m_nX/p.m_nX, m_nY/p.m_nY);
      return pret;
    }
    Point<T> operator/(const T& t) const{
      Point pret(m_nX/t, m_nY/t);
      return pret;
    }
    Point<T>& operator/=(const T& t){
      m_nX/=t;
      m_nY/=t;
      return *this;
    } 
    Point<T>& operator/=(const Point<T>& p){
      m_nX/=p.m_nX;
      m_nY/=p.m_nY;
      return *this;
    } 
    Point<T> operator+(const Point<T>& p) const{
      Point pret(m_nX+p.m_nX, m_nY+p.m_nY);
      return pret;
    }
    Point<T>& operator+=(const Point<T>& p){
      m_nX+=p.m_nX;
      m_nY+=p.m_nY;
      return *this;
    }
    Point<T> operator-(const Point<T>& p) const{
      Point pret(m_nX-p.m_nX, m_nY-p.m_nY);
      return pret;
    }
    Point<T>& operator-=(const Point<T>& p){
      m_nX-=p.m_nX;
      m_nY-=p.m_nY;
      return *this;
    }
    void Constrain(const Point<T>& TopLeft, const Point<T>& BotRight){
      m_nX = constrain(m_nX, TopLeft.m_nX, BotRight.m_nX);
      m_nY = constrain(m_nY, TopLeft.m_nY, BotRight.m_nY);
    }
    Point<T> GetMidPoint(const Point<T>& p){
      Point<T> pret = GetManhattanDist(p) / 2 + *this;
      return pret;
    }
    Point<int> GetMidPointInt(const Point<int>& p){
      Point<float> pretf = GetManhattanDist(p) / 2 + *this;
      Point<int> pret(round(pretf.GetX()),round(pretf.GetY())); 
      return pret;
    }
    T GetDist(const Point<T>& p){
      return sqrt(GetDistX(p)*GetDistX(p)+GetDistY(p)*GetDistY(p));
    }
    Point<T> GetaMidPoint(const Point<T>& p, const T& t){
      Point<T> delta = (p - *this);
      T dist = GetDist(p);
      T distfrac = t / dist;
      Point<T> deltafrac = delta*distfrac;
      Point<T> pret = *this + deltafrac;
      return pret;
    }
};
#endif // POINT_H_INCLUDED
