/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:32:40 by angnavar          #+#    #+#             */
/*   Updated: 2025/08/21 16:15:47 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <unistd.h>
#include <stdio.h>

int ft_atoi(const char *nptr)
{
	int sign;
	int result;

	sign = 1;
	result = 0;
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 12))
		nptr++;
	if (*nptr == '\0')
		return (256);
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + (*nptr - 48);
		nptr++;
	}
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 12))
		nptr++;
	if (*nptr != '\0')
		return (256);
	return (result * sign);
}
