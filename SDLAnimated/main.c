/*
** main.c for main in /home/orafrost/perso/gui
**
** Made by guillame verrier
** Login   <orafrost@epitech.net>
**
** Started on  Wed Nov  2 16:11:13 2016 guillame verrier
** Last update Mon Nov 21 19:24:10 2016 guillame verrier
*/

#include "gui.h"

void    tekpixel(SDL_Surface *pix,
                 t_pos *pos,
                 t_color *color)
{
  SDL_Rect rect;
  if (pos->y < pix->h &&
      pos->y >= 0 && pos->x >= 0 &&
      pos->x < pix->w)
    {
      rect.x = pos->x;
      rect.y = pos->y;
      rect.h = 1;
      rect.w = 1;
      SDL_FillRect(pix, &rect, color->full);
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

  p[0] = ((double)x / 1919);
  p[1] = ((double)y / 1079);
  pos.Rm = p[0] * 3 - 1.3;
  pos.Im = p[1] * 3 - 1.2;
  return (pos);
}

void			fractale(t_complex c, t_gui *gui)
{
  int			i;
  int			b;
  t_complex		pos;
  t_pos	position;
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
	  /* pour la fractale de mandelbrot met
	     t = check_pixel(pos, c);
	     et tu met c.Rm = 0 et c.Im = 0
	     dans le main
	  */
	  t = check_pixel(c, pos);
	  if (t == IT_MAX)
	    col.argb[1] = 255;
	  else
	    {
	      col.argb[1] = ((double)t / IT_MAX * log(t) * 255 / 1.5);
	      col.argb[0] = ((double)t / IT_MAX * t / 2 * 255)* 5;
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
  SDL_Rect rect;
  SDL_Surface *screen;
  SDL_Event event;
  int	a;

  a = 0;
  rect.x = 0;
  rect.y = 0;
  rect.h = 720;
  rect.w = 1280;
  c.Rm = atof(av[1]);
  c.Im = atof(av[2]);
  inter.width = 1920;
  inter.height = 1080;
  inter.win = SDL_CreateWindow("Fractale Animation", 0, 0, inter.width, inter.height, 0);
  inter.pix = SDL_CreateRGBSurface(0, inter.width, inter.height, 32, 0, 0, 0, 0);
  int delay = 0;
  while(1){
    if (a == 0)
      {
	fractale(c, &inter);
	a += 1;
      }
    screen = SDL_GetWindowSurface(inter.win);
    SDL_BlitSurface(inter.pix, NULL, screen, &rect);
    SDL_UpdateWindowSurface(inter.win);
    SDL_PollEvent( &event );
      switch( event.type ){
        case SDL_KEYDOWN:
          return 0;
          break;
      }
      /* printf("%f %f\n", c.Im, c.Rm); */
      /* c.Rm += 0.01; */
      /* c.Im -= 0.01; */
    }
  return (0);
}
