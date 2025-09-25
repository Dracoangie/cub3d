/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 18:37:49 by kpineda-          #+#    #+#             */
/*   Updated: 2025/09/25 01:01:01 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_around(int i, int j, t_data *data)
{
	int	x;
	int	y;

	x = i - 1;
	y = j - 1;
	while (x <= i + 1)
	{
		while (y <= j + 1)
		{
			if (data->map.map[x][y] == ' ' || data->map.map[x][y] == '\0')
				return (0);
			y++;
		}
		y = j - 1;
		x++;
	}
	return (1);
}

int	check_internal_cells(t_data *data, int i, int j)
{
	if (data->map.map[i][j] == '0')
	{
		if (!check_around(i, j, data))
			return (0);
	}
	return (1);
}

int	check_map(t_data *data)
{
	int	i;
	int	j;
	int	size;

	i = 0;
	j = 0;
	size = (ft_strlen(data->map.map[i]) - 1);
	while (data->map.map[i])
	{
		j = 0;
		while (data->map.map[i][j])
		{
			if (i == 0 || j == 0 || i == (data->map.rows - 1) || j == size)
			{
				if (data->map.map[i][j] == '0')
					return (0);
			}
			else if (data->map.map[i][j] == '0')
				check_internal_cells(data, i, j);
			j++;
		}
		i++;
	}
	return (1);
}
