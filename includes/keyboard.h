#ifndef KEYBOARD_H
# define KEYBOARD_H
# define NB_ACT 13

typedef struct	s_keys
{
	int	act[NB_ACT];
	int	act_key[NB_ACT];
}		t_keys;

enum {up, down, left, right, rot_x, rot_inv_x, rot_y, rot_inv_y, rot_z, rot_inv_z, zoom, dezoom, quit};

# include "env.h"

int	key(int keycode, void *params);
int	key_press(int keycode, t_env *env);
int	key_release(int keycode, t_env *env);
void	key_set_default(t_keys *keys);
#endif
