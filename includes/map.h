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

t_map	*make_fake_map();
void	map_to_points(t_map *map, t_point start, t_point rot, t_point scale, t_point center);
void	rotate(t_point *pt, t_point *c, t_point *rot);
void	display_points(t_img *img, t_map *map);

#endif
