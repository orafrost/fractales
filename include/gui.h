/*
** gui.h for gui in /home/orafrost/perso/gui/include
** 
** Made by guillame verrier
** Login   <orafrost@epitech.net>
** 
** Started on  Wed Nov  2 16:22:56 2016 guillame verrier
** Last update Thu Nov 17 21:42:53 2016 guillame verrier
*/

#ifndef _GUI_H_
# define _GUI_H_

# include <lapin.h>
# include "math.h"
# define IT_MAX 300

typedef struct	s_complex
{
  double	Rm;
  double	Im;
}		t_complex;

typedef struct		s_widget
{
  int			size[2];
  int			pos[2];
  struct s_widget	*next;
}			t_widget;

typedef struct		s_gui
{
  int			width;
  int			height;
  t_bunny_window	*win;
  t_bunny_pixelarray	*pix;
  t_widget		*wid;
}			t_gui;

#endif /* _GUI_H_ */
