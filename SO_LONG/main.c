/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aokur <aokur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:09:05 by aokur             #+#    #+#             */
/*   Updated: 2025/10/05 17:32:55 by aokur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <stdlib.h>

#define WIDTH 900
#define HEIGHT 900
#define SPEED 10

typedef struct s_game {
	void *mlx;
	void *win;
	void *img;
	int x;
	int y;
} t_game;

int key_hook(int keycode, t_game *game)
{
	if (keycode == 65307)
	{
		mlx_destroy_image(game->mlx, game->img);
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
	mlx_put_image_to_window(game->mlx, game->win, game->img, game->x, game->y);
	return (0);
}

int main()
{
	t_game g;

	int width;
	int height;
	g.x = WIDTH/2;
	g.y = HEIGHT/2;
	g.mlx = mlx_init();
	g.win = mlx_new_window(g.mlx, WIDTH, HEIGHT, "Pac Man Alpha Beta Demo");
	g.img = mlx_xpm_file_to_image(g.mlx, "pacman.xpm", &width, &height);

	mlx_key_hook(g.win, key_hook, &g);
	mlx_loop_hook(g.mlx, update, &g);
	mlx_loop(g.mlx);
	return (0);
}
