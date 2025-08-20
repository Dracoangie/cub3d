/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 17:44:56 by kpineda-          #+#    #+#             */
/*   Updated: 2025/08/19 19:46:25 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void free_data(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img.img_ptr);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	if (data->file.file)
		ft_free_matrix(data->file.file);
	if (data->map.map)
		ft_free_matrix(data->map.map);
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
