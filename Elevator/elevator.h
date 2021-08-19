#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QMainWindow>
#include <vector>
#include "mymainwindow.h"
//#include "mymainwindow.cpp"
using namespace std;
/*
 * autor:何秋进
 * document Name:elevator.h
 * date:2021/07/15
*/
namespace Ui {
class elevator;
}
class myMainWindow;
class controller;

class elevator : public QMainWindow
{
    Q_OBJECT

public:
    //构造函数
    explicit elevator(QWidget *parent = nullptr);
    //析构函数
    ~elevator();
    /*
     * 功能：请求向上时电梯分配及写入了请求后调用电梯，使其响应请求
     * 输入参数：无
     * 输出参数：无
     * 作者：冯相汉
     * 时间：2021/7/15
     */
    void EleAct();

    /*
     * 功能：请求向上时电梯分配及写入了请求后调用电梯，使其响应请求
     * 输入参数：无
     * 输出参数：无
     * 作者：冯相汉
     * 时间：2021/7/15
     */
    void EleAct2();

    /*
     * function:通过传入参数，使得门打开和关闭
     * param:int i，表示开门，关门的标志
     * author:何秋进
     * date:2021/7/15
    */
    void door(int i);

    /*
     * function:通过传入目的楼层数组，来寻找最大的楼层
     * param:vector<int> a，传入楼层的数组
     * author:何秋进
     * date:2021/7/15
    */
    int maxDestFloor(vector<int> a /*in*/);

    /*
     * function:通过传入目的楼层数组，来寻找最小的楼层
     * param:vector<int> a，传入楼层的数组
     * author:何秋进
     * date:2021/7/15
    */
    int minDestFloor(vector<int> a/*in*/);

    /*
     * function:设置请求乘客楼层，传入请求楼层
     * param:int passCurrFloor,当前请求乘客楼层
     * author:何秋进
     * date:2021/7/15
    */
    void setPassCurrFloor(int passCurrFloor/*in*/);

    /*
     * function:获取电梯当前运行的楼层，通过返回值获取
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    int getCurrFloor();

    /*
     * function:获取乘客当前运行的楼层，通过返回值获取
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    vector<int> getPassCurrFloor();

    /*
     * function:设置电梯的运行状态，
     * param:int Status,表示电梯的运行状态，1：运行，0：停止
     * author:何秋进
     * date:2021/7/15
    */
    void setMoveStatus(int Status/*in*/);

    /*
     * function:获取电梯的运行状态，
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    int getMoveStatus();

    /*
     * function:设置电梯的运行方向，传入方向的信息
     * param:int moveUpOrDown,代表电梯方向，-1：向下，1：向上
     * author:何秋进
     * date:2021/7/15
    */
    void setMoveDirection(int moveUpOrDown/*in*/);

    /*
     * function:获取电梯的运行方向
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    int getMoveDirection();

    /*
     * function:判断是否到达乘客的目的楼层，一一对比数组内容
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    bool isPassengerDestFloor();

    /*
     * function:设置电梯的忙和闲时状态，通过传入忙闲参数
     * param:int busyOrNot,表示忙闲信息的变量
     * author:何秋进
     * date:2021/7/15
    */
    void setBusyBit(int& busyOrNot/*in*/);

    /*
     * function:获取电梯的忙闲，通过返回值实现
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    int getBusyBit();

    /*
     * function:判断电梯里面是否人满了，通过比较最大人数值maxpassengers
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    bool ifFull();

    /*
     * function:乘客进入电梯，电梯里面的人数加1
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    void passengerGoAbroad();

    /*
     * function:乘客出电梯，电梯里面的人数减1
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    void passengerGoOutAbroad();

    /*
     * function:获取当前电梯里面的人数，通过this指针获取
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    int getPassengerNumber();

    /*
     * function:设置乘客方向请求的函数，乘客通过界面的向上和向下按键，发出请求
     * param:int Direction,表示乘客请求的方向
     * author:何秋进
     * date:2021/7/15
    */
    void setDirectionBit(int Direction/*in*/);

    /*
     * function:楼层递增，每调用就增加1
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    void incrementFloor();

    /*
     * function:楼层递减，每调用就减1
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    void decrementFloor();

    /*
     * function:设置乘客的目的楼层，通过按下楼层键，传入参数
     * param:int destFloor,表示按下楼层键的参数
     * author:何秋进
     * date:2021/7/15
    */
    void setPassengerDestFloor(int dsetFloor/*in*/);

    /*
     * function:启动电梯，通过调用moveup函数
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    void StartElevator();

    /*
     * function:响应乘客发出的请求，当有请求到达的时候，进行方向判断，看是否有响应，如果方向相同则进行响应，
     * 方向不同，三层以外响应
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    void ResponseRequst();

    /*
     * function:增加乘客的数量，每次加1
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    void incrementPassNumber();

    /*
     * function:删除已经到达的目的乘客的信息，一个一个排查
     * param:vector<int> a, 表示存储楼层信息的vector，int b,表示要删除的乘客楼层
     * author:何秋进
     * date:2021/7/15
    */
    void deleteDestFloor(vector<int> a, int b);

    /*
     * function:判断是否到达电梯内乘客的目的楼层，利用每次扫描对进行
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    void isDestPassFloor();

    /*
     * function:获取电梯所有乘客的目的楼层，利用返回值，返回一个vector类型
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    vector<int> getPassengerDestFloor();

    /*
     * function:三层以内不响应请求的函数，通过与电梯当前楼层对比，如果绝对值大于
     * 3或等于3，响应乘客请求，否则不响应
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    bool isResponseReqest();

    /*
     * function:上楼和下楼的程序功能，首先判断是否接受响应，然后在判断电梯是否处于运行状态
     * 如果处于运行状态，并且没有出现故障的情况下，电梯运行到乘客请求楼层，将乘客代入电梯
     * 将相应的目的楼层写到目的楼层数组中去，当到达目的楼层时，打开门将乘客放出来
     * param:无
     * author:何秋进
     * date:2021/7/15
    */
    void MoveUp();
    /*
     * 功能：设置电梯的ID编号
     * 输入参数：ID值
     * 输出参数：无
     * 作者：冯相汉
     * 时间：2021/7/15
    */
    void setId(int i);
    /*
     * 功能：设置t值，实现上键的取消功能
     * 输入参数：无
     * 输出参数：无
     * 作者：冯相汉
     * 时间：2021/7/15
    */
    int getT();
    /*
     * 功能：获取t值，实现上键的取消功能
     * 输入参数：ID值
     * 输出参数：无
     * 作者：冯相汉
     * 时间：2021/7/15
    */
    void setT(int i);
    /*
     * 功能：设置t1值，实现下键的取消功能
     * 输入参数：无
     * 输出参数：无
     * 作者：冯相汉
     * 时间：2021/7/15
    */
    int getT1();
    /*
     * 功能：获取t1值，实现下键的取消功能
     * 输入参数：ID值
     * 输出参数：无
     * 作者：冯相汉
     * 时间：2021/7/15
    */
    void setT1(int i);
    /*
     * 功能：将电梯数组地址写入成员
     * 输入参数：电梯数组地址
     * 输出参数：无
     * 作者：冯相汉
     * 时间：2021/7/15
    */
    void setElev(elevator* ele);
    /*
     * 功能：将界面地址写入成员
     * 输入参数：界面地址
     * 输出参数：无
     * 作者：冯相汉
     * 时间：2021/7/15
    */
    void setmyMainwindow(myMainWindow* mymain);
    /*
     * 功能：将控制台地址写入成员
     * 输入参数：控制台地址
     * 输出参数：无
     * 作者：冯相汉
     * 时间：2021/7/15
    */
    void setController(controller* contro);
    //最大载客量
    const static int maxPassengers = 12;
    //最大楼层
    const static int maxFloor = 56;
    //最低楼层
    const static int minFloor = 1;
public:
    //单击和双击取消
    int t = 0;
    int t1 = 0;
    //楼层键的双击取消
    int t2 = 0;int t3 = 0;int t4 = 0;int t5 = 0;int t6 = 0;int t7 = 0;int t8 = 0;int t9 = 0;
    int t10 = 0;int t11 = 0;int t12 = 0;int t13 = 0;int t14 = 0;int t15 = 0;int t16 = 0;int t17 = 0;
    int t18 = 0;int t19 = 0;int t20 = 0;int t21 = 0;int t22 = 0;int t23 = 0;int t24 = 0;int t25 = 0;
    int t26 = 0;int t27 = 0;int t28 = 0;int t29 = 0;int t30 = 0;int t31 = 0;int t32 = 0;int t33 = 0;
    int t34 = 0;int t35 = 0;int t36 = 0;int t37 = 0;int t38 = 0;int t39 = 0;int t40 = 0;int t41 = 0;
    int t42 = 0;int t43 = 0;int t44 = 0;int t45 = 0;int t46 = 0;int t47 = 0;int t48 = 0;int t49 = 0;
    int t50 = 0;int t51 = 0;int t52 = 0;int t53 = 0;int t54 = 0;int t55 = 0;int t56 = 0;int t57 = 0;
private slots:
    //上楼请求按键
    void on_up1_clicked();
    //下楼请求按键
    void on_down1_clicked();
    //紧急请求按键
    void on_trouble_clicked();
    //手动开门键
    void on_opendoor_clicked();
    //手动关门键
    void on_closedoor_clicked();
    //设置乘客目的楼层
    void on_btn1_clicked();void on_btn2_clicked();void on_btn3_clicked();void on_btn4_clicked();
    void on_btn5_clicked();void on_btn6_clicked();void on_btn7_clicked();void on_btn8_clicked();
    void on_btn9_clicked();void on_btn10_clicked();void on_btn11_clicked();void on_btn12_clicked();
    void on_btn13_clicked();void on_btn14_clicked();void on_btn15_clicked();void on_btn16_clicked();
    void on_btn17_clicked();void on_btn18_clicked();void on_btn19_clicked();void on_btn20_clicked();
    void on_btn21_clicked();void on_btn22_clicked();void on_btn23_clicked();void on_btn24_clicked();
    void on_btn25_clicked();void on_btn26_clicked();void on_btn27_clicked();void on_btn28_clicked();
    void on_btn29_clicked();void on_btn30_clicked();void on_btn31_clicked();void on_btn32_clicked();
    void on_btn33_clicked();void on_btn34_clicked();void on_btn35_clicked();void on_btn36_clicked();
    void on_btn37_clicked();void on_btn38_clicked();void on_btn39_clicked();void on_btn40_clicked();
    void on_btn41_clicked();void on_btn42_clicked();void on_btn43_clicked();void on_btn44_clicked();
    void on_btn45_clicked();void on_btn46_clicked();void on_btn47_clicked();void on_btn48_clicked();
    void on_btn49_clicked();void on_btn50_clicked();void on_btn51_clicked();void on_btn52_clicked();
    void on_btn53_clicked();void on_btn54_clicked();void on_btn55_clicked();void on_btn56_clicked();
private:
    Ui::elevator *ui;
    //当前乘客数
    int passengerNumber;
    //运动方向
    int moveUpOrDown;
    //运动状态
    int status;
    int id;
    //乘客目的层
    int destFloor;
    //乘客当前楼层
    vector<int> passCurrFloor;
    //当前楼层
    int currFloor;
    //方向请求位
    int Direction;
    //电梯忙闲位
    int busyOrNot;
    //故障位
    int troubleBit;
    //报警激活位
    int warnningBit;
    //开门和关门位
    int openOrClose;
    //电梯内乘客的目的楼层
    vector<int> eachPassengerDestFloor;
    //存储请求位
    //request req[56];
    //等待用户数
    int passengerCounts[56];
    //请求接受位
    bool flag;
    elevator* eleva=NULL;
    //myMainWindow temp2;
    myMainWindow* mym=NULL;
    //controller temp;
    controller* cont=NULL;
};
#endif // ELEVATOR_H
