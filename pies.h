#ifndef PIES_H
#define PIES_H
class pies
{
public:
    pies()=default;
    pies(int t0,int t1,int x, int y);
    int getxx()const{return xx;}
    int getyy()const{return yy;}
    int gett0()const{return tp0;}
    int gett1()const{return tp1;}
    void setxy(int x,int y){xx=x; yy=y;}
    void settp0(int t){tp0=t;}
    int gtzhi()const{return zhi;}
    bool _sel=false;
 private:
    int tp0;
    int tp1;
    int xx;
    int yy;
    int zhi;
};

#endif // PIES_H
