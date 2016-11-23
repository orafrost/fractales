#ifndef FRACTALE_H_
# define FRACTALE_H_

#define tekpixel(pix, pos, w, color) pix[(pos)->y * w + (pos)->x] = (color)->full
#define ab(pos) (pos.Im * pos.Im + pos.Rm * pos.Rm)

typedef struct fractale_instance
{
  unsigned int width;
  unsigned int height;
  double mandelbrot;
  double im;
  double re;
  double it_max;
  double sx;
  double sy;
  double ox;
  double oy;
  double rc;
  double gc;
  double bc;
} fractale_instance_t;

typedef enum e_params {
  MANDELBROT,
  IM,
  RE,
  IT_MAX,
  SX,
  SY,
  OX,
  OY,
  RC,
  GC,
  BC,
} t_params;

typedef struct s_pos
{
  int x;
  int y;
} t_pos;

typedef union u_color
{
  int full;
  char argb[4];
} t_color;

typedef struct  s_complex
{
  double  Rm;
  double  Im;
}   t_complex;

#endif
