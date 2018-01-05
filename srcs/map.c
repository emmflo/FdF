#include <stdlib.h>
#include <math.h>
#include "graphics.h"
#include "map.h"

void	rotate(t_point *pt, t_point *c, t_point *rot);

t_map	*make_fake_map() {
	t_map	*new_map;
	int		map[10][10] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 10, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 10, 0, 0, 0, 6, 0, 0, 0},
	{0, 0, 10, 10, 5, 5, 6, 0, 0, 0},
	{0, 0, 10, 10, 5, 5, 6, 0, 0, 0},
	{0, 0, 10, 10, 5, 5, 6, 0, 0, 0},
	{0, 0, 10, 10, 5, 5, 6, 0, 0, 0},
	{0, 0, 10, 0, 0, 0, 6, 0, 0, 0},
	{0, 0, 10, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	int		i = 0;
	int		j = 0;

	new_map = (t_map*)malloc(sizeof(t_map));
	new_map->map = (int**)malloc(10 * sizeof(int*));
	while (i < 10) {
		j = 0;
		new_map->map[i] = (int*)malloc(10 * sizeof(int));
		while (j < 10) {
			new_map->map[i][j] = map[i][j];
			j++;
		}
		i++;
	}
	new_map->width = 10;
	new_map->height = 10;
	return (new_map);
}

void	map_to_points(t_map *map, t_point start, t_point rot, t_point scale, t_point center) {
	int			i;
	int 		j;
	t_point		**points;

	points = (t_point**)malloc(map->width * sizeof(t_point*));
	i = 0;
	while (i < map->width)
	{
		points[i] = (t_point*)malloc(map->height * sizeof(t_point));
		j = 0;
		while (j < map->height)
		{
			points[i][j].x = start.x + i * scale.x;
			points[i][j].y = start.y + j * scale.y;
			points[i][j].z = start.z + map->map[j][i] * scale.z;
			rotate(&points[i][j], &center, &rot);
			j++;
		}
		i++;
	}
	map->points = points;
}

void	rotate(t_point *pt, t_point *c, t_point *rot)
{
	t_point	tmp;
	tmp.x = cos(rot->x)*(pt->x-c->x)-sin(rot->x)*(pt->y-c->y)+c->x;
	tmp.y = sin(rot->x)*(pt->x-c->x)+cos(rot->x)*(pt->y-c->y)+c->y;
	pt->x = tmp.x;
	pt->y = tmp.y;
	tmp.y = cos(rot->y)*(pt->y-c->y)-sin(rot->y)*(pt->z-c->z)+c->y;
	tmp.z = sin(rot->y)*(pt->y-c->y)+cos(rot->y)*(pt->z-c->z)+c->z;
	pt->y = tmp.y;
	pt->z = tmp.z;
	tmp.z = cos(rot->z)*(pt->z-c->z)-sin(rot->z)*(pt->x-c->x)+c->z;
	tmp.x = sin(rot->z)*(pt->z-c->z)+cos(rot->z)*(pt->x-c->x)+c->x;
	pt->x = tmp.z;
	pt->x = tmp.x;
}

void	display_points(t_img *img, t_map *map)
{
	int			i;
	int			j;
	t_point_int	a;
	t_point_int	b;

	i = 0;
	while (i < map->width)
	{
		j = 0;
		while (j < map->height)
		{
			if (i > 0)
			{
				a.x = (int)map->points[i][j].x;
				a.y = (int)map->points[i][j].y;
				b.x = (int)map->points[i-1][j].x;
				b.y = (int)map->points[i-1][j].y;
				g_draw_line(img, a, b, 0xFFFFFF);
			}
			if (j > 0)
			{
				a.x = (int)map->points[i][j].x;
				a.y = (int)map->points[i][j].y;
				b.x = (int)map->points[i][j-1].x;
				b.y = (int)map->points[i][j-1].y;
				g_draw_line(img, a, b, 0xFFFFFF);
			}
			j++;
		}
		i++;
	}
}
