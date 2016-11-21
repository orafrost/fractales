/*
** main.c for main in /home/orafrost/perso/gui
**
** Made by guillame verrier
** Login   <orafrost@epitech.net>
**
** Started on  Wed Nov  2 16:11:13 2016 guillame verrier
** Last update Sat Nov 19 18:29:19 2016 guillame verrier
*/

#include <lapin.h>
#include "gui.h"

void    tekpixel(t_bunny_pixelarray *pix,
                 t_bunny_position *pos,
                 t_color *color)
{
  int   i;

  if (pos->y < pix->clipable.clip_height &&
      pos->y >= 0 && pos->x >= 0 &&
      pos->x < pix->clipable.clip_width)
    {
      i = pos->y * pix->clipable.clip_width + pos->x;
      ((unsigned int *)pix->pixels)[i] = color->full;
  }
}

double		ab(t_complex pos)
{
  double	ret;

  ret = pos.Im * pos.Im + pos.Rm * pos.Rm;
  return (ret);
}

int		check_pixel(t_complex c, t_complex pos)
{
  double	dist;
  int		i;
  double	temp;

  i = 0;
  dist = 0;
  while (dist < 4 && i < IT_MAX)
    {
      temp = pos.Rm;
      pos.Rm = pos.Rm * pos.Rm - pos.Im * pos.Im + c.Rm;
      pos.Im = 2 * temp * pos.Im + c.Im;
      i += 1;
      dist = ab(pos);
    }
  return (i);
}

t_complex	convert(int x, int y)
{
  t_complex	pos;
  double	p[2];

  p[0] = ((double)x / 999);
  p[1] = ((double)y / 899);
  pos.Rm = p[0] * 3 - 1.5;
  pos.Im = p[1] * 3 - 1.5;
  return (pos);
}

void			fractale(t_complex c, t_gui *gui)
{
  int			i;
  int			b;
  t_complex		pos;
  t_bunny_position	position;
  t_color		col;
  int			t;

  i = 0;
  col.argb[0] = 0;
  col.argb[3] = 255;
  while (i < gui->width)
    {
      b = 0;
      while (b < gui->height)
	{
	  col.argb[1] = 0;
	  col.argb[2] = 0;
	  pos = convert(i, b);
	  position.x = i;
	  position.y = b;
	  t = check_pixel(c, pos);
	  if (t == IT_MAX)
	    col.argb[1] = 255;
	  else
	    {
	      col.argb[1] = ((double)t / IT_MAX * log(t) * 255 / 1.5);
	      col.argb[0] = ((double)t / IT_MAX * t / 2 * 255)* 5;
	      col.argb[2] = ((double)t / IT_MAX  * exp(t) * 255);
	    }
	  tekpixel(gui->pix, &position, &col);
	  b += 1;
	}
      i += 1;
    }
}

int	main(int ac, char **av)
{
  t_gui		inter;
  t_complex	c;

  c.Rm = atof(av[1]);
  c.Im = atof(av[2]);
  inter.width = 1000;
  inter.height = 900;
  inter.win = bunny_start(inter.width, inter.height, false, "gui");
  inter.pix = bunny_new_pixelarray(inter.width, inter.height);
  fractale(c, &inter);
  bunny_blit(&inter.win->buffer, &inter.pix->clipable, NULL);
  bunny_display(inter.win);
  bunny_loop(inter.win, 100, &inter);
  return (0);
}
