/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tu_nombre_de_usuario <tu_email@ejemplo.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:40:47 by kpineda-          #+#    #+#             */
/*   Updated: 2025/08/21 22:21:32 by tu_nombre_d      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double ft_degree_to_radian(double degrees)
{
	return (degrees * (3.1415 / 180.0));
}

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

static inline double clamp(double value, double min_value, double max_value)
{
    if (value < min_value)
        return min_value;
    if (value > max_value)
        return max_value;
    return value;
}


void render_3d_column(t_data *data, t_point intersection, double ray_angle, int col)
{
    double raw = hypot((double)intersection.x - (double)data->player.x , 
				(double)intersection.y - (double)data->player.y );
    double perp = raw * cos(ray_angle - ft_degree_to_radian(data->player.rotation));

    double proj_plane = (double)WIDTH / 2.0 / tan(3.1415/3 / 2.0);
    int wall_h = (int)(((double)20 * proj_plane) / perp + 0.5);
    int ytop = (int)clamp(HEIGHT / 2 - wall_h / 2, 0, HEIGHT - 1);
    int ybot = (int)clamp(HEIGHT / 2 + wall_h / 2, 0, HEIGHT - 1);
    if (ytop > 0)
        render_line(data, (t_point){col, 0}, (t_point){col, ytop-1}, WHITE);
    render_line(data, (t_point){col, ytop}, (t_point){col, ybot}, GRAY);
    if (ybot < HEIGHT - 1)
        render_line(data, (t_point){col, ybot+1}, (t_point){col, HEIGHT - 1}, BLACK);
}

void render_2d_vision(t_data *data)
{
	int i = 0;
	double angle;
	double auxangle = ft_degree_to_radian(data->player.rotation - 30);
	int x ;
	int y;
	t_point intersection;
	t_point intersectionaux;

	while (i < WIDTH)
	{
		x = 0;
		y = 0;
		angle = ((ft_degree_to_radian(60) * i) / WIDTH) + auxangle;
		int angle_x = (int)(sin(angle) * 800) + data->player.x + data->player.scale;
		int angle_y = (int)(cos(angle) * 800) + data->player.y + data->player.scale;
		intersectionaux = (t_point){angle_x, angle_y};
		while(data->map.map[y])
		{
			while(data->map.map[y][x])
			{
				if (data->map.map[y][x] == '1')
				{
					if (col_squareline((t_rect){ x * 20,  y * 20, 20, 20, RED}, (t_line){(t_point){data->player.x + data->player.scale, data->player.y + data->player.scale}, (t_point){angle_x, angle_y}}, &intersection, data))
					{
						double intersectiondist = pow(intersection.x - data->player.x, 2) + pow(intersection.y - data->player.y, 2);
						double auxdist = pow(intersectionaux.x - data->player.x, 2) + pow(intersectionaux.y - data->player.y, 2);
						if(intersectiondist < auxdist)
							intersectionaux = (t_point){intersection.x, intersection.y};
					}
						
				}
				x++;
			}
			x = 0;
			y++;
		}
		render_3d_column(data, intersectionaux, angle, i);
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
