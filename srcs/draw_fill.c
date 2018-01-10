/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_fill.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eflorenz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/10 19:25:17 by eflorenz          #+#    #+#             */
/*   Updated: 2018/01/10 19:25:28 by eflorenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"
#include <math.h>
#include <stdlib.h>

void	g_rect_fill(t_img *img, t_point_int a, t_point_int b, int color)
{
	int	y_start;

	y_start = a.y;
	while (a.x < b.x)
	{
		a.y = y_start;
		while (a.y < b.y)
		{
			g_put_pixel(img, a, color);
			a.y++;
		}
		a.x++;
	}
}

void	g_img_fill(t_img *img, int color)
{
	t_point_int	a;
	t_point_int	b;

	a.x = 0;
	a.y = 0;
	b.x = img->width;
	b.y = img->height;
	g_rect_fill(img, a, b, color);
}

void	g_window_fill(t_window *win, int color)
{
	t_img		*screen;

	screen = win->img_front;
	g_img_fill(screen, color);
}
