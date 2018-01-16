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

void	add_to_lines(t_map *map, t_point_int a, t_point_int b, t_gradient color, double z)
{
	//t_list		*elem;
	//t_list		*prev;
	t_line	*new;

	if (!(new = (t_line*)malloc(sizeof(t_line))))
		return ;
	new->start = a;
	new->end = b;
	new->color = color;
	new->z_index = z;
	//prev = NULL;
	//elem = map->lines;
	ft_lstadd(&(map->lines), ft_lstnew(new, sizeof(t_line)));
	//printf("%d %d; %d %d\n", new->start.x, new->start.y, new->end.x, new->end.y);
	/*if (elem == NULL)
	{
		map->lines = ft_lstnew(new, sizeof(t_line));
		return ;
	}
	while (elem != NULL)
	{
		if (((t_line*)elem->content)->z_index > z)
		{
			if (prev == NULL)
			{
				map->lines = ft_lstnew(new, sizeof(t_line));
				map->lines->next = elem;
			}
			else
			{
				prev->next = ft_lstnew(new, sizeof(t_line));
				prev->next->next = elem;
			}
			return ;
		}
		prev = elem;
		elem = elem->next;
	}
	if (elem == NULL)
		prev->next = ft_lstnew(new, sizeof(t_line));*/
}

void	add_line(t_img *img, t_map *map, int i, int j, int k, int l)
{
	t_gradient	tmp;
	t_gradient	color;
	t_point_int	a;
	t_point_int	b;

	a.x = (int)map->points[i][j].x;
	a.y = (int)map->points[i][j].y;
	b.x = (int)map->points[k][l].x;
	b.y = (int)map->points[k][l].y;
	if (((a.x < 0 || a.x > img->width || a.y < 0 || a.y > img->height) &&
		(b.x < 0 || b.x > img->width || b.y < 0 || b.y > img->height)))
		return ;
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
	//g_draw_line(img, a, b, color);

	add_to_lines(map, a, b, color, map->points[i][j].z);
}

void	add_points(t_img *img, t_map *map)
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
				add_line(img, map, i, j, i - 1, j);
			if (j > 0)
				add_line(img, map, i, j, i, j - 1);
			j++;
		}
		i++;
	}
}

int	sort_lines(void *a, void *b)
{
	if (((t_line*)a)->z_index > ((t_line*)b)->z_index)
		return (1);
	else
		return (-1);
}

void	delete_points(t_map *map)
{
	int	i;
	t_list *elem;
	t_line *line;
	t_list *tmp;

	elem = map->lines;
	while (elem != NULL)
	{
		line = (t_line*)elem->content;
		//printf("%d %d; %d %d\n", line->start.x, line->start.y, line->end.x, line->end.y);
		//g_draw_line(img, line->start, line->end, line->color);
		free(line);
		tmp = elem;
		elem = elem->next;
		free(tmp);
	}
	map->lines = NULL;
}

t_list		*ft_merge_sort(t_list *list, int cmp(void *, void *));
void	display_lines(t_img *img, t_map *map)
{
	t_list *elem;
	//t_list *tmp;
	t_line *line;

	map->lines = ft_merge_sort(map->lines, &sort_lines);
	elem = map->lines;
	while (elem != NULL)
	{
		line = (t_line*)elem->content;
		//printf("%d %d; %d %d\n", line->start.x, line->start.y, line->end.x, line->end.y);
		g_draw_line(img, line->start, line->end, line->color);
		//free(line);
		//tmp = elem;
		elem = elem->next;
		//free(tmp);
	}
	//map->lines = NULL;
}
