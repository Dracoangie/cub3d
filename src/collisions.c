/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:28:38 by tu_nombre_d       #+#    #+#             */
/*   Updated: 2025/09/24 23:30:46 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	col_squaresquare(t_rect rect, t_rect rect2)
{
	int	hit;

	hit = 0;
	if (rect.x < rect2.x + rect2.width && rect.x + rect.width > rect2.x
		&& rect.y < rect2.y + rect2.height && rect.y + rect.height > rect2.y)
		hit = 1;
	return (hit);
}

int	aabb_overlap(t_rect rect, t_line line)
{
	double	l[4];
	double	r[4];

	l[0] = fmin(line.start.x, line.end.x);
	l[1] = fmin(line.start.y, line.end.y);
	l[2] = fmax(line.start.x, line.end.x);
	l[3] = fmax(line.start.y, line.end.y);
	r[0] = rect.x;
	r[1] = rect.y;
	r[2] = rect.x + rect.width;
	r[3] = rect.y + rect.height;
	return (!(l[2] < r[0] || r[2] < l[0] || l[3] < r[1] || r[3] < l[1]));
}

void	rect_to_points(t_rect rect, t_point r[4])
{
	r[0].x = rect.x;
	r[0].y = rect.y;
	r[1].x = rect.x + rect.width;
	r[1].y = rect.y;
	r[2].x = rect.x + rect.width;
	r[2].y = rect.y + rect.height;
	r[3].x = rect.x;
	r[3].y = rect.y + rect.height;
}

int	col_squareline(t_rect rect, t_line line, t_point *inter, t_data *data)
{
	t_col_state	s;

	s.hit = 0;
	s.closedist = 20000000;
	rect_to_points(rect, s.r);
	if (!aabb_overlap(rect, line))
		return (0);
	s.i = -1;
	while (++s.i < 4)
	{
		if (col_lineline(line, (t_line){s.r[s.i], s.r[(s.i + 1) % 4]}, &s.iaux))
		{
			s.hit = 1;
			s.auxdist = pow(s.iaux.x - data->player.x, 2) + pow(s.iaux.y
					- data->player.y, 2);
			if (s.auxdist < s.closedist)
			{
				*inter = s.iaux;
				s.closedist = s.auxdist;
			}
		}
	}
	return (s.hit);
}

int	col_lineline(t_line l1, t_line l2, t_point *intersection)
{
	double	div;
	double	ua;
	double	ub;

	div = (double)((l2.end.y - l2.start.y) * (l1.end.x - l1.start.x)
			- (l2.end.x - l2.start.x) * (l1.end.y - l1.start.y));
	if (div == 0.0)
		return (0);
	ua = ((l2.end.x - l2.start.x) * (l1.start.y - l2.start.y)
			- (l2.end.y - l2.start.y) * (l1.start.x - l2.start.x)) / div;
	ub = ((l1.end.x - l1.start.x) * (l1.start.y - l2.start.y)
			- (l1.end.y - l1.start.y) * (l1.start.x - l2.start.x)) / div;
	if (ua >= 0.0 && ua <= 1.0 && ub >= 0.0 && ub <= 1.0)
	{
		intersection->x = (int)(l1.start.x + ua * (l1.end.x - l1.start.x));
		intersection->y = (int)(l1.start.y + ua * (l1.end.y - l1.start.y));
		return (1);
	}
	return (0);
}
