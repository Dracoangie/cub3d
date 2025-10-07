/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:40:47 by kpineda-          #+#    #+#             */
/*   Updated: 2025/10/07 17:40:20 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	render_2d_aux(t_data *data, t_point cords, t_point angle,
		t_point *intersectionaux)
{
	t_point	intersection;
	double	intersectiondist;
	double	auxdist;

	if (data->map.map[cords.y][cords.x] == '1')
	{
		if (col_squareline((t_rect){cords.x * 20, cords.y * 20, 20, 20, RED},
			(t_line){(t_point){data->player.x
			+ data->player.scale, data->player.y
			+ data->player.scale}, angle},
			&intersection,
			data))
		{
			intersectiondist = pow(intersection.x - data->player.x, 2)
				+ pow(intersection.y - data->player.y, 2);
			auxdist = pow(intersectionaux->x - data->player.x, 2)
				+ pow(intersectionaux->y - data->player.y, 2);
			if (intersectiondist < auxdist)
				*intersectionaux = (t_point){intersection.x,
					intersection.y};
		}
	}
}

double	render_2d_aux_2(double auxangle, int i, t_data *data,
		t_point *intersectionaux)
{
	t_point	angle_point;
	t_point	x_y;
	double	angle;

	x_y.x = 0;
	x_y.y = 0;
	angle = ((ft_d_r(60) * i) / WIDTH) + auxangle;
	angle_point.x = (int)(sin(angle) * WIDTH) + data->player.x
		+ data->player.scale;
	angle_point.y = (int)(cos(angle) * WIDTH) + data->player.y
		+ data->player.scale;
	*intersectionaux = (t_point){angle_point.x, angle_point.y};
	while (data->map.map[x_y.y])
	{
		while (data->map.map[x_y.y][x_y.x])
		{
			render_2d_aux(data, x_y, angle_point, intersectionaux);
			x_y.x++;
		}
		x_y.x = 0;
		x_y.y++;
	}
	return (angle);
}

void	render_2d_vision(t_data *data)
{
	int		i;
	double	angle;
	double	auxangle;
	t_point	intersectionaux;

	i = 0;
	auxangle = ft_d_r(data->player.rotation - 30);
	while (i < WIDTH)
	{
		angle = render_2d_aux_2(auxangle, i, data, &intersectionaux);
		render_3d_column(data, intersectionaux, angle, WIDTH - 1 - i);
		i++;
	}
}

void	render_minimap_simple(t_data *data, double tile_px_d)
{
	t_point	size;
	t_point	pos;
	int		i;
	int		j;

	size.x = ((int)tile_px_d * data->map.cols) / data->map.cols;
	size.y = ((int)tile_px_d * data->map.rows) / data->map.rows;
	if (size.x < 1)
		size.x = 1;
	if (size.y < 1)
		size.y = 1;
	i = -1;
	while (++i < data->map.rows)
	{
		j = -1;
		while (++j < (int)ft_strlen(data->map.map[i]) && j < data->map.cols)
		{
			pos.x = j * size.x;
			pos.y = i * size.y;
			if (data->map.map[i][j] == '1')
				render_rect(data, pos, size, D_GRAY);
			else if (data->map.map[i][j] == '0')
				render_rect(data, pos, size, GRAY);
		}
	}
}

void	draw(t_data *data)
{
	double	tile_px_d;
	double	world_w;
	double	world_h;
	int		aux_a;
	int		aux_b;

	tile_px_d = floor(fmin(200 / (double)data->map.cols,
				200 / (double)data->map.rows));
	if (tile_px_d < 1.0)
		tile_px_d = 1.0;
	world_w = data->map.cols * TILE;
	world_h = data->map.rows * TILE;
	render_2d_vision(data);
	render_minimap_simple(data, tile_px_d);
	if (world_w > 0.0 && world_h > 0.0)
	{
		aux_a = (int)((data->player.x / world_w) * ((int)tile_px_d
					* data->map.cols) + 0.5);
		aux_b = (int)((data->player.y / world_h) * ((int)tile_px_d
					* data->map.rows) + 0.5);
		render_rect(data, (t_point){aux_a, aux_b}, (t_point){4, 4}, BLUE);
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}
