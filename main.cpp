#include<QApplication>
#include"netcces.h"
#include<QMessageBox>
int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
   QMessageBox::StandardButton ret=QMessageBox::question(NULL,"select","作为服务器启动?");
   int bo=1;
  if(ret==QMessageBox::Yes) bo=-1;
  netcces w(bo);
    w.setFixedSize(400,440);
    w.show();
    return app.exec();
}
