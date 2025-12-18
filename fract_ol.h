/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fract-ol.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkulivar <tkulivar@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-06 10:51:15 by tkulivar          #+#    #+#             */
/*   Updated: 2025-12-06 10:51:15 by tkulivar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACT_OL_H
# define FRACT_OL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <math.h>
# include "MLX42.h"
# include "libft.h"

# define MAX_ITERATIONS 40
# define HEIGHT 500
# define WIDTH 500
# define ZOOM 2
# define X_OFFSET - 0.0f
# define Y_OFFSET - 0.0f
# define PARSE_SUCCESS 0
# define PARSE_FAILURE 1

typedef struct s_complex
{
	double	real;
	double	imag;
}	t_complex;

typedef struct s_fractal
{
	mlx_t			*mlx;
	mlx_image_t		*image;
	double			zoom;
	t_complex		c_param;
	int				power;
	int				fractal_type;
	int				needs_redraw;
}	t_fractal;

// parsing_utils.c
void			print_usage(void);
int				is_valid_power(char *str);
int				is_valid_complex_value(double value);

// parsing.c
int				setup_julia(int argc, char **argv, t_fractal *fractal);
int				setup_mandelbrot(int argc, char **argv, t_fractal *fractal);
int				parse_arguments(int argc, char **argv, t_fractal *fractal);

// complex.c
double			get_magnitude_squared(t_complex num);
t_complex		complex_num_power(t_complex num, int power);
t_complex		complex_num_sum(t_complex num1, t_complex num2);

// fractal.c
int				check_explodes(t_complex c, t_complex z, int power);
t_complex		map_pixel_to_complex(int x, int y, double zoom);
int				get_rgba(int r, int g, int b, int a);
int				get_color_from_iterations(int iterations);

// main.c
void			draw_julia(t_fractal *fractal);
void			draw_mandelbrot(t_fractal *fractal);
int				setup_fractal(t_fractal *fractal);

// hooks.c
void			scroll_hook(double xdelta, double ydelta, void *param);
void			key_hook(mlx_key_data_t keydata, void *param);
void			loop_hook(void *param);

#endif
