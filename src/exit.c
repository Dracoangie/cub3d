/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 17:44:56 by kpineda-          #+#    #+#             */
/*   Updated: 2025/09/24 14:17:05 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void free_data(t_data *data)
{
	int i;
	
	mlx_destroy_image(data->mlx, data->img.img_ptr);
	mlx_destroy_window(data->mlx, data->win);
	if (data->file.file)
		ft_free_matrix(data->file.file);
	if (data->map.map)
		ft_free_matrix(data->map.map);
	i = 0;
	while (i < 4)
	{
		if (!data->tex[i].img_ptr)
			break ;
		if (data->mlx && data->tex[i].img_ptr)
			mlx_destroy_image(data->mlx, data->tex[i].img_ptr);
		data->tex[i].img_ptr = NULL;
		i++;
	}
	mlx_destroy_display(data->mlx);
	free(data->mlx);
}

void exit_error(t_data *data)
{
	write(2, "Error\n", 7);
	free_data(data);
	exit(1);
}

void exit_program(t_data *data)
{
	free_data(data);
	exit(0);
}
