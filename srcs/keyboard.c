/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eflorenz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/09 21:24:33 by eflorenz          #+#    #+#             */
/*   Updated: 2018/01/09 21:25:16 by eflorenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "keyboard.h"
#include "graphics.h"
#include "env.h"

int		key_press(int keycode, t_env *env)
{
	int	i;

	printf("PRESS %d\n", keycode);
	i = 0;
	while (i < NB_ACT)
	{
		if (keycode == env->keys.act_key[i])
			env->keys.act[i] = 1;
		i++;
	}
}

int		key_release(int keycode, t_env *env)
{
	int	i;

	i = 0;
	while (i < NB_ACT)
	{
		if (keycode == env->keys.act_key[i])
			env->keys.act[i] = 0;
		i++;
	}
}

void	key_set_default(t_keys *keys)
{
	int	i;

	i = 0;
	while (i < NB_ACT)
	{
		keys->act_key[i] = -1;
		keys->act[i] = 0;
		keys->last_act[i] = 0;
		i++;
	}
	keys->act_key[up] = XK_w;
	keys->act_key[left] = XK_a;
	keys->act_key[down] = XK_s;
	keys->act_key[right] = XK_d;
	keys->act_key[rot_inv_x] = XK_Left;
	keys->act_key[rot_x] = XK_Right;
	keys->act_key[rot_inv_y] = XK_Down;
	keys->act_key[rot_y] = XK_Up;
	keys->act_key[zoom] = XK_q;
	keys->act_key[dezoom] = XK_e;
	keys->act_key[quit] = XK_Escape;
	keys->act_key[scale_z_up] = XK_Page_Up;
	keys->act_key[scale_z_down] = XK_Page_Down;
	keys->act_key[color_auto] = XK_c;
	keys->act_key[menu] = XK_Tab;
}

void	key_last_act_update(t_env *env)
{
	int	i;

	i = 0;
	while (i < NB_ACT)
	{
		env->keys.last_act[i] = env->keys.act[i];
		i++;
	}
}
