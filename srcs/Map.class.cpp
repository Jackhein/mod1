/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.class.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/26 18:46:28 by tbalea            #+#    #+#             */
/*   Updated: 2015/02/14 22:11:48 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Map.class.hpp"

//	To Do :
//		Add Exeption
//		Flattern and link hill

/* ************************************************************************** */
// Public Functions:

//	Constructor:
Map::Map( void ) : _sizeX(1000), _sizeY(1000) {
	this->setMap();
}

Map::Map( unsigned int sizeX, unsigned int sizeY ) : _sizeX(sizeX), _sizeY(sizeY) {
	this->setMap();
}


//	Copy:
Map::Map( const Map & map ) {
	*this = map;
}

Map & Map::operator=( const Map & map ) {
	if ( this != & map ) {
		this->ClearMapHill();
		this->ClearMapHeight();
		this->ClearMap();
		this->_sizeX = map._sizeX;
		this->_sizeY = map._sizeY;
		this->setMap();
		this->setMapHill( map.getMapHill() );
		this->setMapHeight();
	}
	return *this;
}


//	Destructor
Map::~Map( void ) {
	this->ClearMapHill();
	this->ClearMap();
}


//	Setter
void Map::setMapHill( std::list<t_map> mapHill ) {
	this->_mapHill = mapHill;

	std::list<t_map>::iterator i;
	std::list<t_map>::iterator ite = this->_mapHill.end();

	i = this->_mapHill.begin();
	for (i = this->_mapHill.begin(); i != ite; i++)
	{
		if ( (*i).x > this->_sizeX || (*i).y > this->_sizeY ) {
			this->_mapHill.erase(i);
		}
	}
	this->setMapHeight();
}

void Map::setMap( void ) {
	_map = new float*[this->_sizeX];
	for ( unsigned int i = 0; i < this->_sizeX; i++ ) {
		_map[i] = new float[this->_sizeY];
		for (unsigned int j = 0; j < this->_sizeY; j++)
			_map[i][j] = 0;
	}
}

void Map::setMapHeight( void ) {
	this->HeightHill();
//	this->HeightLink();
	this->HeightNorm();
}


//	Getter
std::list<t_map> Map::getMapHill( void ) const {
	return this->_mapHill;
}

float** Map::getMap( void ) const {
	return this->_map;
}


//	Function
//	void Map::RandomMap( void );

/* ************************************************************************** */
// Private Functions:

//	Calcul Map
//		Add  Hills
void Map::HeightHill( void ) {
	//	Iterator to check all Hills
	std::list<t_map>::iterator i = this->_mapHill.begin();
	std::list<t_map>::iterator ite = this->_mapHill.end();
	std::list<t_map>::iterator j;

	//	X radius attribute
	unsigned int xMin;
	unsigned int yMin;

	//	Y radius attribute
	unsigned int xMax;
	unsigned int yMax;

	//	Height data;
	float zHill;
	float z;
	float dist;

	while ( i != ite ) {
		zHill = sqrt((*i).z);

		//	Init X radius
		if ( (*i).x < zHill + 1 )
			xMin = 0;
		else
			xMin = (*i).x - zHill - 1;
		if ( (xMax = (*i).x + zHill + 1) >= this->_sizeX )
			xMax = this->_sizeX - 1;

		//	Init Y radius
		if ( (*i).y < zHill + 1 )
			yMin = 0;
		else
			yMin = (*i).y - zHill  - 1;
		if ( (yMax = (*i).y + zHill  + 1) >= this->_sizeY )
			yMax = this->_sizeY - 1;

		//	Seek all affected point
		for ( unsigned int x = xMin; x <= xMax; ++x) {
			for ( unsigned int y = yMin; y <= yMax; ++y) {

				//	Seek distance from center of hill
				dist = ( (*i).x - x ) * ( (*i).x - x ) + ( (*i).y - y ) * ( (*i).y - y );
				//	Determine height for this point for THIS Hill
				if ( (z = (*i).z - dist) < 0 )
					z = 0;

				//	Check correct value of Hill
				if ( z > this->_map[x][y] )
				{
					//	Seek conflict with other Hill
					j = this->_mapHill.begin();
					while ( j != ite ) {
						//	Don't compare for himself
						if ( j == i && ++j == ite)
							break ;
						dist = ( (*j).x - x ) * ( (*j).x - x ) + ( (*j).y - y ) * ( (*j).y - y );
						if ( (dist + (*j).z) < z )
							z = dist + (*j).z;
						j++;
					}
					this->_map[x][y] = z;
				}
			}
		}
		i++;
	}
}
/*
//		Link Hills
void Map::HeightLink( void ) {
	std::map< unsigned int, std::list >::iterator i = this->_mapHill.begin();
	std::list< unsigned int >::iterator j;
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int d;
	unsigned int dmax;

	while ( i != this->_mapHill.end() ) {
		j = *i; //take list
		//	use : z = ((1 + sin( ((d * pi)/(2 * dmax)) - (pi / 2)) )/ 2) * (zmax - zmin) + zmin;
	}
}
*/
//		Normalize
void Map::HeightNorm( void ) {
	unsigned int minZ = this->_map[0][0];
	unsigned int maxZ = this->_map[0][0];

	//	Serach min and max z
	for ( unsigned int x = 0; x < this->_sizeX; ++x) {
		for ( unsigned int y = 0; y < this->_sizeY; ++y) {
			if ( minZ > this->_map[x][y])
				minZ = this->_map[x][y];
			if ( maxZ < this->_map[x][y])
				maxZ = this->_map[x][y];
		}
	}
	//	Force normalize if z value to low
	if ( maxZ < 15000 )
		maxZ = 15000;
	//	Normalize if it's not flat
	if ( maxZ != minZ ) {
		for ( unsigned int x = 0; x < this->_sizeX; ++x) {
			for ( unsigned int y = 0; y < this->_sizeY; ++y) {
				this->_map[x][y] = (this->_map[x][y] - minZ) / (maxZ - minZ);
			}
		}
	} else
		this->ClearMapHill();
	//	Flattern
}

//	Clear list of Hills
void Map::ClearMapHill( void ) {
	this->_mapHill.clear();
}

//	Clear height value of map
void Map::ClearMapHeight( void ) {
	for ( unsigned int x = 0; x < this->_sizeX; x++ ) {
		for ( unsigned int y = 0; y < this->_sizeY; y++ ) {
			this->_map[x][y] = 0;
		}
	}
}

//	Clear map
void Map::ClearMap( void ) {
	for ( unsigned int i = 0; i < _sizeX; i++ )
		delete [] this->_map[i];
	delete [] this->_map;
}
