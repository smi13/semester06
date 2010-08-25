#include <Windows.h>
#include <cstdio>
#include <iostream>

#include <fcntl.h>
#include <io.h>
#include <ios>
#include <iostream>
#include <fstream>

#include "cruler.h"
#include "crerror.h"

HWND hChildWnd;
se_task::Cruler my_ruler;

static const WORD MAX_CONSOLE_LINES = 500;

/**
 * Function, which gives representation of how to use CRuler
 */
LRESULT CALLBACK _childwndProc( HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
   int tmp, x, y;
   char save_pars_string[] = "Save parameters";
   static int lborder = 25, rborder = 10;
   se_task::SettingsBuf buf;
   HDC hDC;
   PAINTSTRUCT ps;
   RECT rc;
   HPEN hpn = CreatePen(PS_SOLID, 1, RGB(200, 100, 200));

   switch (Msg)
   {
   case WM_CREATE:
      try 
      {
         my_ruler.attach(hWnd);
      }
      catch ( se_task::CrError &e )
      {
         printf(e.what());
      }
      return 0;
   case WM_SIZE:
      try 
      {
         my_ruler.resize();
      }
      catch ( se_task::CrError &e )
      {
         printf(e.what());
      }
      return 0;
   case WM_PAINT:
      hDC = BeginPaint(hWnd, &ps);

      GetWindowRect(hWnd, &rc);

      if (200 + lborder < rborder)
      {
         Ellipse(hDC, lborder, 200, 200 + lborder, 400);
         rc.left = lborder;
         rc.right = lborder + 200;
         rc.top = 200;
         rc.bottom = 400;
         DrawText(hDC, save_pars_string, strlen(save_pars_string), &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
      }
      else
      {
         Ellipse(hDC, lborder, 200, rborder, 400);
         rc.left = lborder;
         rc.right = rborder;
         rc.top = 200;
         rc.bottom = 400;
         DrawText(hDC, save_pars_string, strlen(save_pars_string), &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
      }

      GetWindowRect(hWnd, &rc);

      SelectObject(hDC, hpn);

      MoveToEx(hDC, lborder, 0, NULL);
      LineTo(hDC, lborder, rc.bottom);

      MoveToEx(hDC, rborder, 0, NULL);
      LineTo(hDC, rborder, rc.bottom);

      DeleteObject(hpn);
      
      EndPaint(hWnd, &ps);
      return 0;
   case WM_LBUTTONDOWN:
      x = LOWORD(lParam);
      y = HIWORD(lParam);

      if (x >= lborder && x <= lborder + 200 && y >= 200 && y <= 400)
      {
         my_ruler.getState(buf);

         std::cout << std::endl << "Cruler settings buffer saved: " << 
            std::endl << "Left border: " << buf.realLeftBorder <<
            std::endl << "Right border: " << buf.realRightBorder << std::endl <<
            "Tabs: ";

         for (int i = 0; i != buf.tabs.size(); i++)
            std::cout << buf.tabs[i] << " ";

         std::cout << std::endl;
      }

      return 0;
   case se_task::Cruler::CR_LBORDER_CHANGED:

      tmp = HIWORD(lParam);
      lborder = LOWORD(lParam);
      std::cout << std::endl << "CR_LBORDER_CHANGED caught." <<
         std::endl << "Screen units: " << lborder << 
         std::endl << "Centimeters: " << se_task::Cruler::logic2cent(tmp) <<
         std::endl << std::endl;
      InvalidateRect(hWnd, NULL, true);


      return 0;
   case se_task::Cruler::CR_RBORDER_CHANGED:
      tmp = HIWORD(lParam);
      rborder = LOWORD(lParam);
      std::cout << std::endl << "CR_RBORDER_CHANGED caught." <<
         std::endl << "Screen units: " << rborder << 
         std::endl << "Centimeters: " << se_task::Cruler::logic2cent(tmp) <<
         std::endl << std::endl;
      InvalidateRect(hWnd, NULL, true);
      return 0;
   case WM_CLOSE:
      try 
      {
         my_ruler.detach();
      }
      catch ( se_task::CrError &e )
      {
         printf(e.what());
      }
      return 0;
   }

   return DefWindowProc(hWnd, Msg, wParam, lParam);
}

void RedirectIOToConsole()
{
   int hConHandle;
   long lStdHandle;
   CONSOLE_SCREEN_BUFFER_INFO coninfo;
   FILE *fp;

   AllocConsole();

   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
      &coninfo);

   coninfo.dwSize.Y = MAX_CONSOLE_LINES;

   SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
      coninfo.dwSize);

   lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
   hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

   fp = _fdopen(hConHandle, "w");

   *stdout = *fp;

   setvbuf( stdout, NULL, _IONBF, 0 );

   lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
   hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

   fp = _fdopen(hConHandle, "r");

   *stdin = *fp;

   setvbuf(stdin, NULL, _IONBF, 0);

   lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
   hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

   fp = _fdopen( hConHandle, "w" );

   *stderr = *fp;

   setvbuf(stderr, NULL, _IONBF, 0);
   
   std::ios_base::sync_with_stdio();
}



LRESULT CALLBACK _wndProc( HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
   int w = GetSystemMetrics(SM_CXFULLSCREEN), h = GetSystemMetrics(SM_CYFULLSCREEN);

   switch (Msg)
   {
   case WM_CREATE:
      WNDCLASSEX wce;
      RECT rc;

      GetWindowRect(hWnd, &rc);

      wce.cbSize = sizeof(WNDCLASSEX);
      wce.style = CS_HREDRAW | CS_VREDRAW;
      wce.lpfnWndProc = _childwndProc;
      wce.cbClsExtra = 0;
      wce.cbWndExtra = 0;
      wce.hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
      wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
      wce.hCursor = LoadCursor(NULL, IDC_ARROW);
      wce.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
      wce.lpszMenuName = 0;
      wce.lpszClassName = "ChildWCLass";
      wce.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

      RegisterClassEx(&wce);

      hChildWnd = CreateWindowEx(0, "ChildWClass", NULL, WS_CHILD | WS_BORDER, 
         (int)(w * 0.2), (int)(h * 0.05), (int)(w * 0.65), (int)(h * 0.65 * 1.414), 
         hWnd, 0, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

      ShowWindow(hChildWnd, SW_SHOWDEFAULT);
      UpdateWindow(hChildWnd);
      
      return 0;   
   case WM_CLOSE: 
      DestroyWindow(hWnd); 
      return 0;
   case WM_DESTROY: 
      PostQuitMessage(0); 
      return 0;
   }
   
   return DefWindowProc(hWnd, Msg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   char *CmdLine, int CmdShow )
{
   static char AppName[] = "CrulerTest";
   HWND hWnd;
   MSG msg; 
   WNDCLASSEX wndclass;

   RedirectIOToConsole();

   /* Window class structure fulfill */
   wndclass.cbSize = sizeof(wndclass);
   wndclass.style = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc = _wndProc;
   wndclass.cbClsExtra = 0;
   wndclass.cbWndExtra = 0;
   wndclass.hInstance = hInstance;
   wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
   wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
   wndclass.lpszMenuName = 0;
   wndclass.lpszClassName = AppName;
   wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

   if (!RegisterClassEx(&wndclass))
      return 1;

   /* Create window & show it */
   hWnd = CreateWindow(AppName,
      "CrulerTest",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      NULL,
      NULL,
      hInstance,
      CmdLine);

   if (hWnd == NULL)
      return 1;

   ShowWindow(hWnd, SW_SHOWMAXIMIZED);
   UpdateWindow(hWnd);

   /* Main message loop */
   while (GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   return (int)msg.wParam;
}