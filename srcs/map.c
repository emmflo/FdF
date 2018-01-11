/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eflorenz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/10 18:37:51 by eflorenz          #+#    #+#             */
/*   Updated: 2018/01/10 19:15:40 by eflorenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include "graphics.h"
#include "map.h"

void	map_to_points(t_map *map, t_params *params)
{
	int		i;
	int		j;
	t_point	**points;

	points = (t_point**)malloc(map->width * sizeof(t_point*));
	i = 0;
	while (i < map->width)
	{
		points[i] = (t_point*)malloc(map->height * sizeof(t_point));
		j = 0;
		while (j < map->height)
		{
			points[i][j].x = params->start.x + i * params->scale.x;
			points[i][j].y = params->start.y + j * params->scale.y;
			points[i][j].z = params->start.z + map->map[j][i] * params->scale.z;
			rotate(&points[i][j], params);
			j++;
		}
		i++;
	}
	map->points = points;
}

void	rotate(t_point *pt, t_params *params)
{
	t_point	tmp;
	t_point	*c;

	c = &(params->center);
	tmp.x = cos(params->rot.x) * (pt->x - c->x)
		- sin(params->rot.x) * (pt->y - c->y) + c->x;
	tmp.y = sin(params->rot.x) * (pt->x - c->x)
		+ cos(params->rot.x) * (pt->y - c->y) + c->y;
	pt->x = tmp.x;
	pt->y = tmp.y;
	tmp.y = cos(params->rot.y) * (pt->y - c->y) -
		sin(params->rot.y) * (pt->z - c->z) + c->y;
	tmp.z = sin(params->rot.y) * (pt->y - c->y) +
		cos(params->rot.y) * (pt->z - c->z) + c->z;
	pt->y = tmp.y;
	pt->z = tmp.z;
	tmp.z = cos(params->rot.z) * (pt->z - c->z) -
		sin(params->rot.z) * (pt->x - c->x) + c->z;
	tmp.x = sin(params->rot.z) * (pt->z - c->z) +
		cos(params->rot.z) * (pt->x - c->x) + c->x;
	pt->x = tmp.z;
	pt->x = tmp.x;
}

void	display_line(t_img *img, t_map *map, int i, int j, int k, int l)
{
	t_gradient	tmp;
	t_gradient	color;
	t_point_int	a;
	t_point_int	b;

	a.x = (int)map->points[i][j].x;
	a.y = (int)map->points[i][j].y;
	b.x = (int)map->points[k][l].x;
	b.y = (int)map->points[k][l].y;
	if (map->auto_color)
	{
		color.start = lerp_rbg(map->color,
			(map->map[j][i] - *(map->min)) / (double)(*(map->max) - *(map->min)));
		color.end = lerp_rbg(map->color,
			(map->map[l][k] - *(map->min)) / (double)(*(map->max) - *(map->min)));
	}
	else
	{
		color.start = map->colors[j][i];
		color.end = map->colors[l][k];
	}
	g_draw_line(img, a, b, color);
}

void	display_points(t_img *img, t_map *map)
{
	int			i;
	int			j;

	i = 0;
	while (i < map->width)
	{
		j = 0;
		while (j < map->height)
		{
			if (i > 0)
				display_line(img, map, i, j, i - 1, j);
			if (j > 0)
				display_line(img, map, i, j, i, j - 1);
			j++;
		}
		i++;
	}
}
