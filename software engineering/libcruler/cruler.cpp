#include <windows.h>
#include <cstdio>
#include <cmath>

#include "cruler.h"
#include "crerror.h"

#define CRULER_CLASS_NAME "cruler"

static HFONT hFont;

se_task::Cruler::Cruler() : _hWnd(NULL), _hParentWnd(NULL)
{
   hFont = CreateFont(11, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
      CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, 0);
   _state.leftBorder = 1;
   _state.rightBorder = 1;
}

se_task::Cruler::~Cruler()
{
   detach();
   _state.tabs.clear();
}

void se_task::Cruler::attach( HWND _hMainWnd )
{
   if (_hWnd != NULL)
      throw CR_ERROR("Trying to attach CRuler to two windows", "This is impossible");

   _hParentWnd = _hMainWnd;

   _registerClass();

   RECT rc;

   GetClientRect(_hMainWnd, &rc);

   _hWnd = CreateWindow(CRULER_CLASS_NAME, NULL, WS_CHILD | WS_VISIBLE,
      0, 0, rc.right, CRULER_HEIGHT,
      _hParentWnd, NULL, (HINSTANCE)GetWindowLong(_hParentWnd, GWL_HINSTANCE), NULL);

   if (_hWnd == NULL)
      throw CR_ERROR("CRuler window creating procedure failed", "Check CRuler initialization parameters");

   _state.screenUnit = 0.25 * (rc.right - rc.left) / (double)CRULER_CENTIMETERS;

   _state.mouseHooked = 1;
   _updateBorders();
   _state.mouseHooked = -1;
   _updateBorders();
   _state.mouseHooked = 0;
   
   SetWindowLong(_hWnd, GWL_USERDATA, (LONG)(this));
}

void se_task::Cruler::reattach( HWND _hMainWnd )
{
   detach();
   attach(_hMainWnd);
}

void se_task::Cruler::detach()
{
   if (_hWnd == NULL)
      throw CR_ERROR("Trying to detach not attached CRuler", "Check detach call");

   DestroyWindow(_hWnd);
   _hWnd = _hParentWnd = NULL;
}

void se_task::Cruler::getState( SettingsBuf &buf )
{
   buf.realLeftBorder = logic2cent(_state.leftBorder);
   buf.realRightBorder = logic2cent(_state.rightBorder);

   for (int i = 0; i != _state.tabs.size(); i++)
      buf.tabs.push_back(logic2cent(_state.tabs[i]));
}

void se_task::Cruler::setState( const SettingsBuf &buf )
{
   _state.leftBorder = cent2logic(buf.realLeftBorder);
   _state.rightBorder = cent2logic(buf.realRightBorder);

   _state.tabs.resize(buf.tabs.size());

   for (int i = 0; i != buf.tabs.size(); i++)
      _state.tabs[i] = cent2logic(buf.tabs[i]);
}

void se_task::Cruler::_draw( HWND hWnd, HDC hDC )
{
   int height_offset;
   RECT rc;

   GetWindowRect(hWnd, &rc);

   SelectObject(hDC, hFont);
 
   int realLeftBorder = _state.leftBorder * _state.screenUnit,
      realRightBorder = (rc.right - rc.left) - _state.rightBorder * _state.screenUnit;

   height_offset = (rc.bottom - rc.top) * 0.2;

   HPEN hpn = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
   HBRUSH hbr = CreateSolidBrush(RGB(130, 150, 255));

   SelectObject(hDC, hpn);

   SelectObject(hDC, hbr);
   Rectangle(hDC, 0, height_offset, _state.leftBorder * _state.screenUnit, CRULER_HEIGHT - height_offset);
   Rectangle(hDC, (rc.right - rc.left) - _state.rightBorder * _state.screenUnit, height_offset, 
      rc.right, CRULER_HEIGHT - height_offset);

   SelectObject(hDC, GetStockObject(WHITE_BRUSH));
   Rectangle(hDC, _state.leftBorder * _state.screenUnit, height_offset, 
      (rc.right - rc.left) - _state.rightBorder * _state.screenUnit, 
      CRULER_HEIGHT - height_offset);

   DeleteObject(hpn);
   DeleteObject(hbr);

   SetBkMode(hDC, TRANSPARENT);

   TEXTMETRIC tm;
   GetTextMetrics(hDC, &tm);

   hpn = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

   SelectObject(hDC, hpn);

   for (int i = -_state.leftBorder + _state.leftBorder % 4; i < CRULER_CENTIMETERS * 4; i += 4)
   {
      if (i != 0 && (i + _state.leftBorder + _state.rightBorder) != CRULER_CENTIMETERS * 4)
      {
         char buf[3] = { 0 };
         sprintf(buf, "%i", abs(i / 4));
         
         int pos_x = realLeftBorder + _state.screenUnit * i;

         if (pos_x < rc.right - rc.left - (int)strlen(buf) * tm.tmAveCharWidth / 2 && pos_x > 0)
            TextOut(hDC, pos_x - strlen(buf) * tm.tmAveCharWidth / 2, 
               CRULER_HEIGHT / 2 - tm.tmHeight / 2, buf, strlen(buf));
      }

      SetPixel(hDC, realLeftBorder + _state.screenUnit * (i + 1), CRULER_HEIGHT / 2, RGB(0, 0 ,0));

      MoveToEx(hDC, realLeftBorder + _state.screenUnit * (i + 2), CRULER_HEIGHT / 2 - 2, 0);
      LineTo(hDC, realLeftBorder + _state.screenUnit * (i + 2), CRULER_HEIGHT / 2 + 2);
      
      SetPixel(hDC, realLeftBorder + _state.screenUnit * (i + 3), CRULER_HEIGHT / 2, RGB(0, 0 ,0));
   }

   DeleteObject(hpn);

   hpn = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

   SelectObject(hDC, hpn);

   for (int i = 0; i != _state.tabs.size(); i++)
   {
      int pos = _state.tabs[i] * _state.screenUnit;

      MoveToEx(hDC, pos, CRULER_HEIGHT / 2, NULL);
      LineTo(hDC, pos, CRULER_HEIGHT / 2 + height_offset);

      MoveToEx(hDC, pos, CRULER_HEIGHT / 2 + height_offset, NULL);
      LineTo(hDC, pos + height_offset / 2, CRULER_HEIGHT / 2 + height_offset);
   }

   DeleteObject(hpn);
}

void se_task::Cruler::_registerClass()
{
   DWORD err_code;
   WNDCLASSEX wce;

   if (!GetClassInfoEx((HINSTANCE)GetWindowLong(_hParentWnd, GWL_HINSTANCE), CRULER_CLASS_NAME, &wce))
   {
      err_code = GetLastError();

      if (err_code == ERROR_CLASS_DOES_NOT_EXIST)
      {
         WNDCLASSEX cruler_class;

         cruler_class.cbSize = sizeof(WNDCLASSEX);
         cruler_class.style = CS_HREDRAW | CS_VREDRAW;
         cruler_class.lpfnWndProc = Cruler::_wndProc;
         cruler_class.cbClsExtra = 0;
         cruler_class.cbWndExtra = 0;
         cruler_class.hInstance = 0;
         cruler_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
         cruler_class.hCursor = LoadCursor(NULL, IDC_ARROW);
         cruler_class.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
         cruler_class.lpszMenuName = 0;
         cruler_class.lpszClassName = CRULER_CLASS_NAME;
         cruler_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

         if (!RegisterClassEx(&cruler_class))
            exit(0); //exception here, cruler class can not be registered
      }
      else
      {
         //throw Exception();
         //exception here: unknown error
      }
   }
}

void se_task::Cruler::resize()
{
   SendMessage(_hWnd, WM_SIZE, 0, 0);
}

void se_task::Cruler::_updateBorders()
{
   if (_state.mouseHooked != 0)
   {
      RECT rc;
      GetWindowRect(_hWnd, &rc);

      int realLeftBorder = _state.leftBorder * _state.screenUnit,
         realRightBorder = (rc.right - rc.left) - _state.rightBorder * _state.screenUnit;

      if (_state.mouseHooked == 1)
         PostMessage(_hParentWnd, CR_LBORDER_CHANGED, MAKEWPARAM(_state.screenUnit, 0), 
         MAKELPARAM(realLeftBorder, _state.leftBorder));
      else if (_state.mouseHooked == -1) 
         PostMessage(_hParentWnd, CR_RBORDER_CHANGED, MAKEWPARAM(_state.screenUnit, 0), 
         MAKELPARAM(realRightBorder, _state.rightBorder));
   }
}

void se_task::Cruler::_addTab( int tab )
{
   for (int i = 0; i != _state.tabs.size(); i++)
   {
      if (_state.tabs[i] == tab)
         return;
   }

   _state.tabs.push_back(tab);

   PostMessage(_hParentWnd, CR_TAB_ADDED, MAKEWPARAM(_state.screenUnit, 0), 
      MAKELPARAM(tab * _state.screenUnit, tab));
}

double se_task::Cruler::logic2cent( int logic_var )
{
   return logic_var / 4 + 0.25 * (logic_var % 4);
}

int se_task::Cruler::cent2logic( double cent_var )
{
   double tmp1, tmp2;

   tmp1 = cent_var - floor(cent_var);

   if (tmp1 > 0.5)
      tmp2 = ceil(cent_var);
   else
      tmp2 = floor(cent_var);

   tmp2 += tmp1 / 0.25;

   return (int)tmp2;
}