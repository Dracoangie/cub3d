/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:25:12 by kpineda-          #+#    #+#             */
/*   Updated: 2025/10/06 18:47:17 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	key_hook(int key, t_data *data)
{
	int	speed;

	speed = 5;
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

void	init_data(t_data *data, char **av)
{
	int	i;

	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3d");
	data->img.img_ptr = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->img.img_pixels_ptr = mlx_get_data_addr(data->img.img_ptr,
			&data->img.bits_per_pixel,
			&data->img.line_len,
			&data->img.endian);
	i = -1;
	while (++i < 4)
		data->tex[i].img_ptr = NULL;
	if (!read_file(data, av[1]))
		exit_error(data);
	if (!set_map(data))
		exit_error(data);
	data->player.scale = 7;
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 2)
		return (write(2, "Error\n", 7), 1);
	init_data(&data, av);
	draw(&data);
	mlx_hook(data.win, 2, KeyPressMask, (void *)key_hook, &data);
	mlx_hook(data.win, DestroyNotify, NoEventMask, (void *)exit_program, &data);
	mlx_loop(data.mlx);
}
