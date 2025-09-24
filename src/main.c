/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:25:12 by kpineda-          #+#    #+#             */
/*   Updated: 2025/09/23 21:56:58 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void draw(t_data *data);

void player_move(t_data *data, int dir, int speed)
{
	int y = data->player.y + (int)(cos(ft_degree_to_radian(data->player.rotation + dir)) * speed);
	int x = data->player.x + (int)(sin(ft_degree_to_radian(data->player.rotation + dir)) * speed);
	int i = 0;
	int j;
	while (data->map.map[i])
	{
		j = 0;
		while (data->map.map[i][j])
		{
			if (data->map.map[i][j] == '1')
			{
				if (col_squaresquare((t_rect){x, y, data->player.scale * 2, data->player.scale * 2, BLACK}, (t_rect){j * 20, i * 20, 20, 20, BLACK}))
					return;
			}
			j++;
		}
		i++;
	}
	data->player.y += (int)(cos(ft_degree_to_radian(data->player.rotation + dir)) * speed);
	data->player.x += (int)(sin(ft_degree_to_radian(data->player.rotation + dir)) * speed);
}

void key_hook(int key, t_data *data)
{
	int speed = 5;

	if (key == XK_Escape)
		exit_program(data);
	else if (key == XK_s)
		player_move(data, 180, speed);
	else if (key == XK_w)
		player_move(data, 0, speed);
	else if (key == XK_a)
		player_move(data, 90, speed);
	else if (key == XK_d)
		player_move(data, 270, speed);
	if (key == XK_Right)
		data->player.rotation -= speed;
	else if (key == XK_Left)
		data->player.rotation += speed;
	draw(data);
}

void init_data(t_data *data, char **av)
{
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3d");
	data->img.img_ptr = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->img.img_pixels_ptr = mlx_get_data_addr(data->img.img_ptr,
												 &data->img.bits_per_pixel, &data->img.line_len, &data->img.endian);
	if (!read_file(data, av[1]))
		exit_error(data);
	if (!set_map(data))
		exit_error(data);
	data->player.scale = 7;

	// Load wall texture
	data->tex_wall.img_ptr = mlx_xpm_file_to_image(
		data->mlx, "pared.xpm", &data->tex_wall.width, &data->tex_wall.height);
	if (!data->tex_wall.img_ptr)
		exit_error(data);

	data->tex_wall.img_pixels_ptr = mlx_get_data_addr(
		data->tex_wall.img_ptr,
		&data->tex_wall.bits_per_pixel,
		&data->tex_wall.line_len,
		&data->tex_wall.endian);
}

int main(int ac, char **av)
{
	t_data data;
	if (ac != 2)
		return (write(2, "Error\n", 7), 1);
	init_data(&data, av);
	draw(&data);
	mlx_hook(data.win, 2, KeyPressMask, (void *)key_hook, &data);
	mlx_hook(data.win, DestroyNotify, NoEventMask, (void *)exit_program, &data);
	mlx_loop(data.mlx);
}
