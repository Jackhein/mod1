/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.class.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/27 17:44:25 by tbalea            #+#    #+#             */
/*   Updated: 2015/05/18 17:05:35 by mmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_CLASS_HPP
# define MAP_CLASS_HPP

# include <cmath>
# include <iostream>
# include <list>
# include <string>
# include "mod1.h"

class Map {
	public:
		//Functions
		//	Constructor
		Map( void );
		Map( unsigned int sizeX, unsigned int sizeY );

		//	Copy
		Map( const Map & map );
		Map & operator=( const Map & map );

		//	Destructor
		~Map( void );

		//	Setter
		void setMapHill( std::list<t_map> mapHill );
		void setMap( void );
		void setMapHeight( void );

		//	Getter
		std::list<t_map> getMapHill( void ) const;
		float** getMap( void ) const;

		//	Function
		//void RandomMap( void );
	private:
		//Functions
		//	Calcul Map
		void HeightHill( void );
//		void HeightLink( void );
		void HeightNorm( void );

		//	Clear
		void ClearMapHill( void );
		void ClearMapHeight( void );
		void ClearMap( void );

		//Attribute
		//	Size of map
		unsigned int _sizeX;
		unsigned int _sizeY;

		//	Map
		float ** _map;

		//	List of hills;
		std::list<t_map> _mapHill;
};

#endif
