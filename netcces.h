#ifndef NETCCES_H
#define NETCCES_H
#include"cces.h"
#include<QTcpServer>
#include<QTcpSocket>
class netcces : public cces
{
    Q_OBJECT
public:
    netcces(int da);
    QTcpServer * serv;
    QTcpSocket* sk;
    void mouseReleaseEvent(QMouseEvent *e1v);
  public slots:
    void slotion();
    void slotrecv();
};

#endif // NETCCES_H
