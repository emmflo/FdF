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
	img->height = height;
	img->width = width;
	return (img);
}

void	g_put_pixel(t_img *img, t_point_int pt, int color)
{
	int	i;

	if (pt.x < 0 || pt.x >= img->width || pt.y < 0 || pt.y >= img->height)
		return ;
	i = *img->size_line * pt.y + pt.x * (*img->bits_per_pixel / 8);
	img->data[i] = color & 0xff;
	img->data[++i] = color >> 8 & 0xff;
	img->data[++i] = color >> 16;
}

int	lerp_rbg(int start, int end, double t)
{
	int	color;

	printf("%f\n", t);
	color = 0;
	color += (int)((start & 0xFF) + ((end & 0xFF) - (start & 0xFF)) * t);
	color += (int)((((start >> 8) & 0xFF) + (((end >> 8) & 0xFF) - ((start >> 8) & 0xFF)) * t)) << 8;
	color += (int)(((start >> 16) + ((end >> 16) - (start >> 16)) * t)) << 16;
	return (color);
}

void	g_draw_line_1(t_img *img, t_point_int a, t_point_int b, t_point_int delta, int start_color, int end_color)
{
	int	err;
	int	size;
	int	start;

	err = -delta.x>>1;
	size = a.x - b.x;
	if (size == 0)
		return ;
	start = a.x;
	while (a.x <= b.x)
	{
		g_put_pixel(img, a, lerp_rbg(start_color, end_color, ((start - a.x) / (double)size)));
		err += delta.y;
		if (err > 0)
		{
			err -= delta.x;
			a.y++;
		}
		a.x++;
	}
}

void	g_draw_line_3(t_img *img, t_point_int a, t_point_int b, t_point_int delta, int start_color, int end_color)
{
	int	err;
	int	size;
	int	start;

	err = (-delta.x)>>1;
	size = a.x - b.x;
	if (size == 0)
		return ;
	start = a.x;
	while (a.x <= b.x)
	{
		g_put_pixel(img, a, lerp_rbg(start_color, end_color, ((start - a.x) / (double)size)));
		err -= delta.y;
		if (err > 0)
		{
			err -= delta.x;
			a.y--;
		}
		a.x++;
	}
}

void	g_draw_line_2(t_img *img, t_point_int a, t_point_int b, t_point_int delta, int start_color, int end_color)
{
	int	err;
	int	size;
	int	start;

	err = -delta.y>>1;
	size = b.y - a.y;
	if (size == 0)
		return ;
	start = a.y;
	while (a.y <= b.y)
	{
		g_put_pixel(img, a, lerp_rbg(start_color, end_color, ((a.y - start) / (double)size)));
		err += delta.x;
		if (err > 0)
		{
			err -= delta.y;
			a.x++;
		}
		a.y++;
	}
}

void	g_draw_line_4(t_img *img, t_point_int a, t_point_int b, t_point_int delta, int start_color, int end_color)
{
	int	err;
	int	size;
	int	start;

	err = delta.y>>1;
	size = a.y - b.y;
	if (size == 0)
		return ;
	start = a.y;
	while (a.y >= b.y)
	{
		g_put_pixel(img, a, lerp_rbg(start_color, end_color, ((start - a.y) / (double)size)));
		err += delta.x;
		if (err > 0)
		{
			err += delta.y;
			a.x++;
		}
		a.y--;
	}
}

void	g_draw_line(t_img *img, t_point_int a, t_point_int b, int start_color, int end_color)
{
	t_point_int	delta;

	if (a.x > b.x)
	{
		g_draw_line(img, b, a, end_color, start_color);
		return ;
	}
	delta.x = b.x - a.x;
	delta.y = b.y - a.y;
	if (abs(delta.x) >= abs(delta.y))
	{
		if (a.y <= b.y)
			g_draw_line_1(img, a, b, delta, start_color, end_color);
		else
			g_draw_line_3(img, a, b, delta, start_color, end_color);
	}
	else
	{
		if (a.y <= b.y)
			g_draw_line_2(img, a, b, delta, start_color, end_color);
		else
			g_draw_line_4(img, a, b, delta, start_color, end_color);
	}
}

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
