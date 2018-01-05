#ifndef GRAPHICS_H
# define GRAPHICS_H
# include <mlx.h>

typedef struct	s_img
{
	char	*data;
	void	*img;
	int		*bits_per_pixel;
	int		*size_line;
	int		*endian;
}				t_img;

typedef struct	s_window {
	void	*mlx_ptr;
	void	*win_ptr;	
	int		size_x;
	int		size_y;
	char	*title;
	t_img	*img_front;
	t_img	*img_back;
	t_img	**img_current;
	t_img	**img_next;
}				t_window;

typedef struct	s_point {
	double	x;
	double	y;
	double	z;
}				t_point;

typedef struct s_point_int {
	int		x;
	int		y;
	int		z;
}				t_point_int;

t_window	*g_new_window(void *mlx_ptr, int size_x, int size_y, char *title);
int			g_update(t_window *win);
t_img		*g_new_img(void *mlx_ptr, int width, int height);
void		g_put_pixel(t_img *img, t_point_int pt, int color);
void		g_draw_line(t_img *img, t_point_int a, t_point_int b, int color);

#endif
