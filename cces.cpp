#include "cces.h"
#include<QPainter>
#include<QMouseEvent>
#include <QRect>
#include<QTextOption>
#define CL1  "red"
#define CL2  "blue"

cces::cces(int dd,QWidget *parent) :dt(dd),
    QWidget(parent)
{  
    for(int i=0;i!=9;++i)
        for(int j=0;j!=10;++j)
            cbod[i][j]=-1;
       piess={
           pies(-dt,5,0,0),pies(-dt,4,1,0), pies(-dt,3,2,0),pies(-dt,2,3,0),
           pies(-dt,1,4,0), pies(-dt,2,5,0), pies(-dt,3,6,0),pies(-dt,4,7,0),pies(-dt,5,8,0),
           pies(-dt,6,1,2), pies(-dt,6,7,2),
           pies(-dt,7,0,3),pies(-dt,7,2,3),pies(-dt,7,4,3),pies(-dt,7,6,3),pies(-dt,7,8,3) ,

           pies(dt,7,0,6),pies(dt,7,2,6),pies(dt,7,4,6),pies(dt,7,6,6), pies(dt,7,8,6) ,
           pies(dt,6,1,7), pies(dt,6,7,7),
           pies(dt,5,0,9), pies(dt,4,1,9),pies(dt,3,2,9), pies(dt,2,3,9), pies(dt,1,4,9),
           pies(dt,2,5,9),pies(dt,3,6,9),   pies(dt,4,7,9),  pies(dt,5,8,9),    //车 23  24
              //炮  25  26
         //兵 27~31
         };
       for(int i=0;i!=32;++i)
       {
           cbod[piess[i].getxx()][piess[i].getyy()]=i;
       }
}
void cces::paintEvent(QPaintEvent*)
{
    QPainter painte(this);
    for(int i=1;i!=11;++i)
       {
        if(i==1||i==10) painte.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        painte.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
         if(i==1||i==10) painte.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    }
    for(int j=1;j!=10;++j)
    {
        if(j==1||j==9){
            painte.setPen(QPen(Qt::black, 3, Qt::SolidLine));
            painte.drawLine(QPoint(j*d,d),QPoint(j*d,10*d));
            painte.setPen(QPen(Qt::black, 1, Qt::SolidLine));
        }
        else{
        painte.drawLine(QPoint(j*d,d),QPoint(j*d,5*d));
        painte.drawLine(QPoint(j*d,6*d),QPoint(j*d,10*d));
        }
    }
    int a[16][2][2]={  {{3,1},{1,3}},{{3,1},{7,5}}, {{4,1},{6,3}},{{6,1},{4,3}},
       {{7,1},{3,5}},{{7,1},{9,3}}, {{1,3},{3,5}},  {{7,5},{9,3}},   {{3,6},{1,8}},
        {{3,6},{7,10}},    {{7,6},{3,10}},   {{7,6},{9,8}},{{1,8},{3,10}},  {{4,10},{6,8}},
  {{6,10},{4,8}},  {{7,10},{9,8}}
    };
    for(int i=0;i!=16;++i)
    {
        painte.drawLine(QPoint(a[i][0][0]*d,a[i][0][1]*d),QPoint(a[i][1][0]*d,a[i][1][1]*d));
    }
    painte.setFont(QFont("楷体",18,80));
    painte.drawText(QRect(2*d,4*d+d/2,80,80),Qt::AlignCenter,"楚 河");
    painte.drawText(QRect(6*d,4*d+d/2,80,80),Qt::AlignCenter,"汉 界");
    for(int i=0;i!=32;++i)
    {
        if(piess[i].gett0()==0) continue;
        if(piess[i]._sel)
        {
            painte.setBrush(QBrush(QColor("yellow")));
        }
        else
        {
           painte.setBrush(QBrush(QColor("orange")));
        }
        QRect rect((piess[i].getxx()+1)*d-d/2,(piess[i].getyy()+1)*d-d/2,d,d);
        piess[i].gett0()==1?painte.setPen(QColor(CL2)):painte.setPen(QColor(CL1));
         painte.drawEllipse(QPoint((piess[i].getxx()+1)*d,(piess[i].getyy()+1)*d),d/2,d/2);
         switch(piess[i].gett1())
         {
         case 1:
            piess[i].gett0()==1? painte.drawText(rect,Qt::AlignCenter,"将"): painte.drawText(rect,Qt::AlignCenter,"帅");
             break;
         case 2:
            piess[i].gett0() ==1? painte.drawText(rect,Qt::AlignCenter,"仕"): painte.drawText(rect,Qt::AlignCenter,"士");
                break;
         case 3:
          piess[i].gett0()==1? painte.drawText(rect,Qt::AlignCenter,"象"): painte.drawText(rect,Qt::AlignCenter,"相");
             break;
         case 5:
             painte.drawText(rect,Qt::AlignCenter,"車");
                break;
         case 4:
            piess[i].gett0 ()==1? painte.drawText(rect,Qt::AlignCenter,"馬"): painte.drawText(rect,Qt::AlignCenter,"马");
             break;
         case 6:
             piess[i].gett0()==1? painte.drawText(rect,Qt::AlignCenter,"砲"): painte.drawText(rect,Qt::AlignCenter,"炮");
                break;
         case 7:
             piess[i].gett0()==1? painte.drawText(rect,Qt::AlignCenter,"卒"): painte.drawText(rect,Qt::AlignCenter,"兵");
                break;
         }
    }
}
void cces::mouseReleaseEvent(QMouseEvent *ev)
{
    if(piess[4].gett0()==0||piess[27].gett0()==0) return;
     int t1=ev->pos().x();
     int t2=ev->pos().y();
     if(t1<d/2||t1>=(9*d+d/2)||t2<d/2||t2>=(10*d+d/2)) return;
     QPoint tep(((t1+d/2)/d)*d,((t2+d/2)/d)*d);
     int tx=tep.x()/40-1;
     int ty=tep.y()/40-1;
     if(id==-1)
     {
         if(cbod[tx][ty]==-1) return;
         if(mov01&&piess[cbod[tx][ty]].gett0()==dt) return;
         if(!mov01&&piess[cbod[tx][ty]].gett0()==-dt)return;
         id=cbod[tx][ty];
         piess[id]._sel=true;
     }
     else{
         if(tx==piess[id].getxx()&&ty==piess[id].getyy()) return;
         if(cbod[tx][ty]!=-1&&piess[cbod[tx][ty]].gett0()==piess[id].gett0())
           {
             piess[id]._sel=false;
             id=cbod[tx][ty];
             piess[id]._sel=true;
           }
         else{
         if(!canMove(id,tx,ty)) return;
         if(cbod[tx][ty]!=-1) piess[cbod[tx][ty]].settp0(0);
         cbod[piess[id].getxx()][piess[id].getyy()]=-1;
         cbod[tx][ty]=id;
         piess[id]._sel=false;
         piess[id].setxy(tx,ty);
         id=-1;
         mov01=!mov01;
         }
     }
    update();
}
bool cces::canMove(int iid, int ixx, int iyy)
{
   int ix=piess[iid].getxx();
   int iy=piess[iid].getyy();
   int t0=piess[iid].gett0();
   switch (piess[iid].gett1()) {
   case 1:
   {
        if(((ix-ixx==1&&ixx>=3)||(ix-ixx==-1&&ixx<=5))&&iy==iyy) return true;
        if(ix==ixx&&((iy-iyy==1)||(iy-iyy==-1))){
          if(t0==-dt&&iyy<=2&&iyy>=0)  return true;
         else if(t0==dt&&iyy>=7&&iyy<=9) return true;
        }
        int m=(id==4?27:4);
        if(piess[4].getxx()==piess[27].getxx()&&ix==ixx&&(iyy==piess[m].getyy()))
        {
            for(int n=piess[4].getyy()+1;n!=piess[27].getyy();++n)
            {
               if(cbod[ix][n]!=-1)  return false;
            }
            return true;
        }
        return false;
   }
   case 2:
  {
        if(ixx<3||ixx>5)return false;
        if(t0==dt&&(iyy<7||iyy>9))return false;
        if(t0==-dt&&(iyy<0||iyy>2))return false;
        if((ix-ixx==-1||ix-ixx==1)&&(iy-iyy==-1||iy-iyy==1)) return true;
        return false;
  }
   case 3:
{
      if((t0==dt&&iyy<5)||(t0==-dt&&iyy>4)) return false;
      if((ixx-ix==2||ixx-ix==-2)&&(iyy-iy==2||iyy-iy==-2))
     {
         if(cbod[(ixx+ix)/2][(iyy+iy)/2]!=-1) return false;
         return true;
     }
       return false;
 }
   case 5:
{
      if(ixx!=ix&&iyy!=iy) return false;
      if(ixx==ix&&iyy!=iy)
      {
          int ty,tx;
          if(iyy>iy)
          {
              ty=iyy;
              tx=iy;
          }
          else{
              ty=iy;
              tx=iyy;
          }
          for(int n=tx+1;n!=ty;++n)
          {
              if(cbod[ixx][n]!=-1) return false;
          }
          return true;
      }
       if(ixx!=ix&&iyy==iy)
       {
           int ty,tx;
           if(ixx>ix)
           {
               ty=ixx;
               tx=ix;
           }
           else{
               ty=ix;
               tx=ixx;
           }
           for(int n=tx+1;n!=ty;++n)
           {
               if(cbod[n][iyy]!=-1) return false;
           }
           return true;
       }
        return false;
}
   case 4:
{
      if(((ixx-ix==-2)||(ixx-ix==2))&&((iyy-iy==1)||(iyy-iy==-1)) ){
          if(cbod[(ixx+ix)/2][iy]!=-1) return false;
          return true;
      }
      if(((ixx-ix==-1)||(ixx-ix==1))&&((iyy-iy==2)||(iyy-iy==-2)) ){
          if(cbod[ix][(iy+iyy)/2]!=-1) return false;
          return true;
      }
       return false;
 }
   case 6:
{
       if(ixx!=ix&&iyy!=iy) return false;
       if(ixx==ix&&iyy!=iy)
       {
           int ty,tx,tm=0;
           if(iyy>iy)
           {
               ty=iyy;
               tx=iy;
           }
           else{
               ty=iy;
               tx=iyy;
           }
           for(int n=tx+1;n!=ty;++n)
           {
               if(cbod[ixx][n]!=-1)++tm;
                if(tm>2) return false;
           }
           if(tm==0&&cbod[ixx][iyy]==-1) return true;
           if(tm==1&&cbod[ixx][iyy]!=-1) return true;
           return false;
       }
       if(ixx!=ix&&iyy==iy)
       {
           int ty,tx,tm=0;
           if(ixx>ix)
           {
               ty=ixx;
               tx=ix;
           }
           else{
               ty=ix;
               tx=ixx;
           }
           for(int n=tx+1;n!=ty;++n)
           {
               if(cbod[n][iyy]!=-1) ++tm;
               if(tm>1) return false;
           }
           if(tm==0&&cbod[ixx][iyy]==-1) return true;
           if(tm==1&&cbod[ixx][iyy]!=-1) return true;
           return false;
       }
       return false;
 }
   case 7:
       if(t0==-dt)
       {
          if(ixx==ix&&iyy-iy==1) return true;
           if(iy>=5&&((ixx-ix==1||ixx-ix==-1)&&iyy==iy) )return true;
           return false;
       }
       if(t0==dt)
       {
           if(ixx==ix&&iyy-iy==-1) return true;
           if(iy<=4&&((ixx-ix==1||ixx-ix==-1)&&iyy==iy) )return true;
           return false;
       }
       return false;
   default:
       return false;
   }
}
