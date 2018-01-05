#include "graphics.h"
#include <math.h>
#include <stdlib.h>

t_img	*g_new_img(void *mlx_ptr, int width, int height)
{
	t_img	*img;
	
	if(!(img = (t_img*)malloc(sizeof(t_img))))
		return (NULL);
	if(!(img->bits_per_pixel = (int*)malloc(sizeof(int))))
		return (NULL);
	if(!(img->size_line = (int*)malloc(sizeof(int))))
		return (NULL);
	if(!(img->endian = (int*)malloc(sizeof(int))))
		return (NULL);
	img->img = mlx_new_image(mlx_ptr, width, height);
	img->data = mlx_get_data_addr(img->img, img->bits_per_pixel, img->size_line, img->endian);
	return (img);
}

void	g_put_pixel(t_img *img, t_point_int pt, int color)
{
	int	i;

	i = *img->size_line * pt.y + pt.x * (*img->bits_per_pixel / 8);
	img->data[i] = color & 0xff;
	img->data[++i] = color >> 8 & 0xff;
	img->data[++i] = color >> 16;
}

void	g_draw_line_1(t_img *img, t_point_int a, t_point_int b, t_point_int delta, int color)
{
	int	err;

	err = -delta.x>>1;
	while (a.x <= b.x)
	{
		g_put_pixel(img, a, color);
		err += delta.y;
		if (err > 0)
		{
			err -= delta.x;
			a.y++;
		}
		a.x++;
	}
}

void	g_draw_line_3(t_img *img, t_point_int a, t_point_int b, t_point_int delta, int color)
{
	int	err;

	err = (-delta.x)>>1;
	while (a.x <= b.x)
	{
		g_put_pixel(img, a, color);
		err -= delta.y;
		if (err > 0)
		{
			err -= delta.x;
			a.y--;
		}
		a.x++;
	}
}

void	g_draw_line_2(t_img *img, t_point_int a, t_point_int b, t_point_int delta, int color)
{
	int	err;

	err = -delta.y>>1;
	while (a.y <= b.y)
	{
		g_put_pixel(img, a, color);
		err += delta.x;
		if (err > 0)
		{
			err -= delta.y;
			a.x++;
		}
		a.y++;
	}
}

void	g_draw_line_4(t_img *img, t_point_int a, t_point_int b, t_point_int delta, int color)
{
	int	err;

	err = delta.y>>1;
	while (a.y >= b.y)
	{
		g_put_pixel(img, a, color);
		err += delta.x;
		if (err > 0)
		{
			err += delta.y;
			a.x++;
		}
		a.y--;
	}
}

void	g_draw_line(t_img *img, t_point_int a, t_point_int b, int color)
{
	t_point_int	delta;

	if (a.x > b.x)
	{
		g_draw_line(img, b, a, color);
		return ;
	}
	delta.x = b.x - a.x;
	delta.y = b.y - a.y;
	if (abs(delta.x) >= abs(delta.y))
	{
		if (a.y <= b.y)
			g_draw_line_1(img, a, b, delta, color);
		else
			g_draw_line_3(img, a, b, delta, color);
	}
	else
	{
		if (a.y <= b.y)
			g_draw_line_2(img, a, b, delta, color);
		else
			g_draw_line_4(img, a, b, delta, color);
	}
}
