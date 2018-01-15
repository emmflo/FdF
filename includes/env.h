#ifndef ENV_H
# define ENV_H
# include "map.h"
# include "keyboard.h"
# include "imgui.h"

typedef struct	s_env
{
	t_window	*win;
	t_keys		keys;
	t_map		*map;
	t_params	*params;
	t_ui_state	*ui;
	int		menu;
}				t_env;

#endif
