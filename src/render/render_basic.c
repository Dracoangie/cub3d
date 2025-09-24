/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_basic.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 13:38:31 by tu_nombre_d       #+#    #+#             */
/*   Updated: 2025/09/24 13:49:54 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void render_vline(t_data *data, t_point begin, t_point end, int color)
{

	int y0 = (int)(begin.y + 0.5);
	int y1 = (int)(end.y + 0.5);
	int x = (int)(begin.x + 0.5);
	int y = y0;

	if (x < 0 || x >= WIDTH)
		return;
	if (y0 > y1)
	{
		int t = y0;
		y0 = y1;
		y1 = t;
	}
	if (y1 < 0 || y0 >= HEIGHT)
		return;
	if (y0 < 0)
		y0 = 0;
	if (y1 >= HEIGHT)
		y1 = HEIGHT - 1;
	y = y0;
	while (y <= y1)
	{
		put_pixel(data, x, y, color);
		++y;
	}
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