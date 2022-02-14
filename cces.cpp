#include "cces.h"
#include <QPainter>
#include <QMouseEvent>
#include <QRect>
#include <QTextOption>
#include <QPainterPath>
#define CL1  "red"
#define CL2  "blue"
 QMutex m_mutex;
 QWaitCondition waitcond;
cces::cces(int dd, int piesRad,QWidget *parent) : dt(dd),
    d(piesRad),
    QWidget(parent)
{
    for (int i = 0; i != 9; ++i)
        for (int j = 0; j != 10; ++j) cbod[i][j] = -1;

    pies temp[32] = {
        pies(-dt, 5, 0, 0), pies(-dt, 4,  1,  0),  pies(-dt,  3,   2,    0),
        pies(
            -dt,
            2,
            3,
            0),
        pies(-dt, 1, 4, 0), pies(-dt, 2,  5,  0),  pies(-dt,  3,   6,    0),
        pies(
            -dt,
            4,
            7,
            0),
        pies(-dt, 5, 8, 0),
        pies(-dt, 6, 1, 2), pies(-dt, 6,  7,  2),
        pies(-dt, 7, 0, 3), pies(-dt, 7,  2,  3),  pies(-dt,  7,   4,    3),
        pies(
            -dt,
            7,
            6,
            3),
        pies(-dt, 7, 8, 3),

        pies(dt,  7, 0, 6), pies(dt,  7,  2,  6),  pies(dt,   7,   4,    6),
        pies(
            dt,
            7,
            6,
            6),
        pies(dt, 7, 8, 6),
        pies(dt, 6, 1, 7), pies(dt,  6,  7,  7),
        pies(dt, 5, 0, 9), pies(dt,  4,  1,  9),  pies(dt,   3,   2,    9),
        pies(
            dt,
            2,
            3,
            9),
        pies(dt, 1, 4, 9),
        pies(dt, 2, 5, 9), pies(dt,  3,  6,  9),  pies(dt,   4,   7,    9),
        pies(
            dt,
            5,
            8,
            9), // 车 23  24
        // 炮  25  26
        // 兵 27~31
    };

    for(int i=0;i<32;i++)
        piess.append(temp[i]);


    //  piess=temp;
    for (int i = 0; i != 32; ++i)
    {
        cbod[piess[i].getxx()][piess[i].getyy()] = i;
    }
    setFixedSize(d*10, d*11);
}

cces::~cces()
{

}

void cces::paintEvent(QPaintEvent *)
{
    QPainter painte(this);
    painte.setRenderHint(QPainter::Antialiasing,true);
    for (int i = 1; i != 11; ++i)
    {
        if ((i == 1) ||
            (i == 10)) painte.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        painte.drawLine(QPoint(d, i * d), QPoint(9 * d, i * d));

        if ((i == 1) ||
            (i == 10)) painte.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    }

    for (int j = 1; j != 10; ++j)
    {
        if ((j == 1) || (j == 9)) {
            painte.setPen(QPen(Qt::black, 3, Qt::SolidLine));
            painte.drawLine(QPoint(j * d, d), QPoint(j * d, 10 * d));
            painte.setPen(QPen(Qt::black, 1, Qt::SolidLine));
        }
        else {
            painte.drawLine(QPoint(j * d, d),     QPoint(j * d, 5 * d));
            painte.drawLine(QPoint(j * d, 6 * d), QPoint(j * d, 10 * d));
        }
    }
    int a[16][2][2] =
    {   { { 3, 1  }, { 1, 3  } },    { { 3, 1  }, { 7, 5  } },
        { { 4, 1  }, { 6, 3  } },    { { 6, 1  }, { 4, 3  } },
        { { 7, 1  }, { 3, 5  } },    { { 7, 1  }, { 9, 3  } },
        { { 1, 3  }, { 3, 5  } },    { { 7, 5  }, { 9, 3  } },
        { { 3, 6  }, { 1, 8  } },
        { { 3, 6  }, { 7, 10 } },    { { 7, 6  }, { 3, 10 } },
        { { 7, 6  }, { 9, 8  } },    { { 1, 8  }, { 3, 10 } },
        { { 4, 10 }, { 6, 8  } },
        { { 6, 10 }, { 4, 8  } },    { { 7, 10 }, { 9, 8  } }

    };

    for (int i = 0; i != 16; ++i)
    {
        painte.drawLine(QPoint(a[i][0][0] * d, a[i][0][1] * d),
                        QPoint(a[i][1][0] * d, a[i][1][1] * d));
    }
    int paoxian[4][2] = {
        {2,3},{8,3},{2,8},{8,8}
    };
    QPainterPath path;
    for (int i=0;i<4;i++)
    { 
        path.moveTo(paoxian[i][0] * d - d / 8, paoxian[i][1] * d - d / 4);
        path.lineTo(paoxian[i][0] * d - d / 8, paoxian[i][1] * d - d / 8);
        path.lineTo(paoxian[i][0] * d - d / 4, paoxian[i][1] * d - d / 8);
        painte.drawPath(path);
        path.moveTo(paoxian[i][0] * d + d / 8, paoxian[i][1] * d - d / 4);
        path.lineTo(paoxian[i][0] * d + d / 8, paoxian[i][1] * d - d / 8);
        path.lineTo(paoxian[i][0] * d + d / 4, paoxian[i][1] * d - d / 8);
        painte.drawPath(path);
        path.moveTo(paoxian[i][0] * d - d / 8, paoxian[i][1] * d + d / 4);
        path.lineTo(paoxian[i][0] * d - d / 8, paoxian[i][1] * d + d / 8);
        path.lineTo(paoxian[i][0] * d - d / 4, paoxian[i][1] * d + d / 8);
        painte.drawPath(path);
        path.moveTo(paoxian[i][0] * d + d / 8, paoxian[i][1] * d + d / 4);
        path.lineTo(paoxian[i][0] * d + d / 8, paoxian[i][1] * d + d / 8);
        path.lineTo(paoxian[i][0] * d + d / 4, paoxian[i][1] * d + d / 8);
        painte.drawPath(path);
    }
    painte.setFont(QFont(CHTXT("楷体"), d/2,  d));
    painte.drawText(QRect(2 * d, 4 * d + d / 2, 2*d, 2 * d), Qt::AlignCenter,
                    CHTXT("楚 河"));
    painte.drawText(QRect(6 * d, 4 * d + d / 2, 2 * d, 2 * d), Qt::AlignCenter,
                    CHTXT("汉 界"));
    for (int i = 0; i != 32; ++i)
    {
        if (piess[i].gett0() == 0) continue;

        if (piess[i]._sel)
        {
            painte.setBrush(QBrush(QColor("yellow")));
        }
        else
        {
            painte.setBrush(QBrush(QColor("orange")));
        }
        QRect rect((piess[i].getxx() + 1) * d - d / 2,
                   (piess[i].getyy() + 1) * d - d / 2, d, d);
        piess[i].gett0() ==
        1 ? painte.setPen(QColor(CL2)) : painte.setPen(QColor(CL1));
        painte.drawEllipse(QPoint((piess[i].getxx() + 1) * d,
                                  (piess[i].getyy() + 1) * d), d / 2, d / 2);

        switch (piess[i].gett1())
        {
        case 1:
            piess[i].gett0() ==
            1 ? painte.drawText(rect, Qt::AlignCenter,
                                CHTXT("将")) : painte
            .drawText(rect,
                      Qt::AlignCenter,
                                CHTXT("帅"));
            break;

        case 2:
            piess[i].gett0() ==
            1 ? painte.drawText(rect, Qt::AlignCenter,
                                CHTXT("仕")) : painte
            .drawText(rect,
                      Qt::AlignCenter,
                                CHTXT("士"));
            break;

        case 3:
            piess[i].gett0() ==
            1 ? painte.drawText(rect, Qt::AlignCenter,
                                CHTXT("象")) : painte
            .drawText(rect,
                      Qt::AlignCenter,
                      CHTXT("相"));
            break;

        case 5:
            piess[i].gett0() ==
                1 ? painte.drawText(rect, Qt::AlignCenter,
                    CHTXT("車")) : painte
                .drawText(rect,
                    Qt::AlignCenter,
                    CHTXT("车"));
            break;

        case 4:
            piess[i].gett0() ==
            1 ? painte.drawText(rect, Qt::AlignCenter,
                                CHTXT("馬")) : painte
            .drawText(rect,
                      Qt::AlignCenter,
                                CHTXT("马"));
            break;

        case 6:
            piess[i].gett0() ==
            1 ? painte.drawText(rect, Qt::AlignCenter,
                                CHTXT("砲")) : painte
            .drawText(rect,
                      Qt::AlignCenter,
                                CHTXT("炮"));
            break;

        case 7:
            piess[i].gett0() ==
            1 ? painte.drawText(rect, Qt::AlignCenter,
                                CHTXT("卒")) : painte
            .drawText(rect,
                      Qt::AlignCenter,
                      CHTXT("兵"));
            break;
        }
    }
}

void cces::mouseReleaseEvent(QMouseEvent *ev)
{
    if ((piess[4].gett0() == 0) || (piess[27].gett0() == 0)) return;

    int t1 = ev->pos().x();
    int t2 = ev->pos().y();

    if ((t1 < d / 2) || (t1 >= (9 * d + d / 2)) || (t2 < d / 2) ||
        (t2 >= (10 * d + d / 2))) return;

    QPoint tep(((t1 + d / 2) / d) * d, ((t2 + d / 2) / d) * d);
    int    tx = tep.x() / d - 1;
    int    ty = tep.y() / d - 1;

    if (id == -1)
    {
        if (cbod[tx][ty] == -1) return;

        if (mov01 && (piess[cbod[tx][ty]].gett0() == dt)) return;

        if (!mov01 && (piess[cbod[tx][ty]].gett0() == -dt)) return;

        id = cbod[tx][ty];
        piess[id]._sel = true;
    }
    else {
        if ((tx == piess[id].getxx()) && (ty == piess[id].getyy())) return;

        if ((cbod[tx][ty] != -1) &&
            (piess[cbod[tx][ty]].gett0() == piess[id].gett0()))
        {
            piess[id]._sel = false;
            id = cbod[tx][ty];
            piess[id]._sel = true;
        }
        else {
            if (!canMove(id, tx, ty)) return;

            if (cbod[tx][ty] != -1) piess[cbod[tx][ty]].settp0(0);
            cbod[piess[id].getxx()][piess[id].getyy()] = -1;
            cbod[tx][ty] = id;
            piess[id]._sel = false;
            piess[id].setxy(tx, ty);
            id = -1;
            mov01 = !mov01;
        }
    }
    update();
}

bool cces::canMove(int iid, int ixx, int iyy)
{
    QVector<QPair<int, int> > v;
    getPath(iid,v);
    return v.contains(QPair<int, int>(ixx,iyy));
}

void cces::getPath(int idd, QVector<QPair<int, int> > &v)
{

    int x=piess[idd].getxx();
    int y=piess[idd].getyy();
    switch (piess[idd].gett1())
    {
    case 1:
    {
        int xx= x+1,yy = y;
        if(xx<6&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
            v.push_back(QPair<int, int>(xx,yy));
        xx = x-1;
        if(xx>2&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
            v.push_back(QPair<int, int>(xx,yy));
        xx = x;
        yy = y+1;
        if(y<3)
        {
            if(yy<3&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
                v.push_back(QPair<int, int>(xx,yy));

        }else
        {
            if(yy<10&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx = x;
        yy = y-1;
        if(y<3)
        {
            if(yy>-1&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
                v.push_back(QPair<int, int>(xx,yy));

        }else
        {
            if(yy>6&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
                v.push_back(QPair<int, int>(xx,yy));
        }

        if(piess[4].getxx() == piess[27].getxx())
        {
            for (int n = piess[4].getyy() + 1; n != piess[27].getyy(); ++n)
            {
                if (cbod[x][n] != -1) return;
            }
            if(idd == 4)
                v.push_back(QPair<int, int>(x,piess[27].getyy()));
            else
                v.push_back(QPair<int, int>(x,piess[4].getyy()));
        }

        break;
    }
    case 2:
    {
        int  xx= x+1,yy = y+1;
        if(y<3)
        {
            if((yy<3&&xx<6)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
                v.push_back(QPair<int, int>(xx,yy));

        }else
        {
            if((yy<10&&xx<6)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx= x+1,yy = y-1;
        if(y<3)
        {
            if((yy>-1&&xx<6)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
                v.push_back(QPair<int, int>(xx,yy));
        }else
        {
            if((yy>6&xx<6)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx= x-1,yy = y+1;
        if(y<3)
        {
            if((yy<3&&xx>2)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
                v.push_back(QPair<int, int>(xx,yy));
        }else
        {
            if((yy<10&&xx>2)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx= x-1,yy = y-1;
        if(y<3)
        {
            if((yy>-1&&xx>2)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
                v.push_back(QPair<int, int>(xx,yy));
        }else
        {
            if((yy>6&&xx>2)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0()))
                v.push_back(QPair<int, int>(xx,yy));
        }
        break;
    }
    case 3:
    {
        int  xx= x+2,yy = y+2;
        if(y<5)
        {
            if((yy<5&&xx<9)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x+1][y+1]==-1)
                v.push_back(QPair<int, int>(xx,yy));

        }else
        {
            if((yy<10&&xx<9)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x+1][y+1]==-1)
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx= x+2,yy = y-2;
        if(y<5)
        {
            if((yy>-1&&xx<9)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x+1][y-1]==-1)
                v.push_back(QPair<int, int>(xx,yy));
        }else
        {
            if((yy>4&xx<9)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x+1][y-1]==-1)
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx= x-2,yy = y+2;
        if(y<5)
        {
            if((yy<5&&xx>-1)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x-1][y+1]==-1)
                v.push_back(QPair<int, int>(xx,yy));
        }else
        {
            if((yy<10&&xx>-1)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x-1][y+1]==-1)
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx= x-2,yy = y-2;
        if(y<5)
        {
            if((yy>-1&&xx>-1)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x-1][y-1]==-1)
                v.push_back(QPair<int, int>(xx,yy));
        }else
        {
            if((yy>4&&xx>-1)&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x-1][y-1]==-1)
                v.push_back(QPair<int, int>(xx,yy));
        }
        break;
    }
    case 5:
    {
        for(int i = x+1;i<9;i++)
        {
            if(cbod[i][y] ==-1)
            {
                v.push_back(QPair<int, int>(i,y));
            }else{
                if(piess[cbod[i][y]].gett0()!=piess[idd].gett0())
                    v.push_back(QPair<int, int>(i,y));
                break;
            }
        }
        for(int i = x-1;i>-1;i--)
        {
            if(cbod[i][y] ==-1)
            {
                v.push_back(QPair<int, int>(i,y));
            }else{
                if(piess[cbod[i][y]].gett0()!=piess[idd].gett0())
                    v.push_back(QPair<int, int>(i,y));
                break;
            }
        }
        for(int i = y+1;i<10;i++)
        {
            if(cbod[x][i] ==-1)
            {
                v.push_back(QPair<int, int>(x,i));
            }else{
                if(piess[cbod[x][i]].gett0()!=piess[idd].gett0())
                    v.push_back(QPair<int, int>(x,i));
                break;
            }
        }
        for(int i = y-1;i>-1;i--)
        {
            if(cbod[x][i] ==-1)
            {
                v.push_back(QPair<int, int>(x,i));
            }else{
                if(piess[cbod[x][i]].gett0()!=piess[idd].gett0())
                    v.push_back(QPair<int, int>(x,i));
                break;
            }
        }
        break;
    }
    case 4:
    {
        int  xx= x+2,yy = y+1;
        if(yy<10&&xx<9&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x+1][y]==-1)
        {
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx= x+2,yy = y-1;
        if(yy>-1&&xx<9&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x+1][y]==-1)
        {
                v.push_back(QPair<int, int>(xx,yy));
        }

        xx= x-2,yy = y+1;
        if(yy<10&&xx>-1&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x-1][y]==-1)
        {
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx= x-2,yy = y-1;
        if(yy>-1&&xx>-1&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x-1][y]==-1)
        {
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx= x+1,yy = y+2;
        if(yy<10&&xx<9&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x][y+1]==-1)
        {
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx= x+1,yy = y-2;
        if(yy>-1&&xx<9&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x][y-1]==-1)
        {
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx= x-1,yy = y+2;
        if(yy<10&&xx>-1&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x][y+1]==-1)
        {
                v.push_back(QPair<int, int>(xx,yy));
        }
        xx= x-1,yy = y-2;
        if(yy>-1&&xx>-1&&(cbod[xx][yy]==-1||piess[cbod[xx][yy]].gett0()!=piess[idd].gett0())&&cbod[x][y-1]==-1)
        {
                v.push_back(QPair<int, int>(xx,yy));
        }
        break;
    }
    case 6:
    {
        bool stat =false;
        for(int i = x+1;i<9;i++)
        {
            if(cbod[i][y] ==-1)
            {
                if(!stat)
                     v.push_back(QPair<int, int>(i,y));
            }else{
                if(stat)
                {
                    if(piess[cbod[i][y]].gett0()!=piess[idd].gett0())
                        v.push_back(QPair<int, int>(i,y));
                    break;
                }

                stat = true;

            }
        }
        stat =false;
        for(int i = x-1;i>-1;i--)
        {
            if(cbod[i][y] ==-1)
            {
                if(!stat)
                v.push_back(QPair<int, int>(i,y));
            }else{
                if(stat)
                {
                    if(piess[cbod[i][y]].gett0()!=piess[idd].gett0())
                        v.push_back(QPair<int, int>(i,y));
                    break;
                }

                stat = true;
            }
        }
        stat =false;
        for(int i = y+1;i<10;i++)
        {
            if(cbod[x][i] ==-1)
            {
                if(!stat)
                v.push_back(QPair<int, int>(x,i));
            }else{
                if(stat)
                {
                    if(piess[cbod[x][i]].gett0()!=piess[idd].gett0())
                        v.push_back(QPair<int, int>(x,i));
                    break;
                }

                stat = true;
            }
        }
        stat =false;
        for(int i = y-1;i>-1;i--)
        {
            if(cbod[x][i] ==-1)
            {
                if(!stat)
                v.push_back(QPair<int, int>(x,i));
            }else{
                if(stat)
                {
                    if(piess[cbod[x][i]].gett0()!=piess[idd].gett0())
                        v.push_back(QPair<int, int>(x,i));
                    break;
                }

                stat = true;
            }
        }
        break;
    }
    case 7:
        if(idd < 16)
        {
            if(y<5)
            {
                if(cbod[x][y+1]==-1||piess[cbod[x][y+1]].gett0()!=piess[idd].gett0())
                    v.push_back(QPair<int, int>(x,y+1));
            }else
            {
                if(y+1<10&&(cbod[x][y+1]==-1||piess[cbod[x][y+1]].gett0()!=piess[idd].gett0()))
                    v.push_back(QPair<int, int>(x,y+1));
                if(x+1<9&&(cbod[x+1][y]==-1||piess[cbod[x+1][y]].gett0()!=piess[idd].gett0()))
                    v.push_back(QPair<int, int>(x+1,y));
                if(x-1>-1&&(cbod[x-1][y]==-1||piess[cbod[x-1][y]].gett0()!=piess[idd].gett0()))
                    v.push_back(QPair<int, int>(x-1,y));
            }

        }else
        {
            if(y<5)
            {
                if(y-1>-1&&(cbod[x][y-1]==-1||piess[cbod[x][y-1]].gett0()!=piess[idd].gett0()))
                    v.push_back(QPair<int, int>(x,y-1));
                if(x+1<9&&(cbod[x+1][y]==-1||piess[cbod[x+1][y]].gett0()!=piess[idd].gett0()))
                    v.push_back(QPair<int, int>(x+1,y));
                if(x-1>-1&&(cbod[x-1][y]==-1||piess[cbod[x-1][y]].gett0()!=piess[idd].gett0()))
                    v.push_back(QPair<int, int>(x-1,y));
            }else
            {
                if(cbod[x][y-1]==-1||piess[cbod[x][y-1]].gett0()!=piess[idd].gett0())
                    v.push_back(QPair<int, int>(x,y-1));
            }
        }

        break;
    default:
        break;
    }
}
