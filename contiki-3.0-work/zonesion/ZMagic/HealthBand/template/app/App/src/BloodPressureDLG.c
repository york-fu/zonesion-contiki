/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.32                          *
*        Compiled Oct  8 2015, 11:59:02                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "bloodpressure.h"
#include "sensor.h"	

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_BUTTON_0   (GUI_ID_USER + 0x01)
#define ID_TEXT_0   (GUI_ID_USER + 0x02)
#define ID_GRAPH_0   (GUI_ID_USER + 0x03)
#define ID_TEXT_1   (GUI_ID_USER + 0x04)
#define ID_BUTTON_1   (GUI_ID_USER + 0x05)
#define ID_BUTTON_2   (GUI_ID_USER + 0x06)


// USER START (Optionally insert additional defines)
#define BLOODPRESSURE_BUFFER_SIZE  270
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
const char BloodPressure_TitleText[][32] = {
  "Blood pressure curve",
  "血压曲线",
};

const char BloodPressure_Text1[][64] = {
  "Current value(SBP/DBP):",
  "当前测量值(收缩/舒张压)：",
};

const char BloodPressure_Button1[][32] = {
  "Stop",
  "停止测量",
};

const char BloodPressure_Button2[][32] = {
  "Start",
  "开始测量",
};

static GRAPH_DATA_Handle hData1;
static GRAPH_DATA_Handle hData2;
static short DataBuffer1[BLOODPRESSURE_BUFFER_SIZE] = {0};
static short DataBuffer2[BLOODPRESSURE_BUFFER_SIZE] = {0};
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "BloodPressure", ID_WINDOW_0, 0, 20, 320, 220, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "<<", ID_BUTTON_0, 0, 0, 40, 25, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "title", ID_TEXT_0, 40, 0, 240, 25, 0, 0x64, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 10, 30, 300, 130, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 10, 165, 300, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "stop", ID_BUTTON_1, 40, 190, 80, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "start", ID_BUTTON_2, 200, 190, 80, 25, 0, 0x0, 0 },
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
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  static short tick = 0;
  GRAPH_SCALE_Handle hScale;
  int16_t sbp = 0,dbp = 0;
  short x1_value = 0,x2_value = 0;
  char buf[64];
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of '<<'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, FontList16[System.font]);
    //
    // Initialization of 'title'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, FontList16[System.font]);
    TEXT_SetText(hItem, BloodPressure_TitleText[System.language]);
    //
    // Initialization of 'Graph'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
    GRAPH_SetBorder(hItem, 25, 5, 5, 5);
    GRAPH_SetGridVis(hItem,1);
    GRAPH_SetGridDistX(hItem,20);
    GRAPH_SetGridDistY(hItem,20);
    hData1 = GRAPH_DATA_YT_Create(GUI_GREEN, BLOODPRESSURE_BUFFER_SIZE, DataBuffer1, 0);
    GRAPH_AttachData(hItem, hData1);
    hData2 = GRAPH_DATA_YT_Create(GUI_RED, BLOODPRESSURE_BUFFER_SIZE, DataBuffer2, 0);
    GRAPH_AttachData(hItem, hData2);
    hScale = GRAPH_SCALE_Create(23, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 20);
    GRAPH_AttachScale(hItem, hScale);
    GRAPH_SCALE_SetFactor(hScale,1.25);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, FontList16[System.font]);
    sprintf(buf,"%s 000",BloodPressure_Text1[System.language]);
    TEXT_SetText(hItem, buf);
    //
    // Initialization of 'stop'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, FontList16[System.font]);
    BUTTON_SetText(hItem, BloodPressure_Button1[System.language]);
    //
    // Initialization of 'start'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetFont(hItem, FontList16[System.font]);
    BUTTON_SetText(hItem, BloodPressure_Button2[System.language]);
    // USER START (Optionally insert additional code for further widget initialization)
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
        CreateAppList();
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'stop'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        blood_stop();
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'start'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        blood_start();
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
  case WM_TIMER:
    WM_RestartTimer(pMsg->Data.v, 200);
    
    sbp = SBP_Get();
    dbp = DBP_Get();
    x1_value = (short)((float)sbp*0.6);
    x2_value = (short)((float)dbp*0.6);
    GRAPH_DATA_YT_AddValue(hData1,x1_value);
    GRAPH_DATA_YT_AddValue(hData2,x2_value);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    sprintf(buf,"%s %d/%dbpm",BloodPressure_Text1[System.language],sbp,dbp);
    TEXT_SetText(hItem, buf);
    
    tick++;
    if(tick >= 5)
    {
      tick = 0;
      sprintf(buf,"{A3=%d,A4=%d}",sbp,dbp);
      RFSendData(buf);    
    }
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
*       CreateBloodPressure
*/
WM_HWIN CreateBloodPressure(void);
WM_HWIN CreateBloodPressure(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  WM_CreateTimer(WM_GetClientWindow(hWin), 0x08, 200, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
