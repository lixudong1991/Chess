#include "bot.h"
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QTextOption>
#define CL1 "red"
#define CL2 "blue"


bot::bot(QWidget *parent)
    :cces(1,parent)
{

}

bot::~bot()
{

}


void bot::bove( int idd, QVector<QPair<int, int> > &v ) {
    for ( int i = 0; i != 9; ++i ) {
        for ( int j = 0; j != 10; ++j ) {
            if ( ( cbod[ i ][ j ] != -1 ) &&
                 ( piess[ cbod[ i ][ j ] ].gett0() == piess[ idd ].gett0() ) )
                continue;

            if ( canMove( idd, i, j ) ) v.push_back( QPair<int, int>( i, j ) );
        }
    }
}

void bot::botmove() {
    int id1, xx1, yy1, zhi1 = -10000;

    for ( int i = 0; i != 16; ++i ) {
        if ( piess[ i ].gett0() == 0 ) continue;
        QVector<QPair<int, int> > vi;
        bove( i, vi );

        if ( vi.isEmpty() ) continue;
        int tex = piess[ i ].getxx(), tey = piess[ i ].getyy();
        cbod[ tex ][ tey ] = -1;

        for ( QVector<QPair<int, int> >::const_iterator ite = vi.begin();
              ite != vi.end(); ++ite ) {
            int idi  = cbod[ ite->first ][ ite->second ];
            int zhi2 = 0, zhi3 = 0;

            if ( idi != -1 ) {
                zhi2 = piess[ idi ].gtzhi();
                piess[ idi ].settp0( 0 );
            }

            cbod[ ite->first ][ ite->second ] = i;
            piess[ i ].setxy( ite->first, ite->second );

            for ( int j = 16; j != 32; ++j ) {
                if ( piess[ j ].gett0() == 0 ) continue;
                QVector<QPair<int, int> > vj;
                bove( j, vj );

                if ( vi.isEmpty() ) continue;

                for ( QVector<QPair<int, int> >::const_iterator ite1 =
                          vj.begin();
                      ite1 != vj.end(); ++ite1 ) {
                    int idj = cbod[ ite1->first ][ ite1->second ];

                    if ( ( idj != -1 ) && ( zhi3 > piess[ idj ].gtzhi() ) )
                        zhi3 = piess[ idj ].gtzhi();
                }
            }

            if ( zhi1 < zhi2 + zhi3 )
            {
                zhi1 = zhi2 + zhi3;
                id1  = i;
                xx1  = ite->first;
                yy1  = ite->second;
            }
            piess[ i ].setxy( tex, tey );

            if ( idi == -1 )
                cbod[ ite->first ][ ite->second ] = -1;
            else {
                cbod[ ite->first ][ ite->second ] = idi;
                piess[ idi ].settp0( 1 );
            }
        }
        cbod[ tex ][ tey ] = i;
    }

    if ( cbod[ xx1 ][ yy1 ] != -1 ) piess[ cbod[ xx1 ][ yy1 ] ].settp0( 0 );
    cbod[ xx1 ][ yy1 ]                                   = id1;
    cbod[ piess[ id1 ].getxx() ][ piess[ id1 ].getyy() ] = -1;
    piess[ id1 ].setxy( xx1, yy1 );
    mov01 = !mov01;
}

void bot::mouseReleaseEvent( QMouseEvent *ev ) {
    if ( mov01 ) return;

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
            mov01 = !mov01;
        }
    }
    update();

    if ( mov01 ) {
        botmove();
        update();
    }
}
