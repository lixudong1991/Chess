#include "pies.h"

pies::pies(int t0, int t1, int x, int y) : tp0(t0),tp_0(t0),tp1(t1), xx(x), yy(y) {
  switch (tp1) {
    case 1:
      zhi = 10000 * tp0;
      m_img=(tp0 ==1?QPixmap(":/9.png"):QPixmap(":/4.png"));
      break;  //将
    case 2:
      zhi = 3 * tp0;
      m_img=(tp0 ==1?QPixmap(":/11.png"):QPixmap(":/3.png"));
      break;  //士
    case 3:
      zhi = 3 * tp0;
      m_img=(tp0 ==1?QPixmap(":/12.png"):QPixmap(":/2.png"));
      break;  //象
    case 4:
      zhi = 8 * tp0;
      m_img=(tp0 ==1?QPixmap(":/8.png"):QPixmap(":/1.png"));
      break;  //马
    case 5:
      zhi = 20 * tp0;
      m_img=(tp0 ==1?QPixmap(":/7.png"):QPixmap(":/0.png"));
      break;  //车
    case 6:
      zhi = 10 * tp0;  //炮
      m_img=(tp0 ==1?QPixmap(":/10.png"):QPixmap(":/5.png"));
      break;
    case 7:
      zhi = 1 * tp0;  //兵
      m_img=(tp0 ==1?QPixmap(":/13.png"):QPixmap(":/6.png"));
      break;
    default:
      break;
  }
}
