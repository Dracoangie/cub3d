/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:40:47 by kpineda-          #+#    #+#             */
/*   Updated: 2025/08/19 19:47:43 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void render_line(t_data *data, t_point begin, t_point end, int color)
{
	double deltaX = end.x - begin.x;
	double deltaY = end.y - begin.y;
	int pixels = sqrt((deltaX * deltaX) + (deltaY * deltaY));
	deltaX /= pixels;
	deltaY /= pixels;

	double pixelX = begin.x;
	double pixelY = begin.y;
	while (pixels)
	{
		if (!(pixelX <= WIDTH && pixelY <= HEIGHT) ||
			!(pixelX >= 0 && pixelY >= 0))
			break;
		put_pixel(data, pixelX, pixelY, color);
		pixelX += deltaX;
		pixelY += deltaY;
		--pixels;
	}
}

double ft_degree_to_radian(double degrees)
{
	return (degrees * (3.1415 / 180.0));
}

void render_2d_vision(t_data *data)
{
	int i = 0;
	double angle;
	double auxangle = ft_degree_to_radian(data->player.rotation - 30);
	// printf("angle : %f\n", angle);

	while (i < WIDTH)
	{
		angle = ((ft_degree_to_radian(60) * i) / WIDTH) + auxangle;
		int angle_x = (int)(sin(angle) * 200) + data->player.x + data->player.scale;
		int angle_y = (int)(cos(angle) * 200) + data->player.y + data->player.scale;
		render_line(data, (t_point){data->player.x + data->player.scale, data->player.y + data->player.scale}, (t_point){angle_x, angle_y}, WHITE);
		i++;
	}
}

void render_rect(t_data *data, int x, int y, int height, int width, int color)
{
	int i = 0;
	int j = 0;

	while (i < width)
	{
		while (j < height)
		{
			put_pixel(data, i + x, j + y, color);
			j++;
		}
		j = 0;
		i++;
	}
}

int render_circle(t_data *data, int x, int y, int radius)
{
	int dist, i, j;
	i = 0;
	{
		while (i <= radius * 2)
		{
			j = 0;
			while (j <= radius * 2)
			{
				dist = sqrt((i - radius) * (i - radius) + (j - radius) * (j - radius));
				{
					if (dist <= radius)
						put_pixel(data, i + x, j + y, CYAN);
				}
				j++;
			}
			i++;
		}
	}
	return 0;
}

void draw(t_data *data)
{
	int i = 0;
	int j = 0;
	int x = 0;
	int y = 0;

	render_rect(data, 0, 0, 800, 800, BLACK);

	while (i < data->map.rows)
	{
		while (j < data->map.cols)
		{
			if (data->map.map[i][j] == '1')
				render_rect(data, x, y, 20, 20, BLACK);
			else if (data->map.map[i][j] == '0')
				render_rect(data, x, y, 20, 20, GRAY);
			else if (data->map.map[i][j] == ' ')
				render_rect(data, x, y, 20, 20, BLACK);
			else
			{
				render_rect(data, x, y, 20, 20, BLACK);
				j = data->map.cols;
			}
			x += 20;
			j++;
		}
		x = 0;
		y += 20;
		j = 0;
		i++;
	}
	render_2d_vision(data);
	render_circle(data, data->player.x, data->player.y, data->player.scale);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}

void put_pixel(t_data *data, int x, int y, int color)
{
	char *pixel;

	pixel = data->img.img_pixels_ptr + (x * 4) + (y * data->img.line_len);
	*(int *)pixel = color;
}
