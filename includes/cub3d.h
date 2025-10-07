/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angnavar <angnavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:56:31 by angnavar          #+#    #+#             */
/*   Updated: 2025/10/07 17:48:11 by angnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../includes/libft/libft.h"
# include "../includes/minilibx-linux/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <fcntl.h>
# include <math.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define BUFFER_SIZE 1000000
# define WIDTH 1200
# define HEIGHT 800
# define TILE 20
# define RED 0xFF0000
# define GREEN 0x00FF00
# define BLUE 0x0000FF
# define BLACK 0x000000
# define D_GRAY 0x404040
# define GRAY 0x808080
# define WHITE 0xFFFFFF
# define CYAN 0x007080

typedef enum e_face
{
	TEX_N = 0,
	TEX_S = 1,
	TEX_E = 2,
	TEX_W = 3
}				t_face;

/* Structs */
typedef struct s_img
{
	void		*img_ptr;
	char		*img_pixels_ptr;
	int			bits_per_pixel;
	int			endian;
	int			line_len;
	int			width;
	int			height;
}				t_img;

typedef struct s_player
{
	int			x;
	int			y;
	int			scale;
	int			rotation;
}				t_player;

typedef struct s_map
{
	int			rows;
	int			cols;
	char		**map;
}				t_map;

typedef struct s_file
{
	int			ls;
	char		**file;
}				t_file;

typedef struct s_color
{
	int			red;
	int			green;
	int			blue;
}				t_color;

typedef struct s_data
{
	void		*mlx;
	void		*win;
	t_img		img;
	t_map		map;
	t_player	player;
	t_file		file;
	t_color		color[2];
	t_img		tex[4];
}				t_data;

typedef struct s_point
{
	int			x;
	int			y;
}				t_point;

typedef struct s_line
{
	t_point		start;
	t_point		end;
}				t_line;

typedef struct s_rect
{
	int			x;
	int			y;
	int			width;
	int			height;
	int			color;
}				t_rect;

typedef struct s_col_state
{
	int			hit;
	double		closedist;
	t_point		iaux;
	int			i;
	t_point		r[4];
	double		auxdist;
}				t_col_state;

typedef struct s_column_params
{
	int			col;
	int			y0;
	int			y1;
	t_point		hit;
	double		ray_angle;
}				t_column_params;

typedef struct s_tex_params
{
	t_data		*data;
	t_point		hit;
	double		ray_dir_x;
	double		ray_dir_y;
	int			vertical;
	int			tex_x_val;
	t_face		face_val;
	int			*tex_x;
	t_face		*face;
	t_img		*tex;
}				t_tex_params;

// collisions
int				col_squaresquare(t_rect rect, t_rect rect2);
int				col_squareline(t_rect rect, t_line line, t_point *intersection,
					t_data *data);
int				col_lineline(t_line l1, t_line l2, t_point *intersection);

// utils
int				ft_chrcmp(char *str, char *str2);
void			ft_free_matrix(char **matrix);
void			ft_dup_matrix(char **cpy, char **dest);
double			ft_clamp(double value, double min_value, double max_value);

// render basics
void			render_vline(t_data *data, t_point begin, t_point end,
					int color);
void			render_rect(t_data *data, t_point point, t_point size,
					int color);
int				render_circle(t_data *data, int x, int y, int radius);

// render
void			player_move(t_data *data, int dir, int spd);
void			render_2d_vision(t_data *data);
void			render_3d_column(t_data *data, t_point hit, double ray_angle,
					int col);
void			render_textured_column(t_data *data, t_column_params p);
void			draw(t_data *data);

//render utils
void			put_pixel(t_data *data, int x, int y, int color);
int				get_texel(const t_img *tex, int x, int y);
int				is_vertical_hit_eps(t_point hit, double ray_dir_x,
					double ray_dir_y);
double			ft_d_r(double degrees);

//checker
int				check_coords(t_data *data, t_point *point, char coor,
					char post_coor);
int				check_floor_ceiling(t_data *data, t_point *point, char c,
					int i);
int				coma_case(t_data *data, t_point *point, int len, int i);

// parse_file
int				set_player(t_data *data, int i);
int				read_file(t_data *data, char *name);
int				set_map(t_data *data);
int				parse_file_textures(t_data *data);
int				clean_matrix(t_data *data, int i);
int				parse_coords(t_data *data);

// parse map
int				check_around(int i, int j, t_data *data);
int				check_map(t_data *data);

// exits
void			exit_error(t_data *data);
void			exit_program(t_data *data);

// gnl
char			*is_line_break(char **buffer);
char			*check_readed(ssize_t i_readed, char **buffer,
					char *buffer_read);
char			*set_buffer(int fd, char **buffer, char *buffer_read);
char			*get_next_line(int fd);

// gnl utils
size_t			ft_strlen(const char *s);
char			*ft_substr(const char *str, unsigned int start, size_t len);
char			*ft_strdup(const char *s);
char			*ft_strjoin(const char *s1, const char *s2);
char			*ft_strchr(const char *s, int c);

#endif