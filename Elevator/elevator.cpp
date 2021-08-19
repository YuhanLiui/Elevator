#include "elevator.h"
#include "ui_elevator.h"
#include <windows.h>
#include <iostream>
#include <QDebug>
#include "mymainwindow.h"
using namespace std;
//构造函数
elevator::elevator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::elevator)
{
    //界面
    ui->setupUi(this);
    //初始化当前楼层
    this->currFloor = 1;
    //初始化目的楼层
    //this->destFloor = 1;
    //初始化运行状态，0:停止,1:运行，-1:故障
    this->status = 1;
    //初始化运动方向,1:向上，-1:向下
    this->moveUpOrDown = 1;
    //初始化请求位,0:无请求,1:向上请求位，-1:向下请求位
    this->Direction = 0;
    //初始化忙闲位，0:不忙，1:忙
    this->busyOrNot = 0;
    //初始化故障位
    this->troubleBit = 0;
    //初始化报警位
    this->warnningBit = 0;
    //初始化开关门位
    this->openOrClose = 0;
    //初始化电梯内人数
    this->passengerNumber = 0;
    //请求接受位
    this->flag = false;

}

/*
 * 功能：请求向上时电梯分配及写入了请求后调用电梯，使其响应请求
 * 输入参数：无
 * 输出参数：无
 * 作者：冯相汉
 * 时间：2021/7/15
 */
void elevator::EleAct(){
    //拿到控件

        ui->up1->setIcon(QIcon(":/img/up.png"));
        ui->up1->setIconSize(QSize(41,41));
        if(1 == t){
            //判断是否接受请求
            setDirectionBit(1);
            ResponseRequst();
        }
        if(2 == t){
            t = 0;
            setDirectionBit(0);
            flag = false;
        }
}

/*
 * 功能：请求向上时电梯分配及写入了请求后调用电梯，使其响应请求
 * 输入参数：无
 * 输出参数：无
 * 作者：冯相汉
 * 时间：2021/7/15
 */
void elevator::EleAct2(){
    //拿到控件
      ui->down1->setIcon(QIcon(":/img/down.png"));
      ui->down1->setIconSize(QSize(41,41));
      //t1++;s
      if(1 == t1){

          setDirectionBit(-1);
          //判断是否接受请求
          ResponseRequst();
      }
      if(2 == t1){


          t1 = 0;
          setDirectionBit(0);
          flag = false;
      }
}
//析构函数
elevator::~elevator()
{
    delete ui;
}
void elevator::setElev(elevator* ele){
    this->eleva=ele;
}
void elevator::setmyMainwindow(myMainWindow* mymain){
    this->mym=mymain;
}
void elevator::setController(controller* contro){
    this->cont=contro;
}
/*
 * function:通过传入参数，使得门打开和关闭
 * param:int i，表示开门，关门的标志
 * author:何秋进
 * date:2021/7/15
*/
void elevator::door(int i){
    //this->openOrClose = 1;
       //开门
       if(1 == i){
           this->status = 0;
           ui->leftdoor->setStyleSheet("border-image:url(:/img/doorcolor1.png)");
           ui->leftdoor1->setStyleSheet("border-image:url(:/img/doorcolor1.png)");
           ui->rightdoor->setStyleSheet("border-image:url(:/img/doorcolor2.png)");
           ui->rightdoor1->setStyleSheet("border-image:url(:/img/doorcolor2.png)");
           QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
           Sleep(1000);
           qDebug()<<"开门";
           i = 0;
       }
       //关门
       if(0 == i){
           this->status = 1;
           ui->leftdoor->setStyleSheet("border-image:url(:/img/door1.png)");
           ui->leftdoor1->setStyleSheet("border-image:url(:/img/door1.png)");
           ui->rightdoor->setStyleSheet("border-image:url(:/img/door2.png)");
           ui->rightdoor1->setStyleSheet("border-image:url(:/img/door2.png)");
           QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
           qDebug()<<"关门";
           Sleep(2000);
       }
}

/*
 * function:通过传入目的楼层数组，来寻找最大的楼层
 * param:vector<int> a，传入楼层的数组
 * author:何秋进 Update:刘宇涵
 * date:2021/7/15 2021/7/23
*/
int elevator::maxDestFloor(vector<int> a){
    int max = a[0];
    for(unsigned int i = 0; i < a.size(); ++i){
        if(max < a[i]){
            max = a[i];
        }
    }
    return max;
}

/*
 * function:通过传入目的楼层数组，来寻找最小的楼层
 * param:vector<int> a，传入楼层的数组
 * author:何秋进 Update:刘宇涵
 * date:2021/7/15; 2021/7/23
*/
int elevator::minDestFloor(vector<int> a){
    int max = a[0];
    for(unsigned int i = 0; i < a.size(); ++i){
        if(max < a[i]){
            max = a[i];
        }
    }
    return max;
}

/*
 * function:设置请求乘客楼层，传入请求楼层
 * param:int passCurrFloor,当前请求乘客楼层
 * author:何秋进
 * date:2021/7/15
*/
void elevator::setPassCurrFloor(int passCurrFloor1/*in*/){
    //将乘客的目的楼层存入到数组
    passCurrFloor.push_back(passCurrFloor1);
}


/*
 * function:获取电梯当前运行的楼层，通过返回值获取
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
int elevator::getCurrFloor(){
    return this->currFloor;
}

/*
 * function:获取乘客当前运行的楼层，通过返回值获取
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
vector<int> elevator::getPassCurrFloor(){
    return passCurrFloor;
}

/*
 * function:设置电梯的运行状态，
 * param:int Status,表示电梯的运行状态，1：运行，0：停止
 * author:何秋进
 * date:2021/7/15
*/
void elevator::setMoveStatus(int Status/*in*/){
    this->status = Status;
}

/*
 * function:获取电梯的运行状态，
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
int elevator::getMoveStatus(){
    return this->status;
}

/*
 * function:设置电梯的运行方向，传入方向的信息
 * param:int moveUpOrDown,代表电梯方向，-1：向下，1：向上
 * author:何秋进
 * date:2021/7/15
*/
void elevator::setMoveDirection(int moveUpOrDown/*in*/){
    this->moveUpOrDown = moveUpOrDown;
}

/*
 * function:获取电梯的运行方向
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
int elevator::getMoveDirection(){
    return this->moveUpOrDown;
}

/*
 * function:判断是否到达乘客的目的楼层，一一对比数组内容
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
bool elevator::isPassengerDestFloor(){
    for(vector<int>::iterator it = passCurrFloor.begin(); it != passCurrFloor.end(); ++it){
        if(this->currFloor == *it){
            deleteDestFloor(passCurrFloor,*it);
            return true;
        }
    }
    return false;
}

/*
 * function:设置电梯的忙和闲时状态，通过传入忙闲参数
 * param:int busyOrNot,表示忙闲信息的变量
 * author:何秋进
 * date:2021/7/15
*/
void elevator::setBusyBit(int& busyOrNot/*in*/){
    this->busyOrNot = busyOrNot;
}

/*
 * function:获取电梯的忙闲，通过返回值实现
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
int elevator::getBusyBit(){
    return this->busyOrNot;
}

/*
 * function:判断电梯里面是否人满了，通过比较最大人数值maxpassengers
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
bool elevator::ifFull(){
    if(maxPassengers == this->passengerNumber && this->eachPassengerDestFloor.size()){
        return true;
    }
    return false;
}

/*
 * function:乘客进入电梯，电梯里面的人数加1
 * param:无
 * author:何秋进
 * date:2021/7/19
*/
void elevator::passengerGoAbroad(){
    //判断是否超载
    if(ifFull()){
        return;
    }
    else{
        this->passengerNumber++;
        return;
    }
}

/*
 * function:乘客出电梯，电梯里面的人数减1
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
void elevator::passengerGoOutAbroad(){
    this->passengerNumber--;
}

/*
 * function:获取当前电梯里面的人数，通过this指针获取
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
int elevator::getPassengerNumber(){
    return passengerNumber;
}

/*
 * function:设置乘客方向请求的函数，乘客通过界面的向上和向下按键，发出请求
 * param:int Direction,表示乘客请求的方向
 * author:何秋进
 * date:2021/7/15
*/
void elevator::setDirectionBit(int Direction/*in*/){
    this->Direction = Direction;
}

/*
 * function:楼层递增，每调用就增加1
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
void elevator::incrementFloor(){
    this->currFloor++;
}

/*
 * function:楼层递减，每调用就减1
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
void elevator::decrementFloor(){
    this->currFloor--;
}

/*
 * function:设置乘客的目的楼层，通过按下楼层键，传入参数
 * param:int destFloor,表示按下楼层键的参数
 * author:何秋进
 * date:2021/7/15
*/
void elevator::setPassengerDestFloor(int destFloor){
    if(this->maxPassengers > eachPassengerDestFloor.size()){
        eachPassengerDestFloor.push_back(destFloor);
    }
    qDebug()<<"写入"<<destFloor<<"楼的请求到电梯内";
}

/*
 * function:启动电梯，通过调用moveup函数
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
void elevator::StartElevator(){
    MoveUp();
}

/*
 * function:响应乘客发出的请求，当有请求到达的时候，进行方向判断，看是否有响应，如果方向相同则进行响应，
 * 方向不同，三层以外响应
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
void elevator::ResponseRequst(){
    if((0 == this->status && 0 == this->troubleBit && 0 == this->passengerNumber)
            ||(1 == this->status && 0 == this->troubleBit)){
        if((Direction == moveUpOrDown  && !this->ifFull())){
            //设置运动方向为向上
            //若是在停止状态下启动
            if(0 == this->status){
                setMoveStatus(1);
            }
            //设置乘客当前请求楼层，先选，在按键
            if(1 == this->currFloor){
                //等待选择目的楼层
                QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
                Sleep(1000);
            }

            QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
            Sleep(2000);
            flag = true;
            StartElevator();
            cout << " isahdfoiasho:" << endl;
        }
        if(Direction != moveUpOrDown ){
            //设置运动方向
            setMoveDirection(Direction);
            //若处于停止状态
            if(0 == this->status){
                setMoveStatus(1);
            }
            if(1 == this->currFloor){
                //等待选择目的楼层
                QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
                Sleep(1000);
            }
            QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
            Sleep(2000);
            flag = true;
            StartElevator();
        }
    }
    flag = false;
}

/*
 * function:增加乘客的数量，每次加1
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
void elevator::incrementPassNumber(){
    for(vector<int>::iterator it = passCurrFloor.begin(); it != passCurrFloor.end(); ++it){
        //判断是否到达乘客请求楼梯
        if(this->currFloor == *it){
            door(1);
            passengerGoAbroad();
            deleteDestFloor(passCurrFloor,*it);
            QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
            Sleep(3000);
            cout<<"isResponse!" << endl;
        }
    }
}

/*
 * function:删除已经到达的目的乘客的信息，一个一个排查
 * param:vector<int> a, 表示存储楼层信息的vector，int b,表示要删除的乘客楼层
 * author:何秋进
 * date:2021/7/15
*/
void elevator::deleteDestFloor(vector<int> a, int b){
    for(vector<int>::iterator it=a.begin(); it!= a.end();++it){
        if(*it == b){
            it = a.erase(it);
            break;
        }
    }
}

/*
 * function:判断是否到达电梯内乘客的目的楼层，利用每次扫描对进行
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
void elevator::isDestPassFloor(){
    for(vector<int>::iterator it = eachPassengerDestFloor.begin();it != eachPassengerDestFloor.end(); ++it){
        if(this->currFloor == *it){
            cout << "someone leave:" << *it << endl;
            door(1);
            passengerGoOutAbroad();
            it = eachPassengerDestFloor.erase(it);
            cout << " shi fou zheng que :" << endl;
            break;
        }
    }
}

/*
 * function:获取电梯所有乘客的目的楼层，利用返回值，返回一个vector类型
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
vector<int> elevator::getPassengerDestFloor(){
    return eachPassengerDestFloor;
}

/*
 * function:三层以内不响应请求的函数，通过与电梯当前楼层对比，如果绝对值大于
 * 3或等于3，响应乘客请求，否则不响应
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
bool elevator::isResponseReqest(){
    for(vector<int>::iterator it=passCurrFloor.begin(); it != passCurrFloor.end(); ++it){
        if(abs(this->currFloor - *it) >= 3){
            return true;
        }
    }
    return false;
}

/*
 * function:上楼和下楼的程序功能，首先判断是否接受响应，然后在判断电梯是否处于运行状态
 * 如果处于运行状态，并且没有出现故障的情况下，电梯运行到乘客请求楼层，将乘客代入电梯
 * 将相应的目的楼层写到目的楼层数组中去，当到达目的楼层时，打开门将乘客放出来
 * param:无
 * author:何秋进
 * date:2021/7/15
*/
void elevator::MoveUp(){
    if(flag){
           if(1 == this->status && 0 == this->troubleBit){
               //判断是否改变方向
               if(1 == this->moveUpOrDown){
                   cout << "sdjfasdhfoas:" << endl;
                   //响应请求
                   if(flag){

                       //判断是否在乘客请求楼层
                       int maxx = maxDestFloor(passCurrFloor);
                       cout << "sdjfasdhfoas:" << endl;
                       while(this->currFloor < maxx){
                           incrementFloor();
                           QString currFloor=QString::number(this->currFloor);
                           QString style1 = "<p><img src=':/img/up3.png'> <font size=\"%1\" color=\"%2\">%3</font></p>";
                           QString style = style1.arg(10).arg("red").arg(currFloor);
                           ui->moveAndFloor->setHtml(style);
                           ui->moveAndFloor->setAlignment(Qt::AlignCenter);
                           ui->moveAndFloor1->setHtml(style);
                           ui->moveAndFloor1->setAlignment(Qt::AlignCenter);
                           QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
                           Sleep(1000);
                       }
                       while(this->currFloor > maxx){
                           decrementFloor();
                           QString currFloor=QString::number(this->currFloor);
                           QString style1 = "<p><img src=':/img/down3.png'> <font size=\"%1\" color=\"%2\">%3</font></p>";
                           QString style = style1.arg(10).arg("red").arg(currFloor);
                           ui->moveAndFloor->setHtml(style);
                           ui->moveAndFloor->setAlignment(Qt::AlignCenter);
                           ui->moveAndFloor1->setHtml(style);
                           ui->moveAndFloor1->setAlignment(Qt::AlignCenter);
                           QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
                           Sleep(1000);
                       }
                       if(this->currFloor <= this->maxFloor){
                           incrementPassNumber();
                       }
                       //停留时间，写入乘客的目的楼层
                       QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
                       Sleep(1000);
                   }
                   int max;
                   if(0 ==eachPassengerDestFloor.size()){
                       //停到请求中最大楼层
                       max = maxDestFloor(passCurrFloor);
                   }
                   else{
                       max = maxDestFloor(eachPassengerDestFloor);
                   }
                   while(this->currFloor <= max){
                       cout<<"currFloor:" << this->currFloor << endl;
                       //显示状态和楼层信息
                       QString currFloor=QString::number(this->currFloor);
                       QString style1 = "<p><img src=':/img/up3.png'> <font size=\"%1\" color=\"%2\">%3</font></p>";
                       QString style = style1.arg(10).arg("red").arg(currFloor);
                       ui->moveAndFloor->setHtml(style);
                       ui->moveAndFloor->setAlignment(Qt::AlignCenter);
                       ui->moveAndFloor1->setHtml(style);
                       ui->moveAndFloor1->setAlignment(Qt::AlignCenter);
                       QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
                       Sleep(1000);
                       //判断是否到达目的楼层
                       isDestPassFloor();
                       this->incrementFloor();
                   }
                   cout << " shi fou zheng que 1111:" << endl;
                   //判断是否改变方向
                   if(0 != this->passengerNumber){
                       int max1;
                       if(this->currFloor <= 56 && 0 == eachPassengerDestFloor.size()){
                           max1 = this->currFloor;
                       }
                       else{
                           max1 = minDestFloor(eachPassengerDestFloor);
                       }
                       if(this->currFloor > max1 && 1 == moveUpOrDown && 0 != max1){
                           this->moveUpOrDown = -1;
                           if(0 != passCurrFloor.size()){
                                StartElevator();
                           }
                           else{
                               setMoveStatus(0);
                               setMoveDirection(0);
                           }
                       }
                   }
               }
               if(-1 == this->moveUpOrDown){
                   //响应请求后
                   if(flag){
                       //判断是否在乘客请求楼层
                       int maxx = maxDestFloor(passCurrFloor);  //下方向请求最大值
                       while(this->currFloor > maxx){
                               decrementFloor();
                               QString currFloor=QString::number(this->currFloor);
                               QString style1 = "<p><img src=':/img/down3.png'> <font size=\"%1\" color=\"%2\">%3</font></p>";
                               QString style = style1.arg(10).arg("red").arg(currFloor);
                               cout << "shi fou zheng que ne!" << endl;
                               ui->moveAndFloor->setHtml(style);
                               ui->moveAndFloor->setAlignment(Qt::AlignCenter);
                               ui->moveAndFloor1->setHtml(style);
                               ui->moveAndFloor1->setAlignment(Qt::AlignCenter);
                               QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
                               Sleep(1000);
                       }
                       while(this->currFloor < maxx){
                               incrementFloor();
                               QString currFloor=QString::number(this->currFloor);
                               QString style1 = "<p><img src=':/img/up3.png'> <font size=\"%1\" color=\"%2\">%3</font></p>";
                               QString style = style1.arg(10).arg("red").arg(currFloor);
                               ui->moveAndFloor->setHtml(style);
                               ui->moveAndFloor->setAlignment(Qt::AlignCenter);
                               ui->moveAndFloor1->setHtml(style);
                               ui->moveAndFloor1->setAlignment(Qt::AlignCenter);
                               QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
                               Sleep(1000);
                       }
                       if(this->currFloor > this->minFloor){
                           incrementPassNumber();
                       }
                       //停留时间，写入乘客的目的楼层
                       QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
                       Sleep(1000);
                   }
                   //求最小楼层
                   int min;
                   if(0 == eachPassengerDestFloor.size()){
                       //到最小请求楼层
                        min = minDestFloor(passCurrFloor);
                   }
                   else{
                        min= minDestFloor(eachPassengerDestFloor);
                   }
                   while(this->currFloor >= min){
                       cout<< "currFloor:" << this->currFloor << endl;
                       //显示状态和楼层信息
                       QString currFloor=QString::number(this->currFloor);
                       QString style1 = "<p><img src=':/img/down3.png'> <font size=\"%1\" color=\"%2\">%3</font></p>";
                       QString style = style1.arg(10).arg("red").arg(currFloor);
                       ui->moveAndFloor->setHtml(style);
                       ui->moveAndFloor->setAlignment(Qt::AlignCenter);
                       ui->moveAndFloor1->setHtml(style);
                       ui->moveAndFloor1->setAlignment(Qt::AlignCenter);
                       QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
                       Sleep(1000);
                       isDestPassFloor();
                       //当到1楼时，不在向下
                       this->decrementFloor();
                   }
                   //是否改变方向
                   if(0 != this->passengerNumber){
                       cout << " shifou zhengque:" << endl;
                       cout << "renshu shi fou mei you:" << eachPassengerDestFloor.size()<<endl;
                       int min1;
                       if(this->currFloor >= 0 && 0 == eachPassengerDestFloor.size()){
                           if(this->currFloor == 0){
                               min1 = this->currFloor+1;
                           }
                           min1 = this->currFloor;
                       }
                       else{
                           min1 = minDestFloor(eachPassengerDestFloor);
                       }
                       if(this->currFloor < min1 && -1 == moveUpOrDown && 0 != min1){
                           this->moveUpOrDown = 1;
                           cout << passCurrFloor.size() << endl;
                           if(0 != passCurrFloor.size()){
                               StartElevator();
                           }
                           else{
                               setMoveStatus(0);
                               setMoveDirection(0);
                           }
                       }
                   }
               }
           //上楼后还有人下楼
           if((0 == this->passengerNumber && 0 == eachPassengerDestFloor.size())){
               this->status = 0;
           }
       }
   }
}
//上楼按键
void elevator::on_up1_clicked()
{
    (*cont).getRequest(ui->comboBox->currentIndex())[id-1].direction=1;
    (*cont).assignAct((*cont),(*mym),eleva);
    return;
}

//下楼按键
void elevator::on_down1_clicked()
{
    (*cont).getRequest(ui->comboBox->currentIndex())[id-1].direction=-1;
    (*cont).assignAct((*cont),(*mym),eleva);
    return;
}

void elevator::setId(int i){
    this->id=i;
}
int elevator::getT(){
    return t;
}
void elevator::setT(int i){
    t=i;
}
int elevator::getT1(){
    return t1;
}
void elevator::setT1(int i){
    t1=i;
}
//紧急按键
void elevator::on_trouble_clicked()
{
    ui->trouble->setIcon(QIcon(":/img/call.png"));
    ui->trouble->setIconSize(QSize(41,41));
    this->warnningBit = 1;
    if(!((*cont).getFault())[id-1]){
        ((*cont).getFault())[id-1]=true;
    }
    else if(((*cont).getFault())[id-1]){
        ((*cont).getFault())[id-1]=false;
    }
    (*cont).faultAct((*cont).getFault(),*mym);
    QString currFloor=QString::number(getCurrFloor());
    QString style1 = "<p><img src=':/img/up3.png'> <font size=\"%1\" color=\"%2\">%3</font></p>";
    QString style = style1.arg(10).arg("red").arg(currFloor);
    ui->moveAndFloor->setHtml(style);
    ui->moveAndFloor->setAlignment(Qt::AlignCenter);
    ui->moveAndFloor1->setHtml(style);
    ui->moveAndFloor1->setAlignment(Qt::AlignCenter);
    door(1);
    QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
    Sleep(2000);
    //(*cont).Reset((*cont).getFault(),*mym);
}
//手动开门键
void elevator::on_opendoor_clicked()
{
    ui->opendoor->setIcon(QIcon(":/img/opendoor.png"));
    ui->opendoor->setIconSize(QSize(41,41));
    door(1);
    QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
    Sleep(5000);
}
//手动关门键
void elevator::on_closedoor_clicked()
{
    ui->closedoor->setIcon(QIcon(":/img/closedoor.png"));
    ui->closedoor->setIconSize(QSize(41,41));
    door(1);
    QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
    Sleep(2000);
}


//设置1楼
void elevator::on_btn1_clicked()
{
    t2++;
    if(1 == t2){
        setPassengerDestFloor(1);
    }
    if(2 == t2){
        deleteDestFloor(eachPassengerDestFloor,1);
    }
}

void elevator::on_btn2_clicked()
{

    t3++;
    if(1 == t3){
        setPassengerDestFloor(2);
    }
    if(2 == t3){
        deleteDestFloor(eachPassengerDestFloor,2);
    }
}

void elevator::on_btn3_clicked()
{
    t4++;
    if(1 == t4){
        setPassengerDestFloor(3);
    }
    if(2 == t4){
        deleteDestFloor(eachPassengerDestFloor,3);
    }
}

void elevator::on_btn4_clicked()
{
    t5++;
    if(1 == t5){
        setPassengerDestFloor(4);
    }
    if(2 == t5){
        deleteDestFloor(eachPassengerDestFloor,4);
    }
}

void elevator::on_btn5_clicked()
{
    t6++;
    if(1 == t6){
        setPassengerDestFloor(5);
    }
    if(2 == t6){
        deleteDestFloor(eachPassengerDestFloor,5);
    }
}

void elevator::on_btn6_clicked()
{
    t7++;
    if(1 == t7){
        setPassengerDestFloor(6);
    }
    if(2 == t7){
        deleteDestFloor(eachPassengerDestFloor,6);
    }
}

void elevator::on_btn7_clicked()
{
    t8++;
    if(1 == t8){
        setPassengerDestFloor(7);
    }
    if(2 == t8){
        deleteDestFloor(eachPassengerDestFloor,7);
    }
}

void elevator::on_btn8_clicked()
{
    t9++;
    if(1 == t9){
        setPassengerDestFloor(8);
    }
    if(2 == t9){
        deleteDestFloor(eachPassengerDestFloor,8);
    }
}

void elevator::on_btn9_clicked()
{
    t10++;
    if(1 == t10){
        setPassengerDestFloor(9);
        //StartElevator();
    }
    if(2 == t10){
        deleteDestFloor(eachPassengerDestFloor,9);
    }
}


void elevator::on_btn10_clicked()
{
    t11++;
    if(1 == t11){
        setPassengerDestFloor(10);
    }
    if(2 == t11){
        deleteDestFloor(eachPassengerDestFloor,10);
    }
}

void elevator::on_btn11_clicked()
{
    t12++;
    if(1 == t12){
        setPassengerDestFloor(11);
    }
    if(2 == t12){
        deleteDestFloor(eachPassengerDestFloor,11);
    }
}

void elevator::on_btn12_clicked()
{
    t13++;
    if(1 == t13){
        setPassengerDestFloor(12);
    }
    if(2 == t13){
        deleteDestFloor(eachPassengerDestFloor,12);
    }
}

void elevator::on_btn13_clicked()
{
    t14++;
    if(1 == t14){
        setPassengerDestFloor(13);
    }
    if(2 == t14){
        deleteDestFloor(eachPassengerDestFloor,13);
    }
}

void elevator::on_btn14_clicked()
{
    t15++;
    if(1 == t15){
        setPassengerDestFloor(14);
    }
    if(2 == t15){
        deleteDestFloor(eachPassengerDestFloor,14);
    }
}

void elevator::on_btn15_clicked()
{
    t16++;
    if(1 == t16){
        setPassengerDestFloor(15);
    }
    if(2 == t16){
        deleteDestFloor(eachPassengerDestFloor,15);
    }
}

void elevator::on_btn16_clicked()
{
    t17++;
    if(1 == t17){
        setPassengerDestFloor(16);
    }
    if(2 == t17){
        deleteDestFloor(eachPassengerDestFloor,16);
    }
}

void elevator::on_btn17_clicked()
{
    t18++;
    if(1 == t18){
        setPassengerDestFloor(17);
    }
    if(2 == t18){
        deleteDestFloor(eachPassengerDestFloor,17);
    }
}

void elevator::on_btn18_clicked()
{
    t19++;
    if(1 == t19){
        setPassengerDestFloor(18);
    }
    if(2 == t19){
        deleteDestFloor(eachPassengerDestFloor,18);
    }
}

void elevator::on_btn19_clicked()
{
    t20++;
    if(1 == t20){
        setPassengerDestFloor(19);
    }
    if(2 == t20){
        deleteDestFloor(eachPassengerDestFloor,19);
    }
}

void elevator::on_btn20_clicked()
{
    t21++;
    if(1 == t21){
        setPassengerDestFloor(20);
    }
    if(2 == t21){
        deleteDestFloor(eachPassengerDestFloor,20);
    }
}

void elevator::on_btn21_clicked()
{
    t22++;
    if(1 == t22){
        setPassengerDestFloor(21);
    }
    if(2 == t22){
        deleteDestFloor(eachPassengerDestFloor,21);
    }
}

void elevator::on_btn22_clicked()
{
    t23++;
    if(1 == t23){
        setPassengerDestFloor(22);
    }
    if(2 == t23){
        deleteDestFloor(eachPassengerDestFloor,22);
    }
}

void elevator::on_btn23_clicked()
{
    t24++;
    if(1 == t24){
        setPassengerDestFloor(23);
    }
    if(2 == t24){
        deleteDestFloor(eachPassengerDestFloor,23);
    }
}

void elevator::on_btn24_clicked()
{
    t25++;
    if(1 == t25){
        setPassengerDestFloor(24);
        //MoveUp();
    }
    if(2 == t25){
        deleteDestFloor(eachPassengerDestFloor,24);
    }
}

void elevator::on_btn25_clicked()
{
    t26++;
    if(1 == t26){
        setPassengerDestFloor(25);
    }
    if(2 == t26){
        deleteDestFloor(eachPassengerDestFloor,25);
    }
}

void elevator::on_btn26_clicked()
{
    t27++;
    if(1 == t27){
        setPassengerDestFloor(26);
    }
    if(2 == t27){
        deleteDestFloor(eachPassengerDestFloor,26);
    }
}

void elevator::on_btn27_clicked()
{
    t28++;
    if(1 == t28){
        setPassengerDestFloor(27);
    }
    if(2 == t28){
        deleteDestFloor(eachPassengerDestFloor,27);
    }
}

void elevator::on_btn28_clicked()
{
    t29++;
    if(1 == t29){
        setPassengerDestFloor(28);
    }
    if(2 == t29){
        deleteDestFloor(eachPassengerDestFloor,28);
    }
}

void elevator::on_btn29_clicked()
{
    t30++;
    if(1 == t30){
        setPassengerDestFloor(29);
    }
    if(2 == t30){
        deleteDestFloor(eachPassengerDestFloor,29);
    }
}

void elevator::on_btn30_clicked()
{
    t31++;
    if(1 == t31){
        setPassengerDestFloor(30);
    }
    if(2 == t31){
        deleteDestFloor(eachPassengerDestFloor,30);
    }
}

void elevator::on_btn31_clicked()
{
    t32++;
    if(1 == t32){
        setPassengerDestFloor(31);
    }
    if(2 == t32){
        deleteDestFloor(eachPassengerDestFloor,31);
    }
}

void elevator::on_btn32_clicked()
{
    t33++;
    if(1 == t33){
        setPassengerDestFloor(32);
    }
    if(2 == t33){
        deleteDestFloor(eachPassengerDestFloor,32);
    }
}

void elevator::on_btn33_clicked()
{
    t34++;
    if(1 == t34){
        setPassengerDestFloor(33);
    }
    if(2 == t34){
        deleteDestFloor(eachPassengerDestFloor,33);
    }
}

void elevator::on_btn34_clicked()
{
    t35++;
    if(1 == t35){
        setPassengerDestFloor(34);
    }
    if(2 == t35){
        deleteDestFloor(eachPassengerDestFloor,34);
    }
}

void elevator::on_btn35_clicked()
{
    t36++;
    if(1 == t36){
        setPassengerDestFloor(35);
    }
    if(2 == t36){
        deleteDestFloor(eachPassengerDestFloor,35);
    }
}

void elevator::on_btn36_clicked()
{
    t37++;
    if(1 == t37){
        setPassengerDestFloor(36);
    }
    if(2 == t37){
        deleteDestFloor(eachPassengerDestFloor,36);
    }
}

void elevator::on_btn37_clicked()
{
    t38++;
    if(1 == t38){
        setPassengerDestFloor(37);
    }
    if(2 == t38){
        deleteDestFloor(eachPassengerDestFloor,37);
    }
}

void elevator::on_btn38_clicked()
{
    t39++;
    if(1 == t39){
        setPassengerDestFloor(38);
    }
    if(2 == t39){
        deleteDestFloor(eachPassengerDestFloor,38);
    }
}

void elevator::on_btn39_clicked()
{
    t40++;
    if(1 == t40){
        setPassengerDestFloor(39);
    }
    if(2 == t40){
        deleteDestFloor(eachPassengerDestFloor,39);
    }
}

void elevator::on_btn40_clicked()
{
    t41++;
    if(1 == t41){
        setPassengerDestFloor(40);
    }
    if(2 == t41){
        deleteDestFloor(eachPassengerDestFloor,40);
    }
}

void elevator::on_btn41_clicked()
{
    t42++;
    if(1 == t42){
        setPassengerDestFloor(41);
    }
    if(2 == t42){
        deleteDestFloor(eachPassengerDestFloor,41);
    }
}

void elevator::on_btn42_clicked()
{
    t43++;
    if(1 == t43){
        setPassengerDestFloor(42);
    }
    if(2 == t43){
        deleteDestFloor(eachPassengerDestFloor,42);
    }
}

void elevator::on_btn43_clicked()
{
    t44++;
    if(1 == t44){
        setPassengerDestFloor(43);
    }
    if(2 == t44){
        deleteDestFloor(eachPassengerDestFloor,43);
    }
}

void elevator::on_btn44_clicked()
{
    t45++;
    if(1 == t45){
        setPassengerDestFloor(44);
    }
    if(2 == t45){
        deleteDestFloor(eachPassengerDestFloor,44);
    }
}

void elevator::on_btn45_clicked()
{
    t46++;
    if(1 == t46){
        setPassengerDestFloor(45);
    }
    if(2 == t46){
        deleteDestFloor(eachPassengerDestFloor,45);
    }
}

void elevator::on_btn46_clicked()
{
    t47++;
    if(1 == t47){
        setPassengerDestFloor(46);
    }
    if(2 == t47){
        deleteDestFloor(eachPassengerDestFloor,46);
    }
}

void elevator::on_btn47_clicked()
{
    t48++;
    if(1 == t48){
        setPassengerDestFloor(47);
    }
    if(2 == t48){
        deleteDestFloor(eachPassengerDestFloor,47);
    }
}

void elevator::on_btn48_clicked()
{
    t49++;
    if(1 == t49){
        setPassengerDestFloor(48);
    }
    if(2 == t49){
        deleteDestFloor(eachPassengerDestFloor,48);
    }
}

void elevator::on_btn49_clicked()
{
    t50++;
    if(1 == t50){
        setPassengerDestFloor(49);
    }
    if(2 == t50){
        deleteDestFloor(eachPassengerDestFloor,49);
    }
}

void elevator::on_btn50_clicked()
{
    t51++;
    if(1 == t51){
        setPassengerDestFloor(50);
    }
    if(2 == t51){
        deleteDestFloor(eachPassengerDestFloor,50);
    }
}

void elevator::on_btn51_clicked()
{
    t52++;
    if(1 == t52){
        setPassengerDestFloor(51);
    }
    if(2 == t52){
        deleteDestFloor(eachPassengerDestFloor,51);
    }
}

void elevator::on_btn52_clicked()
{
    t53++;
    if(1 == t53){
        setPassengerDestFloor(52);
    }
    if(2 == t53){
        deleteDestFloor(eachPassengerDestFloor,52);
    }
}

void elevator::on_btn53_clicked()
{
    t54++;
    if(1 == t54){
        setPassengerDestFloor(53);
    }
    if(2 == t54){
        deleteDestFloor(eachPassengerDestFloor,53);
    }
}

void elevator::on_btn54_clicked()
{
    t55++;
    if(1 == t55){
        setPassengerDestFloor(54);
    }
    if(2 == t55){
        deleteDestFloor(eachPassengerDestFloor,54);
    }
}

void elevator::on_btn55_clicked()
{
    t56++;
    if(1 == t56){
        setPassengerDestFloor(55);
    }
    if(2 == t56){
        deleteDestFloor(eachPassengerDestFloor,55);
    }
}

void elevator::on_btn56_clicked()
{
    t57++;
    if(1 == t57){
        setPassengerDestFloor(56);
    }
    if(2 == t57){
        deleteDestFloor(eachPassengerDestFloor,56);
    }
}
