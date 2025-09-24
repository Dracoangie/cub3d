/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:40:47 by kpineda-          #+#    #+#             */
/*   Updated: 2025/09/23 22:09:30 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline int get_texel(const t_img *tex, int x, int y)
{
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x >= tex->width)
		x = tex->width - 1;
	if (y >= tex->height)
		y = tex->height - 1;

	char *p = tex->img_pixels_ptr + y * tex->line_len + x * (tex->bits_per_pixel / 8);
	return *(int *)p;
}

#define TILE 20 // tamaño real del tile del mapa

static inline int is_vertical_hit_eps(t_point hit, double ray_dir_x, double ray_dir_y)
{
	double tile = (double)TILE;

	double fx = fmod((double)hit.x, tile);
	if (fx < 0)
		fx += tile;
	double fy = fmod((double)hit.y, tile);
	if (fy < 0)
		fy += tile;

	double dx = fmin(fx, tile - fx); // distancia a la línea vertical más cercana
	double dy = fmin(fy, tile - fy); // distancia a la línea horizontal más cercana

	const double eps = 1e-4 * tile; // tolerancia para bordes (ajústala si hace falta)

	if (dx + eps < dy)
		return 1; // claramente vertical
	if (dy + eps < dx)
		return 0; // claramente horizontal

	// Empate (muy cerca de una línea o esquina): decide por la dirección del rayo
	// Si el rayo “avanza” más en X que en Y, trata como vertical, y viceversa.
	return fabs(ray_dir_x) >= fabs(ray_dir_y);
}

double ft_degree_to_radian(double degrees)
{
	return (degrees * (3.1415 / 180.0));
}

void render_vline(t_data *data, t_point begin, t_point end, int color)
{

	int y0 = (int)(begin.y + 0.5);
	int y1 = (int)(end.y + 0.5);
	int x = (int)(begin.x + 0.5);
	int y = y0;

	if (x < 0 || x >= WIDTH)
		return;

	if (y0 > y1)
	{
		int t = y0;
		y0 = y1;
		y1 = t;
	}
	if (y1 < 0 || y0 >= HEIGHT)
		return;
	if (y0 < 0)
		y0 = 0;
	if (y1 >= HEIGHT)
		y1 = HEIGHT - 1;
	y = y0;
	while (y <= y1)
	{
		put_pixel(data, x, y, color);
		++y;
	}
}

void render_line(t_data *data, t_point begin, t_point end, int color)
{
	double deltaX = end.x - begin.x;
	double deltaY = end.y - begin.y;
	int pixels = sqrt((deltaX * deltaX) + (deltaY * deltaY));
	deltaX /= pixels;
	deltaY /= pixels;

	double pixelX = begin.x;
	double pixelY = begin.y;
	while (pixels)
	{
		if (!(pixelX <= WIDTH && pixelY <= HEIGHT) ||
			!(pixelX >= 0 && pixelY >= 0))
			break;
		put_pixel(data, pixelX, pixelY, color);
		pixelX += deltaX;
		pixelY += deltaY;
		--pixels;
	}
}
void render_textured_column(t_data *data, int col, int y0, int y1, t_point hit, double ray_angle)
{
	if (y0 > y1)
	{
		int tmp = y0;
		y0 = y1;
		y1 = tmp;
	}
	if (y1 < 0 || y0 >= HEIGHT)
		return;
	if (y0 < 0)
		y0 = 0;
	if (y1 >= HEIGHT)
		y1 = HEIGHT - 1;

	const int slice_h = (y1 - y0) + 1;
	if (slice_h <= 0)
		return;

	// Dirección del rayo (coincide con tu cálculo)
	const double ray_dir_x = sin(ray_angle);
	const double ray_dir_y = cos(ray_angle);

	// ¿impacto vertical u horizontal?
	const int vertical = is_vertical_hit_eps(hit, ray_dir_x, ray_dir_y);

	// Coordenada u dentro del tile [0,1)
	const double tile = (double)TILE;
	double u = vertical
				   ? fmod((double)hit.y, tile) / tile
				   : fmod((double)hit.x, tile) / tile;

	if (u < 0.0)
		u = 0.0;
	if (u >= 1.0)
		u = 0.999999;

	// Corrección de 'flip' para evitar espejado según la cara golpeada
	if ((vertical && ray_dir_x > 0.0) || (!vertical && ray_dir_y < 0.0))
		u = 1.0 - u;

	// X en la textura (fijo para toda la columna)
	int tex_x = (int)(u * (double)data->tex_wall.width);
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= data->tex_wall.width)
		tex_x = data->tex_wall.width - 1;

	// Recorremos la columna en pantalla y mapeamos Y -> tex_y
	for (int y = y0; y <= y1; ++y)
	{
		double t = (double)(y - y0) / (double)slice_h; // [0..1]
		int tex_y = (int)(t * (double)data->tex_wall.height);
		if (tex_y >= data->tex_wall.height)
			tex_y = data->tex_wall.height - 1;

		int color = get_texel(&data->tex_wall, tex_x, tex_y);

		// sombreado leve si impacto vertical (opcional)
		if (vertical)
		{
			unsigned int a = (color >> 24) & 0xFF;
			unsigned int r = (color >> 16) & 0xFF;
			unsigned int g = (color >> 8) & 0xFF;
			unsigned int b = (color) & 0xFF;
			r = (r * 200) / 255;
			g = (g * 200) / 255;
			b = (b * 200) / 255;
			color = (a << 24) | (r << 16) | (g << 8) | b;
		}

		put_pixel(data, col, y, color);
	}
}

void render_3d_column(t_data *data, t_point hit, double ray_angle, int col)
{
	double player_pos[2] = {data->player.x + data->player.scale,
							data->player.y + data->player.scale};

	double distance_to_wall = hypot(hit.x - player_pos[0], hit.y - player_pos[1]);

	double ray_dir[2] = {sin(ray_angle), cos(ray_angle)};
	double player_dir[2] = {sin(ft_degree_to_radian(data->player.rotation)),
							cos(ft_degree_to_radian(data->player.rotation))};

	double cos_angle_difference = ray_dir[0] * player_dir[0] + ray_dir[1] * player_dir[1];
	if (cos_angle_difference < 0.0001)
		cos_angle_difference = 0.0001;

	double projection_plane = (double)WIDTH / (2.0 * tan(3.1415 / 6.0));
	int wall_height = (int)((20.0 * projection_plane) / (distance_to_wall * cos_angle_difference) + 0.5);

	int wall_y[2] = {
		(int)ft_clamp(HEIGHT / 2 - wall_height / 2, 0, HEIGHT - 1),
		(int)ft_clamp(HEIGHT / 2 + wall_height / 2, 0, HEIGHT - 1)};

	if (wall_y[0] > 0)
	{
		render_vline(data, (t_point){col, 0}, (t_point){col, wall_y[0] - 1}, RGB(data->color[0].red, data->color[0].green, data->color[0].blue));
	}
	render_textured_column(data, col, wall_y[0], wall_y[1], hit, ray_angle);
	// render_vline(data, (t_point){col, wall_y[0]}, (t_point){col, wall_y[1]}, GRAY);
	if (wall_y[1] < HEIGHT - 1)
		render_vline(data, (t_point){col, wall_y[1] + 1}, (t_point){col, HEIGHT - 1}, RGB(data->color[1].red, data->color[1].green, data->color[1].blue));
}

void render_2d_vision(t_data *data)
{
	int i = 0;
	double angle;
	double auxangle = ft_degree_to_radian(data->player.rotation - 30);
	int x;
	int y;
	t_point intersection;
	t_point intersectionaux;

	while (i < WIDTH)
	{
		x = 0;
		y = 0;
		angle = ((ft_degree_to_radian(60) * i) / WIDTH) + auxangle;
		int angle_x = (int)(sin(angle) * 800) + data->player.x + data->player.scale;
		int angle_y = (int)(cos(angle) * 800) + data->player.y + data->player.scale;
		intersectionaux = (t_point){angle_x, angle_y};
		while (data->map.map[y])
		{
			while (data->map.map[y][x])
			{
				if (data->map.map[y][x] == '1')
				{
					if (col_squareline((t_rect){x * 20, y * 20, 20, 20, RED}, (t_line){(t_point){data->player.x + data->player.scale, data->player.y + data->player.scale}, (t_point){angle_x, angle_y}}, &intersection, data))
					{
						double intersectiondist = pow(intersection.x - data->player.x, 2) + pow(intersection.y - data->player.y, 2);
						double auxdist = pow(intersectionaux.x - data->player.x, 2) + pow(intersectionaux.y - data->player.y, 2);
						if (intersectiondist < auxdist)
							intersectionaux = (t_point){intersection.x, intersection.y};
					}
				}
				x++;
			}
			x = 0;
			y++;
		}
		render_3d_column(data, intersectionaux, angle, WIDTH - 1 - i);
		i++;
	}
}

void render_rect(t_data *data, int x, int y, int height, int width, int color)
{
	int i = 0;
	int j = 0;

	while (i < width)
	{
		while (j < height)
		{
			put_pixel(data, i + x, j + y, color);
			j++;
		}
		j = 0;
		i++;
	}
}

int render_circle(t_data *data, int x, int y, int radius)
{
	int dist, i, j;
	i = 0;
	{
		while (i <= radius * 2)
		{
			j = 0;
			while (j <= radius * 2)
			{
				dist = sqrt((i - radius) * (i - radius) + (j - radius) * (j - radius));
				{
					if (dist <= radius)
						put_pixel(data, i + x, j + y, CYAN);
				}
				j++;
			}
			i++;
		}
	}
	return 0;
}

void render_minimap_simple(t_data *data)
{
	int tile_scale = 20;
	double tile_px_d = floor(fmin(200 / (double)data->map.cols,
								  200 / (double)data->map.rows));
	if (tile_px_d < 1.0)
		tile_px_d = 1.0;
	int tile_px = (int)tile_px_d;
	int width_map = tile_px * data->map.cols;
	int height_map = tile_px * data->map.rows;
	int aux_a = width_map / data->map.cols;
	int aux_b = height_map / data->map.rows;

	if (aux_a < 1)
		aux_a = 1;
	if (aux_b < 1)
		aux_b = 1;

	int i = 0;
	while (i < data->map.rows)
	{
		size_t rowlen = ft_strlen(data->map.map[i]);
		int j = 0;
		while (j < (int)rowlen && j < data->map.cols)
		{
			char c = data->map.map[i][j];
			int color = DARK_GRAY;
			if (c == '1')
				color = DARK_GRAY;
			else if (c == '0')
				color = GRAY;

			int rx = j * aux_a;
			int ry = i * aux_b;
			render_rect(data, rx, ry, aux_b, aux_a, color);

			j++;
		}
		i++;
	}

	double world_w = data->map.cols * tile_scale;
	double world_h = data->map.rows * tile_scale;
	if (world_w > 0.0 && world_h > 0.0)
	{
		int px = (int)((data->player.x / world_w) * width_map + 0.5);
		int py = (int)((data->player.y / world_h) * height_map + 0.5);
		render_rect(data, px - 2, py - 2, 4, 4, BLUE);
	}
}

void draw(t_data *data)
{
	/*int i = 0;
	int j = 0;
	int x = 0;
	int y = 0;*/

	// render_rect(data, 0, 0, 800, 800, BLACK);

	render_2d_vision(data);
	/*while (i < data->map.rows)
	{
		while (j < data->map.cols)
		{
			if (data->map.map[i][j] == '1')
				render_rect(data, x, y, 20, 20, BLACK);
			else if (data->map.map[i][j] == '0')
				render_rect(data, x, y, 20, 20, GRAY);
			else if (data->map.map[i][j] == ' ')
				render_rect(data, x, y, 20, 20, BLACK);
			else
			{
				render_rect(data, x, y, 20, 20, BLACK);
				j = data->map.cols;
			}
			x += 20;
			j++;
		}
		x = 0;
		y += 20;
		j = 0;
		i++;
	}
	render_rect(data, data->player.x, data->player.y, data->player.scale, data->player.scale, BLUE);*/
	render_minimap_simple(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}

void put_pixel(t_data *data, int x, int y, int color)
{
	char *pixel;

	pixel = data->img.img_pixels_ptr + (x * 4) + (y * data->img.line_len);
	*(int *)pixel = color;
}
