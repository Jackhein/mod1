/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scenario.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/13 11:18:05 by tbalea            #+#    #+#             */
/*   Updated: 2015/05/22 12:02:52 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scenario.class.hpp"

/* ************************************************************************** */
//	Public Functions:

//	Constructor
Scenario::Scenario(float ** Map, unsigned int sizeX, unsigned int sizeY, int type) :
	Liquid(Map, sizeX, sizeY, type)
{
}

//	Destructor
Scenario::~Scenario(void)
{
}

//	Scenario
void Scenario::Rainy(void)
{
	//	Give speed of flowing
	float			rainy = 0.01;

	//	Give density (lowest is the number, higher is the density)
	unsigned int	density = 1000;
	unsigned int	precipitate = (_sizeX * _sizeY) / density;
	unsigned int	x;
	unsigned int	y;

	srand (time(NULL));
	for (unsigned int rain = 0; rain < precipitate; rain++)
	{
		x = rand() % _sizeX;
		y = rand() % _sizeY;
		if (_Map[x][y] + _CurMap[x][y].height + rainy < 1)
		{
			_CurMap[x][y].height += rainy;

			//	Give random direction
//			if (_CurMap[x][y].height == rainy)
//				_CurMap[x][y].dir = ((rand() + 1.0) * g_tab_ppi[8]) / ((1.0 + RAND_MAX) * g_tab_pi[8]);
		}
	}
}

void Scenario::Waves(bool n, bool s, bool e, bool w)
{
	//	Give speed of flowing
	float	waves = 0.01;

	//	Check lowest hight
	float	zmin = WavesMin(n, s, e, w) + waves + 0.06;

	//	Increase hight of wave
	//		North wave
	if (n)
	{
		for (unsigned int x = 0; x < _sizeX; x++)
		{
			if (_CurMap[x][0].height + _Map[x][0] <= zmin)
				_CurMap[x][0].height += waves;
			if (_CurMap[x][0].height + _Map[x][0] > 1)
				_CurMap[x][0].height = 1 - _Map[x][0];
			_CurMap[x][0].dir = g_tab_pi[5];
		}
	}
	//		West wave
	if (w)
	{
		for (unsigned int y = 0; y < _sizeY; y++)
		{
			if (_CurMap[0][y].height + _Map[0][y] <= zmin)
				_CurMap[0][y].height += waves;
			if (_CurMap[0][y].height + _Map[0][y] > 1)
				_CurMap[0][y].height = 1 - _Map[0][y];
			_CurMap[_sizeX-1][y].dir = g_tab_pi[7];
		}
	}
	//		South wave
	if (s)
	{
		for (unsigned int x = 0; x < _sizeX; x++)
		{
			if (_CurMap[x][_sizeY-1].height + _Map[x][_sizeY-1] <= zmin)
				_CurMap[x][_sizeY-1].height += waves;
			if (_CurMap[x][_sizeY-1].height + _Map[x][_sizeY-1] > 1)
				_CurMap[x][_sizeY-1].height = 1 - _Map[x][_sizeY-1];
			_CurMap[x][_sizeY-1].dir = g_tab_pi[1];
		}
	}
	//		Est wave
	if (e)
	{
		for (unsigned int y = 0; y < _sizeY; y++)
		{
			if (_CurMap[_sizeX-1][y].height + _Map[_sizeX-1][y] <= zmin)
				_CurMap[_sizeX-1][y].height += waves;
			if (_CurMap[_sizeX-1][y].height + _Map[_sizeX-1][y] > 1)
				_CurMap[_sizeX-1][y].height = 1 - _Map[_sizeX-1][y];
			_CurMap[0][y].dir = g_tab_pi[3];
		}
	}
}

void Scenario::Flood(void)
{
	//	Give speed of flowing
	float	flood = 0.01;
	float	zmin = _Map[0][0] + _CurMap[0][0].height;

	//	Check lowest point of map
	if (zmin > 0)
	{
		for (unsigned int x = 0; x < _sizeX && zmin > 0; x++)
		{
			for (unsigned int y = 0; y < _sizeY && zmin > 0; y++)
			{
				if (_CurMap[x][y].height + _Map[x][y] < zmin)
				{
					zmin = _Map[x][y] + _CurMap[x][y].height;
				}
			}
		}
	}
	zmin += flood;
	if (zmin > 1)
		zmin = 1;

	//	Add flood
	for (unsigned int x = 0; x < _sizeX; x++)
	{
		for (unsigned int y = 0; y < _sizeY; y++)
		{
			if (_Map[x][y] + _CurMap[x][y].height < zmin)
				_CurMap[x][y].height = zmin - _Map[x][y];
			/*if (_CurMap[x][y].height <= flood && _CurMap[x][y].height + _Map[x][y] == zmin)
				_CurMap[x][y].dir = ((1.0 + rand()) * g_tab_pi[8]) / ((RAND_MAX + 1.0) * g_tab_pi[8]);*/
			//	limit flood
			if (_Map[x][y] + _CurMap[x][y].height > 1)
				_CurMap[x][y].height = 1 - _Map[x][y];
		}
	}
}

void Scenario::Evapor(void)
{

	//	Give evaporate speed;
	float	hot = 0.01;

	for (unsigned int x = 0; x < _sizeX; x++)
	{
		for (unsigned int y = 0; y < _sizeY; y++)
		{
			_CurMap[x][y].height -= hot;
			if (_CurMap[x][y].height <= 0.0)
			{
				_CurMap[x][y].height = 0;
				_CurMap[x][y].speed = 0;
			}
		}
	}
}

void Scenario::DiscWorld(bool n, bool s, bool e, bool w)
{
	double			PI = std::atan(1.0);
	float			zmin = FallsMin(n, s, e, w);
	unsigned int	mx;
	unsigned int	my;

	//	Security
	if ((mx  = _sizeX) > 6)
		mx = 6;
	if ((my  = _sizeY) > 6)
		my = 6;

	//	North fall
	for (unsigned int x = 0; x < _sizeX && n; x++)
	{
		for (unsigned int y = 0; y < my; y++)
		{
			if (_CurMap[x][y].height > zmin)
			{
				_CurMap[x][y].height = zmin;
				_CurMap[x][y].speed = 0;
				_CurMap[x][y].dir = PI;
			}
		}
	}
	//	South fall
	for (unsigned int x = 0; x < _sizeX && s; x++)
	{
		for (unsigned int y = 1; y < my+1; y++)
		{
			if (_CurMap[x][_sizeY-y].height > zmin)
			{
				_CurMap[x][_sizeY-y].height = zmin;
				_CurMap[x][_sizeY-y].speed = 0;
				_CurMap[x][_sizeY-y].dir = 5 * PI;
			}
		}
	}
	//	Est fall
	for (unsigned int y = 0; y < _sizeY && e; y++)
	{
		for (unsigned int x = 0; x < mx; x++)
		{
			if (_CurMap[x][y].height > zmin)
			{
				_CurMap[x][y].height = zmin;
				_CurMap[x][y].speed = 0;
				_CurMap[x][y].dir = 3 * PI;
			}
		}
	}
	//	West fall
	for (unsigned int y = 0; y < _sizeY && w; y++)
	{
		for (unsigned int x = 1; x < mx+1; x++)
		{
			if (_CurMap[_sizeX-x][y].height > zmin)
			{
				_CurMap[_sizeX-x][y].height = zmin;
				_CurMap[_sizeX-x][y].speed = 0;
				_CurMap[_sizeX-x][y].dir = 7 * PI;
			}
		}
	}
}

/* ************************************************************************** */
//	Private Functions:

//	Wave function
float Scenario::WavesMin(bool n, bool s, bool e, bool w)
{
	float	zmin = 1.0;

	//	Check lowest point of the edge
	if (n)
	{
		for (unsigned int x = 0; x < _sizeX; x++)
		{
			if (_CurMap[x][0].height + _Map[x][0] < zmin)
				zmin = _CurMap[x][0].height + _Map[x][0];
		}
	}
	if (w)
	{
		for (unsigned int y = 0; y < _sizeY; y++)
		{
			if (_CurMap[0][y].height + _Map[0][y] < zmin)
				zmin = _CurMap[0][y].height + _Map[0][y];
		}
	}
	if (s)
	{
		for (unsigned int x = 0; x < _sizeX; x++)
		{
			if (_CurMap[x][_sizeY - 1].height + _Map[x][_sizeY - 1] < zmin)
				zmin = _CurMap[x][_sizeY - 1].height + _Map[x][_sizeY - 1];
		}
	}
	if (e)
	{
		for (unsigned int y = 0; y < _sizeY; y++)
		{
			if (_CurMap[_sizeX - 1][y].height + _Map[_sizeX - 1][y] < zmin)
				zmin = _CurMap[_sizeX - 1][y].height + _Map[_sizeX - 1][y];
		}
	}
	return zmin;
}

//	Fall function
float Scenario::FallsMin(bool n, bool s, bool e, bool w)
{
	float	zmin = 1.0;

	//	Check lowest point of the edge
	if (n)
	{
		for (unsigned int x = 0; x < _sizeX; x++)
		{
			if (_Map[x][0] < zmin)
				zmin = _Map[x][0];
		}
	}
	if (w)
	{
		for (unsigned int y = 0; y < _sizeY; y++)
		{
			if (_Map[0][y] < zmin)
				zmin = _Map[0][y];
		}
	}
	if (s)
	{
		for (unsigned int x = 0; x < _sizeX; x++)
		{
			if (_Map[x][_sizeY - 1] < zmin)
				zmin = _Map[x][_sizeY - 1];
		}
	}
	if (e)
	{
		for (unsigned int y = 0; y < _sizeY; y++)
		{
			if (_Map[_sizeX - 1][y] < zmin)
				zmin = _Map[_sizeX - 1][y];
		}
	}
	return zmin;
}
