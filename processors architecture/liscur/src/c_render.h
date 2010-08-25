#ifndef _c_render_h_
#define _c_render_h_

void c_put_pixel( char *data, int x, int y, int bytes_per_line );

void c_put_line( int x0, int y0, int x1, int y1, char *data, int bytes_per_line );

#endif /* _c_render_h_ */
