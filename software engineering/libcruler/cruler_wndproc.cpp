#include <windows.h>
#include <WindowsX.h>
#include <cmath>

#include "cruler.h"
#include "hook.h"

static UINT UWM_MOUSEMOVE = RegisterWindowMessage(UWM_MOUSEMOVE_MSG);
static UINT UWM_LBUTTONUP = RegisterWindowMessage(UWM_LBUTTONUP_MSG);

#define GET_CRULER() ((se_task::Cruler *)(GetWindowLong(hWnd, GWL_USERDATA)))

void se_task::Cruler::_crulerOnPaint( HWND hWnd )
{
   se_task::Cruler *cptr = GET_CRULER();
   PAINTSTRUCT ps;
   HDC hdc;
   hdc = BeginPaint(hWnd, &ps);
   cptr->_draw(hWnd, hdc);
   EndPaint(hWnd, &ps);
}

void se_task::Cruler::_crulerOnSize( HWND hWnd, UINT state, int cx, int cy )
{
   RECT rc;
   GetClientRect(GetParent(hWnd), &rc);
   MoveWindow(hWnd, 0, 0, rc.right, CRULER_HEIGHT, true);
}

void se_task::Cruler::_crulerOnMouseMove( HWND hWnd, int x, int y, UINT keyFlags )
{
   RECT rc;
   GetWindowRect(hWnd, &rc);

   if (x < 0 || x > rc.right - rc.left)
      return;

   se_task::Cruler *cptr = GET_CRULER();
   int mouse_hooked = cptr->_state.mouseHooked;

   int realLeftBorder = cptr->_state.leftBorder * cptr->_state.screenUnit,
      realRightBorder = (rc.right - rc.left) - cptr->_state.rightBorder * cptr->_state.screenUnit;

   if ((x >= realLeftBorder - cptr->_state.screenUnit && 
        x <= realLeftBorder + cptr->_state.screenUnit) ||
       (x >= realRightBorder - cptr->_state.screenUnit && 
        x <= realRightBorder + cptr->_state.screenUnit) || mouse_hooked != 0)
   {
      SetCursor(LoadCursor(0, IDC_SIZEWE));
   }

   if (mouse_hooked != 0)
   {
      if (mouse_hooked== 1)
      {
         if (x > realLeftBorder + cptr->_state.screenUnit / 2)
         {
            if ((CRULER_CENTIMETERS * 4 - cptr->_state.rightBorder) - cptr->_state.leftBorder > 8)
               cptr->_state.leftBorder++;
         }
         else if (x < realLeftBorder - cptr->_state.screenUnit / 2)
         {
            cptr->_state.leftBorder--;
         }

      }
      else if (mouse_hooked == -1)
      {
         if (x < realRightBorder - cptr->_state.screenUnit / 2)
         {
            if ((CRULER_CENTIMETERS * 4 - cptr->_state.rightBorder) - cptr->_state.leftBorder > 8)
               cptr->_state.rightBorder++;
         }
         else if (x > realRightBorder + cptr->_state.screenUnit / 2)
         {
            cptr->_state.rightBorder--;
         }
      }

      InvalidateRect(hWnd, NULL, true);
   }
}

void se_task::Cruler::_crulerOnLButtonDown( HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags )
{
   RECT rc;
   se_task::Cruler *cptr = GET_CRULER();

   GetWindowRect(hWnd, &rc);

   int realLeftBorder = cptr->_state.leftBorder * cptr->_state.screenUnit,
       realRightBorder = (rc.right - rc.left) - cptr->_state.rightBorder * cptr->_state.screenUnit;

   if (x >= realLeftBorder - cptr->_state.screenUnit / 2 && 
       x <= realLeftBorder + cptr->_state.screenUnit / 2)
   {
      SetCursor(LoadCursor(0, IDC_SIZEWE));
      cptr->_state.mouseHooked = 1;
      setMouseHook(hWnd);
      return;
   }

   if (x >= realRightBorder - cptr->_state.screenUnit / 2 && 
       x <= realRightBorder + cptr->_state.screenUnit / 2)
   {
      SetCursor(LoadCursor(0, IDC_SIZEWE));
      cptr->_state.mouseHooked = -1;
      setMouseHook(hWnd);
      return;
   }

   if (x > realLeftBorder + cptr->_state.screenUnit / 2 && 
       x < realRightBorder - cptr->_state.screenUnit / 2)
   {
      double new_tab = x / cptr->_state.screenUnit,
         diff = new_tab - floor(new_tab);

      if (diff > 0.5)
         cptr->_addTab(ceil(new_tab));
      else
         cptr->_addTab(floor(new_tab));
         
      InvalidateRect(hWnd, NULL, true);
   }
} 

void se_task::Cruler::_crulerOnLButtonUp( HWND hWnd, int x, int y, UINT keyFlags )
{
   se_task::Cruler *cptr = GET_CRULER();

   cptr->_updateBorders();
   cptr->_state.mouseHooked = 0;

   clearMouseHook(hWnd);
}

LRESULT CALLBACK se_task::Cruler::_wndProc( HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
   if (Msg == UWM_MOUSEMOVE)
   {
      RECT rc;
      GetWindowRect(hWnd, &rc);

      POINT pt;
      GetCursorPos(&pt);

      Cruler::_crulerOnMouseMove(hWnd, pt.x - rc.left, pt.y - rc.top, wParam);
      return 0;
   }

   if (Msg == UWM_LBUTTONUP)
   {
      Cruler::_crulerOnLButtonUp(hWnd, 0, 0, 0);
      return 0;
   }

   switch (Msg)
   {
      HANDLE_MSG(hWnd, WM_LBUTTONDOWN, Cruler::_crulerOnLButtonDown);
      HANDLE_MSG(hWnd, WM_LBUTTONUP, Cruler::_crulerOnLButtonUp);
      HANDLE_MSG(hWnd, WM_MOUSEMOVE, Cruler::_crulerOnMouseMove);
      HANDLE_MSG(hWnd, WM_PAINT, Cruler::_crulerOnPaint);
      HANDLE_MSG(hWnd, WM_SIZE, Cruler::_crulerOnSize);
   
   }

   return DefWindowProc(hWnd, Msg, wParam, lParam);
}