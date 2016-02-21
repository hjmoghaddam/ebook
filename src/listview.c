/*
 * listview.c
 *
 *  Created on: ?? Ï? ???? åž.Ô.
 *      Author: Admin
 */
/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2011  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.12 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDEMO_Listview.c
Purpose     : Listview demo
----------------------------------------------------------------------
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math_lib.h>
#include "GUIDEMO.h"
#include "integer.h"

#if (SHOW_GUIDEMO_LISTVIEW && GUI_WINSUPPORT)

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LISTVIEW_SIZE_X 764
#define MIN_SPACE       0

char *temp_c;

char * pText[5];
char   acText[5][50] = {0};
int i_log;
int Index;
int T_Color;
unsigned ColoredCol   = 0;
unsigned ColoredRow   = 0;
extern WORD Buff_hmi[500];
int code_error[50];
int Max_event;
int Type;
WORD Time1,Time2,Time3;
BYTE Status_fd;
BYTE temp_low;
BYTE Num_fd;
WM_HWIN hList;


extern int T_minet;
extern int T_hour;
extern int Time_day;
extern int Time_mounth;
extern int Time_year;
char _Date[],_Time[];

#define Num_Column 5
/*********************************************************************
*
*       Types
*
**********************************************************************
*/
enum {
  JOB_ADDROWS = 1,
  JOB_SETSORT,
  JOB_REVERSE,
  JOB_INCSEL,
  JOB_DECSEL,
  JOB_COLOR
};

/*********************************************************************
*
*       COL_PROP
*/
typedef struct {
  char * pText;
  int    Width;
  int    Align;
  int (* fpCompare)(const void * p0, const void * p1);
} COL_PROP;

/*********************************************************************
*
*       ACTION_ITEM
*/
typedef struct {
  int          Delay;
  int          Job;
  const char * pDescription;
} ACTION_ITEM;

/*********************************************************************
*
*       Static data
*
**********************************************************************
***********************************************************************
*       _Status,NumFeeder,_Comment
*/
char * _status[] = {
"Gas Low","Battery Charge Fail","External AC Power Loss","Oil Level","Oil Temp",
"RTU Door","Station Door","Local/Remote ","Unsolicited Class1","Unsolicited Class2","Unsolicited Class3",
"Open_C1","Open_C2","Open_C3","Open_C4","Open_C5","Open_C6","Open_C7","Open_C8","Open_C9",
"Close_C1","Close_C2","Close_C3","Close_C4","Close_C5","Close_C6","Close_C7","Close_C8","Close_C9",
"Earth_C1","Earth_C2","Earth_C3","Earth_C4","Earth_C5","Earth_C6","Earth_C7","Earth_C8","Earth_C9",
"Lock_C1","Lock_C2","Lock_C3","Lock_C4","Lock_C5","Lock_C6","Lock_C7","Lock_C8","Lock_C9",
"Unlock_C1","Unlock_C2","Unlock_C3","Unlock_C4","Unlock_C5","Unlock_C6","Unlock_C7","Unlock_C8","Unlock_C9",
"FI PhaseA_C1","FI PhaseA_C2","FI PhaseA_C3","FI PhaseA_C4","FI PhaseA_C5","FI PhaseA_C6","FI PhaseA_C7","FI PhaseA_C8","FI PhaseA_C9",
"FI PhaseB_C1","FI PhaseB_C2","FI PhaseB_C3","FI PhaseB_C4","FI PhaseB_C5","FI PhaseB_C6","FI PhaseB_C7","FI PhaseB_C8","FI PhaseB_C9",
"FI PhaseC_C1","FI PhaseC_C2","FI PhaseC_C3","FI PhaseC_C4","FI PhaseC_C5","FI PhaseC_C6","FI PhaseC_C7","FI PhaseC_C8","FI PhaseC_C9",
"FI PhaseN_C1","FI PhaseN_C2","FI PhaseN_C3","FI PhaseN_C4","FI PhaseN_C5","FI PhaseN_C6","FI PhaseN_C7","FI PhaseN_C8","FI PhaseN_C9",
"VI PhaseA_C1","VI PhaseA_C2","VI PhaseA_C3","VI PhaseA_C4","VI PhaseA_C5","VI PhaseA_C6","VI PhaseA_C7","VI PhaseA_C8","VI PhaseA_C9",
"VI PhaseB_C1","VI PhaseB_C2","VI PhaseB_C3","VI PhaseB_C4","VI PhaseB_C5","VI PhaseB_C6","VI PhaseB_C7","VI PhaseB_C8","VI PhaseB_C9",
"VI PhaseC_C1","VI PhaseC_C2","VI PhaseC_C3","VI PhaseC_C4","VI PhaseC_C5","VI PhaseC_C6","VI PhaseC_C7","VI PhaseC_C8","VI PhaseC_C9",
"DP_C1","DP_C2","DP_C3","DP_C4","DP_C5","DP_C6","DP_C7","DP_C8","DP_C9",
"Battery Test","Fault Indicator Reset",
"Order_Close_C1","Order_Close_C2","Order_Close_C3","Order_Close_C4","Order_Close_C5","Order_Close_C6","Order_Close_C7","Order_Close_C8","Order_Close_C9",
"Order_Open_C1","Order_Open_C2","Order_Open_C3","Order_Open_C4","Order_Open_C5","Order_Open_C6","Order_Open_C7","Order_Open_C8","Order_Open_C9",
"Order_Lock_C1","Order_Lock_C2","Order_Lock_C3","Order_Lock_C4","Order_Lock_C5","Order_Lock_C6","Order_Lock_C7","Order_Lock_C8","Order_Lock_C9",
"Order_Unlock_C1","Order_Unlock_C2","Order_Unlock_C3","Order_Unlock_C4","Order_Unlock_C5","Order_Unlock_C6","Order_Unlock_C7","Order_Unlock_C8","Order_Unlock_C9",
};

char * _NumFeeder[] = {
"---","1","2","3","4","5","6","7","8","9",
};

char * _Comment[] = {
"","Unnormal","Normal","Open","Close","Local","Remote","Disable","Enable","Order",
};

/*********************************************************************
*
*       _aColor
*/
static const GUI_COLOR _aColor[] = { GUI_WHITE, GUI_GREEN, GUI_RED, GUI_YELLOW };

/*********************************************************************
*
*       _aColProps
*/
static const COL_PROP _aColProps[] = {
  { "Type",    340, GUI_TA_HCENTER,    LISTVIEW_CompareText},
  { "Num_Feeder",  65, GUI_TA_HCENTER, LISTVIEW_CompareText},
  { "Status", 100, GUI_TA_HCENTER,   LISTVIEW_CompareText },
  { "Time", 120, GUI_TA_HCENTER,   LISTVIEW_CompareText },
  { "Date", 130, GUI_TA_HCENTER,   LISTVIEW_CompareText },
};

/*********************************************************************
*
*       _aAction
*/
static const ACTION_ITEM _aAction[] = {
  {   900, JOB_ADDROWS,    "Adding some rows" },
  {  5, JOB_INCSEL,     NULL  },
  {  5, JOB_DECSEL,     NULL  },
};
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _AddRow
*
* Purpose:
*   Inserts or adds a new row of data to the given LISTVIEW
*
* Parameter:
*   hItem  - Handle of LISTVIEW widget
*   Insert - 1 for inserting a row,0 for adding a row
*/
static int _AddRow(WM_HWIN hItem, int Insert) {
	int    i, r;

  // Create name field
  LISTVIEW_SetFont (hItem,GUI_FONT_20_1);

  strcpy(acText[0], _status[Type]);

  strcpy(acText[1], _NumFeeder[Num_fd]);

  strcpy(acText[2], _Comment[Status_fd]);

  for (i = 0; i < Num_Column; i++) {
    pText[i] =acText[i];
  }

  if (Insert) {
    r = LISTVIEW_InsertRow(hItem,0, pText);
  } else {
    r = LISTVIEW_AddRow(hItem, pText);
  }

  LISTVIEW_SetItemBkColor(hList, 0, ColoredRow, LISTVIEW_CI_UNSEL, _aColor[T_Color]);
  LISTVIEW_SetItemBkColor(hList, 1, ColoredRow, LISTVIEW_CI_UNSEL, _aColor[T_Color]);
  LISTVIEW_SetItemBkColor(hList, 2, ColoredRow, LISTVIEW_CI_UNSEL, _aColor[T_Color]);
  LISTVIEW_SetItemBkColor(hList, 3, ColoredRow, LISTVIEW_CI_UNSEL, _aColor[T_Color]);
  LISTVIEW_SetItemBkColor(hList, 4, ColoredRow, LISTVIEW_CI_UNSEL, _aColor[T_Color]);
  return r;
}

/*********************************************************************
*
*       _InitListview
*
* Purpose:
*   Adds some data to the listview widget
*/
static void _InitListview(WM_HWIN hList) {
  int i;
  HEADER_SetDragLimit(LISTVIEW_GetHeader(hList), 1);
  LISTVIEW_SetFont (hList,GUI_FONT_24_1);
  LISTVIEW_SetHeaderHeight(hList,0);
  LISTVIEW_SetAutoScrollV(hList, 1);
  LISTVIEW_SetGridVis(hList, 1);
  LISTVIEW_SetRBorder(hList, 5);
  LISTVIEW_SetLBorder(hList, 5);
  for (i = 0; i < Num_Column ; i++) {
    LISTVIEW_AddColumn(hList, _aColProps[i].Width, _aColProps[i].pText, _aColProps[i].Align);
    LISTVIEW_SetCompareFunc(hList, i, _aColProps[i].fpCompare);
  }
  LISTVIEW_EnableSort(hList);
}

/*********************************************************************
*
*       _Action
*
* Purpose:
*   Does some action with the widget
*/
 void _Action(WM_HWIN hList) {
  unsigned i, NumRows;
  int ColorIndex   = 0;
  int Reverse      = 0;
  int SortedColumn = -1;
  BYTE d;
  Buff_hmi[300]=5;
  Buff_hmi[301]=257;
  Buff_hmi[302]=45066;
  Buff_hmi[303]=10166;
  Buff_hmi[304]=20993;
  Buff_hmi[305]=5952;
  Buff_hmi[306]=32905;
  Buff_hmi[307]=10422;
  Buff_hmi[308]=5936;
  Buff_hmi[309]=34656;
  Buff_hmi[310]=37034;
  Buff_hmi[311]=10678;
  Buff_hmi[312]=5936;
  Buff_hmi[313]=1793;
  Buff_hmi[314]=20613;
  Buff_hmi[315]=10934;
  Buff_hmi[316]=5936;
  Buff_hmi[317]=15440;
  Buff_hmi[318]=20613;
  Buff_hmi[319]=10934;
  Buff_hmi[320]=5936;
  Max_event = Buff_hmi[300];

  Type=0;

    switch (_aAction[Index].Job) {
	   case JOB_INCSEL:
		  LISTVIEW_IncSel(hList);
		  break;
		case JOB_DECSEL:
		  LISTVIEW_DecSel(hList);
		  break;

	   case JOB_ADDROWS:
		 for (i = 0; i <= (Max_event-1); i++) {
			 Type= Get_High_Byte(Buff_hmi[300+((i*4)+1)]);
			 temp_low=Get_Low_Byte(Buff_hmi[300+((i*4)+1)]);
			 Num_fd = (temp_low & 0xf0)>>4;
			 Status_fd = temp_low & 0x0f;
			 if(Type<5) Status_fd =Status_fd+1;
			 else if(Type>4 && Type<7) Status_fd =Status_fd+3;
			 else if(Type==7)Status_fd =Status_fd+5;
			 else if(Type>7 && Type<11)Status_fd =Status_fd+7;
			 else if(Type>128 && Type<167)Status_fd =9;
			 Time1 = Buff_hmi[300+((i*4)+2)];
			 Time2 = Buff_hmi[300+((i*4)+3)];
			 Time3 = Buff_hmi[300+((i*4)+4)];
			 Get_TimeDate(Time1,Time2,Time3);
			 sprintf(_Time,"%d : %d ",T_hour,T_minet);
			 strcpy(acText[3], _Time);
			 sprintf(_Date,"%d / %d / %d" ,Time_year,Time_mounth,Time_day);
			 strcpy(acText[4], _Date);
			 T_Color=0;
			 ColoredRow = i;
			 if(Type>10 && Type<56)T_Color=1;
			 else if(Type>55 && Type<129)T_Color=2;
			 else if(Type>128 && Type<167)T_Color=3;
			_AddRow(hList, 0);
		  }
		  break;
	   case JOB_COLOR:
	        NumRows = LISTVIEW_GetNumRows(hList);
	        ColoredRow=2;
	        ColoredCol =2;
	        LISTVIEW_SetItemBkColor(hList, ColoredCol, ColoredRow, LISTVIEW_CI_UNSEL, _aColor[T_Color]);

	      break;
    }
    GUI_Delay(_aAction[Index].Delay);
}

/*********************************************************************
*
*       _DemoListview
*/
static void _DemoListview(void) {
  int     xSize, ySize, xSpace, ySpace;

  xSize  = LCD_GetXSize();
  ySize  = LCD_GetYSize();
  if ((CONTROL_SIZE_X + MIN_SPACE) < ((xSize - LISTVIEW_SIZE_X) >> 1)) {
    xSpace = (xSize - LISTVIEW_SIZE_X) >> 1;
  } else {
    xSpace = (xSize - LISTVIEW_SIZE_X - CONTROL_SIZE_X) >> 1;
  }
  ySpace = (ySize - INFO_SIZE_Y) >> 3;
  hList  = LISTVIEW_CreateEx(xSpace, INFO_SIZE_Y + ySpace, LISTVIEW_SIZE_X, ySize - INFO_SIZE_Y - (ySpace << 1), WM_HBKWIN, WM_CF_SHOW, 0, GUI_ID_LISTVIEW0);
  WM_SetFocus(hList);
  _InitListview(hList);
  _Action(hList);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDEMO_Listview
*/
void GUIDEMO_Listview(void) {
  _DemoListview();
}

#else

void GUIDEMO_Listview(void) {}

#endif

/*************************** End of file ****************************/



