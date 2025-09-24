/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 13:51:45 by angnavar          #+#    #+#             */
/*   Updated: 2025/09/24 23:47:33 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void render_textured_column(t_data *data, t_column_params p)
{
    int     tmp;
    int     slice_h;
    double  ray_dir_x;
    double  ray_dir_y;
    int     vertical;
    t_face  face;
    t_img   *tex;
    double  tile;
    double  margin;
    int     tex_x;
    int     tex_y;
    int     color;

    if (p.y0 > p.y1)
    {
        tmp = p.y0;
        p.y0 = p.y1;
        p.y1 = tmp;
    }
    if (p.y1 < 0 || p.y0 >= HEIGHT)
        return ;
    if (p.y0 < 0)
        p.y0 = 0;
    if (p.y1 >= HEIGHT)
        p.y1 = HEIGHT - 1;
    slice_h = (p.y1 - p.y0) + 1;
    if (slice_h <= 0)
        return ;
    ray_dir_x = sin(p.ray_angle);
    ray_dir_y = cos(p.ray_angle);
    vertical = is_vertical_hit_eps(p.hit, ray_dir_x, ray_dir_y);
    if (vertical)
        face = (ray_dir_x > 0.0) ? TEX_W : TEX_E;
    else
        face = (ray_dir_y > 0.0) ? TEX_N : TEX_S;
    tex = &data->tex[face];
    if (!tex->img_ptr || !tex->img_pixels_ptr)
        return ;
    tile = (double)TILE;
    double u = vertical ? fmod((double)p.hit.y, tile) / tile
                        : fmod((double)p.hit.x, tile) / tile;
    if (u < 0.0)
        u += 1.0;
    if ((vertical && ray_dir_x > 0.0) || (!vertical && ray_dir_y < 0.0))
        u = 1.0 - u;
    margin = 0.5 / (double)tex->width;
    if (u < margin)
        u = margin;
    else if (u > 1.0 - margin)
        u = 1.0 - margin;
    tex_x = (int)(u * (double)tex->width);
    if (tex_x >= tex->width)
        tex_x = tex->width - 1;
    for (int y = p.y0; y <= p.y1; ++y)
    {
        double t = (double)(y - p.y0) / (double)slice_h;
        tex_y = (int)(t * (double)tex->height);
        if (tex_y >= tex->height)
            tex_y = tex->height - 1;
        color = get_texel(tex, tex_x, tex_y);
        put_pixel(data, p.col, y, color);
    }
}

void	render_3d_column(t_data *data, t_point hit, double ray_angle, int col)
{
	double	distance_to_wall;
	double	ray_dir[2];
	double	cos_angle_difference;
	double	projection_plane;
	int		wall_height;

	double player_pos[2] = {data->player.x + data->player.scale,
							data->player.y + data->player.scale};
	distance_to_wall = hypot(hit.x - player_pos[0], hit.y - player_pos[1]);
	ray_dir[0] = sin(ray_angle);
	ray_dir[1] = cos(ray_angle);
	double player_dir[2] = {sin(ft_d_r(data->player.rotation)),
							cos(ft_d_r(data->player.rotation))};
	cos_angle_difference = ray_dir[0] * player_dir[0] + ray_dir[1]
		* player_dir[1];
	if (cos_angle_difference < 0.0001)
		cos_angle_difference = 0.0001;
	projection_plane = (double)WIDTH / (2.0 * tan(3.1415 / 6.0));
	wall_height = (int)((20.0 * projection_plane) / (distance_to_wall
				* cos_angle_difference) + 0.5);
	int wall_y[2] = {
		(int)ft_clamp(HEIGHT / 2 - wall_height / 2, 0, HEIGHT - 1),
		(int)ft_clamp(HEIGHT / 2 + wall_height / 2, 0, HEIGHT - 1)};
	if (wall_y[0] > 0)
	{
		render_vline(data, (t_point){col, 0}, (t_point){col, wall_y[0] - 1},
				RGB(data->color[0].red, data->color[0].green,
					data->color[0].blue));
	}
	t_column_params params = {col, wall_y[0], wall_y[1], hit, ray_angle};
	render_textured_column(data, params);
	if (wall_y[1] < HEIGHT - 1)
		render_vline(data, (t_point){col, wall_y[1] + 1}, (t_point){col, HEIGHT
				- 1}, RGB(data->color[1].red, data->color[1].green,
					data->color[1].blue));
}
