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
  contrast0r_info->num_params =  11; 
  contrast0r_info->explanation = "Generate Fractale";
}

void f0r_get_param_info(f0r_param_info_t* info, int param_index)
{
  printf("get info : %d\n", param_index);
  switch(param_index)
  {
  case MANDELBROT:
        info->name = "Mandelbrot";
        info->type = F0R_PARAM_BOOL;
        info->explanation = "Display Mandelbrot fractal";
        break;
  case IM:
        info->name = "Imaginaire";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The Imaginaire value";
        break;
  case RE:
        info->name = "Reel";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The Real value";
        break;
  case IT_MAX:
        info->name = "Iteration";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The maximum iteration";
        break;
  case SX:
        info->name = "Scope X";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The Scope X value";
        break;
  case SY:
        info->name = "Scope Y";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The Scope Y value";
        break;
  case OX:
        info->name = "Origin X";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The Origin X value";
        break;
  case OY:
        info->name = "Origin Y";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The Origin Y value";
        break;
  case RC:
        info->name = "Red Component";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The Red component value";
        break;
  case GC:
        info->name = "Green Component";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The Green component value";
        break;
  case BC:
        info->name = "Blue Component";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "The Blue component value";
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
  double value = *((double *)param);
  assert(instance);
  fractale_instance_t* inst = (fractale_instance_t*)instance;
  printf("param %d value %f\n", param_index, value);
  switch(param_index)
  {
    case MANDELBROT: inst->mandelbrot = value; break;  
    case IM: inst->im = value; break;
    case RE: inst->re = value; break;
    case IT_MAX: inst->it_max = value; break;
    case SX: inst->sx = log(7.0) - log(value + 1); break;
    case SY: inst->sy = log(7.0) - log(value + 1); break;
    case OX: inst->ox = value; break;
    case OY: inst->oy = value; break;
    case RC: inst->rc = value; break;
    case GC: inst->gc = value; break;
    case BC: inst->bc = value; break;
  }
}

void f0r_get_param_value(f0r_instance_t instance,
                         f0r_param_t param, int param_index)
{

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

t_complex convert(t_pos *posIn, fractale_instance_t* inst)
{
  t_complex pos;
  double  p[2];

  p[0] = ((double)posIn->x / (inst->width - 1));
  p[1] = ((double)posIn->y / (inst->height - 1));
  pos.Rm = p[0] * (inst->sx + inst->sx) - inst->sx + inst->ox;
  pos.Im = p[1] * (inst->sy + inst->sy) - inst->sy + inst->oy;
  return (pos);
}

void      fractale(t_complex c, uint32_t* pix, fractale_instance_t* inst)
{
  t_complex   pos;
  t_pos position;
  t_color   col;
  int     t;

  position.x = -1;
  printf("%f %f %f\n", inst->bc, inst->gc, inst->rc);
  printf("scope: %f %f\n", inst->sx, inst->sy);
  while (++position.x < inst->width)
    {
      position.y = -1;
      while (++position.y < inst->height)
      {
        col.full = 0;
        col.argb[3]= 255;
        pos = convert(&position, inst);
        t = (inst->mandelbrot) ? check_pixel(pos, c, inst->it_max) : check_pixel(c, pos, inst->it_max);
        if (t == inst->it_max) {
          col.full = 0;
        }
        else {
          col.argb[2] = ((double)t / inst->it_max * t * inst->bc);
          col.argb[1] = ((double)t / inst->it_max * t * inst->gc);
          col.argb[0] = ((double)t / inst->it_max * t * inst->rc);
        }
        tekpixel(pix, &position, inst->width, &col);
      }
    }
}

void f0r_update(f0r_instance_t instance, double time,
                const uint32_t* inframe, uint32_t* outframe)
{
  assert(instance);
  
  fractale_instance_t* inst = (fractale_instance_t*)instance;
  
  t_complex c;
  c.Rm = inst->re;
  c.Im = inst->im;
  
  fractale(c, outframe, inst);
}

