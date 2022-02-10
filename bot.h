#ifndef BOT_H
#define BOT_H
#include <QVector>
#include <QPair>
#include"cces.h"

class bot : public cces
{
    Q_OBJECT
public:
   explicit bot(QWidget *parent = 0);
    ~bot();
   void botmove();
   void bove(int idd,QVector< QPair<int,int> > &v);
protected:
   void mouseReleaseEvent(QMouseEvent *ev);

};

#endif // BOT_H
