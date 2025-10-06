/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_coords.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 14:41:14 by kpineda-          #+#    #+#             */
/*   Updated: 2025/10/02 13:17:38 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	init_tex(t_data *d, t_img *t, const char *path)
{
	t->img_ptr = mlx_xpm_file_to_image(d->mlx, (char *)path, &t->width,
			&t->height);
	if (!t->img_ptr)
		return (0);
	t->img_pixels_ptr = mlx_get_data_addr(t->img_ptr, &t->bits_per_pixel,
			&t->line_len, &t->endian);
	return (t->img_pixels_ptr != NULL);
}

static t_face	face_from(char coor)
{
	if (coor == 'N')
		return (TEX_N);
	if (coor == 'S')
		return (TEX_S);
	if (coor == 'E')
		return (TEX_E);
	return (TEX_W);
}

static char	*check_coords_aux(char *line, int start)
{
	int		y;
	size_t	len;

	y = start;
	while (line[y] == ' ' || line[y] == '\t')
		y++;
	len = ft_strlen(line + y);
	while (len > 0 && (line[y + len - 1] == ' ' || line[y + len - 1] == '\t'
			|| line[y + len - 1] == '\n' || line[y + len - 1] == '\r'))
		len--;
	return (ft_substr(line, y, len));
}

int	check_coords(t_data *data, t_point *point, char coor, char post_coor)
{
	char	*line;
	int		y;
	char	*path;
	t_face	face;
	int		ok;

	while (data->file.file[point->x]
		&& data->file.file[point->x][0] == '\n')
		point->x++;
	line = data->file.file[point->x];
	if (!line)
		return (0);
	if (!(line[0] == coor && (line[1] == post_coor || line[1] == ' ')))
		return (0);
	y = 1;
	if (line[1] == post_coor)
		y = 2;
	path = check_coords_aux(line, y);
	if (!path)
		return (0);
	face = face_from(coor);
	ok = init_tex(data, &data->tex[face], path);
	free(path);
	point->x++;
	return (point->y = 1, ok);
}
