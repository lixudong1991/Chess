#ifndef CCES_H
#define CCES_H

#include <QWidget>
#include"pies.h"
class cces : public QWidget
{
    Q_OBJECT
public:
    explicit cces(int dd=1,QWidget *parent = 0);
   void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent*);
    bool canMove(int iid,int ixx,int iyy);
    int cbod[9][10];
    QVector<pies> piess;
    int id=-1,d=40,dt;
    bool mov01=true;
signals:

public slots:

};

#endif // CCES_H
