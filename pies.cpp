#include "pies.h"

pies::pies(int t0, int t1, int x, int y) : tp0(t0),tp_0(t0),tp1(t1), xx(x), yy(y) {
  switch (tp1) {
    case 1:
      zhi = 10000 * tp0;
      break;  //将
    case 2:
      zhi = 3 * tp0;
      break;  //士
    case 3:
      zhi = 3 * tp0;
      break;  //象
    case 4:
      zhi = 8 * tp0;
      break;  //马
    case 5:
      zhi = 20 * tp0;
      break;  //车
    case 6:
      zhi = 10 * tp0;  //炮
      break;
    case 7:
      zhi = 1 * tp0;  //兵
      break;
    default:
      break;
  }
}
