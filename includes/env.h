#ifndef ENV_H
# define ENV_H
# include "map.h"
# include "keyboard.h"

typedef struct	s_env
{
	t_window	*win;
	t_keys		keys;
	t_point		rot;
	t_map		*map;
	t_point		start;
	t_point		scale;
	t_point		center;
}				t_env;

#endif
