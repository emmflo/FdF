#ifndef MAP_H
# define MAP_H
# include "graphics.h"
# include "libft.h"

typedef struct	s_map_line
{
	int		*line;
	int		*color;
	int		size;
}		t_map_line;

typedef struct	s_map
{
	int		**map;
	int		**colors;
	int		width;
	int		height;
	int		*min;
	int		*max;
	t_point		**points;
	t_list		*lines;
	int		auto_color;
	t_gradient	color;	
	int		changed;
}		t_map;

typedef struct	s_params
{
	t_point		start;
	t_point		scale;
	t_point		center;
	t_point		rot;
}				t_params;

void	map_to_points(t_map *map, t_params *params);
void	rotate(t_point *pt, t_params *params);
void	display_points(t_img *img, t_map *map);

#endif
