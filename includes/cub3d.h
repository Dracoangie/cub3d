/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:56:31 by angnavar          #+#    #+#             */
/*   Updated: 2025/06/30 21:04:16 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../includes/libft/libft.h"
# include "../includes/minilibx-linux/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>

# define WIDTH 500
# define HEIGHT 500
# define RED 0xFF0000

/* Structs */
typedef struct s_img
{
	void	*img_ptr;
	char	*img_pixels_ptr;
	int		bits_per_pixel;
	int		endian;
	int		line_len;
}			t_img;

typedef struct s_data
{
	void	*mlx;
	void	*win;
	t_img	img;
}			t_data;

typedef struct s_rect
{
	int	x;
	int	y;
	int width;
	int height;
	int color;
}	t_rect;

#endif