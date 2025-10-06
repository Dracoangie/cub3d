/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 14:39:16 by kpineda-          #+#    #+#             */
/*   Updated: 2025/10/02 12:44:01 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	alpha(char *str)
{
	while (*str)
	{
		while (*str == ' ')
			str++;
		while (*str >= '0' && *str <= '9')
			str++;
		if (ft_isalpha(*str))
			return (0);
	}
	if (*str != '\0')
		free(str);
	return (1);
}

static int	check_color(char *line, int *idx, int *out, char end)
{
	int		start;
	int		len;
	char	*tmp;

	start = *idx;
	len = 0;
	while (line[*idx] && line[*idx] != end)
	{
		len++;
		(*idx)++;
	}
	tmp = ft_substr(line, start, len);
	if (!tmp)
		return (0);
	if (!alpha(tmp))
		return (free(tmp), 0);
	*out = ft_atoi(tmp);
	free(tmp);
	if (*out < 0 || *out > 255)
		return (0);
	return (1);
}

static int	check_rgb(t_data *data, t_point *p, int i)
{
	char	*line;

	line = data->file.file[p->x];
	while (line[p->y] == ' ')
		p->y++;
	if (!check_color(line, &p->y, &data->color[i].red, ','))
		return (0);
	if (line[p->y] != ',')
		return (0);
	p->y++;
	if (!check_color(line, &p->y, &data->color[i].green, ','))
		return (0);
	if (line[p->y] != ',')
		return (0);
	p->y++;
	if (!check_color(line, &p->y, &data->color[i].blue, '\n'))
		return (0);
	return (1);
}

int	check_floor_ceiling(t_data *data, t_point *point, char c, int i)
{
	char	*line;

	while (data->file.file[point->x]
		&& data->file.file[point->x][0] == '\n')
		point->x++;
	line = data->file.file[point->x];
	if (!line)
		return (0);
	if (line[0] != c || line[1] != ' ')
		return (1);
	point->y = 1;
	if (!check_rgb(data, point, i))
		return (0);
	point->x++;
	point->y = 1;
	return (1);
}
