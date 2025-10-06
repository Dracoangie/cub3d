/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:57:10 by angnavar          #+#    #+#             */
/*   Updated: 2025/10/02 12:59:04 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	set_player_rotation(t_data *data, int i, int j)
{
	if (data->map.map[i][j] == 'N')
		data->player.rotation = 180;
	else if (data->map.map[i][j] == 'S')
		data->player.rotation = 0;
	else if (data->map.map[i][j] == 'W')
		data->player.rotation = 270;
	else if (data->map.map[i][j] == 'E')
		data->player.rotation = 90;
	return (-1);
}

int	set_player(t_data *data, int i)
{
	int	j;
	int	hasOne;

	j = 0;
	hasOne = 0;
	while (data->map.map[i][j])
	{
		if (data->map.map[i][j] == 'N' || data->map.map[i][j] == 'S'
			|| data->map.map[i][j] == 'W' || data->map.map[i][j] == 'E')
		{
			set_player_rotation(data, i, j);
			data->map.map[i][j] = '0';
			data->player.x = j * 20;
			data->player.y = i * 20;
			hasOne++;
		}
		j++;
	}
	return (hasOne);
}

void	player_move(t_data *data, int dir, int spd)
{
	int	y;
	int	x;
	int	i;
	int	j;

	y = data->player.y + (int)(cos(ft_d_r(data->player.rotation + dir)) * spd);
	x = data->player.x + (int)(sin(ft_d_r(data->player.rotation + dir)) * spd);
	i = -1;
	while (data->map.map[++i])
	{
		j = 0;
		while (data->map.map[i][j])
		{
			if (data->map.map[i][j] == '1')
			{
				if (col_squaresquare((t_rect){x, y, data->player.scale * 2,
						data->player.scale * 2, BLACK}, (t_rect){j * 20, i * 20,
					20, 20, BLACK}))
					return ;
			}
			j++;
		}
	}
	data->player.y += (int)(cos(ft_d_r(data->player.rotation + dir)) * spd);
	data->player.x += (int)(sin(ft_d_r(data->player.rotation + dir)) * spd);
}
