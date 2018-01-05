#include <mlx.h>
#include <stdio.h>
#include "graphics.h"
#include "keyboard.h"
#include "map.h"
#include "parse.h"

t_point	rot;
t_map	*map;

int		update(void *params)
{
	t_img	*screen;
	t_point_int	a;
	t_point_int	b;
	t_point	start;
	t_point	scale;
	t_point	center;

	screen = ((t_window*)params)->img_front;
	a.x = 0;
	while (a.x < ((t_window*)params)->size_x)
	{
		a.y = 0;
		while (a.y < ((t_window*)params)->size_y)
		{
			g_put_pixel(screen, a, 0);
			a.y++;
		}
		a.x++;
	}
	/*
	a.x = 300;
	a.y = 300;
	b.x = 400;
	b.y = 300;
	g_draw_line(screen, a, b, 0xffffff);
	*/

	start.x = 50;
	start.y = 50;
	start.z = 50;
	scale.x = 50;
	scale.y = 50;
	scale.z = 50;

	center.x = 250;
	center.y = 250;
	center.z = 250;

	//printf("%d %d %d\n", rot.x, rot.y, rot.z);

	map_to_points(map, start, rot, scale, center);
	display_points(screen, map);
	g_update(((t_window*)params));
}

int		main(int argc, char *argv[])
{
	void		*mlx_ptr;
	t_window	*win;

	rot.x = 0;
	rot.y = 0;
	rot.z = 0;
	if (argc >= 2)
		map = get_map_from_path(argv[1]);
	else
		map = make_fake_map();
	if (!(mlx_ptr = mlx_init()))
		return (1);
	if (!(win = g_new_window(mlx_ptr, 1000, 1000, "Test")))
		return (1);
	mlx_key_hook(win->win_ptr, &key, 0);
	mlx_loop_hook(win->mlx_ptr, &update, (void*)win);
	mlx_loop(mlx_ptr);
	return (0);
}
