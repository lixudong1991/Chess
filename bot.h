#ifndef BOT_H
#define BOT_H
#include <QVector>
#include <QPair>
#include <QThread>
#include <QMutex>

#include <QMutexLocker>
#include"cces.h"

class bot;
class botprocess: public QObject
{
    Q_OBJECT
public:
    botprocess(QObject *parent = 0);
    ~botprocess();
    bot *m_board;
public slots:
    void botmove();
signals:
    void botfinsh();
private:
    void bove( int idd, QVector<QPair<int, int> > &v );
};



class bot : public cces
{
    Q_OBJECT
public:
   explicit bot(QWidget *parent = 0);
    ~bot();
    QMutex     *m_mutex;
signals:
   void botmove();
public slots:
   void botfinsh();
protected:
   void mouseReleaseEvent(QMouseEvent *ev);
private:
   botprocess *m_bot;
   QThread    *m_botThread;

};

#endif // BOT_H
