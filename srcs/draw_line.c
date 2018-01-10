/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eflorenz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/10 19:28:45 by eflorenz          #+#    #+#             */
/*   Updated: 2018/01/10 19:37:06 by eflorenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"
#include <stdlib.h>

void	g_draw_line_1(t_img *img, t_line_int l, t_point_int delta,
		t_gradient color)
{
	int	err;
	int	size;
	int	start;

	err = -delta.x >> 1;
	size = l.a.x - l.b.x;
	if (size == 0)
		return ;
	start = l.a.x;
	while (l.a.x <= l.b.x)
	{
		g_put_pixel(img, l.a,
				lerp_rbg(color, ((start - l.a.x) / (double)size)));
		err += delta.y;
		if (err > 0)
		{
			err -= delta.x;
			l.a.y++;
		}
		l.a.x++;
	}
}

void	g_draw_line_3(t_img *img, t_line_int l, t_point_int delta,
		t_gradient color)
{
	int	err;
	int	size;
	int	start;

	err = (-delta.x) >> 1;
	size = l.a.x - l.b.x;
	if (size == 0)
		return ;
	start = l.a.x;
	while (l.a.x <= l.b.x)
	{
		g_put_pixel(img, l.a,
				lerp_rbg(color, ((start - l.a.x) / (double)size)));
		err -= delta.y;
		if (err > 0)
		{
			err -= delta.x;
			l.a.y--;
		}
		l.a.x++;
	}
}

void	g_draw_line_2(t_img *img, t_line_int l, t_point_int delta,
		t_gradient color)
{
	int	err;
	int	size;
	int	start;

	err = -delta.y >> 1;
	size = l.b.y - l.a.y;
	if (size == 0)
		return ;
	start = l.a.y;
	while (l.a.y <= l.b.y)
	{
		g_put_pixel(img, l.a,
				lerp_rbg(color, ((l.a.y - start) / (double)size)));
		err += delta.x;
		if (err > 0)
		{
			err -= delta.y;
			l.a.x++;
		}
		l.a.y++;
	}
}

void	g_draw_line_4(t_img *img, t_line_int l, t_point_int delta,
		t_gradient color)
{
	int	err;
	int	size;
	int	start;

	err = delta.y >> 1;
	size = l.a.y - l.b.y;
	if (size == 0)
		return ;
	start = l.a.y;
	while (l.a.y >= l.b.y)
	{
		g_put_pixel(img, l.a,
				lerp_rbg(color, ((start - l.a.y) / (double)size)));
		err += delta.x;
		if (err > 0)
		{
			err += delta.y;
			l.a.x++;
		}
		l.a.y--;
	}
}

void	g_draw_line(t_img *img, t_point_int a, t_point_int b,
		t_gradient color)
{
	t_point_int	delta;

	if (a.x > b.x)
		g_draw_line(img, b, a, invert_gradient(color));
	if (a.x > b.x ||
		((a.x < 0 || a.x > img->width || a.y < 0 || a.y > img->height) &&
		(b.x < 0 || b.x > img->width || b.y < 0 || b.y > img->height)))
		return ;
	delta.x = b.x - a.x;
	delta.y = b.y - a.y;
	if (abs(delta.x) >= abs(delta.y))
	{
		if (a.y <= b.y)
			g_draw_line_1(img, pt_to_l(a, b), delta, color);
		else
			g_draw_line_3(img, pt_to_l(a, b), delta, color);
	}
	else
	{
		if (a.y <= b.y)
			g_draw_line_2(img, pt_to_l(a, b), delta, color);
		else
			g_draw_line_4(img, pt_to_l(a, b), delta, color);
	}
}
