#ifndef CCES_H
#define CCES_H

#include <QWidget>
#include <QMutex>

#include <QMutexLocker>
#include <qwaitcondition.h>
#include"pies.h"
#define CHTXT(a)  QString::fromLocal8Bit(a)
extern  QMutex m_mutex;
extern  QWaitCondition waitcond;
class cces : public QWidget
{
    Q_OBJECT
public:
    explicit cces(int dd=1,QWidget *parent = 0);
    ~cces();
    bool canMove(int iid,int ixx,int iyy);
    void getPath(int iid,QVector<QPair<int, int> > &v);
    inline void killPies(int iid){ piess[iid].settp0(0);m_sum -=piess[iid].gtzhi();}
    inline void resumePies(int iid){piess[iid].resume();m_sum +=piess[iid].gtzhi();}
    inline int  getBoardSum()const{ return m_sum;}
    void upDateBoard();
    int cbod[9][10];
    QVector<pies> piess;
    int id=-1,d,dt;
    volatile bool mov01=false;
protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent*);
    QVector<pies> paint_piess;
private:
    int m_sum;
    QPixmap m_bkimg;
};

#endif // CCES_H
