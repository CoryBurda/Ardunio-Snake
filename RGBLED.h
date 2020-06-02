#ifndef RGBLED_H_INCLUDED
#define RGBLED_H_INCLUDED

typedef enum {COMMON_ANNODE, COMMON_CATHODE} enLEDPOLARITY;

class RGBLED{
  int m_nRedPin;
  int m_nGreenPin;
  int m_nBluePin;
  enLEDPOLARITY m_enPolarity;
  public:
  RGBLED(int nRedPin, int nGreenPin, int nBluePin, enLEDPOLARITY enPolarity){
    m_nRedPin=nRedPin;
    m_nGreenPin=nGreenPin;
    m_nBluePin=nBluePin;
    m_enPolarity=enPolarity;
    
    pinMode(m_nRedPin,OUTPUT);
    pinMode(m_nGreenPin,OUTPUT);
    pinMode(m_nBluePin,OUTPUT);
    SetColor(0,0,0);
  }
  void SetColor(unsigned short r, unsigned short g, unsigned short b){
    if(m_enPolarity==COMMON_ANNODE){
      r=255-r;
      g=255-g;
      b=255-b;
    }
    analogWrite(m_nRedPin, r);
    analogWrite(m_nGreenPin, g);
    analogWrite(m_nBluePin, b);
  }
};
#endif // RGBLED_H_INCLUDED
