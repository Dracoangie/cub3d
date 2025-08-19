/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:41:39 by kpineda-          #+#    #+#             */
/*   Updated: 2025/08/11 16:55:25 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void ft_dup_matrix(char **cpy, char **dest)
{
	int i = 0;

	while (cpy[i])
	{
		dest[i] = cpy[i];
		i++;
	}
}

int ft_chrcmp(char *str, char *str2)
{
	int i = 0;
	int j = 0;
	while (str2[j])
	{
		if (str[i] == str2[j])
		{
			j = 0;
			i++;
			continue;
		}
		j++;
	}
	if (str[i] == '\0')
		return (1);
	else
		return (0);
}

void ft_free_matrix(char **matrix)
{
	int i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}
