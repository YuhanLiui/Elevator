#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H
#include <iostream>
#include <stdlib.h>
#include <QMainWindow>
#include <elevator.h>
//#include <elevator.cpp>
#include <mymainwindow.h>
//#include <mymainwindow.cpp>
using namespace std;
namespace Ui {
class myMainWindow;
}
//请求结构体
struct request{
    int floor;//请求所在楼层
    int direction;//请求方向（向上为1，向下为-1,无请求位0）
};
class elevator;
class myMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit myMainWindow(QWidget *parent = nullptr);
    explicit myMainWindow(elevator* ele,QWidget *parent=nullptr);
    ~myMainWindow();
    Ui::myMainWindow* getUi();
    void act();
    bool* getShielding();
    bool* getIsbusy();
    bool* getEmergen();
    elevator* getele();


private slots:
    /*
     * 功能：管理人员点击特殊情况按钮后，判断情况位，是真就将特殊为设置为false，并调用取消特殊情况函数，是假将控制类的特殊情况位设为true，并调用紧急函数
     * 输入参数：无
     * 输出参数：无
    */
    void on_emergencyclicked();
    /*
     * 功能：管理人员根据需求点击电梯1的停用按钮后，判断停用位，是真就将其设置为false，是假将其设为true，并调用停用函数。
     * 输入参数：无
     * 输出参数：无
    */
    void on_diable1clicked();
    /*
     * 功能：管理人员根据需求点击电梯2的停用按钮后，判断停用位，是真就将其设置为false，是假将其设为true，并调用停用函数。
     * 输入参数：无
     * 输出参数：无
    */
    void on_disable2clicked();
    /*
     * 功能：管理人员根据需求点击电梯3的停用按钮后，判断停用位，是真就将其设置为false，是假将其设为true，并调用停用函数。
     * 输入参数：无
     * 输出参数：无
    */
    void on_disable3clicked();
    /*
     * 功能：管理人员根据需求点击电梯4的停用按钮后，判断停用位，是真就将其设置为false，是假将其设为true，并调用停用函数。
     * 输入参数：无
     * 输出参数：无
    */
    void on_disable4clicked();
    /*
     * 功能：管理人员根据需求点击电梯5的停用按钮后，判断停用位，是真就将其设置为false，是假将其设为true，并调用停用函数。
     * 输入参数：无
     * 输出参数：无
    */
    void on_disable5clicked();
    /*
     * 功能：管理人员根据需求点击电梯6的停用按钮后，判断停用位，是真就将其设置为false，是假将其设为true，并调用停用函数。
     * 输入参数：无
     * 输出参数：无
    */
    void on_disable6clicked();
    //设置忙闲状态
    void setIsbusy();

private:
    Ui::myMainWindow *ui;
    elevator* el=NULL;
    bool shielding[6]={false};//屏蔽位数组（true为相应的电梯被屏蔽，false就不屏蔽）
    bool emergency=false;//是有有特殊情况（true为有，false为没有）
    bool isbusy=false;//目前是忙（true）还是闲（false）
};

class controller{
public:
    /*
     * 功能：初始化控制端类（请求屏蔽位为false，故障为为false，特殊情况为false，忙闲为闲，请求全为无请求）；
     * 输入参数：无
     * 输出参数：无
     *
    */
    controller();

    /*
     * 功能：释放类资源；
     * 输入参数：无
     * 输出参数：无
    */
    ~controller();

    /*
     * 功能：如果有电梯发送故障，对相应的电梯进行故障处理（发出某某电梯发生故障警告，调用当前楼层数次下楼函数，然后开门，
     * 并对外呈现故障状态，并屏蔽请求，直到故障被修好，再次点击故障呼叫按钮）；
     * 输入参数：故障位数组,界面的引用
     * 输出参数：无
    */
    void faultAct(bool* fault/*in*/,myMainWindow& ui/*in*/);

    /*
     * 功能：返回控制对象屏蔽位数组；
     * 输入参数：无
     * 输出参数：控制对象的屏蔽位数组的地址
    */
    bool* getShielding();

    /*
     * 功能：返回控制对象故障位数组；
     * 输入参数：无
     * 输出参数：控制对象的故障位数组的地址
    */
    bool* getFault();

    /*
     * 功能：返回控制对象特殊位；
     * 输入参数：无
     * 输出参数：控制对象的特殊位地址
    */
    bool* getEmergen();

    /*
     * 功能：返回控制对象忙闲；
     * 输入参数：无
     * 输出参数：控制对象的忙闲的地址
    */
    bool* getIsbusy();

    /*
     * 功能：返回控制对象请求数组；
     * 输入参数：请求所在楼层数
     * 输出参数：控制对象的该层请求的数组首地址
    */
    request* getRequest(int i);


    /*
     * 功能：出现特殊情况时，对所有电梯进行操作（如果电梯没有运作，对该电梯的所有请求进行屏蔽，如果该电梯在运行，则控制电梯运动
     * 到同向最近楼层，然后开门，然后关门，并屏蔽请求)；
     * 输入参数：紧急情况标志位,界面对象引用，电梯对象数组
     * 输出参数：无
    */
    void emergencyAct(myMainWindow& eui/*in*/,elevator* el/*in*/);

    /*
     * 功能：指派相应的电梯响应相应的请求（先判断当前是闲还是忙，如果是闲，就判断该请求上下五层楼内是否有同向电梯，如果有，就将该请求写入同向最近电梯
     * 内，如果上下五层内，电梯都是异向或上下五层楼内没有电梯则将该请求写入同向最近的电梯中或者最先到达该楼层的异向电梯中，如果忙，就看按规则（第一台1,2
     * 7.....以此类推，第六台可以到达所有楼层）看前五台中可以到达该楼层的电梯以及第六台，看那台同向近，那台近就分配给那台（当第六台满载时就算它近也不分配给它）
     * 如果两台中有一台异向，就分配给同向，如果两个都异向，就分配给最先到达该楼层的电梯中）；
     * 输入参数：请求数组
     * 输出参数：无
    */
    void assignAct(controller& con/*in*/,myMainWindow& eui,elevator* ele);

private:
    bool shielding[6];//相应的第一二三四五六部电梯的请求是否屏蔽（true为屏蔽，false不屏蔽）
    bool fault[6];//相应的第一二三四五六部电梯的请求是否有故障（true为有，false没有）
    bool emergency;//是有有特殊情况（true为有，false为没有）
    bool isbusy;//目前是忙（true）还是闲（false）
    request re[56][6];//接受电梯外部发送的请求
};
#endif // MYMAINWINDOW_H
