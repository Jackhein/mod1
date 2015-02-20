/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmartin <mmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/28 13:45:14 by mmartin           #+#    #+#             */
/*   Updated: 2015/02/16 14:30:43 by mmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include "map.h"
#include "Map.class.hpp"
#include "GraphicalDisplay.class.hpp"

void	ft_get_coord(char *str, std::list<t_map> *map)
{
	char	*tmp;
	int		i;
	t_map	coord;

	i = 0;
	tmp = strtok(str, ",");
	while (tmp)
	{
		if (i == 0)
			coord.x = strtoul(tmp, NULL, 10);
		else if (i == 1)
			coord.y = strtoul(tmp, NULL, 10);
		else
			coord.z = strtoul(tmp, NULL, 10);
		i++;
		tmp = strtok(NULL, ",");
	}
	if (i == 3)
		map->push_back(coord);
}

void	ft_parse(char *argv, std::list<t_map> *map)
{
	std::ifstream	ifs(argv);
	std::string		line;
	char			*str;
	char			*ptr;

	while (getline(ifs, line))
	{
		if (!line.length())
			continue ;
		str = strtok_r(const_cast<char *>(line.c_str()), " ()", &ptr);
		while (str)
		{
			ft_get_coord(str, map);
			str = strtok_r(NULL, " ()", &ptr);
		}
	}
}

int		main(int argc, char **argv)
{
	std::list<t_map>	mapHill;

	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " file" << std::endl;
		return (1);
	}
	ft_parse(argv[1], &mapHill);

	std::list<t_map>::iterator			it;
	std::list<t_map>::const_iterator	ite = mapHill.end();

	GraphicalDisplay		gd(1000, 1000);

	gd.setMap(mapHill);
	gd.setWater();

	gd.run();

	return (0);
}
