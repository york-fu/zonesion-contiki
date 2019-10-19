/*********************************************************************************************
* 文件：api_lcd.c
* 作者：fuyou 2018.11.1
* 说明：lcd与传感器的修改部分
* 修改：
* 注释：
*********************************************************************************************/
#include "api_lcd.h"


extern short A0;                                                       // 心率
extern float A1;                                                       // 红外体温
extern uint16_t A2, A3;                                                   // 收缩压，舒张压
extern unsigned char D1;
extern void sensor_control(unsigned char cmd);

/*********************************************************************************************
* 名称：option_1_Handle()
* 功能：菜单项1的处理函数
* 参数：status:菜单项的选定状态
* 返回：无
* 修改：
* 注释：自定义菜单项1被选定后进行的操作
*********************************************************************************************/
void option_1_Handle(optionStatus_t status)
{
    menuExitHandle();
}

/*********************************************************************************************
* 名称：option_2_Handle()
* 功能：菜单项2的处理函数
* 参数：status:菜单项的选定状态
* 返回：无
* 修改：
* 注释：自定义菜单项2被选定后进行的操作
*********************************************************************************************/
void option_2_Handle(optionStatus_t status)
{
    if(status==SELECT)
    {
        blood_start();                                  //启动测量
    }
    else if(status==UNSELECT)
    {
        blood_stop();                                   //停止测量
    }
    sensor_control(D1);
}

/*********************************************************************************************
* 名称：MenuOption_init()
* 功能：菜单项初始化
* 参数：无
* 返回：无
* 修改：
* 注释：自定义菜单项的内容和功能
*********************************************************************************************/
void MenuOption_init()
{
    //菜单项1的初始化
    optionDefaultName_set(1," 1-返回主界面");     //菜单项默认名称
    optionSelectName_set(1," 1-返回主界面");      //菜单项被选定后的名称
    optionHintInfo_set(1," ");                  //菜单项被选定后的提示信息，空格就是什么都不显示
    optionCallFunc_set(1,option_1_Handle);      //菜单项被选定后的处理函数
    
    //菜单项2的初始化
    optionDefaultName_set(2," 2-启动血压测量");   //菜单项默认名称
    optionSelectName_set(2," 2-停止血压测量");    //菜单项被选定后的名称
    optionHintInfo_set(2,"血压测量启动中，请稍等...");//菜单项被选定后的提示信息
    optionCallFunc_set(2,option_2_Handle);      //菜单项被选定后的处理函数
}

/*********************************************************************************************
* 名称：TableInfo_init()
* 功能：表格信息初始化
* 参数：无
* 返回：无
* 修改：
* 注释：自定义表格需要显示的信息
*********************************************************************************************/
void TableInfo_init()
{
    tableExplain_set(1,"网线接口");
    tableExplain_set(2,"血压");
    tableExplain_set(3,"心率");
    tableExplain_set(4,"-");
    tableExplain_set(5,"红外体温");
}

/*********************************************************************************************
* 名称：lcdSensor_init()
* 功能：lcd与传感器部分初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void lcdSensor_init()
{
    MenuOption_init();
    TableInfo_init();
    SensorName_set("红外体温","心率","收缩/舒张压");
}

/*********************************************************************************************
* 名称：syncSensorStatus()
* 功能：同步控制类传感器状态
* 参数：无
* 返回：无
* 修改：
* 注释：控制类传感器被上层设置后，需要运行此函数同步屏幕上的显示状态
*********************************************************************************************/
void syncSensorStatus()
{
    //没有需要同步的内容
}

/*********************************************************************************************
* 名称：lcdSensor_poll()
* 功能：lcd与传感器部分轮询
* 参数：无
* 返回：无
* 修改：
* 注释：轮询用于传感器数据的更新
*********************************************************************************************/
void lcdSensor_poll()
{
    char pbuf1[16]={0};
    char pbuf2[16]={0};
    char pbuf3[16]={0};
    
    //更新数据/状态
    sprintf(pbuf1,"%4.1f℃",A1);
    sprintf(pbuf2,"%3dbpm",A0);
    if(A2<0xffff)
        sprintf(pbuf3,"%3d/%dKpa",A2,A3);
    else
        strcpy(pbuf3,"测量失败");
    
    //更新页面1的传感器数据
    SensorData_set(pbuf1,pbuf2,pbuf3);
    //更新页面2的表格数据
    tableData_set(1,"-");
    tableData_set(2,pbuf3);
    tableData_set(3,pbuf2);
    tableData_set(4,"-");
    tableData_set(5,pbuf1);
    //同步控制类传感器显示状态
    syncSensorStatus();
}

