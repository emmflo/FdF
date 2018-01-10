/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eflorenz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/10 19:38:28 by eflorenz          #+#    #+#             */
/*   Updated: 2018/01/10 19:38:38 by eflorenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"
#include <string.h>
#include <stdlib.h>

t_window	*g_new_window(void *mlx_ptr, int size_x, int size_y, char *title)
{
	t_window	*win;

	if (!(win = (t_window*)malloc(sizeof(t_window))))
		return (NULL);
	win->win_ptr = mlx_new_window(mlx_ptr, size_x, size_y, title);
	win->size_x = size_x;
	win->size_y = size_y;
	win->title = strdup(title);
	win->img_front = g_new_img(mlx_ptr, size_x, size_y);
	win->img_back = g_new_img(mlx_ptr, size_x, size_y);
	win->img_current = &(win->img_front);
	win->img_next = &(win->img_back);
	win->mlx_ptr = mlx_ptr;
	return (win);
}

int			g_update(t_window *win)
{
	void	**tmp;

	mlx_clear_window(win->mlx_ptr, win->win_ptr);
	if (!(mlx_put_image_to_window(win->mlx_ptr, win->win_ptr,
					(win->img_front)->img, 0, 0)))
		return (0);
	return (1);
}
