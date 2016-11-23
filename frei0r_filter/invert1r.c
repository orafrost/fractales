/* contrast0r.c
 * Copyright (C) 2004 Jean-Sebastien Senecal (js@drone.ws)
 * This file is a Frei0r plugin.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "frei0r.h"
#include "frei0r_math.h"
#include "fractale.h"


int f0r_init()
{
  return 1;
}

void f0r_deinit()
{ /* no initialization required */ }

void f0r_get_plugin_info(f0r_plugin_info_t* contrast0r_info)
{
  contrast0r_info->name = "Fractale-Generator";
  contrast0r_info->author = "Adrien Chaix / Guillaume Verrier";
  contrast0r_info->plugin_type = F0R_PLUGIN_TYPE_FILTER;
  contrast0r_info->color_model = F0R_COLOR_MODEL_RGBA8888;
  contrast0r_info->frei0r_version = FREI0R_MAJOR_VERSION;
  contrast0r_info->major_version = 0; 
  contrast0r_info->minor_version = 2; 
  contrast0r_info->num_params =  5; 
  contrast0r_info->explanation = "Generate Fractale";
}

void f0r_get_param_info(f0r_param_info_t* info, int param_index)
{
  printf("get info : %d\n", param_index);
  switch(param_index)
  {
  case 0:
    info->name = "Imaginaire";
    info->type = F0R_PARAM_DOUBLE;
    info->explanation = "The Imaginaire value";
    break;
  case 1:
        info->name = "Reel";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The Real value";
        break;
  case 2:
        info->name = "Iteration";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The maximum iteration";
        break;
  case 3:
        info->name = "Scope X";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The Scope X value";
        break;
  case 4:
        info->name = "Scope Y";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The Scope Y value";
        break;
  }
}

f0r_instance_t f0r_construct(unsigned int width, unsigned int height)
{
  fractale_instance_t* inst = (fractale_instance_t*)calloc(1, sizeof(*inst));
  inst->width = width; inst->height = height;
  return (f0r_instance_t)inst;
}

void f0r_destruct(f0r_instance_t instance)
{
  free(instance);
}

void f0r_set_param_value(f0r_instance_t instance, 
                         f0r_param_t param, int param_index)
{
  printf("param %d \n", param_index);
  assert(instance);
  fractale_instance_t* inst = (fractale_instance_t*)instance;
  
  printf("param %d value %f\n", param_index, *((double *)param));
  switch(param_index)
  {
  case 0:
    inst->im = *((double *)param);
    break;
  case 1:
    inst->re = *((double *)param);
    break;
  case 2:
    inst->it_max = *((double *)param);
    break;
  case 3:
    inst->sx = *((double *)param);
    break;
  case 4:
    inst->sy = *((double *)param);
    break;
  }
}

void f0r_get_param_value(f0r_instance_t instance,
                         f0r_param_t param, int param_index)
{
  assert(instance);
  fractale_instance_t* inst = (fractale_instance_t*)instance;
  
  switch(param_index)
  {
  case 0:
    printf("Param : %f\n", *((double*)param));
    break;
  }
}



void    tekpixel(uint32_t* pix,
                 t_pos *pos,
                 t_color *color, int w, int h)
{
  pix[pos->y * w + pos->x] = color->full;
}


double    ab(t_complex pos)
{
  double  ret;

  ret = pos.Im * pos.Im + pos.Rm * pos.Rm;
  return (ret);
}



int   check_pixel(t_complex c, t_complex pos, int it_max)
{
  double  dist;
  int   i;
  double  temp;

  i = 0;
  dist = 0;
  while (dist < 4 && i < it_max)
    {
      temp = pos.Rm;
      pos.Rm = pos.Rm * pos.Rm - pos.Im * pos.Im + c.Rm;
      pos.Im = 2 * temp * pos.Im + c.Im;
      i += 1;
      dist = ab(pos);
    }
  return (i);
}

t_complex convert(int x, int y)
{
  t_complex pos;
  double  p[2];

  p[0] = ((double)x / 1919);
  p[1] = ((double)y / 1079);
  pos.Rm = p[0] * 3 - 1.3;
  pos.Im = p[1] * 3 - 1.2;
  return (pos);
}

void      fractale(t_complex c, uint32_t* pix,fractale_instance_t* inst)
{
  int     i;
  int     b;
  t_complex   pos;
  t_pos position;
  t_color   col;
  int     t;

  i = 0;
  col.argb[0] = 0;
  col.argb[3] = 255;
  while (i < inst->width)
    {
      b = 0;
      while (b < inst->height)
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
    t = check_pixel(c, pos, inst->it_max);
    if (t == inst->it_max)
      {
        col.full = 0;
      }
    else
      {
        col.argb[1] = ((double)t / inst->it_max * log(t) * 255 / 1.5);
        col.argb[0] = ((double)t / inst->it_max * exp(t) / 2 * 255)* 5;
      }
    tekpixel(pix, &position, &col, inst->width, inst->height);
    b += 1;
  }
      i += 1;
    }
}

void f0r_update(f0r_instance_t instance, double time,
                const uint32_t* inframe, uint32_t* outframe)
{
  assert(instance);
  fractale_instance_t* inst = (fractale_instance_t*)instance;
  
  printf("frame : %d %d | %f %f %f %f %f\n", inst->height, inst->width, inst->im, inst->re, inst->it_max, inst->sx, inst->sy);
  unsigned int w = inst->width;
  unsigned int h = inst->height;
  unsigned int x,y;
  t_complex c;
  c.Rm = inst->re;
  c.Im = inst->im;
  uint32_t* dst = outframe;  
  fractale(c, dst, inst);
}

