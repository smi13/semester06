#include <math.h>
#include <string.h>
#include <stdio.h>

#include <Xatom.h>
#include <Xlib.h>
#include <keysym.h>

#include "comutils.h"
#include "key_codes.h"

#include "asm1_render.h"
#include "asm2_render.h"
#include "c_render.h"

enum
{
  C_RENDERER = 0,   /* Rendering using C code */
  ASM1_RENDERER,     /* Rendering using simple asm code */
  ASM2_RENDERER
} RENDERER;

int x_modif = 1, y_modif = 1, points_number = 70, cur_renderer = C_RENDERER;
const int img_width = 600, img_height = 600; 
unsigned long white;
unsigned long black;

void (*putline[])( int x0, int y0, int x1, int y1, char *data, int bytes_per_line ) = { c_put_line, asm1_put_line, asm2_put_line };

void fillbuf( char *data, int img_width, int img_height, int bytes_per_line )
{  
  int i;
  double t;
 
  putline[cur_renderer](0, 0, img_width - 1, 0, data, bytes_per_line);
  putline[cur_renderer](0, img_height - 1, img_width - 1, img_height - 1, data, bytes_per_line);

  i = 0;

  int x0 = img_width / 2 + 250,
      y0 = img_height / 2;

  while (1)  
  {
     double alpha = 2 * M_PI * i / points_number;

     int x1 = img_width / 2 + 250 * cos(x_modif * alpha),
	 y1 = img_height / 2 + 250 * sin(y_modif * alpha); 

     putline[cur_renderer](x0, y0, x1, y1, data, bytes_per_line);

     x0 = x1;
     y0 = y1;
     i++;

     if (i == points_number)
     {
        putline[cur_renderer](x0, y0, img_width / 2 + 250, img_height / 2, data, bytes_per_line);
        break;
     }
  }
}

void draw( Display *dsp, Drawable *win )
{
  GC gc = XCreateGC(dsp, *win, 0, NULL);     
  char Buf[100] = { 0 }, Buf1[100] = { 0 }, Buf2[100] = { 0 }, Buf3[100] = { 0 }, Buf4[100] = { 0 };

  XSetForeground(dsp, gc, black);
  XFillRectangle(dsp, *win, gc, 0, 0, 600, 800);

  XTextItem titem_time[] = { Buf, strlen(Buf), -3, None },
	    titem_bufx[] = { Buf1, strlen(Buf1), -3, None },
            titem_bufy[] = { Buf2, strlen(Buf2), -3, None },
	    titem_bufp[] = { Buf3, strlen(Buf3), -3, None },
	    titem_bufr[] = { Buf4, strlen(Buf4), -3, None };

  XSetBackground(dsp, gc, black);
  XSetForeground(dsp, gc, white);

  XImage *img = XGetImage(dsp, *win, 0, 100, img_width, img_height, AllPlanes, ZPixmap);

  /* The main part */
  drawing_time = getTime(); 
  fillbuf(img->data, img_width, img_height, img->bytes_per_line);
  drawing_time = getTime() - drawing_time;
  /*****************/

  XPutImage(dsp, *win, gc, img, 0, 0, 0, 100, 600, 600);

  sprintf(titem_time[0].chars, "Time: %llu\0", drawing_time);
  titem_time[0].nchars = strlen(titem_time[0].chars);


  switch (cur_renderer)
  {
  case C_RENDERER:
    sprintf(titem_bufr[0].chars, "Render type: C ");	  
    break;
  case ASM1_RENDERER:
    sprintf(titem_bufr[0].chars, "Render type: simple asm ");	  
    break;
  case ASM2_RENDERER:
    sprintf(titem_bufr[0].chars, "Render type: asm modified (2) ");	  
    break;
  }

  titem_bufr[0].nchars = strlen(titem_bufr[0].chars);

  sprintf(titem_bufx[0].chars, "X modifier: %i\0", x_modif);
  titem_bufx[0].nchars = strlen(titem_bufx[0].chars);
  
  sprintf(titem_bufy[0].chars, "Y modifier: %i\0", y_modif);
  titem_bufy[0].nchars = strlen(titem_bufy[0].chars);
  
  sprintf(titem_bufp[0].chars, "Number of points: %i\0", points_number);
  titem_bufp[0].nchars = strlen(titem_bufp[0].chars);

  XDrawText(dsp, *win, gc, 10, 30, titem_time, 1);
  XDrawText(dsp, *win, gc, 10, 50, titem_bufr, 1); 
  XDrawText(dsp, *win, gc, 450, 40, titem_bufx, 1);
  XDrawText(dsp, *win, gc, 450, 60, titem_bufy, 1);
  XDrawText(dsp, *win, gc, 450, 80, titem_bufp, 1);

  XFreeGC(dsp, gc);
}

int main()
{
  Display *dsp = XOpenDisplay( NULL );

  if (!dsp)
     return 1; 
  
  int screenNumber = DefaultScreen(dsp);
 
  white = WhitePixel(dsp, screenNumber);
  black = BlackPixel(dsp, screenNumber);

  Window win = XCreateSimpleWindow(dsp,
                               DefaultRootWindow(dsp),
                               0, 0,   
                               600, 800, 
                               0, black, 
                               black );  

  XMapWindow( dsp, win );

  XEvent evt;
  long eventMask = KeyPressMask | ExposureMask;
  XSelectInput(dsp, win, eventMask); 

  {
    int done = 0;
  
    while (!done)
    {
      XNextEvent(dsp, &evt);   

      switch (evt.type)
      {
      case KeyPress:
	 {
            KeySym k_sym = XKeycodeToKeysym(dsp, evt.xkey.keycode, 0);

	    switch (k_sym)
	    {
	    case MY_ESCAPE:
              done = 1;
	      break;
	    case MY_RIGHT:
	      if (x_modif < 9)
	      {
	        x_modif++;
	        draw(dsp, &win);
	      }
	      break;
	    case MY_LEFT:
	      if (x_modif > 0)
	      {
	        x_modif--;
      	        draw(dsp, &win);
	      }
	      break;

	    case MY_PLUS:
	      if (y_modif < 9)
	      {
                y_modif++;
      	        draw(dsp, &win);
	      }
	      break;
	    case MY_MINUS:
	      if (y_modif > 0)
	      {
                y_modif--;
      	        draw(dsp, &win);
	      }
	      break;		    
	    case MY_KP1:
	      cur_renderer = C_RENDERER;
	      draw(dsp, &win);
	      break;
	    case MY_KP2:
	      cur_renderer = ASM1_RENDERER;
	      draw(dsp, &win);
	      break;	     
	    case MY_KP3:
	      cur_renderer = ASM2_RENDERER;
	      draw(dsp, &win);
	      break;	     
	    case MY_KP9:
	      if (points_number < 100)
	      {
	         points_number += 10;
		 draw(dsp, &win);
	      }
	      break;
     	    case MY_KP8:
	      if (points_number > 10)
	      {
	         points_number -= 10;
		 draw(dsp, &win);
	      }
	      break;
	    }
         }
	 break;
      case Expose:
	 draw(dsp, &win);
	 break;
      }
   }
  }

  XDestroyWindow(dsp, win);
  XCloseDisplay(dsp);

  return 0;
}

