/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:56:31 by angnavar          #+#    #+#             */
/*   Updated: 2025/09/23 21:47:25 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

#include "../includes/libft/libft.h"
#include "../includes/minilibx-linux/mlx.h"
#include <X11/X.h>
#include <X11/keysym.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

#define BUFFER_SIZE 1000000
#define WIDTH 800
#define HEIGHT 800
#define TILE 20
#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define BLACK 0x000000
#define DARK_GRAY 0x404040
#define GRAY 0x808080
#define WHITE 0xFFFFFF
#define CYAN 0x007080

/* Structs */
typedef struct s_img
{
	void *img_ptr;
	char *img_pixels_ptr;
	int bits_per_pixel;
	int endian;
	int line_len;
	int width;
	int height;
} t_img;

typedef struct s_player
{
	int x;
	int y;
	int scale;
	int rotation;
} t_player;

typedef struct s_map
{
	int rows;
	int cols;
	char **map;
} t_map;

typedef struct s_file
{
	int file_lines;
	char **file;
} t_file;

typedef struct s_color
{
	int red;
	int green;
	int blue;
} t_color;

typedef struct s_data
{
	void *mlx;
	void *win;
	t_img img;
	t_map map;
	t_player player;
	t_file file;
	t_color color[2];
	t_img tex_wall;
} t_data;

typedef struct s_point
{
	int x;
	int y;
} t_point;

typedef struct s_line
{
	t_point start;
	t_point end;
} t_line;

typedef struct s_rect
{
	int x;
	int y;
	int width;
	int height;
	int color;
} t_rect;

// collisions
int col_squaresquare(t_rect rect, t_rect rect2);
int col_squareline(t_rect rect, t_line line, t_point *intersection, t_data *data);
int col_lineLine(t_point point1, t_point point2, t_point point3, t_point point4, t_point *intersection);

// utils
int ft_chrcmp(char *str, char *str2);
void ft_free_matrix(char **matrix);
void ft_dup_matrix(char **cpy, char **dest);
double ft_degree_to_radian(double degrees);
int ft_strcmp(const char *s1, char **s2, size_t n, int i, int j);
double ft_clamp(double value, double min_value, double max_value);

// render
void render_vline(t_data *data, t_point begin, t_point end, int color);
void render_rect(t_data *data, int x, int y, int height, int width, int color);
void draw(t_data *data);
void put_pixel(t_data *data, int x, int y, int color);

// parse_file
int read_file(t_data *data, char *name);
int set_map(t_data *data);
int parse_file_textures(t_data *data);

// parse map
int check_around(int i, int j, t_data *data);
int check_map(t_data *data);

// exits
void exit_error(t_data *data);
void exit_program(t_data *data);

// gnl
char *is_line_break(char **buffer);
char *check_readed(ssize_t i_readed, char **buffer, char *buffer_read);
char *set_buffer(int fd, char **buffer, char *buffer_read);
char *get_next_line(int fd);

// gnl utils
size_t ft_strlen(const char *s);
char *ft_substr(const char *str, unsigned int start, size_t len);
char *ft_strdup(const char *s);
char *ft_strjoin(const char *s1, const char *s2);
char *ft_strchr(const char *s, int c);

#endif