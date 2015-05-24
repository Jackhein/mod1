/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmartin <mmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/26 13:01:37 by mmartin           #+#    #+#             */
/*   Updated: 2015/05/18 17:05:25 by mmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOD1_H
# define MOD1_H

# include <string.h>

typedef struct		s_map
{
	unsigned int	x;
	unsigned int	y;
	unsigned int	z;
}					t_map;

typedef struct		s_liquid
{
	float			height;
	float			speed;
	float			dir;
	int				type;
}					t_liquid;

typedef struct		s_color
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}					t_color;

typedef struct		s_data
{
	char			*data;
	float			**earth;
	t_liquid		**water;
	size_t			x;
	size_t			y;
	size_t			xmax;
	size_t			ymax;
	int				sizeline;
	size_t			width;
}					t_data;

void				*ft_init_pthread(void *p_data);

#endif
