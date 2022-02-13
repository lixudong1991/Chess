#include <QApplication>
#include <QMessageBox>
#include "bot.h"
#include "netcces.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  /*int  ret=QMessageBox::question(NULL,"select",QString::fromLocal8Bit("作为服务器启动?"),QMessageBox::Yes,QMessageBox::No);
  int  bo=1;
  if (ret == QMessageBox::Yes)
	         bo=-1;
	  netcces w(bo);	
	  */
  bot w;
  w.show();
  return app.exec();
}
