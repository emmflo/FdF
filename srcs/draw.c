/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eflorenz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/09 21:25:40 by eflorenz          #+#    #+#             */
/*   Updated: 2018/01/10 19:37:54 by eflorenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"
#include <math.h>
#include <stdlib.h>

t_img		*g_new_img(void *mlx_ptr, int width, int height)
{
	t_img	*img;

	if (!(img = (t_img*)malloc(sizeof(t_img))))
		return (NULL);
	if (!(img->bits_per_pixel = (int*)malloc(sizeof(int))))
		return (NULL);
	if (!(img->size_line = (int*)malloc(sizeof(int))))
		return (NULL);
	if (!(img->endian = (int*)malloc(sizeof(int))))
		return (NULL);
	img->img = mlx_new_image(mlx_ptr, width, height);
	img->data = mlx_get_data_addr(img->img, img->bits_per_pixel,
			img->size_line, img->endian);
	img->height = height;
	img->width = width;
	return (img);
}

void		g_put_pixel(t_img *img, t_point_int pt, int color)
{
	int	i;

	if (pt.x < 0 || pt.x >= img->width || pt.y < 0 || pt.y >= img->height)
		return ;
	i = *img->size_line * pt.y + pt.x * (*img->bits_per_pixel / 8);
	img->data[i] = color & 0xff;
	img->data[++i] = color >> 8 & 0xff;
	img->data[++i] = color >> 16;
}

int			lerp_rbg(t_gradient color, double t)
{
	int	c;
	int	start;
	int	end;

	start = color.start;
	end = color.end;
	c = 0;
	c += (int)((start & 0xFF) + ((end & 0xFF) - (start & 0xFF)) * t);
	c += (int)((((start >> 8) & 0xFF) + (((end >> 8) & 0xFF)
					- ((start >> 8) & 0xFF)) * t)) << 8;
	c += (int)(((start >> 16) + ((end >> 16) - (start >> 16)) * t)) << 16;
	return (c);
}

t_gradient	invert_gradient(t_gradient color)
{
	int	tmp;

	tmp = color.end;
	color.end = color.start;
	color.start = tmp;
	return (color);
}

t_line_int	pt_to_l(t_point_int a, t_point_int b)
{
	t_line_int	l;

	l.a = a;
	l.b = b;
	return (l);
}
