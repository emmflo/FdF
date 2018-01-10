#ifndef MAP_H
# define MAP_H
# include "graphics.h"

typedef struct	s_line
{
	int		*line;
	int		size;
}		t_line;

typedef struct	s_map
{
	int		**map;
	int		width;
	int		height;
	int		*min;
	int		*max;
	t_point	**points;
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
