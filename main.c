/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:24:36 by angnavar          #+#    #+#             */
/*   Updated: 2025/06/30 21:04:22 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	mouse_hook(int button, int x, int y, t_data *data)
{
	(void)button;
	(void)x;
	(void)y;
	(void)data;
	return (0);
}

void	exit_program(t_data *data)
{
	if (data->img.img_ptr)
		mlx_destroy_image(data->mlx, data->img.img_ptr);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
	exit(0);
}

void	set_data(t_data *data)
{
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3d");
	data->img.img_ptr = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->img.img_pixels_ptr = mlx_get_data_addr(data->img.img_ptr,
			&data->img.bits_per_pixel,
			&data->img.line_len,
			&data->img.endian);
}

int	handle_input(int keysym, t_data *data)
{
	//float	move_step;

	if (keysym == XK_Escape)
		exit_program(data);
	/*else if (keysym == XK_w || keysym == XK_Up)
		data->move_y -= move_step;
	else if (keysym == XK_s || keysym == XK_Down)
		data->move_y += move_step;
	else if (keysym == XK_a || keysym == XK_Left)
		data->move_x -= move_step;
	else if (keysym == XK_d || keysym == XK_Right)
		data->move_x += move_step;*/
	return (0);
}

void	img_pix_put(t_img *img, int x, int y, int color)
{
	char    *pixel;
	int		i;

	i = img->bits_per_pixel - 8;
    pixel = img->img_pixels_ptr + (y * img->line_len + x * (img->bits_per_pixel / 8));
	while (i >= 0)
	{
		if (img->endian != 0)
			*pixel++ = (color >> i) & 0xFF;
		else
			*pixel++ = (color >> (img->bits_per_pixel - 8 - i)) & 0xFF;
		i -= 8;
	}
}

int render_rect(t_img *img, t_rect rect)
{
	int	i;
	int j;

	i = rect.y;
	while (i < rect.y + rect.height)
	{
		j = rect.x;
		while (j < rect.x + rect.width)
			img_pix_put(img, j++, i, rect.color);
		++i;
	}
	return (0);
}

int	draw(t_data *data)
{
	if (data->win == NULL)
		return (1);
	render_rect(&data->img, (t_rect){200, 200, 100, 100, RED});

	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);

	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	(void )argv;
	
	if(argc > 2)
		return (0);

	
	set_data(&data);
	draw(&data);
	mlx_mouse_hook(data.win, mouse_hook, &data);
	mlx_key_hook(data.win, handle_input, &data);
	mlx_hook(data.win, DestroyNotify, NoEventMask, (void *)exit_program, &data);
	mlx_loop(data.mlx);
	return (0);
}
