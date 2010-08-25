#define _COMPILING_44E531B1_14D3_11d5_A025_006067718D04

#include "hook.h"

//Creating shared segment of data (to remember HWND of our window)
#pragma data_seg(".MYDATA")
HWND hWndGlobal = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:.MYDATA,rws")

HINSTANCE hInst;
UINT UWM_MOUSEMOVE;
UINT UWM_LBUTTONUP;
HHOOK hook;

static LRESULT CALLBACK mouseHook( UINT nCode, WPARAM wParam, LPARAM lParam );

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved)
{
   switch (reason)
   {
   case DLL_PROCESS_ATTACH:
      hInst = hInstance;
      UWM_MOUSEMOVE = RegisterWindowMessage(UWM_MOUSEMOVE_MSG);
      UWM_LBUTTONUP = RegisterWindowMessage(UWM_LBUTTONUP_MSG);
      break;
   case DLL_PROCESS_DETACH:
      if (hWndGlobal != NULL)
         clearMouseHook(hWndGlobal);
      break;
   }

   return TRUE;
}

/****************************************************************************
*                                 setMyHook
* Inputs:
*       HWND hWnd: Window to notify
* Result: BOOL
*       TRUE if successful
*	FALSE if error
* Effect: 
*       Sets the hook
****************************************************************************/

__declspec(dllexport) bool setMouseHook( HWND hWnd )
{
   if (hWndGlobal != 0)
      return 0;

   hook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)mouseHook, hInst, 0);

   if (hook != NULL)
   {
      hWndGlobal = hWnd;
      return 1;
   }

   return 0;
}

__declspec(dllexport) bool clearMouseHook( HWND hWnd )
{
   if (hWnd != hWndGlobal || hWnd == 0)
      return 0;

   BOOL unhooked = UnhookWindowsHookEx(hook);

   if (unhooked != 0)
      hWndGlobal = 0;

   return unhooked > 0 ? true : false;
}

/****************************************************************************
*                                   msghook
* Inputs:
*       int nCode: Code value
*	WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       Either 0 or the result of CallNextHookEx
* Effect: 
*       Hook processing function. If the message is a mouse-move message,
*	posts the coordinates to the parent window
****************************************************************************/

static LRESULT CALLBACK mouseHook( UINT nCode, WPARAM wParam, LPARAM lParam )
{
   if (nCode < 0)
   {
      CallNextHookEx(hook, nCode, wParam, lParam);
      return 0;
   }

   MSG *msg = (MSG*)lParam;
   
   if (msg->message == WM_LBUTTONUP || msg->message == WM_NCLBUTTONUP)
      SendMessage(hWndGlobal, UWM_LBUTTONUP, 0, 0);
   
   if (msg->message == WM_MOUSEMOVE || msg->message == WM_NCMOUSEMOVE)
      SendMessage(hWndGlobal, UWM_MOUSEMOVE, 0, 0);
 
   return CallNextHookEx(hook, nCode, wParam, lParam);
}
