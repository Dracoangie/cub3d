/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:19:02 by kpineda-          #+#    #+#             */
/*   Updated: 2025/07/10 19:47:55 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void draw(t_data *data);

void exit_program(t_data *data)
{
	(void)data;
	mlx_destroy_image(data->mlx, data->img.img_ptr);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	exit(0);
}

void key_hook(int key, t_data *data)
{
	if (key == XK_Escape)
		exit_program(data);
	if (key == XK_s)
		data->player.y += 20;
	if (key == XK_w)
		data->player.y -= 20;
	if (key == XK_a)
		data->player.x -= 20;
	if (key == XK_d)
		data->player.x += 20;
	draw(data);
}

void init_data(t_data *data, char **av)
{
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3d");
	data->img.img_ptr = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->img.img_pixels_ptr = mlx_get_data_addr(data->img.img_ptr,
												 &data->img.bits_per_pixel, &data->img.line_len, &data->img.endian);
	data->player.x = 0;
	data->player.y = 0;
	data->map.map = av;
	data->map.rows = 0;
	data->map.cols = 0;
	while (data->map.map[data->map.rows])
	{
		if (data->map.cols < (int)ft_strlen(data->map.map[data->map.rows]))
			data->map.cols = (int)ft_strlen(data->map.map[data->map.rows]);
		data->map.rows++;
	}
	printf("rows: %i\n", data->map.rows);
	printf("cols: %i\n", data->map.cols);
}

void put_pixel(t_data *data, int x, int y, int color)
{
	char *pixel;

	pixel = data->img.img_pixels_ptr + (x * 4) + (y * data->img.line_len);
	*(int *)pixel = color;
}

void render_rect(t_data *data, int x, int y, int height, int width, int color)
{
	int i = 0;
	int j = 0;

	while (i < width)
	{
		while (j < height)
		{
			put_pixel(data, i + x, j + y, color);
			j++;
		}
		j = 0;
		i++;
	}
}
/*
11111
10001
10011
11001
11101
11111
*/

void map(t_data *data, char **get_map, int rows, int cols)
{
	int i = 0;
	int j = 0;
	// int x = 0;
	// int y = 0;
	(void)data;
	(void)get_map;

	while (i < rows)
	{
		printf("i: %i\n", get_map[i][j]);
		while (j < cols)
		{
			// printf("j: %i", j);
			// if (get_map[i][j] == '1')
			// 	render_rect(data, x, y, 20, 20, BLACK);
			// else if (get_map[i][j] == '0')
			// 	render_rect(data, x, y, 20, 20, WHITE);
			// x += 20;
			j++;
		}
		// x = 0;
		// y += 20;
		j = 0;
		i++;
	}
}

void draw(t_data *data)
{
	int	i = 0;
	int	j = 0;
	int x = 0;
	int y = 0;
	while (i < data->map.rows)
	{
		printf("i: %c\n", data->map.map[i][j]);
		while (j < data->map.cols)
		{
			printf("j: %i", j);
			if (data->map.map[i][j] == '1')
				render_rect(data, x, y, 20, 20, BLACK);
			else if (data->map.map[i][j] == '0')
				render_rect(data, x, y, 20, 20, WHITE);
			else
				render_rect(data, x, y, 20, 20, BLACK);
			x += 20;
			j++;
		}
		x = 0;
		y += 20;
		j = 0;
		i++;
	}
	mlx_clear_window(data->mlx, data->win);
	render_rect(data, data->player.x, data->player.y, 20, 20, WHITE);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}

int main(int ac, char **av)
{
	t_data data;
	(void)ac;
	av++; 

	init_data(&data, av);
	draw(&data);
	mlx_hook(data.win, DestroyNotify, NoEventMask, (void *)exit_program, &data);
	mlx_key_hook(data.win, (void *)key_hook, &data);
	mlx_loop(data.mlx);
}