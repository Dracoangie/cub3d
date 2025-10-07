/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_textures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 02:09:32 by angnavar          #+#    #+#             */
/*   Updated: 2025/10/07 17:34:45 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	normalize_column_limits(int *y0, int *y1)
{
	int	tmp;

	if (*y0 > *y1)
	{
		tmp = *y0;
		*y0 = *y1;
		*y1 = tmp;
	}
	if (*y1 < 0 || *y0 >= HEIGHT)
		return (0);
	if (*y0 < 0)
		*y0 = 0;
	if (*y1 >= HEIGHT)
		*y1 = HEIGHT - 1;
	return ((*y1 - *y0 + 1) > 0);
}

int	calculate_tex_x(t_tex_params *p, double bias)
{
	double	u;
	double	coord;
	int		tx;

	if (p->vertical)
		coord = (p->hit.y + 0.0001) / TILE;
	else
		coord = (p->hit.x + 0.0001) / TILE;
	u = coord - floor(coord);
	if ((p->vertical && p->ray_dir_x > 0.0) || (!p->vertical
			&& p->ray_dir_y < 0.0))
		u = 1.0 - u;
	*(p->tex_x) = (int)(u * p->tex->width);
	if (*(p->tex_x) < 0)
		*(p->tex_x) = 0;
	else if (*(p->tex_x) >= p->tex->width)
		*(p->tex_x) = p->tex->width - 1;
	tx = (int)floor((u + bias) * (double)p->tex->width);
	if (tx < 0)
		tx = 0;
	if (tx >= p->tex->width)
		tx = p->tex->width - 1;
	*(p->tex_x) = tx;
	return (1);
}

int	get_tex_and_u(t_tex_params *p)
{
	p->vertical = is_vertical_hit_eps(p->hit, p->ray_dir_x, p->ray_dir_y);
	if (p->vertical == 1)
	{
		if (p->ray_dir_x > 0.0)
			*(p->face) = TEX_W;
		else
			*(p->face) = TEX_E;
	}
	else
	{
		if (p->ray_dir_y > 0.0)
			*(p->face) = TEX_N;
		else
			*(p->face) = TEX_S;
	}
	p->tex = &p->data->tex[*(p->face)];
	if (!p->tex->img_ptr || !p->tex->img_pixels_ptr)
		return (0);
	return (calculate_tex_x(p, 1e-9));
}

void	draw_textured_column(t_data *data, t_column_params p, t_img *tex,
		int tex_x)
{
	int		y;
	int		slice_h;
	int		tex_y;
	double	t;

	y = p.y0;
	slice_h = p.y1 - p.y0 + 1;
	while (y <= p.y1)
	{
		t = (double)(y - p.y0) / (double)slice_h;
		tex_y = (int)floor((t + 1e-9) * (double)tex->height);
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		if (tex_y < 0)
			tex_y = 0;
		put_pixel(data, p.col, y, get_texel(tex, tex_x, tex_y));
		y++;
	}
}

void	render_textured_column(t_data *data, t_column_params p)
{
	t_tex_params	tp;

	tp.data = data;
	tp.hit = p.hit;
	tp.ray_dir_x = sin(p.ray_angle);
	tp.ray_dir_y = cos(p.ray_angle);
	tp.tex_x = &tp.tex_x_val;
	tp.face = &tp.face_val;
	if (!normalize_column_limits(&p.y0, &p.y1))
		return ;
	if (!get_tex_and_u(&tp))
		return ;
	draw_textured_column(data, p, tp.tex, *(tp.tex_x));
}
