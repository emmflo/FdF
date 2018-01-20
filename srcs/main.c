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
#include "imgui.h"
#include <time.h>

int		mouse_move(int x, int y, t_env *env)
{
	//printf("MOUSE X%d Y%d\n", x, y);
	update_ui_coord(env->ui, x, y);
}

int		mouse_press_button(int button, int x, int y, t_env *env)
{
	//printf("BUTTON %d PRESSED\n", button);
	update_ui_coord(env->ui, x, y);
	if (button == 1)
		update_ui_mouse_button(env->ui, 1);
}

int		mouse_release_button(int button, int x, int y, t_env *env)
{
	//printf("BUTTON %d RELEASED\n", button);
	update_ui_coord(env->ui, x, y);
	if (button == 1)
		update_ui_mouse_button(env->ui, 0);
}

void	update_translations(t_env *env)
{
	if (env->keys.act[left])
	{
		env->params->start.x -= 4;
		env->map->changed = 1;
	}
	else if (env->keys.act[right])
	{
		env->params->start.x += 4;
		env->map->changed = 1;
	}
	if (env->keys.act[up])
	{
		env->params->start.y -= 4;
		env->map->changed = 1;
	}
	else if (env->keys.act[down])
	{
		env->params->start.y += 4;
		env->map->changed = 1;
	}
}

void	update_rotations(t_env *env)
{
	if (env->keys.act[rot_x])
	{
		env->params->rot.x += 0.1;
		env->map->changed = 1;
	}
	else if (env->keys.act[rot_inv_x])
	{
		env->params->rot.x -= 0.1;
		env->map->changed = 1;
	}
	if (env->keys.act[rot_y])
	{
		env->params->rot.y += 0.1;
		env->map->changed = 1;
	}
	else if (env->keys.act[rot_inv_y])
	{
		env->params->rot.y -= 0.1;
		env->map->changed = 1;
	}
	if (env->keys.act[rot_z])
	{
		env->params->rot.z += 0.1;
		env->map->changed = 1;
	}
	else if (env->keys.act[rot_inv_z])
	{
		env->params->rot.z -= 0.1;
		env->map->changed = 1;
	}
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
		printf("%f %f %f\n", env->params->scale.x, env->params->scale.y, env->params->scale.z);
		env->map->changed = 1;
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
		printf("%f %f %f\n", env->params->scale.x, env->params->scale.y, env->params->scale.z);
		env->map->changed = 1;
	}
}

void	update_scale(t_env *env)
{
	if (env->keys.act[scale_z_up])
	{
		env->params->scale.z += 1;
		env->map->changed = 1;
	}
	else if (env->keys.act[scale_z_down])
	{
		if (env->params->scale.z > 1)
		{
			env->params->scale.z -= 1;
			env->map->changed = 1;
		}
	}
}

void	update_keys(t_env *env)
{
	update_translations(env);
	update_rotations(env);
	update_zoom(env);
	update_scale(env);
	if (!env->keys.last_act[color_auto] && env->keys.act[color_auto])
	{
		env->map->auto_color = !env->map->auto_color;
		env->map->changed = 1;
	}
	if (env->keys.act[quit])
		exit(0);
	if (!env->keys.last_act[menu] && env->keys.act[menu])
		env->menu = !env->menu;
	key_last_act_update(env);
}

void	update_menu(t_env *env)
{
	static t_hsv	*hsv_color = NULL;
	static t_hsv	*hsv_color_select = NULL;

	if (hsv_color == NULL)
	{
		if (!(hsv_color = (t_hsv*)malloc(sizeof(t_hsv))))
			return ;
		hsv_color->hue = 0;
		hsv_color->value = 0;
		hsv_color->saturation = 0;
	}
	if (hsv_color_select == NULL)
	{
		if (!(hsv_color_select = (t_hsv*)malloc(sizeof(t_hsv))))
			return ;
		hsv_color->hue = 0;
		hsv_color->value = 0;
		hsv_color->saturation = 0;
	}
	if (env->menu)
	{
		g_rect_fill(env->win->img_next, make_rect(0, 0, 240, env->win->size_y), 0x333333);
		if(button(env->win, env->ui, 1, make_rect(20, 20, 64, 32), "Quit"))

			exit(0);
		color_picker(env->win, env->ui, 50, make_rect(20, 100, 200, 200), hsv_color, hsv_color_select);

		g_rect_fill(env->win->img_next, make_rect(20, 320, 50, 50), g_hsv_to_color(*hsv_color));
		g_rect_fill(env->win->img_next, make_rect(100, 320, 50, 50), g_hsv_to_color(*hsv_color_select));
		if (button(env->win, env->ui, 3, make_rect(20, 400, 64, 32), "Start"))
		{
			env->map->color.start = g_hsv_to_color(*hsv_color_select);
			env->map->changed = 1;
		}
		if (button(env->win, env->ui, 4, make_rect(100, 400, 64, 32), "End"))
		{
			env->map->color.end = g_hsv_to_color(*hsv_color_select);
			env->map->changed = 1;
		}
	}
}

void	delete_points(t_map *map);
int		update(t_env *env)
{
	static clock_t	t1 = 0;
	clock_t		t2 = 0;
	t2 = clock();
	printf("%d\n", (t2 - t1));
	if ((t2 - t1) * 20 < CLOCKS_PER_SEC)
		return (0);
	else
		t1 = t2;
	//g_window_fill(env->win, 0);
	update_keys(env);
	if (env->map->changed)
	{
		env->params->center.x = env->params->start.x + (env->params->scale.x * env->map->width / 2);
		env->params->center.y = env->params->start.y + (env->params->scale.y * env->map->height / 2);
		env->params->center.z = env->params->start.z + ((abs(*(env->map->max) - *(env->map->min)) * env->params->scale.z) / 2);
		map_to_points(env->map, env->params);
		env->map->changed = 0;
		delete_points(env->map);
		add_points(env->win->img_next, env->map);
	}
	display_lines(env->win->img_next, env->map);
	update_menu(env);
	g_update(env->win);
	g_render_delete_text_buffer(env->win);
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
	env->menu = 0;
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
		{
			printf("MAP FAIL");
			return (0);
		}
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
		env->map->changed = 1;
		env->map->lines = NULL;
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
	return (1);
}

int		destroy(t_env *env)
{
	printf("TEST");
	exit(0);
	return (0);
}

int		main(int argc, char *argv[])
{
	t_env		*env;

	if (!(env = set_env()))
		return (1);
	if (!init_win(env))
		return (2);
	if (!init_map(argc, argv, env))
		return (3);
	if (!init_env(env))
		return (4);
	if (!(env->ui = make_ui()))
		return (4);
	mlx_hook(env->win->win_ptr, MotionNotify, PointerMotionMask, &(mouse_move), env);
	mlx_hook(env->win->win_ptr, ButtonPress, ButtonPressMask, &(mouse_press_button), env);
	mlx_hook(env->win->win_ptr, ButtonRelease, ButtonReleaseMask, &(mouse_release_button), env);
	mlx_hook(env->win->win_ptr, KeyPress, KeyPressMask, &(key_press), env);
	mlx_hook(env->win->win_ptr, KeyRelease, KeyReleaseMask, &(key_release),	env);
	mlx_hook(env->win->win_ptr, DestroyNotify, StructureNotifyMask, &(destroy), env);
	mlx_loop_hook(env->win->mlx_ptr, &update, env);
	//mlx_expose_hook(env->win->mlx_ptr, &update, (void*)env);
	mlx_loop(env->win->mlx_ptr);
	return (0);
}
