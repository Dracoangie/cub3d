/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 13:47:29 by angnavar          #+#    #+#             */
/*   Updated: 2025/09/24 22:51:30 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void put_pixel(t_data *data, int x, int y, int color)
{
	char *pixel;

	pixel = data->img.img_pixels_ptr + (x * 4) + (y * data->img.line_len);
	*(int *)pixel = color;
}

int get_texel(const t_img *tex, int x, int y)
{
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x >= tex->width)  x = tex->width - 1;
	if (y >= tex->height) y = tex->height - 1;
	char *p = tex->img_pixels_ptr + y * tex->line_len + x * (tex->bits_per_pixel / 8);
	return *(int *)p;
}

int is_vertical_hit_eps(t_point hit, double ray_dir_x, double ray_dir_y)
{
	double tile = (double)TILE;
	double fx = fmod((double)hit.x, tile); if (fx < 0) fx += tile;
	double fy = fmod((double)hit.y, tile); if (fy < 0) fy += tile;
	double dx = fmin(fx, tile - fx);
	double dy = fmin(fy, tile - fy);
	const double eps = 1e-4 * tile;
	if (dx + eps < dy) return 1;
	if (dy + eps < dx) return 0;
	return fabs(ray_dir_x) >= fabs(ray_dir_y);
}

double ft_d_r(double degrees)
{
	return (degrees * (3.1415 / 180.0));
}