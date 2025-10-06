/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:45:51 by kpineda-          #+#    #+#             */
/*   Updated: 2025/10/02 13:00:43 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_coords(t_data *data)
{
	int	i;

	i = 0;
	while (data->file.file[i])
	{
		if (data->file.file[i][0] == '\n')
		{
			i++;
			continue ;
		}
		if (ft_chrcmp(data->file.file[i], " 01NSEW\n"))
			break ;
		i++;
	}
	if (i == data->file.ls)
		return (0);
	return (i);
}

int	parse_file_textures(t_data *data)
{
	t_point	point;

	point.x = 0;
	point.y = 1;
	point = (t_point){0, 1};
	if (!check_coords(data, &point, 'N', 'O'))
		return (0);
	if (!check_coords(data, &point, 'S', 'O'))
		return (0);
	if (!check_coords(data, &point, 'W', 'E'))
		return (0);
	if (!check_coords(data, &point, 'E', 'A'))
		return (0);
	if (!check_floor_ceiling(data, &point, 'F', 0))
		return (0);
	if (!check_floor_ceiling(data, &point, 'C', 1))
		return (0);
	return (1);
}

int	clean_matrix(t_data *data, int i)
{
	int	j;
	int	hasOne;

	j = 0;
	hasOne = 0;
	data->map.map = (char **)malloc(sizeof(char *) * (data->file.ls - i + 1));
	while (data->file.file[i])
	{
		if (data->file.file[i][0] == '\n')
			return (data->map.map[j] = NULL, 0);
		else if (ft_chrcmp(data->file.file[i], " 01NSWE\n"))
		{
			data->map.map[j] = ft_strdup(data->file.file[i]);
			if (data->file.file[i + 1])
				data->map.map[j][ft_strlen(data->file.file[i]) - 1] = 0;
			hasOne += set_player(data, j);
		}
		else
			return (data->map.map[j] = NULL, 0);
		i++;
		j++;
	}
	if (hasOne != 1)
		return (data->map.map[j] = NULL, 0);
	return (data->map.map[j] = NULL, 1);
}

int	read_file(t_data *data, char *name)
{
	int		i;
	char	*line;
	char	**aux;
	int		fd;

	i = 0;
	if ((fd = open(name, O_RDONLY)) < 0)
		return (data->file.file = NULL, data->map.map = NULL, 0);
	data->file.file = (char **)malloc(sizeof(char *) * 2);
	while ((line = get_next_line(fd)))
	{
		aux = data->file.file;
		aux[i] = NULL;
		data->file.file = (char **)malloc(sizeof(char *) * (i + 2));
		ft_dup_matrix(aux, data->file.file);
		data->file.file[i++] = line;
		free(aux);
	}
	free(line);
	data->file.file[i] = NULL;
	data->file.ls = i;
	close(fd);
	return (1);
}
