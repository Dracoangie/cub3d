/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:19:02 by kpineda-          #+#    #+#             */
/*   Updated: 2025/07/01 20:55:03 by kpineda-         ###   ########.fr       */
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
		data->player.y += 10;
	if (key == XK_w)
		data->player.y -= 10;
	if (key == XK_a)
		data->player.x -= 10;
	if (key == XK_d)
		data->player.x += 10;
	draw(data);
}

void init_data(t_data *data)
{
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3d");
	data->img.img_ptr = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->img.img_pixels_ptr = mlx_get_data_addr(data->img.img_ptr,
												 &data->img.bits_per_pixel, &data->img.line_len, &data->img.endian);
	data->player.x = 10;
	data->player.y = 10;
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

void draw(t_data *data)
{
	mlx_clear_window(data->mlx, data->win);
	render_rect(data, 0, 0, 60, 10, RED);
	render_rect(data, 10, 50, 10, 30, RED);
	render_rect(data, 10, 0, 10, 30, RED);
	render_rect(data, 40, 0, 60, 10, RED);

	render_rect(data, 10, 10, 10, 10, BLUE);
	render_rect(data, 10, 20, 10, 10, BLUE);
	render_rect(data, 20, 10, 20, 10, BLUE);
	render_rect(data, 30, 10, 10, 10, BLUE);
	render_rect(data, 30, 20, 10, 20, RED);
	render_rect(data, 10, 30, 10, 10, RED);
	render_rect(data, 20, 30, 10, 20, BLUE);
	render_rect(data, 10, 40, 10, 20, RED);
	render_rect(data, 30, 40, 10, 10, BLUE);
	render_rect(data, data->player.x, data->player.y, 10, 10, WHITE);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}

int main()
{
	t_data data;

	init_data(&data);
	draw(&data);
	mlx_hook(data.win, DestroyNotify, NoEventMask, (void *)exit_program, &data);
	mlx_key_hook(data.win, (void *)key_hook, &data);
	mlx_loop(data.mlx);
}