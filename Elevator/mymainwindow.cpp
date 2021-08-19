#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include <iostream>
#include <stdlib.h>
#include <QDebug>
#include "elevator.h"
#include <QDateTime>
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>
#include <QFile>
myMainWindow::myMainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::myMainWindow)
{
    ui->setupUi(this);
    connect(ui->diable1,&QPushButton::clicked,this,&myMainWindow::on_diable1clicked);
    connect(ui->disable2,&QPushButton::clicked,this,&myMainWindow::on_disable2clicked);
    connect(ui->disable3,&QPushButton::clicked,this,&myMainWindow::on_disable3clicked);
    connect(ui->disable4,&QPushButton::clicked,this,&myMainWindow::on_disable4clicked);
    connect(ui->disable5,&QPushButton::clicked,this,&myMainWindow::on_disable5clicked);
    connect(ui->disable6,&QPushButton::clicked,this,&myMainWindow::on_disable6clicked);
    connect(ui->isbusy,&QPushButton::clicked,this,&myMainWindow::setIsbusy);
}
myMainWindow::myMainWindow(elevator* ele,QWidget *parent) :
QMainWindow(parent),
ui(new Ui::myMainWindow)
{
    ui->setupUi(this);
    el=ele;
    connect(ui->emergency,&QPushButton::clicked,this,&myMainWindow::on_emergencyclicked);
    connect(ui->diable1,&QPushButton::clicked,this,&myMainWindow::on_diable1clicked);
    connect(ui->disable2,&QPushButton::clicked,this,&myMainWindow::on_disable2clicked);
    connect(ui->disable3,&QPushButton::clicked,this,&myMainWindow::on_disable3clicked);
    connect(ui->disable4,&QPushButton::clicked,this,&myMainWindow::on_disable4clicked);
    connect(ui->disable5,&QPushButton::clicked,this,&myMainWindow::on_disable5clicked);
    connect(ui->disable6,&QPushButton::clicked,this,&myMainWindow::on_disable6clicked);
    connect(ui->isbusy,&QPushButton::clicked,this,&myMainWindow::setIsbusy);
}
Ui::myMainWindow* myMainWindow::getUi(){
    return ui;
}
myMainWindow::~myMainWindow()
{
    delete ui;
}
elevator* myMainWindow::getele(){
    return el;
}
/***************************************************************************************************/
/*
* 功能：初始化控制端类（请求屏蔽位为false，故障为为false，特殊情况为false，忙闲为闲，请求全为无请求）；
* 输入参数：无
* 输出参数：无
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/15
*/
controller::controller(){
    //elevotar_ui.show();
    for(int i=0;i<6;i++){//初始化屏蔽位和故障位
        shielding[i]=false;
        fault[i]=false;
    }
    for (int i=0;i<56;++i) {//初始化请求位
        for (int j=0;j<6;j++) {
            re[i][j].floor=i+1;
            //qDebug()<<re[i][j].floor;
            re[i][j].direction=0;
        }
    }
    emergency=false;//初始化紧急位
    isbusy=false;//初始化忙闲位
}
/*
* 功能：释放类资源；
* 输入参数：无
* 输出参数：无
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/15
*/
controller::~controller(){

}
/*
* 功能：返回控制对象屏蔽位数组；
* 输入参数：无
* 输出参数：控制对象的屏蔽位数组的地址
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/15
*/
bool* controller::getShielding(){
    return shielding;
}

/*
* 功能：返回控制对象故障位数组；
* 输入参数：无
* 输出参数：控制对象的故障位数组的地址
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/15
*/
bool* controller::getFault(){
    return fault;
}

/*
* 功能：返回控制对象紧急位；
* 输入参数：无
* 输出参数：控制对象的紧急位地址
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/15
*/
bool* controller::getEmergen(){
    return &emergency;
}

/*
* 功能：返回控制对象忙闲；
* 输入参数：无
* 输出参数：控制对象的忙闲的地址
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/15
*/
bool* controller::getIsbusy(){
    return &isbusy;
}

/*
* 功能：返回控制对象请求数组；
* 输入参数：请求所在楼层数
* 输出参数：控制对象的该层请求的数组首地址
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/15
*/
request* controller::getRequest(int i){
    return re[i];
}

/*
* 功能：如果有电梯发送故障，对相应的电梯进行故障处理（发出某某电梯发生故障警告，调用当前楼层数次下楼函数，然后开门，
* 并对外呈现故障状态，并屏蔽请求，直到复位函数将对外呈现故障位进行取消）；
* 输入参数：故障位数组，界面的引用
* 输出参数：无
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/15
*/
void controller::faultAct(bool* fault,myMainWindow& eui){
    //int i;
    //Test w;
    for (int j=0;j<6;j++) {
        if(fault[j]){
            eui.getShielding()[j]=true;//将相应电梯的请求屏蔽掉
            while(eui.getele()->getCurrFloor()!=1){
                eui.getele()->decrementFloor();
                qDebug()<<"调用电梯对象的下楼函数直到底楼";
            }
            qDebug()<<"第"<<j+1<<"部电梯发生故障";
            QString str="第"+QString::number(j+1)+"部电梯发生故障";
            QMessageBox::warning(&eui,"warning",str);
            if(0==j){//在相应故障显示屏处显示故障
                eui.getUi()->elevator1->setText("故障");
                QDateTime da_time;
                QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
                QFile file("../Elevator/Log.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
                {
                    //QMessageBox::warning(this,"错误","文件打开失败，信息没有保存！","确定");
                    return;
                }

                QTextStream out(&file);//写入
                out <<time_str+":"+ "电梯1发生故障"<<"\n";
                file.close();
            }
            else if(1==j){//在相应故障显示屏处显示故障
                eui.getUi()->elevotar2->setText("故障");
                QDateTime da_time;
                QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
                QFile file("../Elevator/Log.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
                {
                    //QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
                    return;
                }

                QTextStream out(&file);//写入
                out <<time_str+":"+ "电梯2发生故障"<<"\n";
                file.close();
            }
            else if(2==j){//在相应故障显示屏处显示故障
                eui.getUi()->elevotar3->setText("故障");
                QDateTime da_time;
                QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
                QFile file("../Elevator/Log.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
                {
                    //QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
                    return;
                }

                QTextStream out(&file);//写入
                out <<time_str+":"+ "电梯3发生故障"<<"\n";
                file.close();
            }
            else if(3==j){//在相应故障显示屏处显示故障
                eui.getUi()->elevotar4->setText("故障");
                QDateTime da_time;
                QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
                QFile file("../Elevator/Log.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
                {
                    //QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
                    return;
                }

                QTextStream out(&file);//写入
                out <<time_str+":"+ "电梯4发生故障"<<"\n";
                file.close();
            }
            else if(4==j){//在相应故障显示屏处显示故障
                eui.getUi()->elevotar5->setText("故障");
                QDateTime da_time;
                QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
                QFile file("../Elevator/Log.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
                {
                    //QMessageBox::warning(this,"warning","error","ok");
                    return;
                }

                QTextStream out(&file);//写入
                out <<time_str+":"+ "电梯5发生故障"<<"\n";
                file.close();
            }
            else if(5==j){//在相应故障显示屏处显示故障
                eui.getUi()->elevator6->setText("故障");
                QDateTime da_time;
                QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
                QFile file("../Elevator/Log.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
                {
                    //QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
                    return;
                }

                QTextStream out(&file);//写入
                out <<time_str+":"+ "电梯6发生故障"<<"\n";
                file.close();
            }
        }
        else if(!(fault[j])){
            eui.getShielding()[j]=false;//将相应电梯的请求屏蔽解除
            if(0==j){//在相应故障显示屏处显示故障
                eui.getUi()->elevator1->setText(" ");
                QDateTime da_time;
                QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
                QFile file("../Elevator/Log.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
                {
                    //QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
                    return;
                }

                QTextStream out(&file);//写入
                out <<time_str+":"+ "电梯1无故障"<<"\n";
                file.close();
            }
            else if(1==j){//在相应故障显示屏处显示故障
                eui.getUi()->elevotar2->setText(" ");
                QDateTime da_time;
                QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
                QFile file("../Elevator/Log.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
                {
                    //QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
                    return;
                }

                QTextStream out(&file);//写入
                out <<time_str+":"+ "电梯2无故障"<<"\n";
                file.close();
            }
            else if(2==j){//在相应故障显示屏处显示故障
                eui.getUi()->elevotar3->setText(" ");
                QDateTime da_time;
                QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
                QFile file("../Elevator/Log.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
                {
                    //QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
                    return;
                }

                QTextStream out(&file);//写入
                out <<time_str+":"+ "电梯3无故障"<<"\n";
                file.close();
            }
            else if(3==j){//在相应故障显示屏处显示故障
                eui.getUi()->elevotar4->setText(" ");
                QDateTime da_time;
                QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
                QFile file("../Elevator/Log.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
                {
                    //QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
                    return;
                }

                QTextStream out(&file);//写入
                out <<time_str+":"+ "电梯4无故障"<<"\n";
                file.close();
            }
            else if(4==j){//在相应故障显示屏处显示故障
                eui.getUi()->elevotar5->setText(" ");
                QDateTime da_time;
                QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
                QFile file("../Elevator/Log.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
                {
                    //QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
                    return;
                }

                QTextStream out(&file);//写入
                out <<time_str+":"+ "电梯5无故障"<<"\n";
                file.close();
            }
            else if(5==j){//在相应故障显示屏处显示故障
                eui.getUi()->elevator6->setText(" ");
                QDateTime da_time;
                QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
                QFile file("../Elevator/Log.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
                {
                    //QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
                    return;
                }

                QTextStream out(&file);//写入
                out <<time_str+":"+ "电梯6无故障"<<"\n";
                file.close();
            }
        }
    }
    return;
}


/*
* 功能：指派相应的电梯响应相应的请求（先判断当前是闲还是忙，如果是闲，就判断该请求上下五层楼内是否有同向电梯，如果有，就将该请求写入同向最近电梯
* 内，如果上下五层内，电梯都是异向或上下五层楼内没有电梯则将该请求写入同向最近的电梯中或者最先到达该楼层的异向电梯中，如果忙，就看按规则（第一台1,2
* 7.....以此类推，第六台可以到达所有楼层）看前五台中可以到达该楼层的电梯以及第六台，看那台同向近，那台近就分配给那台（当第六台满载时就算它近也不分配给它）
* 如果两台中有一台异向，就分配给同向，如果两个都异向，就分配给最先到达该楼层的电梯中）；
* 输入参数：请求数组
* 输出参数：无
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/15
*/
void controller::assignAct(controller& con/*in*/,myMainWindow& eui,elevator* ele){

    for(int a=0;a<6;a++){
        ele[a].setT(ele[a].getT()+1);
        if(ele[a].getT()==2){
            ele[a].setT(0);
        }
        ele[a].setT1(ele[a].getT1()+1);
        if(ele[a].getT1()==2){
            ele[a].setT1(0);
        }
    }
    for (int i=0;i<56;i++) {//找到请求位置
        for (int j=0;j<6;j++) {
            if(con.getRequest(i)[j].direction!=0){
                if((*(eui.getIsbusy()))&&(!(eui.getShielding()[j]))){//电梯此时为忙，且该部电梯的请求没有被屏蔽
                    if((((i)==1)||((i)==2)||((i)==7)||((i)==12)||((i)==17)||((i)==22)||((i)==27)||((i+1)==32)||((i)==37)||((i)==42)||((i)==47)||((i)==52))&&((!(eui.getShielding()[0]))||(!(eui.getShielding()[5])))){
                        /*如果该请求按规则是在第一台电梯和第六台电梯*/
                        if(con.getRequest(i)[j].direction==1){//请求向上
                            if(eui.getShielding()[0]){
                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();
                                return;
                            }
                            else if(eui.getShielding()[5]){
                                ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();
                                return;
                            }
                            else if(((i+1)>=(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()==1)||(ele[0].getMoveStatus()==0))&&(ele[0].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动且第两部电梯没有满载
                                if(((i+1)-ele[0].getCurrFloor())>((i+1)-ele[5].getCurrFloor())){
                                    //第一部比第六部离得远
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//将请求写入第六部电梯内
                                    return;
                                }
                                else {
                                    ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();//将请求写入第一部电梯中
                                    return;
                                }
                            }
                            else if(((i+1)>=(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()==1)||(ele[0].getMoveStatus()==0))&&(ele[0].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()==1)){
                                //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第六部电梯满载
                                ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();//将请求写入第一部电梯
                                return;
                            }
                            else if(((i+1)>=(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()==1)||(ele[0].getMoveStatus()==0))&&(ele[0].ifFull()==1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第一部电梯满载
                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//将请求写入第六部电梯
                                return;
                            }
                            else if(((i+1)>=(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()==1)||(ele[0].getMoveStatus()==0))&&((i+1)>=(ele[5].getCurrFloor()))&&(ele[5].getMoveDirection()==-1)){
                                //第一和第六部电梯都在请求下，但电梯六的运动方向是向下
                                ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();//将请求写到第一部电梯内
                                return;
                            }
                            else if(((i+1)>=(ele[0].getCurrFloor()))&&(ele[0].getMoveDirection()==-1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))){
                                //第一和第六部电梯都在请求下，但电梯一的运动方向是向下
                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//将请求写到第六部电梯内
                                return;
                            }
                            else if(((i+1)>=(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()==1)||(ele[0].getMoveStatus()==0))&&(ele[0].ifFull()!=1)&&((i+1)<(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()!=0))) {
                                //第一部电梯在请求下且向上运动或者不运动，但第六部电梯在请求上
                                ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();//将请求写入第一部电梯内
                                return;
                            }
                            else if(((i+1)<(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()!=0))&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                //第六部电梯在请求下且向上运动或者不运动，但第一部电梯在请求上
                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//将请求写入第六部电梯内
                                return;
                            }
                            else if(((i+1)>=(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()==1)||(ele[0].getMoveStatus()==0))&&(ele[0].ifFull()!=1)&&((i+1)<(ele[5].getCurrFloor()))&&((ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                //第一部电梯在请求下且向上运动或者不运动，第六部电梯在请求上但不运动
                                if(((i+1)-ele[0].getCurrFloor())>(ele[5].getCurrFloor()-(i+1))){
                                    //第一部电梯离得更远
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//请求写入第六部电梯内
                                    return;
                                }
                                else {
                                    ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();//电梯写入第一部电梯内
                                }
                            }
                            else if(((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)&&((i+1)<(ele[0].getCurrFloor()))&&((ele[0].getMoveStatus()==0))&&(ele[0].ifFull()!=1)) {
                                //第六部电梯在请求下且向上运动或者不运动，第一部电梯在请求上但不运动
                                if(((i+1)-ele[0].getCurrFloor())>(ele[5].getCurrFloor()-(i+1))){
                                    //第一部电梯离得更远
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//请求写入第六部电梯内
                                    return;
                                }
                                else {
                                    ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();//电梯写入第一部电梯内
                                    return;
                                }
                            }
                            /********************************************************************************************************************/
                            else if(((i+1)<ele[0].getCurrFloor())&&(ele[0].getMoveStatus()==0)&&((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)){
                                //电梯1，6都在请求上，但是都不运动
                                if((ele[0].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                    //电梯1离得远
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//写入电梯六
                                    return;
                                }
                                else {
                                    ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();//写入电梯1
                                    return;
                                }
                            }
                            else if(((i+1)<ele[0].getCurrFloor())&&(ele[0].getMoveStatus()==0)&&((i+1)<ele[5].getCurrFloor())){
                                //都在上面，但只有电梯1在不运动
                                ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();//写入电梯一
                                return;
                            }
                            else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)&&((i+1)<ele[0].getCurrFloor())){
                                //都在上面，但只有电梯6在不运动
                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//写入电梯6
                                return;
                            }
                            else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()!=0)&&((i+1)<ele[0].getCurrFloor())&&(ele[0].getMoveStatus()!=0)){
                                //都在上且都在运动
                                if((ele[5].getMoveStatus()==1)&&(ele[0].getMoveStatus()==1)){
                                    //都在向上运动
                                    if((ele[0].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                        //电梯1离得近
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//写入电梯六
                                        return;
                                    }
                                    else {
                                        ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();//写入电梯1
                                        return;
                                    }
                                }
                                else if((ele[5].getMoveStatus()==-1)&&(ele[0].getMoveStatus()==-1)){
                                    //都在向下运动
                                    if((ele[0].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                        //电梯1离得近
                                        ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();//写入电梯一
                                        return;
                                    }
                                    else {
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//写入电梯六
                                        return;
                                    }
                                }
                                else if((ele[5].getMoveStatus()==1)&&(ele[0].getMoveStatus()==-1)){
                                    //六上一下
                                    ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();//写入电梯1
                                    return;
                                }
                                else{
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//写入电梯6
                                    return;
                                }
                            }
                            else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==-1)&&((i+1)>ele[0].getCurrFloor())&&(ele[0].getMoveStatus()==-1)){
                                //都在下且都在向下运动
                                if(((i+1)-ele[0].getCurrFloor())<((i+1)-ele[5].getCurrFloor())){
                                    //电梯1离得近
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//写入电梯6
                                    return;
                                }
                                else {
                                    ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();//写入电梯1
                                    return;
                                }

                            }
                            else if(((i+1)<ele[0].getCurrFloor())&&(ele[0].getMoveStatus()==1)&&((i+1)>ele[5].getCurrFloor())){
                                //1在上向上运动，6在下
                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();//写入6
                                return;
                            }
                            else{
                                ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();
                                return;
                            }
                }


                if(re[i][j].direction==-1){//请求向下
                    if(eui.getShielding()[0]){
                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();
                        return;
                    }
                    else if(eui.getShielding()[5]){
                        ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct();
                        return;
                    }
                    else if(((i+1)<=(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()==-1)||(ele[0].getMoveStatus()==0))&&(ele[0].ifFull()!=1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                        //第一部和第六部电梯都和请求同向或者在请求楼的电梯没有运动且第两部电梯没有满载
                        if(((i+1)-ele[0].getCurrFloor())<((i+1)-ele[5].getCurrFloor())){
                            //第一部比第六部离得远
                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯内
                            return;
                        }
                        else {
                            ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();//将请求写入第一部电梯中
                            return;
                        }
                    }
                    else if(((i+1)<=(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()==-1)||(ele[0].getMoveStatus()==0))&&(ele[0].ifFull()!=1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()==1)){
                        //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第六部电梯满载
                        ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();//将请求写入第一部电梯
                        return;
                    }
                    else if(((i+1)<=(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()==-1)||(ele[0].getMoveStatus()==0))&&(ele[0].ifFull()==1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                        //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第一部电梯满载
                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯
                        return;
                    }
                    else if(((i+1)<=(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()==-1)||(ele[0].getMoveStatus()==0))&&((i+1)<=(ele[5].getCurrFloor()))&&(ele[5].getMoveDirection()==1)){
                        //第一和第六部电梯都在请求下，但电梯六的运动方向是向下
                        ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();//将请求写到第一部电梯内
                        return;
                    }
                    else if(((i+1)<=(ele[0].getCurrFloor()))&&(ele[0].getMoveDirection()==1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))){
                        //第一和第六部电梯都在请求下，但电梯一的运动方向是向下
                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写到第六部电梯内
                        return;
                    }
                    else if(((i+1)<=(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()==-1)||(ele[0].getMoveStatus()==0))&&(ele[0].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()!=0))) {
                        //第一部电梯在请求下且向上运动或者不运动，但第六部电梯在请求上
                        ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();//将请求写入第一部电梯内
                        return;
                    }
                    else if(((i+1)>(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()!=0))&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                        //第六部电梯在请求下且向上运动或者不运动，但第一部电梯在请求上
                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯内
                        return;
                    }
                    else if(((i+1)<=(ele[0].getCurrFloor()))&&((ele[0].getMoveDirection()==-1)||(ele[0].getMoveStatus()==0))&&(ele[0].ifFull()!=1)&&((i+1)>(ele[5].getCurrFloor()))&&((ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                        //第一部电梯在请求下且向上运动或者不运动，第六部电梯在请求上但不运动
                        if(((i+1)-ele[0].getCurrFloor())<(ele[5].getCurrFloor()-(i+1))){
                            //第一部电梯离得更远
                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//请求写入第六部电梯内
                            return;
                        }
                        else {
                            ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();//电梯写入第一部电梯内
                        }
                    }
                    else if(((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)&&((i+1)>(ele[0].getCurrFloor()))&&((ele[0].getMoveStatus()==0))&&(ele[0].ifFull()!=1)) {
                        //第六部电梯在请求下且向上运动或者不运动，第一部电梯在请求上但不运动
                        if(((i+1)-ele[0].getCurrFloor())<(ele[5].getCurrFloor()-(i+1))){
                            //第一部电梯离得更远
                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//请求写入第六部电梯内
                            return;
                        }
                        else {
                            ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();//电梯写入第一部电梯内
                            return;
                        }
                    }
                    /********************************************************************************************************************/
                    else if(((i+1)>ele[0].getCurrFloor())&&(ele[0].getMoveStatus()==0)&&((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)){
                        //电梯1，6都在请求上，但是都不运动
                        if((ele[0].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                            //电梯1离得远
                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                            return;
                        }
                        else {
                            ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();//写入电梯1
                            return;
                        }
                    }
                    else if(((i+1)>ele[0].getCurrFloor())&&(ele[0].getMoveStatus()==0)&&((i+1)>ele[5].getCurrFloor())){
                        //都在上面，但只有电梯1在不运动
                        ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();//写入电梯一
                        return;
                    }
                    else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)&&((i+1)>ele[0].getCurrFloor())){
                        //都在上面，但只有电梯6在不运动
                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                        return;
                    }
                    else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()!=0)&&((i+1)>ele[0].getCurrFloor())&&(ele[0].getMoveStatus()!=0)){
                        //都在上且都在运动
                        if((ele[5].getMoveStatus()==-1)&&(ele[0].getMoveStatus()==-1)){
                            //都在向上运动
                            if((ele[0].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                //电梯1离得近
                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                return;
                            }
                            else {
                                ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();//写入电梯1
                                return;
                            }
                        }
                        else if((ele[5].getMoveStatus()==1)&&(ele[0].getMoveStatus()==1)){
                            //都在向下运动
                            if((ele[0].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                //电梯1离得近
                                ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();//写入电梯一
                                return;
                            }
                            else {
                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                return;
                            }
                        }
                        else if((ele[5].getMoveStatus()==-1)&&(ele[0].getMoveStatus()==1)){
                            //六上一下
                            ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();//写入电梯1
                            return;
                        }
                        else{
                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                            return;
                        }
                    }
                    else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==1)&&((i+1)<ele[0].getCurrFloor())&&(ele[0].getMoveStatus()==1)){
                        //都在下且都在向下运动
                        if(((i+1)-ele[0].getCurrFloor())>((i+1)-ele[5].getCurrFloor())){
                            //电梯1离得近
                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                            return;
                        }
                        else {
                            ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();//写入电梯1
                            return;
                        }

                    }
                    else if(((i+1)>ele[0].getCurrFloor())&&(ele[0].getMoveStatus()==-1)&&((i+1)<ele[5].getCurrFloor())){
                        //1在上向上运动，6在下
                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入6
                        return;
                    }
                    else{
                        ele[0].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[0].EleAct2();
                        return;
                    }
                            }
                        }


                        else if((((i)==1)||((i)==3)||((i)==8)||((i)==13)||((i)==18)||((i)==23)||((i)==28)||((i)==33)||((i)==38)||((i)==43)||((i)==48)||((i)==53))&&((!(eui.getShielding()[1]))||(!(eui.getShielding()[5])))){
                            /*如果该请求按规则是在第一台电梯和第六台电梯*/
                            if(con.getRequest(i)[j].direction==1){//请求向上
                                if(eui.getShielding()[1]){
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();
                                    return;
                                }
                                else if(eui.getShielding()[5]){
                                    ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();
                                    return;
                                }
                                else if(((i+1)>=(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()==1)||(ele[1].getMoveStatus()==0))&&(ele[1].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                    //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动且第两部电梯没有满载
                                    if(((i+1)-ele[1].getCurrFloor())>((i+1)-ele[5].getCurrFloor())){
                                        //第一部比第六部离得远
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//将请求写入第六部电梯内
                                        return;
                                    }
                                    else {
                                        ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//将请求写入第一部电梯中
                                        return;
                                    }
                                }
                                else if(((i+1)>=(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()==1)||(ele[1].getMoveStatus()==0))&&(ele[1].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()==1)){
                                    //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第六部电梯满载
                                    ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//将请求写入第一部电梯
                                    return;
                                }
                                else if(((i+1)>=(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()==1)||(ele[1].getMoveStatus()==0))&&(ele[1].ifFull()==1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                    //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第一部电梯满载
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//将请求写入第六部电梯
                                    return;
                                }
                                else if(((i+1)>=(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()==1)||(ele[1].getMoveStatus()==0))&&((i+1)>=(ele[5].getCurrFloor()))&&(ele[5].getMoveDirection()==-1)){
                                    //第一和第六部电梯都在请求下，但电梯六的运动方向是向下
                                    ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//将请求写到第一部电梯内
                                    return;
                                }
                                else if(((i+1)>=(ele[1].getCurrFloor()))&&(ele[1].getMoveDirection()==-1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))){
                                    //第一和第六部电梯都在请求下，但电梯一的运动方向是向下
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//将请求写到第六部电梯内
                                    return;
                                }
                                else if(((i+1)>=(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()==1)||(ele[1].getMoveStatus()==0))&&(ele[1].ifFull()!=1)&&((i+1)<(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()!=0))) {
                                    //第一部电梯在请求下且向上运动或者不运动，但第六部电梯在请求上
                                    ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//将请求写入第一部电梯内
                                    return;
                                }
                                else if(((i+1)<(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()!=0))&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                    //第六部电梯在请求下且向上运动或者不运动，但第一部电梯在请求上
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//将请求写入第六部电梯内
                                    return;
                                }
                                else if(((i+1)>=(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()==1)||(ele[1].getMoveStatus()==0))&&(ele[1].ifFull()!=1)&&((i+1)<(ele[5].getCurrFloor()))&&((ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                    //第一部电梯在请求下且向上运动或者不运动，第六部电梯在请求上但不运动
                                    if(((i+1)-ele[1].getCurrFloor())>(ele[5].getCurrFloor()-(i+1))){
                                        //第一部电梯离得更远
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//请求写入第六部电梯内
                                        return;
                                    }
                                    else {
                                        ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//电梯写入第一部电梯内
                                    }
                                }
                                else if(((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)&&((i+1)<(ele[1].getCurrFloor()))&&((ele[1].getMoveStatus()==0))&&(ele[1].ifFull()!=1)) {
                                    //第六部电梯在请求下且向上运动或者不运动，第一部电梯在请求上但不运动
                                    if(((i+1)-ele[1].getCurrFloor())>(ele[5].getCurrFloor()-(i+1))){
                                        //第一部电梯离得更远
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//请求写入第六部电梯内
                                        return;
                                    }
                                    else {
                                        ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//电梯写入第一部电梯内
                                        return;
                                    }
                                }
                                /********************************************************************************************************************/
                                else if(((i+1)<ele[1].getCurrFloor())&&(ele[1].getMoveStatus()==0)&&((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)){
                                    //电梯1，6都在请求上，但是都不运动
                                    if((ele[1].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                        //电梯1离得远
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入电梯六
                                        return;
                                    }
                                    else {
                                        ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入电梯1
                                        return;
                                    }
                                }
                                else if(((i+1)<ele[1].getCurrFloor())&&(ele[1].getMoveStatus()==0)&&((i+1)<ele[5].getCurrFloor())){
                                    //都在上面，但只有电梯1在不运动
                                    ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入电梯一
                                    return;
                                }
                                else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)&&((i+1)<ele[1].getCurrFloor())){
                                    //都在上面，但只有电梯6在不运动
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入电梯6
                                    return;
                                }
                                else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()!=0)&&((i+1)<ele[1].getCurrFloor())&&(ele[1].getMoveStatus()!=0)){
                                    //都在上且都在运动
                                    if((ele[5].getMoveStatus()==1)&&(ele[1].getMoveStatus()==1)){
                                        //都在向上运动
                                        if((ele[1].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                            //电梯1离得近
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入电梯六
                                            return;
                                        }
                                        else {
                                            ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入电梯1
                                            return;
                                        }
                                    }
                                    else if((ele[5].getMoveStatus()==-1)&&(ele[1].getMoveStatus()==-1)){
                                        //都在向下运动
                                        if((ele[1].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                            //电梯1离得近
                                            ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入电梯一
                                            return;
                                        }
                                        else {
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入电梯六
                                            return;
                                        }
                                    }
                                    else if((ele[5].getMoveStatus()==1)&&(ele[1].getMoveStatus()==-1)){
                                        //六上一下
                                        ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入电梯1
                                        return;
                                    }
                                    else{
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入电梯6
                                        return;
                                    }
                                }
                                else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==-1)&&((i+1)>ele[1].getCurrFloor())&&(ele[1].getMoveStatus()==-1)){
                                    //都在下且都在向下运动
                                    if(((i+1)-ele[1].getCurrFloor())<((i+1)-ele[5].getCurrFloor())){
                                        //电梯1离得近
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入电梯6
                                        return;
                                    }
                                    else {
                                        ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入电梯1
                                        return;
                                    }

                                }
                                else if(((i+1)<ele[1].getCurrFloor())&&(ele[1].getMoveStatus()==1)&&((i+1)>ele[5].getCurrFloor())){
                                    //1在上向上运动，6在下
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();//写入6
                                    return;
                                }
                                else{
                                    ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();
                                    return;
                                }
                            }


                            if(re[i][j].direction==-1){//请求向下
                                if(eui.getShielding()[1]){
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();
                                    return;
                                }
                                else if(eui.getShielding()[5]){
                                    ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct();
                                    return;
                                }
                                else if(((i+1)<=(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()==-1)||(ele[1].getMoveStatus()==0))&&(ele[1].ifFull()!=1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                    //第一部和第六部电梯都和请求同向或者在请求楼的电梯没有运动且第两部电梯没有满载
                                    if(((i+1)-ele[1].getCurrFloor())<((i+1)-ele[5].getCurrFloor())){
                                        //第一部比第六部离得远
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯内
                                        return;
                                    }
                                    else {
                                        ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();//将请求写入第一部电梯中
                                        return;
                                    }
                                }
                                else if(((i+1)<=(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()==-1)||(ele[1].getMoveStatus()==0))&&(ele[1].ifFull()!=1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()==1)){
                                    //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第六部电梯满载
                                    ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();//将请求写入第一部电梯
                                    return;
                                }
                                else if(((i+1)<=(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()==-1)||(ele[1].getMoveStatus()==0))&&(ele[1].ifFull()==1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                    //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第一部电梯满载
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯
                                    return;
                                }
                                else if(((i+1)<=(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()==-1)||(ele[1].getMoveStatus()==0))&&((i+1)<=(ele[5].getCurrFloor()))&&(ele[5].getMoveDirection()==1)){
                                    //第一和第六部电梯都在请求下，但电梯六的运动方向是向下
                                    ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();//将请求写到第一部电梯内
                                    return;
                                }
                                else if(((i+1)<=(ele[1].getCurrFloor()))&&(ele[1].getMoveDirection()==1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))){
                                    //第一和第六部电梯都在请求下，但电梯一的运动方向是向下
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写到第六部电梯内
                                    return;
                                }
                                else if(((i+1)<=(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()==-1)||(ele[1].getMoveStatus()==0))&&(ele[1].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()!=0))) {
                                    //第一部电梯在请求下且向上运动或者不运动，但第六部电梯在请求上
                                    ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();//将请求写入第一部电梯内
                                    return;
                                }
                                else if(((i+1)>(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()!=0))&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                    //第六部电梯在请求下且向上运动或者不运动，但第一部电梯在请求上
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯内
                                    return;
                                }
                                else if(((i+1)<=(ele[1].getCurrFloor()))&&((ele[1].getMoveDirection()==-1)||(ele[1].getMoveStatus()==0))&&(ele[1].ifFull()!=1)&&((i+1)>(ele[5].getCurrFloor()))&&((ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                    //第一部电梯在请求下且向上运动或者不运动，第六部电梯在请求上但不运动
                                    if(((i+1)-ele[1].getCurrFloor())<(ele[5].getCurrFloor()-(i+1))){
                                        //第一部电梯离得更远
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//请求写入第六部电梯内
                                        return;
                                    }
                                    else {
                                        ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();//电梯写入第一部电梯内
                                    }
                                }
                                else if(((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)&&((i+1)>(ele[1].getCurrFloor()))&&((ele[1].getMoveStatus()==0))&&(ele[1].ifFull()!=1)) {
                                    //第六部电梯在请求下且向上运动或者不运动，第一部电梯在请求上但不运动
                                    if(((i+1)-ele[1].getCurrFloor())<(ele[5].getCurrFloor()-(i+1))){
                                        //第一部电梯离得更远
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//请求写入第六部电梯内
                                        return;
                                    }
                                    else {
                                        ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();//电梯写入第一部电梯内
                                        return;
                                    }
                                }
                                /********************************************************************************************************************/
                                else if(((i+1)>ele[1].getCurrFloor())&&(ele[1].getMoveStatus()==0)&&((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)){
                                    //电梯1，6都在请求上，但是都不运动
                                    if((ele[1].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                        //电梯1离得远
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                        return;
                                    }
                                    else {
                                        ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();//写入电梯1
                                        return;
                                    }
                                }
                                else if(((i+1)>ele[1].getCurrFloor())&&(ele[1].getMoveStatus()==0)&&((i+1)>ele[5].getCurrFloor())){
                                    //都在上面，但只有电梯1在不运动
                                    ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();//写入电梯一
                                    return;
                                }
                                else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)&&((i+1)>ele[1].getCurrFloor())){
                                    //都在上面，但只有电梯6在不运动
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                                    return;
                                }
                                else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()!=0)&&((i+1)>ele[1].getCurrFloor())&&(ele[1].getMoveStatus()!=0)){
                                    //都在上且都在运动
                                    if((ele[5].getMoveStatus()==-1)&&(ele[1].getMoveStatus()==-1)){
                                        //都在向上运动
                                        if((ele[1].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                            //电梯1离得近
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                            return;
                                        }
                                        else {
                                            ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();//写入电梯1
                                            return;
                                        }
                                    }
                                    else if((ele[5].getMoveStatus()==1)&&(ele[1].getMoveStatus()==1)){
                                        //都在向下运动
                                        if((ele[1].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                            //电梯1离得近
                                            ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();//写入电梯一
                                            return;
                                        }
                                        else {
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                            return;
                                        }
                                    }
                                    else if((ele[5].getMoveStatus()==-1)&&(ele[1].getMoveStatus()==1)){
                                        //六上一下
                                        ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();//写入电梯1
                                        return;
                                    }
                                    else{
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                                        return;
                                    }
                                }
                                else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==1)&&((i+1)<ele[1].getCurrFloor())&&(ele[1].getMoveStatus()==1)){
                                    //都在下且都在向下运动
                                    if(((i+1)-ele[1].getCurrFloor())>((i+1)-ele[5].getCurrFloor())){
                                        //电梯1离得近
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                                        return;
                                    }
                                    else {
                                        ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();//写入电梯1
                                        return;
                                    }

                                }
                                else if(((i+1)>ele[1].getCurrFloor())&&(ele[1].getMoveStatus()==-1)&&((i+1)<ele[5].getCurrFloor())){
                                    //1在上向上运动，6在下
                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入6
                                    return;
                                }
                                else{
                                    ele[1].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[1].EleAct2();
                                    return;
                                }
                                    }
                                }


                                else if((((i)==1)||((i)==4)||((i)==9)||((i)==14)||((i)==19)||((i)==24)||((i)==29)||((i)==34)||((i)==39)||((i)==44)||((i)==49)||((i)==54))&&((!(eui.getShielding()[2]))||(!(eui.getShielding()[5])))){
                                    /*如果该请求按规则是在第一台电梯和第六台电梯*/
                                    if(con.getRequest(i)[j].direction==1){//请求向上
                                        if(eui.getShielding()[2]){
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();
                                            return;
                                        }
                                        else if(eui.getShielding()[5]){
                                            ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();
                                            return;
                                        }
                                        else if(((i+1)>=(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()==1)||(ele[2].getMoveStatus()==0))&&(ele[2].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                            //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动且第两部电梯没有满载
                                            if(((i+1)-ele[2].getCurrFloor())>((i+1)-ele[5].getCurrFloor())){
                                                //第一部比第六部离得远
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//将请求写入第六部电梯内
                                                return;
                                            }
                                            else {
                                                ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//将请求写入第一部电梯中
                                                return;
                                            }
                                        }
                                        else if(((i+1)>=(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()==1)||(ele[2].getMoveStatus()==0))&&(ele[2].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()==1)){
                                            //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第六部电梯满载
                                            ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//将请求写入第一部电梯
                                            return;
                                        }
                                        else if(((i+1)>=(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()==1)||(ele[2].getMoveStatus()==0))&&(ele[2].ifFull()==1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                            //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第一部电梯满载
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//将请求写入第六部电梯
                                            return;
                                        }
                                        else if(((i+1)>=(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()==1)||(ele[2].getMoveStatus()==0))&&((i+1)>=(ele[5].getCurrFloor()))&&(ele[5].getMoveDirection()==-1)){
                                            //第一和第六部电梯都在请求下，但电梯六的运动方向是向下
                                            ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//将请求写到第一部电梯内
                                            return;
                                        }
                                        else if(((i+1)>=(ele[2].getCurrFloor()))&&(ele[2].getMoveDirection()==-1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))){
                                            //第一和第六部电梯都在请求下，但电梯一的运动方向是向下
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//将请求写到第六部电梯内
                                            return;
                                        }
                                        else if(((i+1)>=(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()==1)||(ele[2].getMoveStatus()==0))&&(ele[2].ifFull()!=1)&&((i+1)<(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()!=0))) {
                                            //第一部电梯在请求下且向上运动或者不运动，但第六部电梯在请求上
                                            ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//将请求写入第一部电梯内
                                            return;
                                        }
                                        else if(((i+1)<(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()!=0))&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                            //第六部电梯在请求下且向上运动或者不运动，但第一部电梯在请求上
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//将请求写入第六部电梯内
                                            return;
                                        }
                                        else if(((i+1)>=(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()==1)||(ele[2].getMoveStatus()==0))&&(ele[2].ifFull()!=1)&&((i+1)<(ele[5].getCurrFloor()))&&((ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                            //第一部电梯在请求下且向上运动或者不运动，第六部电梯在请求上但不运动
                                            if(((i+1)-ele[2].getCurrFloor())>(ele[5].getCurrFloor()-(i+1))){
                                                //第一部电梯离得更远
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//请求写入第六部电梯内
                                                return;
                                            }
                                            else {
                                                ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//电梯写入第一部电梯内
                                            }
                                        }
                                        else if(((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)&&((i+1)<(ele[2].getCurrFloor()))&&((ele[2].getMoveStatus()==0))&&(ele[2].ifFull()!=1)) {
                                            //第六部电梯在请求下且向上运动或者不运动，第一部电梯在请求上但不运动
                                            if(((i+1)-ele[2].getCurrFloor())>(ele[5].getCurrFloor()-(i+1))){
                                                //第一部电梯离得更远
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//请求写入第六部电梯内
                                                return;
                                            }
                                            else {
                                                ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//电梯写入第一部电梯内
                                                return;
                                            }
                                        }
                                        /********************************************************************************************************************/
                                        else if(((i+1)<ele[2].getCurrFloor())&&(ele[2].getMoveStatus()==0)&&((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)){
                                            //电梯1，6都在请求上，但是都不运动
                                            if((ele[2].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                                //电梯1离得远
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入电梯六
                                                return;
                                            }
                                            else {
                                                ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入电梯1
                                                return;
                                            }
                                        }
                                        else if(((i+1)<ele[2].getCurrFloor())&&(ele[2].getMoveStatus()==0)&&((i+1)<ele[5].getCurrFloor())){
                                            //都在上面，但只有电梯1在不运动
                                            ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入电梯一
                                            return;
                                        }
                                        else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)&&((i+1)<ele[2].getCurrFloor())){
                                            //都在上面，但只有电梯6在不运动
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入电梯6
                                            return;
                                        }
                                        else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()!=0)&&((i+1)<ele[2].getCurrFloor())&&(ele[2].getMoveStatus()!=0)){
                                            //都在上且都在运动
                                            if((ele[5].getMoveStatus()==1)&&(ele[2].getMoveStatus()==1)){
                                                //都在向上运动
                                                if((ele[2].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                                    //电梯1离得近
                                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入电梯六
                                                    return;
                                                }
                                                else {
                                                    ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入电梯1
                                                    return;
                                                }
                                            }
                                            else if((ele[5].getMoveStatus()==-1)&&(ele[2].getMoveStatus()==-1)){
                                                //都在向下运动
                                                if((ele[2].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                                    //电梯1离得近
                                                    ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入电梯一
                                                    return;
                                                }
                                                else {
                                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入电梯六
                                                    return;
                                                }
                                            }
                                            else if((ele[5].getMoveStatus()==1)&&(ele[2].getMoveStatus()==-1)){
                                                //六上一下
                                                ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入电梯1
                                                return;
                                            }
                                            else{
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入电梯6
                                                return;
                                            }
                                        }
                                        else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==-1)&&((i+1)>ele[2].getCurrFloor())&&(ele[2].getMoveStatus()==-1)){
                                            //都在下且都在向下运动
                                            if(((i+1)-ele[2].getCurrFloor())<((i+1)-ele[5].getCurrFloor())){
                                                //电梯1离得近
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入电梯6
                                                return;
                                            }
                                            else {
                                                ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入电梯1
                                                return;
                                            }

                                        }
                                        else if(((i+1)<ele[2].getCurrFloor())&&(ele[2].getMoveStatus()==1)&&((i+1)>ele[5].getCurrFloor())){
                                            //1在上向上运动，6在下
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();//写入6
                                            return;
                                        }
                                        else{
                                            ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();
                                            return;
                                        }
                     }

                     if(re[i][j].direction==-1){//请求向下
                         if(eui.getShielding()[2]){
                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();
                             return;
                         }
                         else if(eui.getShielding()[5]){
                             ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct();
                             return;
                         }
                         else if(((i+1)<=(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()==-1)||(ele[2].getMoveStatus()==0))&&(ele[2].ifFull()!=1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                             //第一部和第六部电梯都和请求同向或者在请求楼的电梯没有运动且第两部电梯没有满载
                             if(((i+1)-ele[2].getCurrFloor())<((i+1)-ele[5].getCurrFloor())){
                                 //第一部比第六部离得远
                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯内
                                 return;
                             }
                             else {
                                 ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();//将请求写入第一部电梯中
                                 return;
                             }
                         }
                         else if(((i+1)<=(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()==-1)||(ele[2].getMoveStatus()==0))&&(ele[2].ifFull()!=1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()==1)){
                             //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第六部电梯满载
                             ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();//将请求写入第一部电梯
                             return;
                         }
                         else if(((i+1)<=(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()==-1)||(ele[2].getMoveStatus()==0))&&(ele[2].ifFull()==1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                             //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第一部电梯满载
                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯
                             return;
                         }
                         else if(((i+1)<=(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()==-1)||(ele[2].getMoveStatus()==0))&&((i+1)<=(ele[5].getCurrFloor()))&&(ele[5].getMoveDirection()==1)){
                             //第一和第六部电梯都在请求下，但电梯六的运动方向是向下
                             ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();//将请求写到第一部电梯内
                             return;
                         }
                         else if(((i+1)<=(ele[2].getCurrFloor()))&&(ele[2].getMoveDirection()==1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))){
                             //第一和第六部电梯都在请求下，但电梯一的运动方向是向下
                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写到第六部电梯内
                             return;
                         }
                         else if(((i+1)<=(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()==-1)||(ele[2].getMoveStatus()==0))&&(ele[2].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()!=0))) {
                             //第一部电梯在请求下且向上运动或者不运动，但第六部电梯在请求上
                             ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();//将请求写入第一部电梯内
                             return;
                         }
                         else if(((i+1)>(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()!=0))&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                             //第六部电梯在请求下且向上运动或者不运动，但第一部电梯在请求上
                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯内
                             return;
                         }
                         else if(((i+1)<=(ele[2].getCurrFloor()))&&((ele[2].getMoveDirection()==-1)||(ele[2].getMoveStatus()==0))&&(ele[2].ifFull()!=1)&&((i+1)>(ele[5].getCurrFloor()))&&((ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                             //第一部电梯在请求下且向上运动或者不运动，第六部电梯在请求上但不运动
                             if(((i+1)-ele[2].getCurrFloor())<(ele[5].getCurrFloor()-(i+1))){
                                 //第一部电梯离得更远
                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//请求写入第六部电梯内
                                 return;
                             }
                             else {
                                 ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();//电梯写入第一部电梯内
                             }
                         }
                         else if(((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)&&((i+1)>(ele[2].getCurrFloor()))&&((ele[2].getMoveStatus()==0))&&(ele[2].ifFull()!=1)) {
                             //第六部电梯在请求下且向上运动或者不运动，第一部电梯在请求上但不运动
                             if(((i+1)-ele[2].getCurrFloor())<(ele[5].getCurrFloor()-(i+1))){
                                 //第一部电梯离得更远
                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//请求写入第六部电梯内
                                 return;
                             }
                             else {
                                 ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();//电梯写入第一部电梯内
                                 return;
                             }
                         }
                         /********************************************************************************************************************/
                         else if(((i+1)>ele[2].getCurrFloor())&&(ele[2].getMoveStatus()==0)&&((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)){
                             //电梯1，6都在请求上，但是都不运动
                             if((ele[2].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                 //电梯1离得远
                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                 return;
                             }
                             else {
                                 ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();//写入电梯1
                                 return;
                             }
                         }
                         else if(((i+1)>ele[2].getCurrFloor())&&(ele[2].getMoveStatus()==0)&&((i+1)>ele[5].getCurrFloor())){
                             //都在上面，但只有电梯1在不运动
                             ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();//写入电梯一
                             return;
                         }
                         else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)&&((i+1)>ele[2].getCurrFloor())){
                             //都在上面，但只有电梯6在不运动
                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                             return;
                         }
                         else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()!=0)&&((i+1)>ele[2].getCurrFloor())&&(ele[2].getMoveStatus()!=0)){
                             //都在上且都在运动
                             if((ele[5].getMoveStatus()==-1)&&(ele[2].getMoveStatus()==-1)){
                                 //都在向上运动
                                 if((ele[2].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                     //电梯1离得近
                                     ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                     return;
                                 }
                                 else {
                                     ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();//写入电梯1
                                     return;
                                 }
                             }
                             else if((ele[5].getMoveStatus()==1)&&(ele[2].getMoveStatus()==1)){
                                 //都在向下运动
                                 if((ele[2].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                     //电梯1离得近
                                     ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();//写入电梯一
                                     return;
                                 }
                                 else {
                                     ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                     return;
                                 }
                             }
                             else if((ele[5].getMoveStatus()==-1)&&(ele[2].getMoveStatus()==1)){
                                 //六上一下
                                 ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();//写入电梯1
                                 return;
                             }
                             else{
                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                                 return;
                             }
                         }
                         else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==1)&&((i+1)<ele[2].getCurrFloor())&&(ele[2].getMoveStatus()==1)){
                             //都在下且都在向下运动
                             if(((i+1)-ele[2].getCurrFloor())>((i+1)-ele[5].getCurrFloor())){
                                 //电梯1离得近
                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                                 return;
                             }
                             else {
                                 ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();//写入电梯1
                                 return;
                             }

                         }
                         else if(((i+1)>ele[2].getCurrFloor())&&(ele[2].getMoveStatus()==-1)&&((i+1)<ele[5].getCurrFloor())){
                             //1在上向上运动，6在下
                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入6
                             return;
                         }
                         else{
                             ele[2].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[2].EleAct2();
                             return;
                         }
                                     }
                                 }


                                 else if((((i)==1)||((i)==5)||((i)==10)||((i)==15)||((i)==20)||((i)==25)||((i)==30)||((i)==35)||((i)==40)||((i)==45)||((i)==50)||((i)==55))&&((!(eui.getShielding()[3]))||(!(eui.getShielding()[5])))){
                                     /*如果该请求按规则是在第一台电梯和第六台电梯*/
                                     if(con.getRequest(i)[j].direction==1){//请求向上
                                         if(eui.getShielding()[3]){
                                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();
                                             return;
                                         }
                                         else if(eui.getShielding()[5]){
                                             ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();
                                             return;
                                         }
                                         else if(((i+1)>=(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()==1)||(ele[3].getMoveStatus()==0))&&(ele[3].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                             //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动且第两部电梯没有满载
                                             if(((i+1)-ele[3].getCurrFloor())>((i+1)-ele[5].getCurrFloor())){
                                                 //第一部比第六部离得远
                                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//将请求写入第六部电梯内
                                                 return;
                                             }
                                             else {
                                                 ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//将请求写入第一部电梯中
                                                 return;
                                             }
                                         }
                                         else if(((i+1)>=(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()==1)||(ele[3].getMoveStatus()==0))&&(ele[3].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()==1)){
                                             //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第六部电梯满载
                                             ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//将请求写入第一部电梯
                                             return;
                                         }
                                         else if(((i+1)>=(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()==1)||(ele[3].getMoveStatus()==0))&&(ele[3].ifFull()==1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                             //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第一部电梯满载
                                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//将请求写入第六部电梯
                                             return;
                                         }
                                         else if(((i+1)>=(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()==1)||(ele[3].getMoveStatus()==0))&&((i+1)>=(ele[5].getCurrFloor()))&&(ele[5].getMoveDirection()==-1)){
                                             //第一和第六部电梯都在请求下，但电梯六的运动方向是向下
                                             ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//将请求写到第一部电梯内
                                             return;
                                         }
                                         else if(((i+1)>=(ele[3].getCurrFloor()))&&(ele[3].getMoveDirection()==-1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))){
                                             //第一和第六部电梯都在请求下，但电梯一的运动方向是向下
                                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//将请求写到第六部电梯内
                                             return;
                                         }
                                         else if(((i+1)>=(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()==1)||(ele[3].getMoveStatus()==0))&&(ele[3].ifFull()!=1)&&((i+1)<(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()!=0))) {
                                             //第一部电梯在请求下且向上运动或者不运动，但第六部电梯在请求上
                                             ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//将请求写入第一部电梯内
                                             return;
                                         }
                                         else if(((i+1)<(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()!=0))&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                             //第六部电梯在请求下且向上运动或者不运动，但第一部电梯在请求上
                                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//将请求写入第六部电梯内
                                             return;
                                         }
                                         else if(((i+1)>=(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()==1)||(ele[3].getMoveStatus()==0))&&(ele[3].ifFull()!=1)&&((i+1)<(ele[5].getCurrFloor()))&&((ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                             //第一部电梯在请求下且向上运动或者不运动，第六部电梯在请求上但不运动
                                             if(((i+1)-ele[3].getCurrFloor())>(ele[5].getCurrFloor()-(i+1))){
                                                 //第一部电梯离得更远
                                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//请求写入第六部电梯内
                                                 return;
                                             }
                                             else {
                                                 ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//电梯写入第一部电梯内
                                             }
                                         }
                                         else if(((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)&&((i+1)<(ele[3].getCurrFloor()))&&((ele[3].getMoveStatus()==0))&&(ele[3].ifFull()!=1)) {
                                             //第六部电梯在请求下且向上运动或者不运动，第一部电梯在请求上但不运动
                                             if(((i+1)-ele[3].getCurrFloor())>(ele[5].getCurrFloor()-(i+1))){
                                                 //第一部电梯离得更远
                                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//请求写入第六部电梯内
                                                 return;
                                             }
                                             else {
                                                 ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//电梯写入第一部电梯内
                                                 return;
                                             }
                                         }
                                         /********************************************************************************************************************/
                                         else if(((i+1)<ele[3].getCurrFloor())&&(ele[3].getMoveStatus()==0)&&((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)){
                                             //电梯1，6都在请求上，但是都不运动
                                             if((ele[3].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                                 //电梯1离得远
                                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入电梯六
                                                 return;
                                             }
                                             else {
                                                 ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入电梯1
                                                 return;
                                             }
                                         }
                                         else if(((i+1)<ele[3].getCurrFloor())&&(ele[3].getMoveStatus()==0)&&((i+1)<ele[5].getCurrFloor())){
                                             //都在上面，但只有电梯1在不运动
                                             ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入电梯一
                                             return;
                                         }
                                         else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)&&((i+1)<ele[3].getCurrFloor())){
                                             //都在上面，但只有电梯6在不运动
                                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入电梯6
                                             return;
                                         }
                                         else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()!=0)&&((i+1)<ele[3].getCurrFloor())&&(ele[3].getMoveStatus()!=0)){
                                             //都在上且都在运动
                                             if((ele[5].getMoveStatus()==1)&&(ele[3].getMoveStatus()==1)){
                                                 //都在向上运动
                                                 if((ele[3].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                                     //电梯1离得近
                                                     ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入电梯六
                                                     return;
                                                 }
                                                 else {
                                                     ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入电梯1
                                                     return;
                                                 }
                                             }
                                             else if((ele[5].getMoveStatus()==-1)&&(ele[3].getMoveStatus()==-1)){
                                                 //都在向下运动
                                                 if((ele[3].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                                     //电梯1离得近
                                                     ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入电梯一
                                                     return;
                                                 }
                                                 else {
                                                     ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入电梯六
                                                     return;
                                                 }
                                             }
                                             else if((ele[5].getMoveStatus()==1)&&(ele[3].getMoveStatus()==-1)){
                                                 //六上一下
                                                 ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入电梯1
                                                 return;
                                             }
                                             else{
                                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入电梯6
                                                 return;
                                             }
                                         }
                                         else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==-1)&&((i+1)>ele[3].getCurrFloor())&&(ele[3].getMoveStatus()==-1)){
                                             //都在下且都在向下运动
                                             if(((i+1)-ele[3].getCurrFloor())<((i+1)-ele[5].getCurrFloor())){
                                                 //电梯1离得近
                                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入电梯6
                                                 return;
                                             }
                                             else {
                                                 ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入电梯1
                                                 return;
                                             }

                                         }
                                         else if(((i+1)<ele[3].getCurrFloor())&&(ele[3].getMoveStatus()==1)&&((i+1)>ele[5].getCurrFloor())){
                                             //1在上向上运动，6在下
                                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();//写入6
                                             return;
                                         }
                                         else{
                                             ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();
                                             return;
                                         }
                                }

                                if(re[i][j].direction==-1){//请求向下
                                    if(eui.getShielding()[3]){
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();
                                        return;
                                    }
                                    else if(eui.getShielding()[5]){
                                        ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct();
                                        return;
                                    }
                                    else if(((i+1)<=(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()==-1)||(ele[3].getMoveStatus()==0))&&(ele[3].ifFull()!=1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                        //第一部和第六部电梯都和请求同向或者在请求楼的电梯没有运动且第两部电梯没有满载
                                        if(((i+1)-ele[3].getCurrFloor())<((i+1)-ele[5].getCurrFloor())){
                                            //第一部比第六部离得远
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯内
                                            return;
                                        }
                                        else {
                                            ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();//将请求写入第一部电梯中
                                            return;
                                        }
                                    }
                                    else if(((i+1)<=(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()==-1)||(ele[3].getMoveStatus()==0))&&(ele[3].ifFull()!=1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()==1)){
                                        //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第六部电梯满载
                                        ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();//将请求写入第一部电梯
                                        return;
                                    }
                                    else if(((i+1)<=(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()==-1)||(ele[3].getMoveStatus()==0))&&(ele[3].ifFull()==1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                        //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第一部电梯满载
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯
                                        return;
                                    }
                                    else if(((i+1)<=(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()==-1)||(ele[3].getMoveStatus()==0))&&((i+1)<=(ele[5].getCurrFloor()))&&(ele[5].getMoveDirection()==1)){
                                        //第一和第六部电梯都在请求下，但电梯六的运动方向是向下
                                        ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();//将请求写到第一部电梯内
                                        return;
                                    }
                                    else if(((i+1)<=(ele[3].getCurrFloor()))&&(ele[3].getMoveDirection()==1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))){
                                        //第一和第六部电梯都在请求下，但电梯一的运动方向是向下
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写到第六部电梯内
                                        return;
                                    }
                                    else if(((i+1)<=(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()==-1)||(ele[3].getMoveStatus()==0))&&(ele[3].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()!=0))) {
                                        //第一部电梯在请求下且向上运动或者不运动，但第六部电梯在请求上
                                        ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();//将请求写入第一部电梯内
                                        return;
                                    }
                                    else if(((i+1)>(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()!=0))&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                        //第六部电梯在请求下且向上运动或者不运动，但第一部电梯在请求上
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯内
                                        return;
                                    }
                                    else if(((i+1)<=(ele[3].getCurrFloor()))&&((ele[3].getMoveDirection()==-1)||(ele[3].getMoveStatus()==0))&&(ele[3].ifFull()!=1)&&((i+1)>(ele[5].getCurrFloor()))&&((ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                        //第一部电梯在请求下且向上运动或者不运动，第六部电梯在请求上但不运动
                                        if(((i+1)-ele[3].getCurrFloor())<(ele[5].getCurrFloor()-(i+1))){
                                            //第一部电梯离得更远
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//请求写入第六部电梯内
                                            return;
                                        }
                                        else {
                                            ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();//电梯写入第一部电梯内
                                        }
                                    }
                                    else if(((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)&&((i+1)>(ele[3].getCurrFloor()))&&((ele[3].getMoveStatus()==0))&&(ele[3].ifFull()!=1)) {
                                        //第六部电梯在请求下且向上运动或者不运动，第一部电梯在请求上但不运动
                                        if(((i+1)-ele[3].getCurrFloor())<(ele[5].getCurrFloor()-(i+1))){
                                            //第一部电梯离得更远
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//请求写入第六部电梯内
                                            return;
                                        }
                                        else {
                                            ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();//电梯写入第一部电梯内
                                            return;
                                        }
                                    }
                                    /********************************************************************************************************************/
                                    else if(((i+1)>ele[3].getCurrFloor())&&(ele[3].getMoveStatus()==0)&&((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)){
                                        //电梯1，6都在请求上，但是都不运动
                                        if((ele[3].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                            //电梯1离得远
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                            return;
                                        }
                                        else {
                                            ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();//写入电梯1
                                            return;
                                        }
                                    }
                                    else if(((i+1)>ele[3].getCurrFloor())&&(ele[3].getMoveStatus()==0)&&((i+1)>ele[5].getCurrFloor())){
                                        //都在上面，但只有电梯1在不运动
                                        ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();//写入电梯一
                                        return;
                                    }
                                    else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)&&((i+1)>ele[3].getCurrFloor())){
                                        //都在上面，但只有电梯6在不运动
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                                        return;
                                    }
                                    else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()!=0)&&((i+1)>ele[3].getCurrFloor())&&(ele[3].getMoveStatus()!=0)){
                                        //都在上且都在运动
                                        if((ele[5].getMoveStatus()==-1)&&(ele[3].getMoveStatus()==-1)){
                                            //都在向上运动
                                            if((ele[3].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                                //电梯1离得近
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                                return;
                                            }
                                            else {
                                                ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();//写入电梯1
                                                return;
                                            }
                                        }
                                        else if((ele[5].getMoveStatus()==1)&&(ele[3].getMoveStatus()==1)){
                                            //都在向下运动
                                            if((ele[3].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                                //电梯1离得近
                                                ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();//写入电梯一
                                                return;
                                            }
                                            else {
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                                return;
                                            }
                                        }
                                        else if((ele[5].getMoveStatus()==-1)&&(ele[3].getMoveStatus()==1)){
                                            //六上一下
                                            ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();//写入电梯1
                                            return;
                                        }
                                        else{
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                                            return;
                                        }
                                    }
                                    else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==1)&&((i+1)<ele[3].getCurrFloor())&&(ele[3].getMoveStatus()==1)){
                                        //都在下且都在向下运动
                                        if(((i+1)-ele[3].getCurrFloor())>((i+1)-ele[5].getCurrFloor())){
                                            //电梯1离得近
                                            ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                                            return;
                                        }
                                        else {
                                            ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();//写入电梯1
                                            return;
                                        }

                                    }
                                    else if(((i+1)>ele[3].getCurrFloor())&&(ele[3].getMoveStatus()==-1)&&((i+1)<ele[5].getCurrFloor())){
                                        //1在上向上运动，6在下
                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入6
                                        return;
                                    }
                                    else{
                                        ele[3].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[3].EleAct2();
                                        return;
                                    }
                                        }
                                    }


                                    else if((((i)==1)||((i)==6)||((i)==11)||((i)==16)||((i)==21)||((i)==26)||((i)==31)||((i)==36)||((i)==41)||((i)==46)||((i)==51)||((i)==56))&&((!(eui.getShielding()[4]))||(!(eui.getShielding()[5])))){
                                        /*如果该请求按规则是在第一台电梯和第六台电梯*/
                                        if(con.getRequest(i)[j].direction==1){//请求向上
                                            if(eui.getShielding()[4]){
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();
                                                return;
                                            }
                                            else if(eui.getShielding()[5]){
                                                ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();
                                                return;
                                            }
                                            else if(((i+1)>=(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()==1)||(ele[4].getMoveStatus()==0))&&(ele[4].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                                //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动且第两部电梯没有满载
                                                if(((i+1)-ele[4].getCurrFloor())>((i+1)-ele[5].getCurrFloor())){
                                                    //第一部比第六部离得远
                                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//将请求写入第六部电梯内
                                                    return;
                                                }
                                                else {
                                                    ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//将请求写入第一部电梯中
                                                    return;
                                                }
                                            }
                                            else if(((i+1)>=(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()==1)||(ele[4].getMoveStatus()==0))&&(ele[4].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()==1)){
                                                //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第六部电梯满载
                                                ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//将请求写入第一部电梯
                                                return;
                                            }
                                            else if(((i+1)>=(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()==1)||(ele[4].getMoveStatus()==0))&&(ele[4].ifFull()==1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                                //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第一部电梯满载
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//将请求写入第六部电梯
                                                return;
                                            }
                                            else if(((i+1)>=(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()==1)||(ele[4].getMoveStatus()==0))&&((i+1)>=(ele[5].getCurrFloor()))&&(ele[5].getMoveDirection()==-1)){
                                                //第一和第六部电梯都在请求下，但电梯六的运动方向是向下
                                                ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//将请求写到第一部电梯内
                                                return;
                                            }
                                            else if(((i+1)>=(ele[4].getCurrFloor()))&&(ele[4].getMoveDirection()==-1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))){
                                                //第一和第六部电梯都在请求下，但电梯一的运动方向是向下
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//将请求写到第六部电梯内
                                                return;
                                            }
                                            else if(((i+1)>=(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()==1)||(ele[4].getMoveStatus()==0))&&(ele[4].ifFull()!=1)&&((i+1)<(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()!=0))) {
                                                //第一部电梯在请求下且向上运动或者不运动，但第六部电梯在请求上
                                                ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//将请求写入第一部电梯内
                                                return;
                                            }
                                            else if(((i+1)<(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()!=0))&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                                //第六部电梯在请求下且向上运动或者不运动，但第一部电梯在请求上
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//将请求写入第六部电梯内
                                                return;
                                            }
                                            else if(((i+1)>=(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()==1)||(ele[4].getMoveStatus()==0))&&(ele[4].ifFull()!=1)&&((i+1)<(ele[5].getCurrFloor()))&&((ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                                //第一部电梯在请求下且向上运动或者不运动，第六部电梯在请求上但不运动
                                                if(((i+1)-ele[4].getCurrFloor())>(ele[5].getCurrFloor()-(i+1))){
                                                    //第一部电梯离得更远
                                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//请求写入第六部电梯内
                                                    return;
                                                }
                                                else {
                                                    ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//电梯写入第一部电梯内
                                                }
                                            }
                                            else if(((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)&&((i+1)<(ele[4].getCurrFloor()))&&((ele[4].getMoveStatus()==0))&&(ele[4].ifFull()!=1)) {
                                                //第六部电梯在请求下且向上运动或者不运动，第一部电梯在请求上但不运动
                                                if(((i+1)-ele[4].getCurrFloor())>(ele[5].getCurrFloor()-(i+1))){
                                                    //第一部电梯离得更远
                                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//请求写入第六部电梯内
                                                    return;
                                                }
                                                else {
                                                    ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//电梯写入第一部电梯内
                                                    return;
                                                }
                                            }
                                            /********************************************************************************************************************/
                                            else if(((i+1)<ele[4].getCurrFloor())&&(ele[4].getMoveStatus()==0)&&((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)){
                                                //电梯1，6都在请求上，但是都不运动
                                                if((ele[4].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                                    //电梯1离得远
                                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入电梯六
                                                    return;
                                                }
                                                else {
                                                    ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入电梯1
                                                    return;
                                                }
                                            }
                                            else if(((i+1)<ele[4].getCurrFloor())&&(ele[4].getMoveStatus()==0)&&((i+1)<ele[5].getCurrFloor())){
                                                //都在上面，但只有电梯1在不运动
                                                ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入电梯一
                                                return;
                                            }
                                            else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)&&((i+1)<ele[4].getCurrFloor())){
                                                //都在上面，但只有电梯6在不运动
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入电梯6
                                                return;
                                            }
                                            else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()!=0)&&((i+1)<ele[4].getCurrFloor())&&(ele[4].getMoveStatus()!=0)){
                                                //都在上且都在运动
                                                if((ele[5].getMoveStatus()==1)&&(ele[4].getMoveStatus()==1)){
                                                    //都在向上运动
                                                    if((ele[4].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                                        //电梯1离得近
                                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入电梯六
                                                        return;
                                                    }
                                                    else {
                                                        ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入电梯1
                                                        return;
                                                    }
                                                }
                                                else if((ele[5].getMoveStatus()==-1)&&(ele[4].getMoveStatus()==-1)){
                                                    //都在向下运动
                                                    if((ele[4].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                                        //电梯1离得近
                                                        ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入电梯一
                                                        return;
                                                    }
                                                    else {
                                                        ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入电梯六
                                                        return;
                                                    }
                                                }
                                                else if((ele[5].getMoveStatus()==1)&&(ele[4].getMoveStatus()==-1)){
                                                    //六上一下
                                                    ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入电梯1
                                                    return;
                                                }
                                                else{
                                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入电梯6
                                                    return;
                                                }
                                            }
                                            else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==-1)&&((i+1)>ele[4].getCurrFloor())&&(ele[4].getMoveStatus()==-1)){
                                                //都在下且都在向下运动
                                                if(((i+1)-ele[4].getCurrFloor())<((i+1)-ele[5].getCurrFloor())){
                                                    //电梯1离得近
                                                    ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入电梯6
                                                    return;
                                                }
                                                else {
                                                    ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入电梯1
                                                    return;
                                                }

                                            }
                                            else if(((i+1)<ele[4].getCurrFloor())&&(ele[4].getMoveStatus()==1)&&((i+1)>ele[5].getCurrFloor())){
                                                //1在上向上运动，6在下
                                                ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();//写入6
                                                return;
                                            }
                                            else{
                                                ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();
                                                return;
                                            }
                                 }

                                 if(re[i][j].direction==-1){//请求向下
                                     if(eui.getShielding()[4]){
                                         ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct();
                                         return;
                                     }
                                     else if(eui.getShielding()[5]){
                                         ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct();
                                         return;
                                     }
                                     else if(((i+1)<=(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()==-1)||(ele[4].getMoveStatus()==0))&&(ele[4].ifFull()!=1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                         //第一部和第六部电梯都和请求同向或者在请求楼的电梯没有运动且第两部电梯没有满载
                                         if(((i+1)-ele[4].getCurrFloor())<((i+1)-ele[5].getCurrFloor())){
                                             //第一部比第六部离得远
                                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯内
                                             return;
                                         }
                                         else {
                                             ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();//将请求写入第一部电梯中
                                             return;
                                         }
                                     }
                                     else if(((i+1)<=(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()==-1)||(ele[4].getMoveStatus()==0))&&(ele[4].ifFull()!=1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()==1)){
                                         //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第六部电梯满载
                                         ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();//将请求写入第一部电梯
                                         return;
                                     }
                                     else if(((i+1)<=(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()==-1)||(ele[4].getMoveStatus()==0))&&(ele[4].ifFull()==1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)){
                                         //第一部和第六部电梯都和请求同向或者在请求楼下的电梯没有运动但第一部电梯满载
                                         ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯
                                         return;
                                     }
                                     else if(((i+1)<=(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()==-1)||(ele[4].getMoveStatus()==0))&&((i+1)<=(ele[5].getCurrFloor()))&&(ele[5].getMoveDirection()==1)){
                                         //第一和第六部电梯都在请求下，但电梯六的运动方向是向下
                                         ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();//将请求写到第一部电梯内
                                         return;
                                     }
                                     else if(((i+1)<=(ele[4].getCurrFloor()))&&(ele[4].getMoveDirection()==1)&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))){
                                         //第一和第六部电梯都在请求下，但电梯一的运动方向是向下
                                         ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写到第六部电梯内
                                         return;
                                     }
                                     else if(((i+1)<=(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()==-1)||(ele[4].getMoveStatus()==0))&&(ele[4].ifFull()!=1)&&((i+1)>=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()!=0))) {
                                         //第一部电梯在请求下且向上运动或者不运动，但第六部电梯在请求上
                                         ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();//将请求写入第一部电梯内
                                         return;
                                     }
                                     else if(((i+1)>(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()!=0))&&((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                         //第六部电梯在请求下且向上运动或者不运动，但第一部电梯在请求上
                                         ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//将请求写入第六部电梯内
                                         return;
                                     }
                                     else if(((i+1)<=(ele[4].getCurrFloor()))&&((ele[4].getMoveDirection()==-1)||(ele[4].getMoveStatus()==0))&&(ele[4].ifFull()!=1)&&((i+1)>(ele[5].getCurrFloor()))&&((ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)) {
                                         //第一部电梯在请求下且向上运动或者不运动，第六部电梯在请求上但不运动
                                         if(((i+1)-ele[4].getCurrFloor())<(ele[5].getCurrFloor()-(i+1))){
                                             //第一部电梯离得更远
                                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//请求写入第六部电梯内
                                             return;
                                         }
                                         else {
                                             ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();//电梯写入第一部电梯内
                                         }
                                     }
                                     else if(((i+1)<=(ele[5].getCurrFloor()))&&((ele[5].getMoveDirection()==-1)||(ele[5].getMoveStatus()==0))&&(ele[5].ifFull()!=1)&&((i+1)>(ele[4].getCurrFloor()))&&((ele[4].getMoveStatus()==0))&&(ele[4].ifFull()!=1)) {
                                         //第六部电梯在请求下且向上运动或者不运动，第一部电梯在请求上但不运动
                                         if(((i+1)-ele[4].getCurrFloor())<(ele[5].getCurrFloor()-(i+1))){
                                             //第一部电梯离得更远
                                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//请求写入第六部电梯内
                                             return;
                                         }
                                         else {
                                             ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();//电梯写入第一部电梯内
                                             return;
                                         }
                                     }
                                     /********************************************************************************************************************/
                                     else if(((i+1)>ele[4].getCurrFloor())&&(ele[4].getMoveStatus()==0)&&((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)){
                                         //电梯1，6都在请求上，但是都不运动
                                         if((ele[4].getCurrFloor()-(i+1))<(ele[5].getCurrFloor()-(i+1))){
                                             //电梯1离得远
                                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                             return;
                                         }
                                         else {
                                             ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();//写入电梯1
                                             return;
                                         }
                                     }
                                     else if(((i+1)>ele[4].getCurrFloor())&&(ele[4].getMoveStatus()==0)&&((i+1)>ele[5].getCurrFloor())){
                                         //都在上面，但只有电梯1在不运动
                                         ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();//写入电梯一
                                         return;
                                     }
                                     else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==0)&&((i+1)>ele[4].getCurrFloor())){
                                         //都在上面，但只有电梯6在不运动
                                         ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                                         return;
                                     }
                                     else if(((i+1)>ele[5].getCurrFloor())&&(ele[5].getMoveStatus()!=0)&&((i+1)>ele[4].getCurrFloor())&&(ele[4].getMoveStatus()!=0)){
                                         //都在上且都在运动
                                         if((ele[5].getMoveStatus()==-1)&&(ele[4].getMoveStatus()==-1)){
                                             //都在向上运动
                                             if((ele[4].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                                 //电梯1离得近
                                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                                 return;
                                             }
                                             else {
                                                 ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();//写入电梯1
                                                 return;
                                             }
                                         }
                                         else if((ele[5].getMoveStatus()==1)&&(ele[4].getMoveStatus()==1)){
                                             //都在向下运动
                                             if((ele[4].getCurrFloor()-(i+1))>(ele[5].getCurrFloor()-(i+1))){
                                                 //电梯1离得近
                                                 ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();//写入电梯一
                                                 return;
                                             }
                                             else {
                                                 ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯六
                                                 return;
                                             }
                                         }
                                         else if((ele[5].getMoveStatus()==-1)&&(ele[4].getMoveStatus()==1)){
                                             //六上一下
                                             ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();//写入电梯1
                                             return;
                                         }
                                         else{
                                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                                             return;
                                         }
                                     }
                                     else if(((i+1)<ele[5].getCurrFloor())&&(ele[5].getMoveStatus()==1)&&((i+1)<ele[4].getCurrFloor())&&(ele[4].getMoveStatus()==1)){
                                         //都在下且都在向下运动
                                         if(((i+1)-ele[4].getCurrFloor())>((i+1)-ele[5].getCurrFloor())){
                                             //电梯1离得近
                                             ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入电梯6
                                             return;
                                         }
                                         else {
                                             ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();//写入电梯1
                                             return;
                                         }

                                     }
                                     else if(((i+1)>ele[4].getCurrFloor())&&(ele[4].getMoveStatus()==-1)&&((i+1)<ele[5].getCurrFloor())){
                                         //1在上向上运动，6在下
                                         ele[5].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[5].EleAct2();//写入6
                                         return;
                                     }
                                     else{
                                         ele[4].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[4].EleAct2();
                                         return;
                                     }
                                 }
                             }
                        }


                        else if ((!(*(eui.getIsbusy())))&&(!(eui.getShielding()[j]))) {//电梯此时为闲，且该部电梯没有被屏蔽
                            int temp[6]={0},min=-1;
                            if(con.getRequest(i)[j].direction==1){//请求方向向上
                                for (int k=0;k<6;k++) {
                                    if(((i+1)>=(ele[k].getCurrFloor()))&&((ele[k].getMoveDirection()==1)||(ele[k].getMoveDirection()==0))&&((((i+1)-ele[k].getCurrFloor())<6))&&(!(eui.getShielding()[k]))){//有与请求同向且在五层以内的电梯或在请求下的电梯
                                        temp[k]=1;
                                        if(min==-1){
                                            min=k;
                                        }
                                    }
                                }
                                for (int k=0;k<6;k++) {
                                    if(temp[k]==1){
                                        if(((i+1)-ele[k].getCurrFloor())<((i+1)-ele[min].getCurrFloor())){
                                            min=k;
                                        }
                                    }
                                }
                                if(min!=-1){
                                    ele[min].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[min].EleAct();
                                    return;//写入最近的电梯
                                }
                                for (int k=0;k<6;k++) {
                                    if((i+1)>=(ele[k].getCurrFloor())&&((ele[k].getMoveDirection()==1)||(ele[k].getMoveDirection()==0))&&(!(eui.getShielding()[k]))){//有与请求同向的电梯或在请求下的电梯
                                        temp[k]=1;
                                        if(min==-1){
                                            min=k;
                                        }
                                    }
                                }
                                for (int k=0;k<6;k++) {
                                    if(temp[k]==1){
                                        if(((i+1)-ele[k].getCurrFloor())<((i+1)-ele[min].getCurrFloor())){
                                            min=k;
                                        }
                                    }
                                }
                                if(min!=-1){
                                    ele[min].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[min].EleAct();//写入最近的电梯
                                    return;

                                }
                                for (int k=0;k<6;k++) {
                                    if((i+1)<(ele[k].getCurrFloor())&&((ele[k].getMoveDirection()==0))&&(((ele[k].getCurrFloor()-(i+1))<6))&&(!(eui.getShielding()[k]))){//有电梯在请求上且在5楼内但没有运动的
                                        temp[k]=1;
                                        if(min==-1){
                                            min=k;
                                        }
                                    }
                                }
                                for (int k=0;k<6;k++) {
                                    if(temp[k]==1){
                                        if(((i+1)-ele[k].getCurrFloor())>((i+1)-ele[min].getCurrFloor())){
                                            min=k;
                                        }
                                    }
                                }
                                if(min!=-1){
                                    ele[min].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[min].EleAct();//写入最近的电梯
                                    return;

                                }
                                for (int k=0;k<6;k++) {
                                    if((i+1)<(ele[k].getCurrFloor())&&((ele[k].getMoveDirection()==0))&&(!(eui.getShielding()[k]))){//有电梯在请求上但没有运动的
                                        temp[k]=1;
                                        if(min==-1){
                                            min=k;
                                        }
                                    }
                                }
                                for (int k=0;k<6;k++) {
                                    if(temp[k]==1){
                                        if(((i+1)-ele[k].getCurrFloor())>((i+1)-ele[min].getCurrFloor())){
                                            min=k;
                                        }
                                    }
                                }
                                if(min!=-1){
                                    ele[min].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[min].EleAct();//写入最近的电梯
                                    return;

                                }
                                for (int k=0;k<6;k++) {
                                    if(((ele[k].getMoveDirection()==-1))&&(!(eui.getShielding()[k]))){//有电梯向下运动
                                        temp[k]=1;
                                        if(min==-1){
                                            min=k;
                                        }
                                    }
                                }
                                for (int k=0;k<6;k++) {
                                    if(temp[k]==1){
                                        if(((i+1)-ele[k].getCurrFloor())>((i+1)-ele[min].getCurrFloor())){
                                            min=k;
                                        }
                                    }
                                }
                                if(min!=-1){
                                    ele[min].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[min].EleAct();//写入最先到达底楼的电梯
                                    return;

                                }
                                for (int k=0;k<6;k++) {
                                    if(((ele[k].getMoveDirection()==1))&&(!(eui.getShielding()[k]))){//所有电梯向上运动
                                        temp[k]=1;
                                        if(min==-1){
                                            min=k;
                                        }
                                    }
                                }
                                for (int k=0;k<6;k++) {
                                    if(temp[k]==1){
                                        if(((i+1)-ele[k].getCurrFloor())<((i+1)-ele[min].getCurrFloor())){
                                            min=k;
                                        }
                                    }
                                }
                                if(min!=-1){
                                    ele[min].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[min].EleAct();//写入最先到达顶楼的电梯
                                    return;

                                }
                            }

                            else if(con.getRequest(i)[j].direction==-1){//请求方向向下
                                for (int k=0;k<6;k++) {
                                    if((i+1)<=(ele[k].getCurrFloor())&&((ele[k].getMoveDirection()==-1)||(ele[k].getMoveDirection()==0))&&((((i+1)-ele[k].getCurrFloor())>-6))&&(!(eui.getShielding()[k]))){//有与请求同向且在五层以内的电梯或在请求下的电梯
                                        temp[k]=1;
                                        if(min==-1){
                                            min=k;
                                        }
                                    }
                                }
                                for (int k=0;k<6;k++) {
                                    if(temp[k]==1){
                                        if(((i+1)-ele[k].getCurrFloor())>((i+1)-ele[min].getCurrFloor())){
                                            min=k;
                                        }
                                    }
                                }
                                if(min!=-1){
                                    ele[min].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[min].EleAct2();//写入最近的电梯
                                    return;
                                }
                                for (int k=0;k<6;k++) {
                                    if((i+1)<=(ele[k].getCurrFloor())&&((ele[k].getMoveDirection()==-1)||(ele[k].getMoveDirection()==0))&&(!(eui.getShielding()[k]))){//有与请求同向的电梯或在请求下的电梯
                                        temp[k]=1;
                                        if(min==-1){
                                            min=k;
                                        }
                                    }
                                }
                                for (int k=0;k<6;k++) {
                                    if(temp[k]==1){
                                        if(((i+1)-ele[k].getCurrFloor())>((i+1)-ele[min].getCurrFloor())){
                                            min=k;
                                        }
                                    }
                                }
                                if(min!=-1){
                                    ele[min].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[min].EleAct2();//写入最近的电梯
                                    return;
                                }
                                for (int k=0;k<6;k++) {
                                    if((i+1)>(ele[k].getCurrFloor())&&((ele[k].getMoveDirection()==0))&&(((ele[k].getCurrFloor()-(i+1))<-6))&&(!(eui.getShielding()[k]))){//有电梯在请求上且在5楼内但没有运动的
                                        temp[k]=1;
                                        if(min==-1){
                                            min=k;
                                        }
                                    }
                                }
                                for (int k=0;k<6;k++) {
                                    if(temp[k]==1){
                                        if(((i+1)-ele[k].getCurrFloor())<((i+1)-ele[min].getCurrFloor())){
                                            min=k;
                                        }
                                    }
                                }
                                if(min!=-1){
                                    ele[min].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[min].EleAct2();//写入最近的电梯
                                    return;
                                }
                                for (int k=0;k<6;k++) {
                                    if((i+1)>(ele[k].getCurrFloor())&&((ele[k].getMoveDirection()==0))&&(!(eui.getShielding()[k]))){//有电梯在请求上但没有运动的
                                        temp[k]=1;
                                        if(min==-1){
                                            min=k;
                                        }
                                    }
                                }
                                for (int k=0;k<6;k++) {
                                    if(temp[k]==1){
                                        if(((i+1)-ele[k].getCurrFloor())<((i+1)-ele[min].getCurrFloor())){
                                            min=k;
                                        }
                                    }
                                }
                                if(min!=-1){
                                    ele[min].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[min].EleAct2();//写入最近的电梯
                                    return;
                                }
                                for (int k=0;k<6;k++) {
                                    if(((ele[k].getMoveDirection()==-1))&&(!(eui.getShielding()[k]))){//有电梯向下运动
                                        temp[k]=1;
                                        if(min==-1){
                                            min=k;
                                        }
                                    }
                                }
                                for (int k=0;k<6;k++) {
                                    if(temp[k]==1){
                                        if(((i+1)-ele[k].getCurrFloor())<((i+1)-ele[min].getCurrFloor())){
                                            min=k;
                                        }
                                    }
                                }
                                if(min!=-1){
                                    ele[min].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[min].EleAct2();//写入最先到达底楼的电梯
                                    return;
                                }
                                for (int k=0;k<6;k++) {
                                    if(((ele[k].getMoveDirection()==1))&&(!(eui.getShielding()[k]))){//所有电梯向上运动
                                        temp[k]=1;
                                        if(min==-1){
                                            min=k;
                                        }
                                    }
                                }
                                for (int k=0;k<6;k++) {
                                    if(temp[k]==1){
                                        if(((i+1)-ele[k].getCurrFloor())>((i+1)-ele[min].getCurrFloor())){
                                            min=k;
                                        }
                                    }
                                }
                                if(min!=-1){
                                    ele[min].setPassCurrFloor(i);con.getRequest(i)[j].direction=0;ele[min].EleAct2();//写入最先到达顶楼的电梯
                                    return;
                                }
                           }

                       }
               }

           }
          }

}
/*
 * 功能：返回控制对象特殊位；
 * 输入参数：无
 * 输出参数：控制对象的特殊位地址
 * 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/15
*/
bool* myMainWindow::getEmergen(){
    return &emergency;
}
/*
 * 功能：返回控制对象屏蔽位数组；
 * 输入参数：无
 * 输出参数：控制对象的屏蔽位数组的地址
 * 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/15
*/
bool* myMainWindow::getShielding(){
    return shielding;
}
/*
 * 功能：返回控制对象忙闲；
 * 输入参数：无
 * 输出参数：控制对象的忙闲的地址
 * 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/15
*/
bool* myMainWindow::getIsbusy(){
    return &isbusy;
}

/*
* 功能：出现特殊情况时，对所有电梯进行操作（如果电梯没有运作，对该电梯的所有请求进行屏蔽，如果该电梯在运行，则控制电梯运动
* 到同向最近楼层，然后开门，然后关门，并屏蔽请求)；
* 输入参数：无
* 输出参数：无
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/18; 2021/7/23
*/
void myMainWindow::on_emergencyclicked()
{
    int m=1;
    if(!(emergency)){//如果特殊情况按钮被按下
        emergency=true;
        for (int i=0;i<6;i++) {//对所有电梯进行操作
            if(el[i].getMoveStatus()){//如果电梯在运作
                if(el[i].getMoveDirection()){//如果电梯在向上运动
                    el[i].incrementFloor();//电梯上一次楼层
                    el[i].door(m);//开门，让电梯内乘客下电梯
                    el[i].setMoveStatus(0);
                    shielding[i]=true;//屏蔽掉电梯的请求
                    if(i==0){
                        ui->elevator1->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                    }
                    else if (i==1) {
                        ui->elevotar2->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                    }
                    else if (i==2) {
                        ui->elevotar3->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                    }
                    else if (i==3) {
                        ui->elevotar4->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                    }
                    else if (i==4) {
                        ui->elevotar5->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                    }
                    else if (i==5) {
                        ui->elevator6->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                    }

                }
                else if(!(el[i].getMoveDirection())){//如果电梯在向下运动
                    el[i].decrementFloor();//电梯下一次楼层
                    el[i].door(m);//开门，让电梯内乘客下电梯
                    shielding[i]=true;//屏蔽掉电梯的请求
                    if(i==0){
                        ui->elevator1->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                    }
                    else if (i==1) {
                        ui->elevotar2->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                    }
                    else if (i==2) {
                        ui->elevotar3->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                    }
                    else if (i==3) {
                        ui->elevotar4->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                    }
                    else if (i==4) {
                        ui->elevotar5->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                    }
                    else if (i==5) {
                        ui->elevator6->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                    }
                }
            }
            else if(!(el[i].getMoveStatus())){//如果电梯没有运作
                shielding[i]=true;//屏蔽掉电梯的请求
                if(i==0){
                    ui->elevator1->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                }
                else if (i==1) {
                    ui->elevotar2->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                }
                else if (i==2) {
                    ui->elevotar3->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                }
                else if (i==3) {
                    ui->elevotar4->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                }
                else if (i==4) {
                    ui->elevotar5->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                }
                else if (i==5) {
                    ui->elevator6->setText("特殊情况，禁止使用");//在界面上显示禁用信息
                }
            }

        }
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "发生特殊情况，电梯全部不运行"<<"\n";
        file.close();
    }
    else if (emergency) {//特殊情况过去了，管理员再次点击按钮，使特殊情况解除
        emergency=false;
        for (int i=0;i<6;i++) {
            shielding[i]=false;//请求不再被屏蔽
            if(i==0){
                ui->elevator1->setText(" ");//在界面上显示禁用信息取消
            }
            else if (i==1) {
                ui->elevotar2->setText(" ");//在界面上显示禁用信息取消
            }
            else if (i==2) {
                ui->elevotar3->setText(" ");//在界面上显示禁用信息取消
            }
            else if (i==3) {
                ui->elevotar4->setText(" ");//在界面上显示禁用信息取消
            }
            else if (i==4) {
                ui->elevotar5->setText(" ");//在界面上显示禁用信息取消
            }
            else if (i==5) {
                ui->elevator6->setText(" ");//在界面上显示禁用信息取消
            }
        }
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "特殊情况解除，电梯恢复运行"<<"\n";
        file.close();
    }

}
/*
* 功能：管理人员根据需求点击电梯1的停用按钮后，判断停用位，是真就将其设置为false，是假将其设为true，并调用停用函数。
* 输入参数：无
* 输出参数：无
* 作者：冯相汉 Update:刘宇涵 Update:刘宇涵
* 时间：2021/7/18; 2021/7/23
*/
void myMainWindow::on_diable1clicked()
{
    if(shielding[0]){//如果停用时间已过，管理人员再次按下停用按钮，就取消请用
        shielding[0]=false;//请求屏蔽解除
        getUi()->diable1->setText(" ");//界面显示停用信息取消
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯1停用解除"<<"\n";
        file.close();
        return;
    }
    else if(!(shielding[0])){//管理人员按下停用按钮，就停用相应的电梯
        shielding[0]=true;//请求屏蔽
        getUi()->diable1->setText("电梯1已经被停用");//界面显示停用信息
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯1被停用"<<"\n";
        file.close();
        return;
    }
}
/*
* 功能：管理人员根据需求点击电梯2的停用按钮后，判断停用位，是真就将其设置为false，是假将其设为true，并调用停用函数。
* 输入参数：无
* 输出参数：无
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/18; 2021/7/23
*/
void myMainWindow::on_disable2clicked()
{
    if(shielding[1]){//如果停用时间已过，管理人员再次按下停用按钮，就取消请用
        shielding[1]=false;//请求屏蔽解除
        getUi()->disable2->setText(" ");//界面显示停用信息取消
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯2停用解除"<<"\n";
        file.close();
    }
    else if(!(shielding[1])){//管理人员按下停用按钮，就停用相应的电梯
        shielding[1]=true;//请求屏蔽
        getUi()->disable2->setText("电梯2已经被停用");//界面显示停用信息
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯2被停用"<<"\n";
        file.close();
    }
}
/*
* 功能：管理人员根据需求点击电梯3的停用按钮后，判断停用位，是真就将其设置为false，是假将其设为true，并调用停用函数。
* 输入参数：无
* 输出参数：无
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/18; 2021/7/23
*/
void myMainWindow::on_disable3clicked()
{
    if(shielding[2]){//如果停用时间已过，管理人员再次按下停用按钮，就取消请用
        shielding[2]=false;//请求屏蔽解除
        getUi()->disable3->setText(" ");//界面显示停用信息取消
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯3停用解除"<<"\n";
        file.close();
    }
    else if(!(shielding[2])){//管理人员按下停用按钮，就停用相应的电梯
        shielding[2]=true;//请求屏蔽
        getUi()->disable3->setText("电梯3已经被停用");//界面显示停用信息
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯3被停用"<<"\n";
        file.close();
    }
}
/*
* 功能：管理人员根据需求点击电梯4的停用按钮后，判断停用位，是真就将其设置为false，是假将其设为true，并调用停用函数。
* 输入参数：无
* 输出参数：无
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/18; 2021/7/23
*/
void myMainWindow::on_disable4clicked()
{
    if(shielding[3]){//如果停用时间已过，管理人员再次按下停用按钮，就取消请用
        shielding[3]=false;//请求屏蔽解除
        getUi()->disable4->setText(" ");//界面显示停用信息取消
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯4停用解除"<<"\n";
        file.close();
    }
    else if(!(shielding[3])){//管理人员按下停用按钮，就停用相应的电梯
        shielding[3]=true;//请求屏蔽
        getUi()->disable4->setText("电梯4已经被停用");//界面显示停用信息
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯4被停用"<<"\n";
        file.close();
    }
}
/*
* 功能：管理人员根据需求点击电梯5的停用按钮后，判断停用位，是真就将其设置为false，是假将其设为true，并调用停用函数。
* 输入参数：无
* 输出参数：无
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/18; 2021/7/23
*/
void myMainWindow::on_disable5clicked()
{
    if(shielding[4]){//如果停用时间已过，管理人员再次按下停用按钮，就取消请用
        shielding[4]=false;//请求屏蔽解除
        getUi()->disable5->setText(" ");//界面显示停用信息取消
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯5停用解除"<<"\n";
        file.close();
    }
    else if(!(shielding[4])){//管理人员按下停用按钮，就停用相应的电梯
        shielding[4]=true;//请求屏蔽
        getUi()->disable5->setText("电梯5已经被停用");//界面显示停用信息
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯5被停用"<<"\n";
        file.close();
    }
}
/*
* 功能：管理人员根据需求点击电梯6的停用按钮后，判断停用位，是真就将其设置为false，是假将其设为true，并调用停用函数。
* 输入参数：无
* 输出参数：无
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/18; 2021/7/23
*/
void myMainWindow::on_disable6clicked()
{
    if(shielding[5]){//如果停用时间已过，管理人员再次按下停用按钮，就取消请用
        shielding[5]=false;//请求屏蔽解除
        getUi()->disable6->setText(" ");//界面显示停用信息取消
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯6停用解除"<<"\n";
        file.close();
    }
    else if(!(shielding[5])){//管理人员按下停用按钮，就停用相应的电梯
        shielding[5]=true;//请求屏蔽
        getUi()->disable6->setText("电梯6已经被停用");//界面显示停用信息
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯6被停用"<<"\n";
        file.close();
    }
}
/*
* 功能：设置忙闲位
* 输入输出参数：无
* 作者：冯相汉 Update:刘宇涵
* 时间：2021/7/18
*/
void myMainWindow::setIsbusy(){
    if(isbusy){//忙时间已过，管理员再次点击按钮，变到闲状态
        isbusy=false;
        getUi()->isbusy->setText("闲");//界面显示现在的忙闲状态
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯由忙变闲"<<"\n";
        file.close();
        return;
    }
    else if(!isbusy){//管理员点击按钮，变到忙状态
        isbusy=true;
        getUi()->isbusy->setText("忙");//界面显示现在的忙闲状态
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file("../Elevator/Log.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
            return;
        }

        QTextStream out(&file);//写入
        out <<time_str+":"+ "电梯由闲变忙"<<"\n";
        file.close();
        return;
    }
}


