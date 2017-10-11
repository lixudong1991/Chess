#ifndef BOT_H
#define BOT_H
#include"cces.h"
class bot : public cces
{
public:
    bot();
   void botmove();
   void bove(int idd,QVector<QPair<int,int>> &v);
   void mouseReleaseEvent(QMouseEvent *ev);
   void paintEvent(QPaintEvent *ev);
};

#endif // BOT_H
