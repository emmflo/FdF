/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eflorenz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/09 21:15:54 by eflorenz          #+#    #+#             */
/*   Updated: 2018/01/10 18:31:34 by eflorenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdio.h>
#include "graphics.h"
#include "keyboard.h"
#include "map.h"
#include "parse.h"
#include "env.h"

void	update_translations(t_env *env)
{
	if (env->keys.act[left])
		env->params->start.x -= 4;
	else if (env->keys.act[right])
		env->params->start.x += 4;
	if (env->keys.act[up])
		env->params->start.y -= 4;
	else if (env->keys.act[down])
		env->params->start.y += 4;
}

void	update_rotations(t_env *env)
{
	if (env->keys.act[rot_x])
		env->params->rot.x += 0.1;
	else if (env->keys.act[rot_inv_x])
		env->params->rot.x -= 0.1;
	if (env->keys.act[rot_y])
		env->params->rot.y += 0.1;
	else if (env->keys.act[rot_inv_y])
		env->params->rot.y -= 0.1;
	if (env->keys.act[rot_z])
		env->params->rot.z += 0.1;
	else if (env->keys.act[rot_inv_z])
		env->params->rot.z -= 0.1;
}

void	update_zoom(t_env *env)
{
	if (env->keys.act[zoom])
	{
		env->params->start.x -= fabs(env->params->center.x - (env->params->start.x + ((env->params->scale.x + 1) * env->map->width / 2)));
		env->params->start.y -= fabs(env->params->center.y - (env->params->start.y + ((env->params->scale.y + 1) * env->map->height / 2)));
		env->params->start.z -= fabs(env->params->center.z - (env->params->start.z + (((abs(*(env->map->max) - *(env->map->min)) * (env->params->scale.z - 1)) / 2))));
		env->params->scale.x += 1;
		env->params->scale.y += 1;
		env->params->scale.z += 1;
	}
	else if (env->keys.act[dezoom])
	{
		if (env->params->scale.x > 1)
			env->params->start.x += fabs(env->params->center.x - (env->params->start.x + ((env->params->scale.x - 1) * env->map->width / 2)));
		if (env->params->scale.y > 1)
			env->params->start.y += fabs(env->params->center.y - (env->params->start.y + ((env->params->scale.y - 1) * env->map->height / 2)));
		if (env->params->scale.z > 1)
			env->params->start.z += fabs(env->params->center.z - (env->params->start.z + (((abs(*(env->map->max) - *(env->map->min)) * (env->params->scale.z - 1)) / 2))));
		if (env->params->scale.x > 1)
			env->params->scale.x -= 1;
		if (env->params->scale.y > 1)
			env->params->scale.y -= 1;
		if (env->params->scale.z > 1)
			env->params->scale.z -= 1;
	}
}

void	update_scale(t_env *env)
{
	if (env->keys.act[scale_z_up])
		env->params->scale.z += 1;
	else if (env->keys.act[scale_z_down])
		if (env->params->scale.z > 1)
			env->params->scale.z -= 1;
}

void	update_keys(t_env *env)
{
	update_translations(env);
	update_rotations(env);
	update_zoom(env);
	update_scale(env);
	if (env->keys.act[color_auto_on])
		env->map->auto_color = 1;
	if (env->keys.act[color_auto_off])
		env->map->auto_color = 0;
	if (env->keys.act[quit])
		exit(0);
}

int		update(t_env *env)
{
	g_window_fill(env->win, 0);
	update_keys(env);
	env->params->center.x = env->params->start.x + (env->params->scale.x * env->map->width / 2);
	env->params->center.y = env->params->start.y + (env->params->scale.y * env->map->height / 2);
	env->params->center.z = env->params->start.z + ((abs(*(env->map->max) - *(env->map->min)) * env->params->scale.z) / 2);
	map_to_points(env->map, env->params);
	display_points(env->win->img_front, env->map);
	g_update(env->win);
}

t_env	*set_env()
{
	t_env	*env;

	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (NULL);
	if (!(env->params = (t_params*)malloc(sizeof(t_params))))
		return (NULL);
	env->params->rot.x = 0;
	env->params->rot.y = 0;
	env->params->rot.z = 0;
	env->params->scale.x = 50;
	env->params->scale.y = 50;
	env->params->scale.z = 50;
	key_set_default(&(env->keys));
	return (env);
}

int		init_win(t_env *env)
{
	void		*mlx_ptr;

	if (!(mlx_ptr = mlx_init()))
		return (0);
	if (!(env->win = g_new_window(mlx_ptr, 1000, 1000, "Test")))
		return (0);
	return (1);
}

int		init_map(int argc, char *argv[], t_env *env)
{
	int	color;

	if (argc >= 2)
	{
		printf("ARGC %d\n", argc);
		if (argc == 3)
			color = strtol(argv[2], NULL, 16);
		else
			color = 0xFFFFFF;
		if (!(env->map = get_map_from_path(argv[1], color)))
			return (0);
		if (argc == 4)
		{
			env->map->color.start = strtol(argv[2], NULL, 16);
			env->map->color.end = strtol(argv[3], NULL, 16);
			env->map->auto_color = 1;
		}
		else
		{
			env->map->color.start = 0xFF00FF;
			env->map->color.end = 0xFFFF00;
			env->map->auto_color = 0;
		}
	}
	else
		return (0);
	return (1);
}

int		init_env(t_env *env)
{
	env->params->start.x = env->win->size_x / 2 - (env->params->scale.x * env->map->width / 2);
	env->params->start.y = env->win->size_y / 2 - (env->params->scale.y * env->map->height / 2);
	env->params->start.z = 0;
	env->params->center.x = env->params->start.x + (env->params->scale.x * env->map->width / 2);
	env->params->center.y = env->params->start.y + (env->params->scale.y * env->map->height / 2);
	env->params->center.z = (abs(*(env->map->max)
				- *(env->map->min)) * env->params->scale.z) / 2;
}


int		main(int argc, char *argv[])
{
	t_env		*env;

	if (!(env = set_env()))
		return (-1);
	if (!init_win(env))
		return (-1);
	if (!init_map(argc, argv, env))
		return (-1);
	if (!init_env(env))
		return (-1);
	mlx_hook(env->win->win_ptr, KeyPress, KeyPressMask, &(key_press), env);
	mlx_hook(env->win->win_ptr, KeyRelease, KeyReleaseMask, &(key_release),
			env);
	mlx_loop_hook(env->win->mlx_ptr, &update, (void*)env);
	mlx_loop(env->win->mlx_ptr);
	return (0);
}
