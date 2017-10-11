#include "netcces.h"
#include<QMouseEvent>
netcces::netcces(int da):cces(da)
{
    serv=NULL;
    sk=NULL;
    if(dt==-1)
    {
        serv=new QTcpServer(this);
        serv->listen(QHostAddress::Any,9999);
        connect(serv,SIGNAL(newConnection()),   this,SLOT(slotion()));
    }
    else{
        sk=new QTcpSocket(this);
        sk->connectToHost(QHostAddress("127.0.0.1"),9999);
        connect(sk,SIGNAL(readyRead()),  this,SLOT(slotrecv()));
    }
}
void netcces::slotion()
{
    if(sk) return;
    sk=serv->nextPendingConnection();
    connect(sk,SIGNAL(readyRead()),
            this,SLOT(slotrecv()));
}
void netcces::slotrecv()
{
   QByteArray ba= sk->readAll();
   if(!ba[0])
   {
       if(id!=-1) piess[id]._sel=false;
        id=31-ba[1];
       piess[id]._sel=true;
       update();
   }
   else{
       int tx=8-ba[2],ty=9-ba[3];
       id=31-ba[1];
       if(cbod[tx][ty]!=-1) piess[cbod[tx][ty]].settp0(0);
       cbod[piess[id].getxx()][piess[id].getyy()]=-1;
       cbod[tx][ty]=id;
       piess[id]._sel=false;
       piess[id].setxy(tx,ty);
        id=-1;
        mov01=!mov01;
        update();
   }
}
void netcces::mouseReleaseEvent(QMouseEvent *e1v)
{
    if(piess[4].gett0()==0||piess[27].gett0()==0) return;
    int t1=e1v->pos().x();
    int t2=e1v->pos().y();
    if(t1<d/2||t1>=(9*d+d/2)||t2<d/2||t2>=(10*d+d/2)) return;
    QPoint tep(((t1+d/2)/d)*d,((t2+d/2)/d)*d);
    int tx=tep.x()/40-1;
    int ty=tep.y()/40-1;
    if((mov01&&dt==-1)||(!mov01&&dt==1)) return;
    if(id==-1)
    {
        if(cbod[tx][ty]==-1) return;
        if((mov01&&piess[cbod[tx][ty]].gett0()==-1)||(!mov01&&piess[cbod[tx][ty]].gett0()==1))return;
        id=cbod[tx][ty];
        piess[id]._sel=true;
        char ch[2]={0,id};
        sk->write(ch,2);
    }
    else{
        if(tx==piess[id].getxx()&&ty==piess[id].getyy()) return;
        if(cbod[tx][ty]!=-1&&piess[cbod[tx][ty]].gett0()==piess[id].gett0())
          {
            piess[id]._sel=false;
            id=cbod[tx][ty];
            piess[id]._sel=true;
            char ch[2]={0,id};
            sk->write(ch,2);
          }
        else{
        if(!canMove(id,tx,ty)) return;
        if(cbod[tx][ty]!=-1) piess[cbod[tx][ty]].settp0(0);
        cbod[piess[id].getxx()][piess[id].getyy()]=-1;
        cbod[tx][ty]=id;
        piess[id]._sel=false;
        piess[id].setxy(tx,ty);
        char ch[4]={1,id,tx,ty};
        sk->write(ch,4);
        id=-1;
        mov01=!mov01;
        }
    }
   update();
}
