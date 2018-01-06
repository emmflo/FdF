#include <mlx.h>
#include <stdio.h>
#include <X11/X.h>
#include "graphics.h"
#include "keyboard.h"
#include "map.h"
#include "parse.h"
#include "env.h"

void	update_keys(t_env *env)
{
	if (env->keys.act[left])
		env->start.x -= 4;
	else if (env->keys.act[right])
		env->start.x += 4;
	if (env->keys.act[up])
		env->start.y -= 4;
	else if (env->keys.act[down])
		env->start.y += 4;
	if (env->keys.act[rot_x])
		env->rot.x += 0.1;
	else if (env->keys.act[rot_inv_x])
		env->rot.x -= 0.1;
	if (env->keys.act[rot_y])
		env->rot.y += 0.1;
	else if (env->keys.act[rot_inv_y])
		env->rot.y -= 0.1;
	if (env->keys.act[rot_z])
		env->rot.z += 0.1;
	else if (env->keys.act[rot_inv_z])
		env->rot.z -= 0.1;
	if (env->keys.act[zoom])
	{
		env->start.x -= fabs(env->center.x - (env->start.x + ((env->scale.x + 1) * env->map->width / 2)));
		env->start.y -= fabs(env->center.y - (env->start.y + ((env->scale.y + 1) * env->map->height / 2)));
		env->start.z -= fabs(env->center.z - (env->start.z + (((abs(*(env->map->max) - *(env->map->min)) * (env->scale.z - 1)) / 2))));
		env->scale.x += 1;
		env->scale.y += 1;
		env->scale.z += 1;
	}
	else if (env->keys.act[dezoom])
	{

		if (env->scale.x > 1)
			env->start.x += fabs(env->center.x - (env->start.x + ((env->scale.x - 1) * env->map->width / 2)));
		if (env->scale.y > 1)
			env->start.y += fabs(env->center.y - (env->start.y + ((env->scale.y - 1) * env->map->height / 2)));
		if (env->scale.z > 1)
			env->start.z += fabs(env->center.z - (env->start.z + (((abs(*(env->map->max) - *(env->map->min)) * (env->scale.z - 1)) / 2))));
		if (env->scale.x > 1)
			env->scale.x -= 1;
		if (env->scale.y > 1)
			env->scale.y -= 1;
		if (env->scale.z > 1)
			env->scale.z -= 1;
	}
	if (env->keys.act[quit])
		exit(0);
}

int	update(t_env *env)
{
	g_window_fill(env->win, 0);

	update_keys(env);
	env->center.x = env->start.x + (env->scale.x * env->map->width / 2);
	env->center.y = env->start.y + (env->scale.y * env->map->height / 2);
	env->center.z = env->start.z + ((abs(*(env->map->max) - *(env->map->min)) * env->scale.z) / 2);
	printf("%d %d\n", *(env->map->max), *(env->map->min));
	printf("%f %f %f\n", env->center.x, env->center.y, env->center.z);

	map_to_points(env->map, env->start, env->rot, env->scale, env->center);
	display_points(env->win->img_front, env->map);
	g_update(env->win);
}

int		main(int argc, char *argv[])
{
	void		*mlx_ptr;
	t_env		*env;

	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (1);
	env->rot.x = 0;
	env->rot.y = 0;
	env->rot.z = 0;

	env->scale.x = 50;
	env->scale.y = 50;
	env->scale.z = 50;


	key_set_default(&(env->keys));
	if (argc >= 2)
		env->map = get_map_from_path(argv[1]);
	else
		env->map = make_fake_map();
	if (!(mlx_ptr = mlx_init()))
		return (1);
	if (!(env->win = g_new_window(mlx_ptr, 1000, 1000, "Test")))
		return (1);

	env->start.x = env->win->size_x / 2 - (env->scale.x * env->map->width / 2);
	env->start.y = env->win->size_y / 2 - (env->scale.y * env->map->height / 2);
	env->start.z = 0;
	env->center.x = env->start.x + (env->scale.x * env->map->width / 2);
	//env->center.x = (env->win->size_x * env->scale.x)/ 2;
	env->center.y = env->start.y + (env->scale.y * env->map->height / 2);
	//env->center.y = (env->win->size_y * env->scale.y) / 2;
	env->center.z = (abs(*(env->map->max) - *(env->map->min)) * env->scale.z) / 2;
	printf("%f %f %f\n", env->center.x, env->center.y, env->center.z);

	//mlx_key_hook(env->win->win_ptr, &key, (void*)env);
	mlx_hook(env->win->win_ptr, KeyPress, KeyPressMask, &(key_press), env);
	mlx_hook(env->win->win_ptr, KeyRelease, KeyReleaseMask, &(key_release), env);
	mlx_loop_hook(env->win->mlx_ptr, &update, (void*)env);
	mlx_expose_hook(env->win->mlx_ptr, &update, (void*)env);
	mlx_loop(mlx_ptr);
	return (0);
}
