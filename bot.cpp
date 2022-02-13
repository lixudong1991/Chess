#include "bot.h"
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QTextOption>
#include <QDebug>

#ifdef _WIN32
#include <Windows.h>
#elif  __linux__
#include <unistd.h>
#endif



#define CL1 "red"
#define CL2 "blue"


bot::bot(QWidget *parent)
    :cces(1,parent)
{
    m_bot = new botprocess;
    m_bot ->m_board =this;
    m_botThread = new QThread();

    m_bot->moveToThread(m_botThread);
    connect(this, SIGNAL(botmove()), m_bot, SLOT(botmove()));
    connect(m_bot, SIGNAL(botfinsh()), this, SLOT(botfinsh()));
    mov01 =false;
    m_botThread->start();
    setFixedSize(d * 10 + 100, d * 11);
    bt = new QPushButton(QString::fromLocal8Bit("ÏÂÒ»²½"),this);

    bt->move(d * 10 + 10, 200); bt->setFixedSize(100,50);
    connect(bt,SIGNAL(clicked(bool)),this,SLOT(onbt()));
}

bot::~bot()
{
    m_botThread->quit();
    m_botThread->wait();
    m_bot->deleteLater();
}

void bot::botfinsh()
{
   update();
}
void bot::onbt()
{
    update();
    waitcond.wakeAll();
}
void bot::mouseReleaseEvent( QMouseEvent *ev ) {

    QMutexLocker lock(&m_mutex);
   // if ( mov01 ) return;

    if ( ( piess[ 4 ].gett0() == 0 ) || ( piess[ 27 ].gett0() == 0 ) ) return;

    int t1 = ev->pos().x();
    int t2 = ev->pos().y();

    if ( ( t1 < d / 2 ) || ( t1 >= ( 9 * d + d / 2 ) ) || ( t2 < d / 2 ) ||
         ( t2 >= ( 10 * d + d / 2 ) ) )
        return;

    QPoint tep( ( ( t1 + d / 2 ) / d ) * d, ( ( t2 + d / 2 ) / d ) * d );
    int    tx = tep.x() / d - 1;
    int    ty = tep.y() / d - 1;

    if ( id == -1 ) {
        if ( cbod[ tx ][ ty ] == -1 ) return;

        if ( piess[ cbod[ tx ][ ty ] ].gett0() == -1 ) return;

        id               = cbod[ tx ][ ty ];
        piess[ id ]._sel = true;

    } else {
        if ( ( tx == piess[ id ].getxx() ) && ( ty == piess[ id ].getyy() ) )
            return;

        if ( ( cbod[ tx ][ ty ] != -1 ) &&
             ( piess[ cbod[ tx ][ ty ] ].gett0() == piess[ id ].gett0() ) ) {
            piess[ id ]._sel = false;
            id               = cbod[ tx ][ ty ];
            piess[ id ]._sel = true;
        } else {
            if ( !canMove( id, tx, ty ) ) return;

            if ( cbod[ tx ][ ty ] != -1 ) piess[ cbod[ tx ][ ty ] ].settp0( 0 );
            cbod[ piess[ id ].getxx() ][ piess[ id ].getyy() ] = -1;
            cbod[ tx ][ ty ]                                   = id;
            piess[ id ]._sel                                   = false;
            piess[ id ].setxy( tx, ty );
            id    = -1;
           // mov01 = true;
            static int sum=0;
            qDebug()<<"m "<<++sum;
            emit botmove();
        }

    }
    update();
}


int botprocess::bove()
{
    int sum=0;
    for(int i=0;i<m_board->piess.size();i++)
    {
        if(m_board->piess.at(i).gett0()==0)
            continue;
        sum+=m_board->piess.at(i).gtzhi();
    }
    return sum;
}




botprocess::botprocess(QObject *parent):
    m_board(NULL)
{
    count = 0;
}

botprocess::~botprocess()
{

}

int botprocess::_botmove(bool b,int minv,int maxv)
{
    ++count;
    int start = b?0:16, end=b?16:32;
    int val = b?12000:-12000;
    int parrminv =minv,parrmaxv=maxv;
    for (int j = start; j != end; ++j)
    {
        if (m_board->piess[j].gett0() == 0) continue;
        QVector<QPair<int, int> > vj;
        m_board->getPath(j, vj);
        if (vj.isEmpty()) continue;
        int xx = m_board->piess[j].getxx();
        int yy = m_board->piess[j].getyy();
        m_board->cbod[xx][yy] = -1;
        for (QVector<QPair<int, int> >::const_iterator ite1 =
            vj.begin();
            ite1 != vj.end(); ++ite1) {
            int idj = m_board->cbod[ite1->first][ite1->second];
            m_board->piess[j].setxy(ite1->first, ite1->second);
            m_board->cbod[ite1->first][ite1->second] = j;
            int t0 = 0;
            if (idj != -1)
            {       
                t0 = m_board->piess[idj].gett0();
                m_board->piess[idj].settp0(0);
            }
            int value;
            if (count <3 && idj!=4)
            {
                value = _botmove(!b,parrminv,parrmaxv);
            }else
            {
                value = bove();
            }
            if(b)
            {
                if(val>value)
                    val =value;
            }
            else
            {
                if(val<value)
                    val =value;
            }
            m_board->cbod[ite1->first][ite1->second] = idj;
            if (idj != -1)
                m_board->piess[idj].settp0(t0);
        }
        m_board->cbod[xx][yy] = j;
        m_board->piess[j].setxy(xx, yy);   
    }
    --count;
    return val;

}

int botprocess::alpahbeta(bool b, int parentminv, int parentmaxv, int depth)
{

    if(depth == 0)
    {
        int v =  bove();
      //  qDebug() << "sum = " << v;
      //  waitcond.wait(&m_mutex);
        return v;
    }
    int start = b?0:16, end=b?16:32;
    int maxv = parentmaxv,minv=parentminv,value=0;
    for (int j = start; j != end; ++j)
    {
        if (m_board->piess[j].gett0() == 0) continue;
        QVector<QPair<int, int> > vj;
        m_board->getPath(j, vj);
        if (vj.isEmpty()) continue;
        int xx = m_board->piess[j].getxx();
        int yy = m_board->piess[j].getyy();
        m_board->cbod[xx][yy] = -1;

        for (QVector<QPair<int, int> >::const_iterator ite1 =vj.begin(); ite1 != vj.end(); ++ite1)
        {
            int idj = m_board->cbod[ite1->first][ite1->second];
            m_board->piess[j].setxy(ite1->first, ite1->second);
            m_board->cbod[ite1->first][ite1->second] = j;
            int t0 = 0;
            if (idj != -1)
            {
                t0 = m_board->piess[idj].gett0();
                m_board->piess[idj].settp0(0);
            } 
                      
            if (depth == 4 && idj ==4)
            {
                int v1 = bove();
                m_board->cbod[ite1->first][ite1->second] = idj;
                m_board->piess[4].settp0(t0);
                m_board->cbod[xx][yy] = j;
                m_board->piess[j].setxy(xx, yy);
                return v1;
            }
            value = alpahbeta(!b,minv,maxv,depth-1);
            if (b)
            {
                if (value < maxv )
                    maxv = value;
            }
            else
            {
                if (value > minv)
                    minv = value;
            }
            if(maxv<=minv)
            {
                m_board->cbod[ite1->first][ite1->second] = idj;
                if (idj != -1)
                    m_board->piess[idj].settp0(t0);
                m_board->cbod[xx][yy] = j;
                m_board->piess[j].setxy(xx, yy);
                goto ret;
            }
            m_board->cbod[ite1->first][ite1->second] = idj;
            if (idj != -1)
                m_board->piess[idj].settp0(t0);

        }
        m_board->cbod[xx][yy] = j;
        m_board->piess[j].setxy(xx, yy);
    }
ret:
    if (b)
    {
        if (maxv != parentmaxv)
            return maxv;
    }
    else
    {
        if (minv != parentminv)
            return minv;
    }

    return value;
}
void botprocess::botmove()
{
    Sleep(100);
    QMutexLocker lock(&m_mutex);
    if (!m_board)
    {
        emit botfinsh();
        return;
    }
    int id1, xx1, yy1,zhi=11000;
    int maxv = 12000,minv=-12000;
    bool bdead=false;
        for (int i = 0; i != 16; ++i) {
            if (m_board->piess[i].gett0() == 0) continue;
            QVector<QPair<int, int> > vi;
            m_board->getPath(i, vi);
            if (vi.isEmpty()) continue;
            int xx = m_board->piess[i].getxx();
            int yy = m_board->piess[i].getyy();
            m_board->cbod[xx][yy] = -1;
            for (QVector<QPair<int, int> >::const_iterator ite1 = vi.begin();ite1 != vi.end(); ++ite1)
            {
                int idj = m_board->cbod[ite1->first][ite1->second];
                m_board->piess[i].setxy(ite1->first, ite1->second);
                m_board->cbod[ite1->first][ite1->second] = i;
                int val = 0, tp0;
                if (idj != -1)
                {
                    tp0 = m_board->piess[idj].gett0();
                    m_board->piess[idj].settp0(0);
                }
                val=alpahbeta(false,minv,maxv,4);
             //   qDebug() << "zhi = " << zhi <<" val = " <<val;
                if (zhi >= val)
                {
                    xx1 = ite1->first;
                    yy1 = ite1->second;
                    id1 = i;
                    zhi = val;
                }
                m_board->cbod[ite1->first][ite1->second] = idj;
                if (idj != -1)
                    m_board->piess[idj].settp0(tp0);
            }
            m_board->cbod[xx][yy] = i;
            m_board->piess[i].setxy(xx, yy);
        }
    m_board->piess[id1]._sel = true;
    emit botfinsh();
    Sleep(300);
    if (m_board->cbod[xx1][yy1] != -1) m_board->piess[m_board->cbod[xx1][yy1]].settp0(0);
    m_board->cbod[xx1][yy1] = id1;
    m_board->cbod[m_board->piess[id1].getxx()][m_board->piess[id1].getyy()] = -1;
    m_board->piess[id1].setxy(xx1, yy1);
    m_board->piess[id1]._sel = false;
    static int sum = 0;
    qDebug() << "b " << ++sum;
    emit botfinsh();
}

/*
void botprocess::botmove()
{
    Sleep(100);
    QMutexLocker lock(&m_mutex);
    if (!m_board)
    {
        emit botfinsh();
        return;
    }
    int id1, xx1, yy1, zhi1 = 12000;
    int max = 12000,min=-12000;
    for (int i = 0; i != 16; ++i) {
        if (m_board->piess[i].gett0() == 0) continue;
        QVector<QPair<int, int> > vi;
        m_board->getPath(i, vi);
        if (vi.isEmpty()) continue;
        int xx = m_board->piess[i].getxx();
        int yy = m_board->piess[i].getyy();
        m_board->cbod[xx][yy] = -1;
        for (QVector<QPair<int, int> >::const_iterator ite1 = vi.begin();ite1 != vi.end(); ++ite1)
        {
            int idj = m_board->cbod[ite1->first][ite1->second];
            m_board->piess[i].setxy(ite1->first, ite1->second);
            m_board->cbod[ite1->first][ite1->second] = i;
            int val = 0, tp0;
            if (idj != -1)
            {
                tp0 = m_board->piess[idj].gett0();
                m_board->piess[idj].settp0(0);
            }
            val=_botmove(false,min,max);

            if (zhi1 > val)
            {
                xx1 = ite1->first;
                yy1 = ite1->second;
                id1 = i;
                zhi1 = val;
            }
            m_board->cbod[ite1->first][ite1->second] = idj;
            if (idj != -1)
                m_board->piess[idj].settp0(tp0);
        }
        m_board->cbod[xx][yy] = i;
        m_board->piess[i].setxy(xx, yy);
    }
    m_board->piess[id1]._sel = true;
    emit botfinsh();
    Sleep(300);
    if (m_board->cbod[xx1][yy1] != -1) m_board->piess[m_board->cbod[xx1][yy1]].settp0(0);
    m_board->cbod[xx1][yy1] = id1;
    m_board->cbod[m_board->piess[id1].getxx()][m_board->piess[id1].getyy()] = -1;
    m_board->piess[id1].setxy(xx1, yy1);
    m_board->piess[id1]._sel = false;
    static int sum = 0;
    qDebug() << "b " << ++sum;
    emit botfinsh();
}
*/
/*
void botprocess::botmove() {
    QMutexLocker lock(m_board->m_mutex);
    if(!m_board)
    {
       emit botfinsh();
       return;
    }
    int id1, xx1, yy1, zhi1 = -10000;

    for ( int i = 0; i != 16; ++i ) {
        if ( m_board->piess[ i ].gett0() == 0 ) continue;
        QVector<QPair<int, int> > vi;
        bove( i, vi );

        if ( vi.isEmpty() ) continue;
        int tex = m_board->piess[ i ].getxx(), tey = m_board->piess[ i ].getyy();
        m_board->cbod[ tex ][ tey ] = -1;

        for ( QVector<QPair<int, int> >::const_iterator ite = vi.begin();
              ite != vi.end(); ++ite ) {
            int idi  = m_board->cbod[ ite->first ][ ite->second ];
            int zhi2 = 0, zhi3 = 0;

            if ( idi != -1 ) {
                zhi2 = m_board->piess[ idi ].gtzhi();
                m_board->piess[ idi ].settp0( 0 );
            }

            m_board->cbod[ ite->first ][ ite->second ] = i;
            m_board->piess[ i ].setxy( ite->first, ite->second );

            for ( int j = 16; j != 32; ++j ) {
                if (m_board->piess[ j ].gett0() == 0 ) continue;
                QVector<QPair<int, int> > vj;
                bove( j, vj );

                if ( vi.isEmpty() ) continue;

                for ( QVector<QPair<int, int> >::const_iterator ite1 =
                          vj.begin();
                      ite1 != vj.end(); ++ite1 ) {
                    int idj = m_board->cbod[ ite1->first ][ ite1->second ];

                    if ( ( idj != -1 ) && ( zhi3 > m_board->piess[ idj ].gtzhi() ) )
                        zhi3 = m_board->piess[ idj ].gtzhi();
                }
            }

            if ( zhi1 < zhi2 + zhi3 )
            {
                zhi1 = zhi2 + zhi3;
                id1  = i;
                xx1  = ite->first;
                yy1  = ite->second;
            }
            m_board->piess[ i ].setxy( tex, tey );

            if ( idi == -1 )
                m_board->cbod[ ite->first ][ ite->second ] = -1;
            else {
                m_board->cbod[ ite->first ][ ite->second ] = idi;
                m_board->piess[ idi ].settp0( 1 );
            }
        }
        m_board->cbod[ tex ][ tey ] = i;
    }
    m_board->piess[id1]._sel=true;
    emit botfinsh();
    Sleep(300);
    if (m_board->cbod[ xx1 ][ yy1 ] != -1 ) m_board->piess[m_board-> cbod[ xx1 ][ yy1 ] ].settp0( 0 );
    m_board->cbod[ xx1 ][ yy1 ]                                   = id1;
    m_board->cbod[ m_board->piess[ id1 ].getxx() ][ m_board->piess[ id1 ].getyy() ] = -1;
    m_board->piess[ id1 ].setxy( xx1, yy1 );
    m_board->piess[id1]._sel=false;
    static int sum=0;
    qDebug()<<"b "<<++sum;
    emit botfinsh();
}


*/
