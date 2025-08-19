/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:45:51 by kpineda-          #+#    #+#             */
/*   Updated: 2025/08/16 18:34:20 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int parse_coords(t_data *data)
{
	int i = 0;
	while (data->file.file[i])
	{
		if (data->file.file[i][0] == '\n')
		{
			i++;
			continue;
		}
		if (ft_chrcmp(data->file.file[i], " 01NSEW\n"))
			break;
		i++;
	}
	if (i == data->file.file_lines)
		return (0);
	return (i);
}

int set_player(t_data *data, int i)
{
	int j = 0;
	int hasOne = 0;

	while (data->map.map[i][j])
	{
		if (data->map.map[i][j] == 'N' || data->map.map[i][j] == 'S' || data->map.map[i][j] == 'W' || data->map.map[i][j] == 'E')
		{
			if (data->map.map[i][j] == 'N')
				data->player.rotation = 180;
			else if (data->map.map[i][j] == 'S')
				data->player.rotation = 0;
			else if (data->map.map[i][j] == 'W')
				data->player.rotation = 270;
			else if (data->map.map[i][j] == 'E')
				data->player.rotation = 90;
			data->map.map[i][j] = '0';
			data->player.x = j * 20;
			data->player.y = i * 20;
			hasOne++;
		}
		j++;
	}
	return (hasOne);
}

int clean_matrix(t_data *data, int i)
{
	int j = 0;
	int hasOne = 0;
	data->map.map = (char **)malloc(sizeof(char *) * (data->file.file_lines - i + 1));
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

int set_map(t_data *data)
{
	if (!clean_matrix(data, parse_coords(data)))
		return (0);
	data->map.rows = 0;
	data->map.cols = 0;
	while (data->map.map[data->map.rows])
	{
		if (data->map.cols < (int)ft_strlen(data->map.map[data->map.rows]))
			data->map.cols = (int)ft_strlen(data->map.map[data->map.rows]);
		data->map.rows++;
	}
	if (!check_map(data))
		return (0);
	return (1);
}

int read_file(t_data *data, char *name)
{
	int i = 0;
	char *line;
	char **aux;
	int fd;
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
	data->file.file_lines = i;
	close(fd);
	return (1);
}
