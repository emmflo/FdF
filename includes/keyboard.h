#ifndef KEYBOARD_H
# define KEYBOARD_H
# ifdef __APPLE__
#  include "key_defs.h"
# else
#  include <X11/keysym.h>
# endif
# define NB_ACT 17

typedef struct	s_keys
{
	int	act[NB_ACT];
	int	last_act[NB_ACT];
	int	act_key[NB_ACT];
}		t_keys;

enum {up, down, left, right, rot_x, rot_inv_x, rot_y, rot_inv_y, rot_z, rot_inv_z, zoom, dezoom, quit, scale_z_up, scale_z_down, color_auto, menu};

# include "env.h"

int	key(int keycode, void *params);
int	key_press(int keycode, t_env *env);
int	key_release(int keycode, t_env *env);
void	key_set_default(t_keys *keys);
void	key_last_act_update(t_env *env);
#endif
