/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:45:51 by kpineda-          #+#    #+#             */
/*   Updated: 2025/08/21 20:30:02 by kpineda-         ###   ########.fr       */
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
int check_coords(t_data *data, t_point *point, char *str, char coor, char post_coor)
{
	while (data->file.file[point->x][0] == '\n')
		point->x++;
	if (data->file.file[point->x][0] == coor && (data->file.file[point->x][1] == post_coor || data->file.file[point->x][1] == ' '))
	{
		int n = 0;
		while (data->file.file[point->x][point->y] == ' ' || data->file.file[point->x][point->y] == post_coor)
			point->y++;
		n = (ft_strlen(data->file.file[point->x]) - point->y);
		if (!ft_strcmp(str, data->file.file, n, point->x, point->y))
			return (0);
		if (ft_strcmp(str, data->file.file, n, point->x, point->y))
		{
			point->x++;
			point->y = 1;
			return (1);
		}
	}
	return (0);
}
int alpha(char *str)
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

int coma_case(t_data *data, t_point *point, int len)
{
	if (data->file.file[point->x][point->y] == ',')
	{
		point->y++;
		while (data->file.file[point->x][point->y] != ',')
		{
			len++;
			point->y++;
		}
		int g = ft_atoi(ft_substr(data->file.file[point->x], point->y - len, len));
		if (g < 0 || g > 255)
			return (0);
		len = 0;
		if (data->file.file[point->x][point->y] == ',')
			point->y++;
		while (data->file.file[point->x][point->y] != '\n')
		{
			len++;
			point->y++;
		}
		int b = ft_atoi(ft_substr(data->file.file[point->x], point->y - len, len));
		if (b < 0 || b > 255)
			return (0);
	}
	return (1);
}

int check_floor_ceiling(t_data *data, t_point *point, char c)
{
	int len = 0;
	while (data->file.file[point->x][0] == '\n')
		point->x++;
	if (data->file.file[point->x][0] != c || data->file.file[point->x][1] != ' ')
		return (1);
	while (data->file.file[point->x][point->y] == ' ')
		point->y++;
	while (data->file.file[point->x][point->y] != ',')
	{
		len++;
		point->y++;
	}
	char *r = ft_substr(data->file.file[point->x], point->y - len, len);
	if (!alpha(r))
		return (0);
	r = ft_substr(data->file.file[point->x], point->y - len, len);
	int p = ft_atoi(r);
	if (p < 0 || p > 255)
		return (0);
	len = 0;
	if (!coma_case(data, point, len))
		return (0);
	return (point->x++, point->y = 1, 1);
}

int parse_file_textures(t_data *data)
{
	t_point point;
	point.x = 0;
	point.y = 1;

	point = (t_point){0, 1};
	if (!check_coords(data, &point, "./path_to_the_north_texture", 'N', 'O'))
		return (0);
	if (!check_coords(data, &point, "./path_to_the_south_texture", 'S', 'O'))
		return (0);
	if (!check_coords(data, &point, "./path_to_the_west_texture", 'W', 'E'))
		return (0);
	if (!check_coords(data, &point, "./path_to_the_east_texture", 'E', 'A'))
		return (0);
	if (!check_floor_ceiling(data, &point, 'F'))
		return (0);
	if (!check_floor_ceiling(data, &point, 'C'))
		return (0);
	return (1);
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
	if (!parse_file_textures(data))
		return (data->map.map = NULL, 0);
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
