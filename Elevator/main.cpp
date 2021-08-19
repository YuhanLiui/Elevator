#include "mymainwindow.h"
#include <QApplication>
#include <iostream>
#include <stdlib.h>
#include <QDebug>
#include <QDateTime>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    controller con;
    elevator ele[6];
    myMainWindow w(ele);
    for(int i=0;i<6;i++){//初始设置
        ele[i].setWindowTitle("elevator"+QString::number(i+1));
        ele[i].setElev(ele);
        ele[i].setmyMainwindow(&w);
        ele[i].setController(&con);
        ele[i].setId(i+1);
        ele[i].show();
    }
    w.show();
    return a.exec();
}
