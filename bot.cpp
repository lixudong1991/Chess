#include "bot.h"
#include<QPainter>
#include<QMouseEvent>
#include <QRect>
#include<QTextOption>
#define CL1  "red"
#define CL2  "blue"
bot::bot():cces(1)
{
}
void bot::bove(int idd,QVector<QPair<int,int>> &v)
{
    for(int i=0;i!=9;++i)
    {
        for(int j=0;j!=10;++j)
        {
          if(cbod[i][j]!=-1&&piess[cbod[i][j]].gett0()==piess[idd].gett0()) continue;
          if(canMove(idd,i,j))  v.push_back(QPair<int,int>(i,j));
        }
    }
}
void bot::botmove()
{
   int id1,xx1,yy1,zhi1=-10000;
    for(int i=0;i!=16;++i)
  {
      if(piess[i].gett0()==0) continue;
      QVector<QPair<int,int>> vi;
      bove(i,vi);
      if(vi.isEmpty()) continue;
      int tex=piess[i].getxx(),tey=piess[i].getyy();
      cbod[tex][tey]=-1;
      for(QVector<QPair<int,int>>::const_iterator ite=vi.cbegin();ite!=vi.cend();++ite)
      {
          int idi=cbod[ite->first][ite->second];
          int zhi2=0 ,zhi3=0;;
          if(idi!=-1)
          {
             zhi2= piess[idi].gtzhi();
             piess[idi].settp0(0);
          }
         cbod[ite->first][ite->second]=i;
         piess[i].setxy(ite->first,ite->second);
         for(int j=16;j!=32;++j){
             if(piess[j].gett0()==0) continue;
             QVector<QPair<int,int>> vj;
             bove(j,vj);
             if(vi.isEmpty()) continue;
             for(QVector<QPair<int,int>>::const_iterator ite1=vj.cbegin();ite1!=vj.cend();++ite1)
             {
                   int idj=cbod[ite1->first][ite1->second];
                   if(idj!=-1&&zhi3>piess[idj].gtzhi()) zhi3=piess[idj].gtzhi();
             }
         }
         if(zhi1<zhi2+zhi3)
            {
             zhi1=zhi2+zhi3;
             id1=i;
             xx1=ite->first;
             yy1=ite->second;
            }
         piess[i].setxy(tex,tey);
         if(idi==-1) cbod[ite->first][ite->second] =-1;
         else
         {
             cbod[ite->first][ite->second]=idi;
             piess[idi].settp0(1);
         }

      }
      cbod[tex][tey]=i;
  }
    if(cbod[xx1][yy1]!=-1)
      piess[cbod[xx1][yy1]].settp0(0);
    cbod[xx1][yy1]=id1;
    cbod[piess[id1].getxx()][piess[id1].getyy()]=-1;
    piess[id1].setxy(xx1,yy1);
    mov01=!mov01;
}
void bot::mouseReleaseEvent(QMouseEvent *ev)
{
    if(piess[4].gett0()==0||piess[27].gett0()==0) return;
     int t1=ev->pos().x();
     int t2=ev->pos().y();
     if(t1<d/2||t1>=(9*d+d/2)||t2<d/2||t2>=(10*d+d/2)) return;
     QPoint tep(((t1+d/2)/d)*d,((t2+d/2)/d)*d);
     int tx=tep.x()/40-1;
     int ty=tep.y()/40-1;
     if(mov01) return;
     if(id==-1)
     {
         if(cbod[tx][ty]==-1) return;
         if(piess[cbod[tx][ty]].gett0()==-1) return;
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
void bot::paintEvent(QPaintEvent *ev)
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
    painte.setFont(QFont("AR PL UKai TW",18,80));
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
   if(mov01)
   {
       botmove();
       update();
   }

}
