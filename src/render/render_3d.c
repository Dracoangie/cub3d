/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 13:51:45 by angnavar          #+#    #+#             */
/*   Updated: 2025/10/06 19:12:41 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	calculate_wall_limits(t_data *data, t_point hit, double ray_angle,
		t_point *wall_limits)
{
	double	dist_wall;
	double	difference;
	int		wall_height;
	double	player_pos[2];
	double	player_dir[2];

	player_pos[0] = data->player.x + data->player.scale;
	player_pos[1] = data->player.y + data->player.scale;
	dist_wall = hypot(hit.x - player_pos[0], hit.y - player_pos[1]);
	player_dir[0] = sin(ft_d_r(data->player.rotation));
	player_dir[1] = cos(ft_d_r(data->player.rotation));
	difference = sin(ray_angle) * player_dir[0] + cos(ray_angle)
		* player_dir[1];
	if (difference < 0.0001)
		difference = 0.0001;
	wall_height = (int)((20.0 * ((double)WIDTH / (2.0 * tan(3.1415 / 6.0))))
			/ (dist_wall * difference) + 0.5);
	wall_limits->x = (int)ft_clamp(HEIGHT / 2 - wall_height / 2, 0, HEIGHT - 1);
	wall_limits->y = (int)ft_clamp(HEIGHT / 2 + wall_height / 2, 0, HEIGHT - 1);
}

int	rgb(int red, int green, int blue)
{
	return (((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF));
}

void	render_ceiling_floor_column(t_data *data, int col, t_point wall_limits)
{
	if (wall_limits.x > 0)
		render_vline(data, (t_point){col, 0}, (t_point){col, wall_limits.x - 1},
			rgb(data->color[0].red, data->color[0].green,
				data->color[0].blue));
	if (wall_limits.y < HEIGHT - 1)
		render_vline(data, (t_point){col, wall_limits.y + 1}, (t_point){col,
			HEIGHT - 1}, rgb(data->color[1].red, data->color[1].green,
				data->color[1].blue));
}

void	render_3d_column(t_data *data, t_point hit, double ray_angle, int col)
{
	t_column_params	params;
	t_point			wall_limits;

	calculate_wall_limits(data, hit, ray_angle, &wall_limits);
	params.col = col;
	params.y0 = wall_limits.x;
	params.y1 = wall_limits.y;
	params.hit = hit;
	params.ray_angle = ray_angle;
	render_textured_column(data, params);
	render_ceiling_floor_column(data, col, wall_limits);
}
