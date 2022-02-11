#include "bot.h"
#include <unistd.h>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QTextOption>
#include <QDebug>
#define CL1 "red"
#define CL2 "blue"


bot::bot(QWidget *parent)
    :cces(1,parent)
{
    m_bot = new botprocess;
    m_bot ->m_board =this;
    m_botThread = new QThread();
    m_mutex     =new QMutex;

    m_bot->moveToThread(m_botThread);
    connect(this, SIGNAL(botmove()), m_bot, SLOT(botmove()));
    connect(m_bot, SIGNAL(botfinsh()), this, SLOT(botfinsh()));
    mov01 =false;
    m_botThread->start();

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
void bot::mouseReleaseEvent( QMouseEvent *ev ) {

    QMutexLocker lock(m_mutex);
   // if ( mov01 ) return;

    if ( ( piess[ 4 ].gett0() == 0 ) || ( piess[ 27 ].gett0() == 0 ) ) return;

    int t1 = ev->pos().x();
    int t2 = ev->pos().y();

    if ( ( t1 < d / 2 ) || ( t1 >= ( 9 * d + d / 2 ) ) || ( t2 < d / 2 ) ||
         ( t2 >= ( 10 * d + d / 2 ) ) )
        return;

    QPoint tep( ( ( t1 + d / 2 ) / d ) * d, ( ( t2 + d / 2 ) / d ) * d );
    int    tx = tep.x() / 40 - 1;
    int    ty = tep.y() / 40 - 1;

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


void botprocess::bove( int idd, QVector<QPair<int, int> > &v ) {

//    for ( int i = 0; i != 9; ++i ) {
//        for ( int j = 0; j != 10; ++j ) {
//            if ( ( m_board->cbod[ i ][ j ] != -1 ) &&
//                 ( m_board->piess[ m_board->cbod[ i ][ j ] ].gett0() == m_board->piess[ idd ].gett0() ) )
//                continue;

//            if ( m_board->canMove( idd, i, j ) ) v.push_back( QPair<int, int>( i, j ) );
//        }
//    }
    m_board->getPath(idd,v);
}

botprocess::botprocess(QObject *parent):
    m_board(NULL)
{

}

botprocess::~botprocess()
{

}

void botprocess::botmove() {
    if(!m_board)
    {
       emit botfinsh();
       return;
    }

    QMutexLocker lock(m_board->m_mutex);
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
    usleep(500000);
    if (m_board->cbod[ xx1 ][ yy1 ] != -1 ) m_board->piess[m_board-> cbod[ xx1 ][ yy1 ] ].settp0( 0 );
    m_board->cbod[ xx1 ][ yy1 ]                                   = id1;
    m_board->cbod[ m_board->piess[ id1 ].getxx() ][ m_board->piess[ id1 ].getyy() ] = -1;
    m_board->piess[ id1 ].setxy( xx1, yy1 );
    m_board->piess[id1]._sel=false;
    static int sum=0;
    qDebug()<<"b "<<++sum;
    emit botfinsh();
}


