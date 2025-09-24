/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 13:51:45 by angnavar          #+#    #+#             */
/*   Updated: 2025/09/24 13:52:45 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void render_textured_column(t_data *data, int col, int y0, int y1, t_point hit, double ray_angle)
{
	if (y0 > y1) { int tmp = y0; y0 = y1; y1 = tmp; }
	if (y1 < 0 || y0 >= HEIGHT) return;
	if (y0 < 0) y0 = 0;
	if (y1 >= HEIGHT) y1 = HEIGHT - 1;

	const int slice_h = (y1 - y0) + 1;
	if (slice_h <= 0) return;

	// Dirección del rayo (usa tu convención actual)
	const double ray_dir_x = sin(ray_angle);
	const double ray_dir_y = cos(ray_angle);

	// ¿golpe vertical u horizontal?
	const int vertical = is_vertical_hit_eps(hit, ray_dir_x, ray_dir_y);

	// Selección de cara N/S/E/W
	t_face face;
	if (vertical)
		face = (ray_dir_x > 0.0) ? TEX_W : TEX_E;     // mov. +X => pared mirando a W
	else
		face = (ray_dir_y > 0.0) ? TEX_N : TEX_S;   // mov. +Y => pared mirando a N

	const t_img *tex = &data->tex[face];
	if (!tex->img_ptr || !tex->img_pixels_ptr) return;

	// Coordenada U dentro del tile [0..1)
	const double tile = (double)TILE;
	double u = vertical ? fmod((double)hit.y, tile) / tile
	                    : fmod((double)hit.x, tile) / tile;
	if (u < 0.0) u += 1.0;

	// Flip para mantener orientación consistente
	if ((vertical && ray_dir_x > 0.0) || (!vertical && ray_dir_y < 0.0))
		u = 1.0 - u;

	// Margen medio texel para evitar costuras
	double margin = 0.5 / (double)tex->width;
	if (u < margin) u = margin;
	else if (u > 1.0 - margin) u = 1.0 - margin;

	int tex_x = (int)(u * (double)tex->width);
	if (tex_x >= tex->width) tex_x = tex->width - 1;

	// Barrido vertical
	for (int y = y0; y <= y1; ++y)
	{
		double t = (double)(y - y0) / (double)slice_h;     // [0..1]
		int tex_y = (int)(t * (double)tex->height);
		if (tex_y >= tex->height) tex_y = tex->height - 1;

		int color = get_texel(tex, tex_x, tex_y);
		put_pixel(data, col, y, color);
	}
}


void render_3d_column(t_data *data, t_point hit, double ray_angle, int col)
{
	double player_pos[2] = {data->player.x + data->player.scale,
							data->player.y + data->player.scale};
	double distance_to_wall = hypot(hit.x - player_pos[0], hit.y - player_pos[1]);
	double ray_dir[2] = {sin(ray_angle), cos(ray_angle)};
	double player_dir[2] = {sin(ft_degree_to_radian(data->player.rotation)),
							cos(ft_degree_to_radian(data->player.rotation))};
	double cos_angle_difference = ray_dir[0] * player_dir[0] + ray_dir[1] * player_dir[1];
	if (cos_angle_difference < 0.0001)
		cos_angle_difference = 0.0001;
	double projection_plane = (double)WIDTH / (2.0 * tan(3.1415 / 6.0));
	int wall_height = (int)((20.0 * projection_plane) / (distance_to_wall * cos_angle_difference) + 0.5);
	int wall_y[2] = {
		(int)ft_clamp(HEIGHT / 2 - wall_height / 2, 0, HEIGHT - 1),
		(int)ft_clamp(HEIGHT / 2 + wall_height / 2, 0, HEIGHT - 1)};
	if (wall_y[0] > 0)
	{
		render_vline(data, (t_point){col, 0}, (t_point){col, wall_y[0] - 1}, RGB(data->color[0].red, data->color[0].green, data->color[0].blue));
	}
	render_textured_column(data, col, wall_y[0], wall_y[1], hit, ray_angle);
	if (wall_y[1] < HEIGHT - 1)
		render_vline(data, (t_point){col, wall_y[1] + 1}, (t_point){col, HEIGHT - 1}, RGB(data->color[1].red, data->color[1].green, data->color[1].blue));
}
