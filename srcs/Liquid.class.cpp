/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Liquid.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/13 11:18:05 by tbalea            #+#    #+#             */
/*   Updated: 2015/05/22 11:51:25 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Liquid.class.hpp"

const double PI = std::atan(1.0);
const double g_tab_pi[] = {0, PI, 2 * PI, 3 * PI, 4 * PI, 5 * PI, 6 * PI, 7 * PI, 8 * PI};
const double g_tab_halfpi[] = {0.5 * PI, 1.5 * PI, 2.5 * PI, 3.5 * PI, 4.5 * PI, 5.5 * PI, 6.5 * PI, 7.5 * PI};

/* ************************************************************************** */
//	Public Functions:

//	Constructor
Liquid::Liquid(float ** Map, unsigned int sizeX, unsigned int sizeY, int type) :
	_w(true), _s(true), _e(true), _n(true), _Map(Map), _sizeX(sizeX), _sizeY(sizeY), _type(type)
{
	_CurMap = new t_liquid*[_sizeX];
	for (unsigned int x = 0; x < _sizeX; x++)
	{
		_CurMap[x] = new t_liquid[_sizeY];
		for (unsigned int y = 0; y < _sizeY; y++)
		{
			_CurMap[x][y].height = 0;
			_CurMap[x][y].speed = 0;
			_CurMap[x][y].dir = g_tab_pi[8];
		}
	}
}

//	Destructor
Liquid::~Liquid(void)
{
	for (unsigned int x = 0; x < _sizeX; x++)
		delete [] this->_CurMap[x];
	delete [] this->_CurMap;
}

//	Getter
t_liquid ** Liquid::getCurMap(void)
{
	return this->_CurMap;
}

//	Clear
void Liquid::ClearCurMap(void)
{
	for (unsigned x = 0; x < _sizeX; x++)
	{
		for (unsigned y = 0; y < _sizeY; y++)
		{
			_CurMap[x][y].height = 0.0;
			_CurMap[x][y].speed = 0.0;
			_CurMap[x][y].dir =  g_tab_pi[8];
		}
	}
}

//	Flowing
void Liquid::Flow(void)
{
	Drop();
	Speed();
}

/* ************************************************************************** */
//	Private Functions:

void Liquid::Drop(void)
{
	float	drop = 0.0001;
	float	speed;
	float	new_speed;
	float	hgt;
	float	dir = g_tab_pi[1];
	int		random;

	srand(time(NULL));
	for (unsigned int x = 0; x < _sizeX; x++)
	{
		for (unsigned int y = 0; y < _sizeY; y++)
		{
			random = rand();
			if (_CurMap[x][y].height > drop)
			{
				speed = drop;
				hgt = _CurMap[x][y].height + _Map[x][y];

				//	Check highest difference
/*				if (x + 1 < _sizeX && y > 0
						&& speed < (new_speed = hgt - _CurMap[x+1][y-1].height - _Map[x+1][y-1]))
				{
					speed = new_speed;
					dir = g_tab_pi[8];
				}*/
				if (x + 1 < _sizeX
						&& (speed < (new_speed = hgt - _CurMap[x+1][y].height - _Map[x+1][y])
						/*|| (speed == new_speed
						&& random % 2 == 0)*/))
				{
					speed = new_speed;
					dir = g_tab_pi[7];
				}/*
				if (x + 1 < _sizeX && y + 1 < _sizeY
						&& (speed < (new_speed = hgt - _CurMap[x+1][y+1].height - _Map[x+1][y+1])
						|| (speed == new_speed
						&& random % 2 == 0)))
				{
					speed = new_speed;
					dir = g_tab_pi[6];
				}*/
				if (y + 1 < _sizeY
						&& (speed < (new_speed = hgt - _CurMap[x][y+1].height - _Map[x][y+1])
						|| (speed == new_speed
						&& random % 2 == 0)))
				{
					speed = new_speed;
					dir = g_tab_pi[5];
				}/*
				if (y + 1 < _sizeY && x > 0
						&& (speed < (new_speed = hgt - _CurMap[x-1][y+1].height - _Map[x-1][y+1])
						|| (speed == new_speed
						&& random % 2 == 0)))
				{
					speed = new_speed;
					dir = g_tab_pi[4];
				}*/
				if (x > 0 && (speed < (new_speed = hgt - _CurMap[x-1][y].height - _Map[x-1][y])
						|| (speed == new_speed
						&& random % 2 == 0)))
				{
					speed = new_speed;
					dir = g_tab_pi[3];
				}/*
				if (y > 0 && x > 0
						&& (speed < (new_speed = hgt - _CurMap[x-1][y-1].height - _Map[x-1][y-1])
						|| (speed == new_speed
						&& random % 2 == 0)))
				{
					speed = new_speed;
					dir = g_tab_pi[2];
				}*/
				if (y > 0 && (speed < (new_speed = hgt - _CurMap[x][y-1].height - _Map[x][y-1])
						|| (speed == new_speed
						&& random % 2 == 0)))
				{
					speed = new_speed;
					dir = g_tab_pi[1];
				}

				if (speed > drop)
					DropNew(x, y, dir, hgt);
			}
		}
	}
}

//		Drop modification
void Liquid::DropNew(unsigned int x, unsigned int y, float dir, float speed)
{
	float	way = _CurMap[x][y].dir;

	NewDir(x, y, dir, speed);
	NewSpd(x, y, dir, speed, way);
}

void Liquid::Speed(void)
{
	float	lim = 0.0001;

	for (unsigned int x = 0; x < _sizeX; x++)
	{
		for (unsigned int y = 0; y < _sizeY; y++)
		{
			if (_CurMap[x][y].speed > lim)
			{
				//	Verify valid direction (sens && edge && height)
				//	x + 1
				if (_CurMap[x][y].dir > g_tab_halfpi[6] && _CurMap[x][y].dir <= g_tab_halfpi[7])
				{
					for (unsigned int n = 1;
							(x + n) < _sizeX &&
							_CurMap[x][y].speed > _CurMap[x+n][y].height + _Map[x+n][y] &&
							_CurMap[x][y].height >= lim;
							n++)
						Transfert(x, y, x+n, y);
				}
				// x + 1 && y + 1
				if (_CurMap[x][y].dir > g_tab_halfpi[5] && _CurMap[x][y].dir <= g_tab_halfpi[6])
				{
					for (unsigned int n = 1;
							(x + n) < _sizeX && (y + n) < _sizeY &&
							_CurMap[x][y].speed > _CurMap[x+n][y+n].height + _Map[x+n][y+n] &&
							_CurMap[x][y].height >= lim;
							n++)
						Transfert(x, y, x+n, y+n);
				}
				//	y + 1
				if (_CurMap[x][y].dir > g_tab_halfpi[4] && _CurMap[x][y].dir <= g_tab_halfpi[5])
				{
					for (unsigned int n = 1;
							(y + n) < _sizeY &&
							_CurMap[x][y].speed > _CurMap[x][y+n].height + _Map[x][y+n] &&
							_CurMap[x][y].height >= lim;
							n++)
						Transfert(x, y, x, y+n);
				}
				// x - 1 && y + 1
				if (_CurMap[x][y].dir > g_tab_halfpi[3] && _CurMap[x][y].dir <= g_tab_halfpi[4])
				{
					for (unsigned int n = 1;
							x >= n && (y + n) < _sizeY &&
							_CurMap[x][y].speed > _CurMap[x-n][y+n].height + _Map[x-n][y+n] &&
							_CurMap[x][y].height >= lim;
							n++)
						Transfert(x, y, x-n, y+n);
				}
				// x - 1
				if (_CurMap[x][y].dir > g_tab_halfpi[2] && _CurMap[x][y].dir <= g_tab_halfpi[3])
				{
					for (unsigned int n = 1;
							x >= n &&
							_CurMap[x][y].speed > _CurMap[x-n][y].height + _Map[x-n][y] &&
							_CurMap[x][y].height >= lim;
							n++)
						Transfert(x, y, x-n, y);
				}
				// x - 1 && y - 1
				if (_CurMap[x][y].dir > g_tab_halfpi[1] && _CurMap[x][y].dir <= g_tab_halfpi[2])
				{
					for (unsigned int n = 1;
							x >= n && y >= n &&
							_CurMap[x][y].speed > _CurMap[x-n][y-n].height + _Map[x-n][y-n] &&
							_CurMap[x][y].height >= lim;
							n++)
						Transfert(x, y, x-n, y-n);
				}
				// y - 1
				if (_CurMap[x][y].dir > g_tab_halfpi[0] && _CurMap[x][y].dir <= g_tab_halfpi[1])
				{
					for (unsigned int n = 1;
							y >= n &&
							_CurMap[x][y].speed > _CurMap[x][y-n].height + _Map[x][y-n] &&
							_CurMap[x][y].height >= lim;
							n++)
						Transfert(x, y, x, y-n);
				}
				// x + 1 && y - 1
				if (_CurMap[x][y].dir > g_tab_halfpi[7] && _CurMap[x][y].dir <= g_tab_halfpi[0])
				{
					for (unsigned int n = 1;
							(x + n) < _sizeX && y >= n &&
							_CurMap[x][y].speed > _CurMap[x+n][y-n].height + _Map[x+n][y-n] &&
							_CurMap[x][y].height >= lim;
							n++)
						Transfert(x, y, x+n, y-n);
				}
			}
		}
	}
}

//		Speed modification
void Liquid::Transfert(unsigned int x1, unsigned int y1,
		unsigned int x2, unsigned int y2)
{
	float	drop = 0.001;
	float	slow = 0.0;
	float	fp1 = _CurMap[x1][y1].height + _Map[x1][y1];
	float	fp2 = _CurMap[x2][y2].height + _Map[x2][y2];
	float	newspeed;
	float	way = _CurMap[x2][y2].dir;

	//	Transfert of Liquid
	if (drop > _CurMap[x1][y1].height)
		drop = _CurMap[x1][y1].height;
	if (_CurMap[x2][y2].height + _Map[x2][y2] + drop > 1)
		drop = 1 - _CurMap[x2][y2].height - _Map[x2][y2];
	_CurMap[x1][y1].height -= drop;
	_CurMap[x2][y2].height += drop;

	//	Transfert of Power
	newspeed = _CurMap[x1][y1].speed * drop;
	/*if (newspeed < 0.0)
		newspeed = 0.0;
	else if (newspeed > 1.0)
		newspeed = 1.0;*/
	//	new value of x2,y2 speed and way
	NewDir(x2, y2, _CurMap[x1][y1].dir, newspeed);
	NewSpd(x2, y2, _CurMap[x1][y1].dir, newspeed, way);
	//	new value of x1,y1 speed
	if ((slow = fp1 - fp2) < 0)
		slow = -slow;
	if (slow / fp1 < _CurMap[x1][y1].height)
		_CurMap[x1][y1].speed = slow;
	return ;
}

//			new value of way ((x / (x + y)) + (y / (x + y)))
void Liquid::NewDir(unsigned int x, unsigned int y, float dir, float speed)
{
	float	speedTotal = speed + _CurMap[x][y].speed;

	if (speedTotal == 0)
		return ;
	if (dir - _CurMap[x][y].dir > g_tab_pi[4])
		_CurMap[x][y].dir += g_tab_pi[8];
	else if (_CurMap[x][y].dir - dir > g_tab_pi[4])
		dir += 8 * g_tab_pi[8];
	_CurMap[x][y].dir = _CurMap[x][y].dir * (_CurMap[x][y].speed / speedTotal);
	_CurMap[x][y].dir += dir * (speed / speedTotal);
	while (_CurMap[x][y].dir > g_tab_pi[8])
		_CurMap[x][y].dir -= g_tab_pi[8];
	while (_CurMap[x][y].dir <= g_tab_pi[0])
		_CurMap[x][y].dir += g_tab_pi[8];
}

//			new value of speed (V(x * x + y * y))
void Liquid::NewSpd(unsigned int x, unsigned int y, float dir, float spd, float odir)
{
	float	rlow = 0.01;
	float	tmp;

	tmp = pow(sin(odir) * _CurMap[x][y].speed + (sin(dir) * spd), 2);
	tmp += pow(cos(odir) * _CurMap[x][y].speed + (cos(dir) * spd), 2);
	_CurMap[x][y].speed = sqrt(tmp);
	if ((_CurMap[x][y].speed = _CurMap[x][y].speed - rlow) < 0)
		_CurMap[x][y].speed = 0;
	if (_CurMap[x][y].speed > 1)
		_CurMap[x][y].speed = 1;
	return ;
}
