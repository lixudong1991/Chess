#ifndef PIES_H
#define PIES_H
#include <QPixmap>
class pies
{
public:
    pies(){}
    pies(int t0,int t1,int x, int y);
    inline int getxx()const{return xx;}
    inline int getyy()const{return yy;}
    inline int gett0()const{return tp0;}
    inline int gett1()const{return tp1;}
    inline void setxy(int x,int y){xx=x; yy=y;}
    inline void settp0(int t){tp0=t;}
    inline int gtzhi()const{return zhi;}
    inline void resume(){ tp0=tp_0;}
    inline bool isAlive(){ return tp0 == tp_0;}
    bool _sel=false;
    QPixmap m_img;
 private:
    int tp0,tp_0;
    int tp1;
    int xx;
    int yy;
    int zhi;
};

#endif // PIES_H
