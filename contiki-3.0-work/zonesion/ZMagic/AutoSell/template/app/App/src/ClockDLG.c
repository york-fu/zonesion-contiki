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

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_MULTIPAGE_0   (GUI_ID_USER + 0x01)
#define ID_BUTTON_0   (GUI_ID_USER + 0x02)


// USER START (Optionally insert additional defines
extern WM_HWIN CreateClockPage(void);
extern WM_HWIN CreateAlarmSetting(void);
extern WM_HWIN CreateCountTime(void);
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
WM_HWIN hWinCalendar = 0;
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Calendar", ID_WINDOW_0, 0, 20, 320, 220, 0, 0x0, 0 },
  { MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE_0, 0, 0, 320, 220, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "return", ID_BUTTON_0, 0, 0, 40, 25, 0, 0x0, 0 },
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
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Multipage'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);
    MULTIPAGE_SetAlign(hItem,MULTIPAGE_ALIGN_BOTTOM);
    MULTIPAGE_SetFont(hItem,FontList20[System.font]);
    MULTIPAGE_AddEmptyPage(hItem, CreateClockPage(),    "  Clock  ");
    MULTIPAGE_AddEmptyPage(hItem, CreateAlarmSetting(), "  Alarm  ");
    MULTIPAGE_AddEmptyPage(hItem, CreateCountTime(),    "  Timer  ");
    MULTIPAGE_SelectPage(hItem,0);
    //
    // Initialization of 'return'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, FontList16[System.font]);
    WM_SetStayOnTop(hItem, 1);
    BUTTON_SetText(hItem, "<<");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_MULTIPAGE_0: // Notifications sent by 'Multipage'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'return'
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
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
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
*       CreateClock
*/
WM_HWIN CreateClock(void);
WM_HWIN CreateClock(void) {
  WM_HWIN hWin;

  hWinCalendar = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  hWin = hWinCalendar;
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/