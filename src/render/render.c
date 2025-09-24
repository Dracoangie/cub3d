/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:40:47 by kpineda-          #+#    #+#             */
/*   Updated: 2025/09/24 22:51:30 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void render_2d_vision(t_data *data)
{
	int i = 0;
	double angle;
	double auxangle = ft_d_r(data->player.rotation - 30);
	int x;
	int y;
	t_point intersection;
	t_point intersectionaux;

	while (i < WIDTH)
	{
		x = 0;
		y = 0;
		angle = ((ft_d_r(60) * i) / WIDTH) + auxangle;
		int angle_x = (int)(sin(angle) * 800) + data->player.x + data->player.scale;
		int angle_y = (int)(cos(angle) * 800) + data->player.y + data->player.scale;
		intersectionaux = (t_point){angle_x, angle_y};
		while (data->map.map[y])
		{
			while (data->map.map[y][x])
			{
				if (data->map.map[y][x] == '1')
				{
					if (col_squareline((t_rect){x * 20, y * 20, 20, 20, RED}, (t_line){(t_point){data->player.x + data->player.scale, data->player.y + data->player.scale}, (t_point){angle_x, angle_y}}, &intersection, data))
					{
						double intersectiondist = pow(intersection.x - data->player.x, 2) + pow(intersection.y - data->player.y, 2);
						double auxdist = pow(intersectionaux.x - data->player.x, 2) + pow(intersectionaux.y - data->player.y, 2);
						if (intersectiondist < auxdist)
							intersectionaux = (t_point){intersection.x, intersection.y};
					}
				}
				x++;
			}
			x = 0;
			y++;
		}
		render_3d_column(data, intersectionaux, angle, WIDTH - 1 - i);
		i++;
	}
}

void render_minimap_simple(t_data *data)
{
	double tile_px_d = floor(fmin(200 / (double)data->map.cols,
	                              200 / (double)data->map.rows));
	if (tile_px_d < 1.0)
		tile_px_d = 1.0;
	int width_map  = (int)tile_px_d * data->map.cols;
	int height_map = (int)tile_px_d * data->map.rows;
	int aux_a = width_map / data->map.cols;
	int aux_b = height_map / data->map.rows;

	if (aux_a < 1)
		aux_a = 1;
	if (aux_b < 1)
		aux_b = 1;

	int i = 0;
	while (i < data->map.rows)
	{
		int j = 0;
		while (j < (int)ft_strlen(data->map.map[i]) && j < data->map.cols)
		{
			int rx = j * aux_a;
			int ry = i * aux_b;
			if (data->map.map[i][j] == '1')
				render_rect(data, rx, ry, aux_b, aux_a, DARK_GRAY);
			else if (data->map.map[i][j] == '0')
				render_rect(data, rx, ry, aux_b, aux_a, GRAY);
			j++;
		}
		i++;
	}

	double world_w = data->map.cols * TILE;
	double world_h = data->map.rows * TILE;
	if (world_w > 0.0 && world_h > 0.0)
	{
		aux_a = (int)((data->player.x / world_w) * width_map  + 0.5);
		aux_b = (int)((data->player.y / world_h) * height_map + 0.5);
		render_rect(data, aux_a - 2, aux_b - 2, 4, 4, BLUE);
	}
}

void draw(t_data *data)
{
	render_2d_vision(data);
	render_minimap_simple(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}