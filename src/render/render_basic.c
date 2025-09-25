/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_basic.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 13:38:31 by tu_nombre_d       #+#    #+#             */
/*   Updated: 2025/09/25 00:23:20 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	render_rect(t_data *data, t_point point, t_point size, int color)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < size.x)
	{
		while (j < size.y)
		{
			put_pixel(data, i + point.x, j + point.y, color);
			j++;
		}
		j = 0;
		i++;
	}
}

int	render_circle(t_data *data, int x, int y, int radius)
{
	int	dist;
	int	i;
	int	j;

	i = 0;
	while (i <= radius * 2)
	{
		j = 0;
		while (j <= radius * 2)
		{
			dist = sqrt((i - radius) * (i - radius) + (j - radius) * (j
						- radius));
			{
				if (dist <= radius)
					put_pixel(data, i + x, j + y, CYAN);
			}
			j++;
		}
		i++;
	}
	return (0);
}

void	render_vline(t_data *data, t_point begin, t_point end, int color)
{
	t_point	p0;
	t_point	p1;
	int		y;
	t_point	tmp;

	p0 = (t_point){(int)(begin.x + 0.5), (int)(begin.y + 0.5)};
	p1 = (t_point){(int)(end.x + 0.5), (int)(end.y + 0.5)};
	if (p0.x < 0 || p0.x >= WIDTH)
		return ;
	if (p0.y > p1.y)
	{
		tmp = p0;
		p0 = p1;
		p1 = tmp;
	}
	if (p1.y < 0 || p0.y >= HEIGHT)
		return ;
	if (p0.y < 0)
		p0.y = 0;
	if (p1.y >= HEIGHT)
		p1.y = HEIGHT - 1;
	y = p0.y - 1;
	while (y++ <= p1.y)
		put_pixel(data, p0.x, y, color);
}

void	render_line(t_data *data, t_point begin, t_point end, int color)
{
	double	delta_x;
	double	delta_y;
	int		pixels;
	double	pixel_x;
	double	pixel_y;

	delta_x = end.x - begin.x;
	delta_y = end.y - begin.y;
	pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
	pixel_x = begin.x;
	pixel_y = begin.y;
	delta_x /= pixels;
	delta_y /= pixels;
	while (pixels)
	{
		if (!(pixel_x <= WIDTH && pixel_y <= HEIGHT)
			|| !(pixel_x >= 0 && pixel_y >= 0))
			break ;
		put_pixel(data, pixel_x, pixel_y, color);
		pixel_x += delta_x;
		pixel_y += delta_y;
		--pixels;
	}
}
