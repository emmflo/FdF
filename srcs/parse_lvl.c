#include <libft.h>
#include <stdlib.h>
#include "map.h"
#include <fcntl.h>
#include <stdio.h>

int		check_line(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!(('0' <= str[i] && str[i] <= '9') || str[i] == '-' || str[i] == ' '))
			return (0);
		i++;
	}
	return (1);
}

size_t	get_nb_pt(char *str)
{
	int		i;
	int		in_nb;
	size_t	count;

	i = 0;
	in_nb = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (!('0' <= str[i] && str[i] <= '9'))
			in_nb = 0;
		else if (!in_nb)
		{
			count++;
			in_nb = 1;
		}
		i++;
	}
	return (count);
}

t_line	*parse_line(char *str, int **min, int **max)
{
	t_line	*line;
	int		in_nb;
	int		pos;
	int		i;

	if (!check_line(str))
		return (NULL);
	if (!(line = (t_line*)malloc(sizeof(t_line))))
		return (NULL);
	line->size = get_nb_pt(str);
	if (!(line->line = (int*)malloc(sizeof(int) * line->size)))
		return (NULL);
	in_nb = 0;
	pos = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (!(('0' <= str[i] && str[i] <= '9') || str[i] == '-'))
			in_nb = 0;
		else if (!in_nb)
		{
			line->line[pos] = atoi(&str[i]);
			printf("%p\n", *min);
			if (!*min || line->line[pos] < **min)
				*min = &(line->line[pos]);
			if (!*max || line->line[pos] > **max)
				*max = &(line->line[pos]);
			in_nb = 1;
			pos++;
		}
		i++;
	}
	return (line);
}

int		**resize_tab(int ***tab, int old_size, int new_size)
{
	int	**new_tab;
	int	i;

	printf("TEST");
	if (!(new_tab = (int**)malloc(sizeof(int*) * new_size)))
		return (NULL);
	i = 0;
	while (i < old_size) {
		new_tab[i] = (*tab)[i];
		i++;
	}
	free(*tab);
	*tab = new_tab;
	return (new_tab);
}

t_map	*get_map_from_fd(int fd)
{
	char	*line;
	t_line	*parsed_line;
	t_map	*map;
	int		size;

	if (!(map = (t_map*)malloc(sizeof(t_map))))
		return (NULL);
	map->width = -1;
	map->height = 0;
	map->min = NULL;
	map->max = NULL;
	size = 50;
	if (!(map->map = (int**)malloc(sizeof(int*)*size)))
		return (NULL);
	while (get_next_line(fd, &line) > 0)
	{
		printf("%s\n", line);
		if (!(parsed_line = parse_line(line, &(map->min), &(map->max))))
		{
			//delete_map();
			return (NULL);
		}
		if (map->width == -1)
			map->width = parsed_line->size;
		else if (map->width != parsed_line->size)
		{
			//delete_map(map);
			return (NULL);
		}
		if (map->height >= size)
		{
			if (!resize_tab(&(map->map), size, size * 2))
			{
				//delete_map();
				return (NULL);
			}
			size *= 2;
		}
		map->map[map->height] = parsed_line->line;
		map->height += 1;
	}
	printf("WIDTH %d HEIGHT %d\n", map->width, map->height);
	return (map);
}

t_map	*get_map_from_path(char *path)
{
	t_map	*map;
	int	fd;

	fd = open(path, O_RDONLY);
	map = get_map_from_fd(fd);
	close(fd);
	return (map);
}
