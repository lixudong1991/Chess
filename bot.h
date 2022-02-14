#ifndef BOT_H
#define BOT_H
#include <QVector>
#include <QPair>
#include <QThread>
#include <qpushbutton.h>
#include"cces.h"

class bot;
typedef struct NextStep{
    int id;
    int x,y;
}NextStep;
class botprocess: public QObject
{
    Q_OBJECT
public:
    botprocess(QObject *parent = 0);
    ~botprocess();
    bot *m_board;
    int _botmove(bool b,int minv,int maxv);
    int alpahbeta(bool b,int minv,int maxv,int depth);
public slots:
    void botmove();
signals:
    void botfinsh();
private:
    int bove();
    int count;
};



class bot : public cces
{
    Q_OBJECT
public:
   explicit bot(QWidget *parent = 0);
    ~bot();
signals:
   void botmove();
public slots:
   void botfinsh();
   void onbt();
protected:
   void mouseReleaseEvent(QMouseEvent *ev);
private:
   botprocess *m_bot;
   QThread    *m_botThread;
   QPushButton  *bt;

};

#endif // BOT_H
