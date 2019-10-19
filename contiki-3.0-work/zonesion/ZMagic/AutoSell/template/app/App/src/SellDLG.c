/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.26                          *
*        Compiled Aug 18 2014, 17:12:05                              *
*        (c) 2014 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "SellDLG.h"
#include "fml_audio/syn6288.h"
#include "fml_stepmotor/fml_stepmotor.h"
#include "apl_picture.h"
#include "ble/ble-net.h"
#include "lte/lte-ec20.h"
#include "lte/lte_zhiyun.h"
#include "rfUart.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_BUTTON_0   (GUI_ID_USER + 0x01)
#define ID_IMAGE_0   (GUI_ID_USER + 0x02)
#define ID_TEXT_0   (GUI_ID_USER + 0x03)
#define ID_TEXT_1   (GUI_ID_USER + 0x04)
#define ID_TEXT_2   (GUI_ID_USER + 0x05)

// USER START (Optionally insert additional defines)
#define QR_WINDOW_SIZE    130
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)

GoodsLast_t GoodsList[GOODS_NUM] = {0};
Sell_t Sell = {NULL};
double MoneyTotal = 0;

static uint16_t Timeout;

const char IntroText[][GOODS_NUM][20]={
  {"Drinks","Cola","Water",},
  {"饮料","可乐","纯净水",},
};

const char QrText[][30]={
  "Scan payment",
  "扫码支付",
};

const char HintText[][30]={
  "The goods are sold out!",
  "商品已售空！",
};

const char CountDown[][2][40]={
  {
    "Remaining payment time:",
    "剩余支付时间:",
  },
  {
    "Payment success",
    "支付成功",
  },
};

// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Sell", ID_WINDOW_0, 0, 20, 320, 220, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "<<", ID_BUTTON_0, 0, 0, 40, 25, 0, 0x0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 10, 40, 160, 120, 0, 0, 0 },
  { TEXT_CreateIndirect, "intro", ID_TEXT_0, 10, 160, 160, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "qrText", ID_TEXT_1, 180, 170, 130, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "count down", ID_TEXT_2, 50, 0, 220, 20, 0, 0x64, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN      hItem;
  int          NCode;
  int          Id;
  // USER START (Optionally insert additional variables)
  char buf[256];
  lte_config_t LteConfig;
  gsm_info_t LteInfo;
  short x,y;
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Sell'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, 0x00C0C0C0);
    //
    // Initialization of '<<'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
    BUTTON_SetText(hItem, "<<");
    //
    // Initialization of 'intro'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, FontList16[System.font]);
    sprintf(buf,"%s ￥%.2f",IntroText[System.font][Sell.index],GoodsList[Sell.index].price);
    TEXT_SetText(hItem, buf);
    //
    // Initialization of 'qrText'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, FontList16[System.font]);
    TEXT_SetText(hItem, QrText[System.font]);
    //
    // Initialization of 'count down'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, FontList16[System.font]);
    // USER START (Optionally insert additional code for further widget initialization)
    Timeout = 60;
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    sprintf(buf,"%s %02ds",CountDown[0][System.language],Timeout);
    TEXT_SetText(hItem, buf);
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by '<<'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        WM_DeleteWindow(pMsg->hWin);
        process_post_synch(&SellProcess,AppCloseEvent,NULL);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  case WM_PAINT:
    x = 10,y = 40;
    sprintf(buf,"0:/Sell/bmp/goods%02d.bmp",Sell.index+1);
    _ShowBMPEx(buf,x,y);
    
    x = 180,y = 35;
    if(GoodsList[Sell.index].num > 0)
    {
      LteConfig = Lte_GetConfig();
      LteInfo = lte_GetInof();
      sprintf(buf,"{ID:%s;KEY:%s;SERVER:%s;BLE:%s;LTE:%s;DATA:%02d,%.2f}",
              LteConfig.id, LteConfig.key, LteConfig.ip, bleGat_mac(), LteInfo.imei, Sell.index+1, GoodsList[Sell.index].price);
      DISPLAY_RENCODE_TO_TFT(x,y,(uint8_t*)buf,QR_WINDOW_SIZE);
    }
    else
    {
      GUI_SetBkColor(GUI_WHITE);
      GUI_FillRect(x,y,x+QR_WINDOW_SIZE-1,y+QR_WINDOW_SIZE-1);
      
      GUI_SetColor(GUI_RED);
      GUI_SetFont(FontList16[System.font]);
      GUI_SetTextMode(GUI_TEXTMODE_TRANS);
      GUI_DispStringAt(HintText[System.language],x,y+(QR_WINDOW_SIZE/2)-8);
    }
    break;
  case WM_TIMER://定时器消息,到时间时有效
    WM_RestartTimer(pMsg->Data.v,1000);
    Timeout--;
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    sprintf(buf,"%s %02ds",CountDown[Sell.payment][System.language],Timeout);
    TEXT_SetText(hItem, buf);
    if(Timeout < 1)
    {
      WM_DeleteWindow(pMsg->hWin);
      process_post_synch(&SellProcess,AppCloseEvent,NULL);
    }
    break;
  case WM_USERMSG_PAYMENT:
    Timeout = 5;
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateSell
*/
WM_HWIN CreateSellDLG(void);
WM_HWIN CreateSellDLG(void) {
  WM_HWIN hWin;

  StatusBarSet(1);
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  WM_CreateTimer(hWin, 0x02, 1000, 0); //创建一个软件定时器
  return hWin;
}

// USER START (Optionally insert additional public code)
void SellPaymentSet(uint8_t status)
{
  if(status)
    Sell.payment = 1;
  else
    Sell.payment = 0;
}

uint8_t SellRequestGet()
{
  return Sell.request;
}

double Sell_MoneyTotalGet()
{
  return MoneyTotal;
}

void SellPayment_ZxbeeGet(char* buf)
{
  sprintf(buf,"%d",Sell.request+Sell.payment);
}

void SellPayment_ZxbeeSet(char* str)
{
  uint8_t temp = atoi(str);
  if(temp & 0x02)
  {
    Sell.payment = 1;
    Sell.request = 0;
    process_post(&SellProcess,AppInputEvent,NULL);
  }
}

void SellNum_ZxbeeGet(char* buf)
{
    sprintf(buf,"%d/%d/%d",
            GoodsList[0].num,GoodsList[1].num,GoodsList[2].num);
}

void SellNum_ZxbeeSet(char* str)
{
  char* ptr = str;
  GoodsList[0].num = atoi(ptr);
  for(short i=1;i<GOODS_NUM;i++)
  {
    ptr = strstr((const char*)ptr,"/");
    GoodsList[i].num = atoi(++ptr);
  }
}

void SellPrice_ZxbeeGet(char* buf)
{
    sprintf(buf,"%.2f/%.2f/%.2f",
            GoodsList[0].price,GoodsList[1].price,GoodsList[2].price);
}

void SellPrice_ZxbeeSet(char* str)
{
  char* ptr = str;
  GoodsList[0].price = atof(ptr);
  for(short i=1;i<GOODS_NUM;i++)
  {
    ptr = strstr((const char*)ptr,"/");
    GoodsList[i].price = atof(++ptr);
  }
}

void SellOpenHandle()
{
  if(bsp_result & BSP_SD)
  {
    /* 挂载文件系统 -- SD卡 */
    FRESULT f_result = f_mount(&fs[0],"0:",1);	/* Mount a logical drive */
    if(f_result!=FR_OK)
    {
      ErrorDialog("Sell","SD card or FatFs error!");
    }
    else 
      bsp_result &= ~BSP_SD;
  }
  
  char* ptr = bleGat_mac();
  if(ptr == NULL)
  {
    bsp_result |= BSP_RF1;
    ErrorDialog("Sell","Get RF1 info error!");
  }
  else
    bsp_result &= ~BSP_RF1;
  
  gsm_info_t LteInfo = lte_GetInof();
  if(LteInfo.imei == NULL)
  {
    bsp_result |= BSP_RF2;
    ErrorDialog("Sell","Get RF2 info error!");
  }
  else
    bsp_result &= ~BSP_RF2;
  
  if(!(bsp_result & BSP_SD) && (!(bsp_result & BSP_RF1) || !(bsp_result & BSP_RF2)))
  {
    if(GoodsList[Sell.index].num > 0)
    {
      Sell.request = 1;
    }
    Sell.hWin = CreateSellDLG();
    if(System.voice)
    {
      syn6288_init();
      uint16_t us_buf[128];
      char text[]={"请，扫码支付"};
      short len = GUI_UC_ConvertUTF82UC(text,strlen(text),us_buf,GUI_COUNTOF(us_buf));
      syn6288_play_unicode(us_buf,len);
    }
  }
  else
  {
    process_post(&SellProcess,AppCloseEvent,NULL);
  }
}


void SellProcessInit()
{
  float temp[GOODS_NUM][2] = {
    {2,4},{2,3.5},{2,2},
  };
  
  for(short i=0;i<GOODS_NUM;i++)
  {
    GoodsList[i].id = i;
    GoodsList[i].num = (uint8_t)temp[i][0];
    GoodsList[i].price = temp[i][1];
  }
  
  Sell.index = -1;
  
  if(System.voice)
  {
    uint16_t us_buf[128];
    char text[]={"欢迎使用，小智售货机"};
    short len = GUI_UC_ConvertUTF82UC(text,strlen(text),us_buf,128);
    syn6288_play_unicode(us_buf,len);
  }
}

void SellProcessPoll()
{
  static uint8_t tick = 0;
  char buf[32] = {0};
  
  if(Sell.request)
  {
    if(tick % 5 == 0)
    {
      sprintf(buf,"{V5=1}");
      RFSendData(buf);
    }
    tick++;
  }
  else if(tick)
    tick = 0;
}

PROCESS(SellProcess, "SellProcess");

PROCESS_THREAD(SellProcess, ev, data)
{
  static struct etimer et_Period; 
  
  PROCESS_BEGIN();
  
  SellProcessInit();
  process_post(&SellProcess,PROCESS_EVENT_TIMER,NULL);
  
  while (1)
  {
    PROCESS_WAIT_EVENT();  
    if(ev == PROCESS_EVENT_TIMER)
    {
      etimer_set(&et_Period,100);
      SellProcessPoll();
    }
    if(ev == AppOpenEvent)
    {
      SellOpenHandle();
    }
    if(ev == AppCloseEvent)
    {
      Sell.index = -1;
      Sell.payment = 0;
      Sell.request = 0;
      CreateDesktop();
    }
    if(ev == AppInputEvent)
    {
      MoneyTotal += GoodsList[Sell.index].price;
      GoodsList[Sell.index].num--;
      
      if(Sell.hWin != 0)
        WM_SendMessageNoPara(Sell.hWin,WM_USERMSG_PAYMENT);
      
      switch(Sell.index)
      {
      case 0:Stepmotor_Run(1,1,1000);
        break;
      case 1:Stepmotor_Run(2,1,1000);
        break;
      case 2:Stepmotor_Run(3,1,1000);
        break;
      }
      
      if(System.voice)
      {
        syn6288_init();
        uint16_t us_buf[128];
        char text[]={"支付成功"};
        short len = GUI_UC_ConvertUTF82UC(text,strlen(text),us_buf,128);
        syn6288_play_unicode(us_buf,len);
      }
    }
  }
  
  PROCESS_END();
}
// USER END

/*************************** End of file ****************************/
