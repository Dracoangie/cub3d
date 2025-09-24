/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:28:38 by tu_nombre_d       #+#    #+#             */
/*   Updated: 2025/09/24 13:59:23 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int col_squaresquare(t_rect rect, t_rect rect2)
{
	int hit;

	hit = 0;
	if (rect.x < rect2.x + rect2.width && rect.x + rect.width > rect2.x &&
		rect.y < rect2.y + rect2.height && rect.y + rect.height > rect2.y)
		hit = 1;
    return (hit);
}

int aabb_overlap(t_rect rect, t_line line)
{
    double l[4] = { fmin(line.start.x, line.end.x), fmin(line.start.y, line.end.y),
        fmax(line.start.x, line.end.x), fmax(line.start.y, line.end.y) };
	double r[4] = { rect.x, rect.y,
        rect.x + rect.width,
        rect.y + rect.height
    };
    return (!(l[2] < r[0] || r[2] < l[0] || l[3] < r[1] || r[3] < l[1]));
}


int col_squareline(t_rect rect, t_line line, t_point* inter, t_data *data)
{
    int hit = 0;
    double closedist = 20000000;
    t_point iaux;
	int i;

    t_point r[4] = { {rect.x, rect.y}, {rect.x + rect.width, rect.y},
        {rect.x + rect.width, rect.y + rect.height},
        {rect.x, rect.y + rect.height}
    };
	if (!aabb_overlap(rect, line))
        return (0);
	i = -1;
    while (++i < 4)
	{
        if (col_lineLine(line.start, line.end, r[i], r[(i+1)%4], &iaux))
		{
            hit = 1;
            double auxdist = pow(iaux.x - data->player.x, 2) + pow(iaux.y - data->player.y, 2);
            if (auxdist < closedist)
                *inter = iaux;
            if (auxdist < closedist)
                closedist = auxdist;
        }
    }
    return (hit);
}

int col_lineLine(t_point point1, t_point point2, t_point point3, t_point point4, t_point* intersection)
{
	double div = (double)((point4.y-point3.y)*(point2.x-point1.x) - (point4.x-point3.x)*(point2.y-point1.y));
	if (div == 0.0)
		return 0;
	double uA = ((point4.x-point3.x)*(point1.y-point3.y) - (point4.y-point3.y)*(point1.x-point3.x)) / div;
	double uB = ((point2.x-point1.x)*(point1.y-point3.y) - (point2.y-point1.y)*(point1.x-point3.x)) / div;

	if (uA >= 0.0 && uA <= 1.0 && uB >= 0.0 && uB <= 1.0)
	{
		intersection->x = (int)(point1.x + uA * (point2.x - point1.x));
		intersection->y = (int)(point1.y + uA * (point2.y - point1.y));
		return (1);
	}
  return (0);
}
