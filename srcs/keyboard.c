#include <stdio.h>
#include <stdlib.h>
#include <X11/keysym.h>
#include "keyboard.h"
#include "graphics.h"
#include "env.h"

int		key(int keycode, void *params)
{
	t_env	*env;

	env = (t_env*)params;
  	printf("Key in Win1 : %d\n", keycode);
  	if (keycode==0xFF1B)
  	  exit(0);
	else if (keycode==97)
		env->rot.x -= 0.1;
	else if (keycode==100)
		env->rot.x += 0.1;
	else if (keycode==119)
		env->rot.y += 0.1;
	else if (keycode==115)
		env->rot.y -= 0.1;
	else if (keycode==113)
		env->rot.z -= 0.1;
	else if (keycode==101)
		env->rot.z += 0.1;
}

int	key_press(int keycode, t_env *env)
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

int	key_release(int keycode, t_env *env)
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
}
