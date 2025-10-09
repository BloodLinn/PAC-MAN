/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aokur <aokur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:09:05 by aokur             #+#    #+#             */
/*   Updated: 2025/10/10 02:42:09 by aokur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define WIDTH 900
#define HEIGHT 900
#define SPEED 10
#define BUFFER_SIZE 1

typedef struct s_game {
	void *mlx;
	void *win;

	char **map;
	int map_height;
	int map_width;

	void *img_floor;
	void *img_player;
	void *img_wall;

	int x;
	int y;
} t_game;

int	gnl_strlen(char	*dest)
{
	int	i;

	i = 0;
	if (!dest)
		return (0);
	while (dest[i])
		i++;
	return (i);
}

char	*gnl_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	if (!dest && !src)
		return (0);
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*gnl_strchr(char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == (unsigned char )c)
			return ((char *)&s[i]);
		i++;
	}
	if ((unsigned char )c == '\0')
		return ((char *)&s[i]);
	return (NULL);
}

void	*gnl_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	i;

	i = 0;
	if (size != 0 && nmemb > 9223372036854775807 / size)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	while (i < nmemb * size)
	{
		((unsigned char *)ptr)[i] = '\0';
		i++;
	}
	return (ptr);
}

char	*gnl_strjoin(char *s1, char *s2)
{
	size_t	l;
	size_t	b;
	char	*tmp;

	l = 0;
	b = 0;
	if (!s1 && !s2)
		return (NULL);
	tmp = gnl_calloc((((gnl_strlen(s1)) + gnl_strlen(s2) + 1)), (sizeof(char)));
	if (!tmp)
		return (NULL);
	while (s1[l])
	{
		tmp[l] = s1[l];
		l++;
	}
	while (s2[b])
	{
		tmp[l + b] = s2[b];
		b++;
	}
	tmp[l + b] = '\0';
	return (tmp);
}

char	*gnl_substr(char *s, unsigned int start, size_t len)
{
	size_t	a;
	size_t	i;
	char	*tmp;

	if (!s)
		return (NULL);
	i = 0;
	a = gnl_strlen(s);
	if (start >= a)
		return (gnl_calloc(1, sizeof(char)));
	if (a < start + len)
		len = a - start;
	tmp = gnl_calloc((len + 1), sizeof(char));
	if (!tmp)
		return (NULL);
	while (i < len)
	{
		tmp[i] = s[start + i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

char	*get_first_line(char *lines)
{
	int		i;
	char	*tmp1;

	if (!lines)
		return (NULL);
	i = 0;
	while (lines[i] && lines[i] != '\n')
		i++;
	if (lines[i] == '\n')
		i++;
	tmp1 = gnl_substr(lines, 0, i);
	return (tmp1);
}

char	*get_remains_line(char *lines)
{
	int		i;
	int		t;
	char	*tmp2;

	if (!lines)
		return (NULL);
	i = 0;
	t = gnl_strlen(lines);
	while (lines[i] && lines[i] != '\n')
		i++;
	if (!lines[i])
		return (free(lines), NULL);
	i++;
	tmp2 = gnl_substr(lines, i, t - i);
	free(lines);
	return (tmp2);
}

char	*get_read_line(char *lines, int fd)
{
	char	*line;
	ssize_t	count;
	char	*tmp;

	count = 1;
	if (!lines)
		lines = gnl_calloc(1, 1);
	line = gnl_calloc((BUFFER_SIZE + 1), 1);
	if (!line || !lines)
		return (free(lines), free(line), NULL);
	while (!gnl_strchr(lines, '\n') && count > 0)
	{
		count = read(fd, line, BUFFER_SIZE);
		if (count == -1)
			return (free(lines), free(line), NULL);
		line[count] = 0;
		tmp = lines;
		lines = gnl_strjoin(lines, line);
		free(tmp);
	}
	if (gnl_strlen (lines) == 0)
		return (free(lines), free(line), NULL);
	free(line);
	return (lines);
}

char	*get_next_line(int fd)
{
	static char	*lines;
	char		*first;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	lines = get_read_line(lines, fd);
	if (!lines)
		return (NULL);
	first = get_first_line(lines);
	lines = get_remains_line(lines);
	return (first);
}

int key_hook(int keycode, t_game *game)
{
	if (keycode == 65307)
	{
		mlx_destroy_image(game->mlx, game->img_player);
		mlx_destroy_window(game->mlx, game->win);
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		exit(0);
	}
	if (keycode == 65364)
		game->y += 10;
	if (keycode == 65362)
		game->y -= 10;
	if (keycode == 65361)
		game->x -= 10;
	if (keycode == 65363)
		game->x += 10;
	return (0);
}

int update(void *param)
{
	t_game *game = (t_game *)param;

	mlx_clear_window(game->mlx, game->win);
	mlx_put_image_to_window(game->mlx, game->win, game->img_player, game->x, game->y);
	return (0);
}

int map_height(char *param)
{
	int		fd;
	int		map_height;
	char	*line;

	map_height = 0;
	fd = open(param,  O_RDONLY);
	
	if (fd < 0)
	{
		printf("Error");
		return (0);
	}
	
	while((line = get_next_line(fd)) != NULL)
	{
		map_height++;
		free(line);
	}

	close(fd);
	return (map_height);
}

char	**read_map(char *param)
{
	char	**map;
	int		height;
	int		fd;
	int		i;
	char	*line;

	height = map_height(param);
	if (height <= 0)
		return (NULL);
		
	map = malloc((height + 1) * sizeof(char *));
	if (!map)
		return (NULL);
		
	fd = open(param, O_RDONLY);
	if (fd < 0)
	{
		free(map);
		return (NULL);
	}
	
	i = 0;
	while((line = get_next_line(fd)) != NULL)
	{
		map[i] = line;
		i++;
	}
// buraya neden free(line) gelemiyor sebebini bul
	map[i] = NULL;
	close(fd);
	return (map);
}

int	is_map_rectangular(char **map)
{
	int tmp;
	int	j;

	j = 1;
	tmp = gnl_strlen(map[0]);
	while (map[j] != NULL)
	{
		if (gnl_strlen(map[j]) == tmp)
			j++;
		else
			return (0);
	}
	return (1);
}

int	is_map_closed(char **map)
{
	int	j;
	int	i;
	int w;

	i = 0;
	while (map[i] != NULL)
		i++;
	j = 0;
	//genişlik
	while (map[0][j] != '\0')
	{
		if (((map[0][j] != '1') || (map[i - 1][j] != '1')))
			return (0);
		j++;
	}
	j = 0;
	w = gnl_strlen(map[0]);
	//yükseklik
	while (j < i)
	{
		if (map[j][w - 1] != '1' || map[j][0] != '1')
			return (0);
		j++;
	}
	return (1);
}

int	is_map_valid(char **map)
{
	int	is_p;
	int	is_b;
	int	is_s;
	int	i;
	int	j;

	i = 0;
	is_p = 0;
	while (map[i] != NULL)
	{
		j = 0;
		while(map[i][j] != '\0')
		{
			if (map[i][j] == 'P')
				is_p++;
			else if (map[i][j] != '1' && map[i][j] != '0' && map[i][j] != 'P')
				return (0);
			j++;
		}
		i++;
	}
	if (is_p != 1)
		return (0);
}

//int	draw_map(char *param)
//{
//	t_game *game = (t_game *)param;
//	int	i;
//	int	j;

//	i = 0;
//	while ()
//}

int main()
{

	t_game g;

	int width;
	int height;
	g.x = WIDTH/2;
	g.y = HEIGHT/2;
	g.mlx = mlx_init();
	g.win = mlx_new_window(g.mlx, WIDTH, HEIGHT, "Pac Man Alpha Beta Demo");
	g.img_player = mlx_xpm_file_to_image(g.mlx, "pacman.xpm", &width, &height);
	

	mlx_key_hook(g.win, key_hook, &g);
	mlx_loop_hook(g.mlx, update, &g);
	mlx_loop(g.mlx);
	return (0);
}
