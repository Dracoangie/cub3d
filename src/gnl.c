/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 14:16:53 by angnavar          #+#    #+#             */
/*   Updated: 2025/08/19 19:47:05 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*is_line_break(char **buffer)
{
	char	*newline;
	size_t	line_length;
	char	*line;
	char	*aux;

	if (!buffer || !*buffer)
		return (NULL);
	newline = ft_strchr (*buffer, '\n');
	if (!newline)
		return (NULL);
	line_length = newline - *buffer + 1;
	line = ft_substr (*buffer, 0, line_length);
	if (!line)
		return (NULL);
	aux = ft_strdup (newline + 1);
	if (!aux)
	{
		free (line);
		return (NULL);
	}
	free (*buffer);
	*buffer = aux;
	line[ft_strlen (line)] = '\0';
	return (line);
}

char	*check_readed(ssize_t i_readed, char **buffer, char *buffer_read)
{
	char	*aux;

	free(buffer_read);
	if (i_readed == 0 && *buffer && ft_strlen(*buffer) > 0)
	{
		aux = *buffer;
		*buffer = NULL;
		return (aux);
	}
	if (*buffer)
		free(*buffer);
	*buffer = NULL;
	return (NULL);
}

char	*set_buffer(int fd, char **buffer, char *buffer_read)
{
	ssize_t	i_readed;
	char	*aux;

	while (1)
	{
		aux = is_line_break(buffer);
		if (aux)
			free(buffer_read);
		if (aux)
			return (aux);
		i_readed = read(fd, buffer_read, BUFFER_SIZE);
		if (i_readed <= 0)
			return (check_readed(i_readed, buffer, buffer_read));
		buffer_read[i_readed] = '\0';
		aux = ft_strjoin(*buffer, buffer_read);
		if (!aux)
		{
			free(buffer_read);
			free(*buffer);
			*buffer = NULL;
			return (NULL);
		}
		free(*buffer);
		*buffer = aux;
	}
}

char	*get_next_line(int fd)
{
	static char	*buffer = NULL;
	char		*buffer_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer)
	{
		buffer = ft_strdup("");
		if (!buffer)
			return (NULL);
	}
	buffer_read = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer_read)
		return (NULL);
	return (set_buffer(fd, &buffer, buffer_read));
}
