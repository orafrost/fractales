#ifndef FRACTALE_H_
# define FRACTALE_H_

typedef struct fractale_instance
{
  unsigned int width;
  unsigned int height;
  double im;
  double re;
  double it_max;
  double sx;
  double sy;
} fractale_instance_t;

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
