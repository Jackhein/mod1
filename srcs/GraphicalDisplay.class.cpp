/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GraphicalDisplay.class.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmartin <mmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/31 19:36:40 by mmartin           #+#    #+#             */
/*   Updated: 2015/05/22 10:44:18 by mmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <X11/Xutil.h>
#include "GraphicalDisplay.class.hpp"
#include "mod1.h"

/*
**	Constructor / destructor
*/
GraphicalDisplay::GraphicalDisplay(unsigned int width, unsigned int height): _dis(NULL)
{
	XSetWindowAttributes	attributes;
	XSizeHints				hints;
	Visual					*visual;
	int						screen;
	int						depth;

	_width = width;
	_height = height;
	_map = new Map(_width, _height);
	_water = NULL;

	_dis = XOpenDisplay(NULL);
	if (!_dis)
		throw std::runtime_error("X11 initilisation fail");
	screen = DefaultScreen(_dis);
	visual = DefaultVisual(_dis, screen);
	depth = DefaultDepth(_dis, screen);
	attributes.background_pixel = XBlackPixel(_dis, screen);

	_win = XCreateWindow(_dis, XRootWindow(_dis, screen), 0, 0, 1500, 900, 1, depth, InputOutput, visual, CWBackPixel, &attributes);
	if (!_win)
		throw std::runtime_error("X11 create window fail");

	XGetWMNormalHints(_dis, _win, &hints, NULL);
	hints.width = _width;
	hints.height = _height;
	hints.min_width = _width;
	hints.min_height = _height;
	hints.max_width = _width;
	hints.max_height = _height;
	hints.flags = PPosition | PSize | PMinSize | PMaxSize;
	XSetWMNormalHints(_dis, _win, &hints);

	XMapWindow(_dis, _win);
	XSelectInput(_dis, _win, ExposureMask | KeyPressMask | ButtonPressMask);
	_gc = XCreateGC(_dis, _win, 0, 0);

	_data = new char[_width * _height * 4];
	_image = XCreateImage(_dis, visual, depth, ZPixmap, 0, _data, _width, _height, 32, 0);
	_dataWater = new char[_width * _height * 4];
	_imageWater = XCreateImage(_dis, visual, depth, ZPixmap, 0, _dataWater, _width, _height, 32, 0);

	_dataGrey = new char[200 * 25 * 4];
	_greyBG = XCreateImage(_dis, visual, depth, ZPixmap, 0, _dataGrey, 200, 25, 32, 0);
	_dataWhite = new char[200 * 25 * 4];
	_whiteBG = XCreateImage(_dis, visual, depth, ZPixmap, 0, _dataWhite, 200, 25, 32, 0);

	rise = false;
	rain = false;
	evaporate = false;

	sWave = false;
	eWave = false;
	nWave = false;
	wWave = false;

	sFall = false;
	eFall = false;
	nFall = false;
	wFall = false;
}

GraphicalDisplay::~GraphicalDisplay(void)
{
	if (_map)
		delete _map;
	if (_water)
		delete _water;
	XFreeGC(_dis, _gc);
	XDestroyImage(_image);
	XDestroyImage(_imageWater);
	XDestroyImage(_greyBG);
	XDestroyImage(_whiteBG);
	XCloseDisplay(_dis);
}

/*
**	Getter
*/
Map			*GraphicalDisplay::getMap(void) const
{
	return (_map);
}

Water		*GraphicalDisplay::getWater(void) const
{
	return (_water);
}

/*
**	Setter
*/
bool		GraphicalDisplay::setMap(std::list<t_map> &mapHill)
{
	if (!_map)
		return (false);

	_map->setMapHill(mapHill);

	return (true);
}

bool		GraphicalDisplay::setWater(void)
{
	_water = new Water(_map->getMap(), _width, _height);
	return (true);
}

float		GraphicalDisplay::getColor(float earth, float water, int *r, int *g, int *b)
{
	float			y;
	float			color;
	static t_color	blue[11] =
	{
		{36,185,228},
		{33,174,219},
		{31,164,211},
		{28,153,202},
		{26,142,194},
		{24,131,185},
		{21,120,177},
		{19,109,168},
		{16,99,160},
		{14,88,151},
		{11,77,143}
	};
	int				i = water * 10;

	*r = blue[i].r;
	*g = blue[i].g;
	*b = blue[i].b;
	if (!water)
	{
		color = earth * 255;
		*r = (earth > 0.01 ? 100 + color : 1);
		*g = (earth > 0.01 ? 60 + color : 121);
		*b = (earth > 0.01 ? 20 + color : 111);
	}
	*r = (*r > 255 ? 255 : *r);
	*g = (*g > 255 ? 255 : *g);
	*b = (*b > 255 ? 255 : *b);
	y = (water + earth) * -100;

	return (y);
}

void		GraphicalDisplay::draw(float **tab)
{
	int			r;
	int			g;
	int			b;
	float		proj_x;
	float		proj_y;
	float		tmp_x = 500;
	float		tmp_y = 200;
	float		height;
	int			i;

	for (size_t x = 0; x < _width; x++)
	{
		proj_x = tmp_x;
		proj_y = tmp_y;
		for (size_t y = 0; y < _height; y++)
		{
			height = getColor(tab[x][y], 0, &r, &g, &b);
			i = (int)(proj_y + height) * _image->bytes_per_line + (int)proj_x * 4;
			_data[i] = b;
			_data[i + 1] = g;
			_data[i + 2] = r;
			if (x == _width - 1)
			{
				for (float earth = 0.02; earth < tab[x][y]; earth += 0.01)
				{
					height = getColor(earth, 0, &r, &g, &b);
					i = (int)(proj_y + height) * _image->bytes_per_line + (int)proj_x * 4;
					_data[i] = b;
					_data[i + 1] = g;
					_data[i + 2] = r;
				}
			}
			proj_x -= 0.5f;
			proj_y += 0.25f;
		}
		for (float earth = 0.02; earth < tab[x][_height - 1]; earth += 0.01)
		{
			height = getColor(earth, 0, &r, &g, &b);
			i = (int)(proj_y + height) * _image->bytes_per_line + (int)proj_x * 4;
			_data[i] = b;
			_data[i + 1] = g;
			_data[i + 2] = r;
		}
		tmp_x += 0.5f;
		tmp_y += 0.25f;
	}
	memcpy(_dataWater, _data, _height * _width * 4);
}

/*
**	Draw water in image.
*/
void		GraphicalDisplay::drawWater(float **tab)
{
	int			r;
	int			g;
	int			b;
	t_liquid	**map;
	float		proj_x;
	float		proj_y;
	float		tmp_x = 500;
	float		tmp_y = 200;
	float		height;
	int			i;

	map = _water->getCurMap();
	for (size_t x = 0; x < _width; x++)
	{
		proj_x = tmp_x;
		proj_y = tmp_y;
		for (size_t y = 0; y < _height; y++)
		{
			height = getColor(tab[x][y], map[x][y].height, &r, &g, &b);
			i = (int)(proj_y + height) * _imageWater->bytes_per_line + (int)proj_x * 4;
			_dataWater[i] = b;
			_dataWater[i + 1] = g;
			_dataWater[i + 2] = r;
			if (x == 0 || y == 0 )
			{
				for (float water = map[x][y].height + tab[x][y]; water < 1; water += 0.01)
				{
					i = (int)(proj_y + (water * -100)) * _imageWater->bytes_per_line + (int)proj_x * 4;
					_dataWater[i] = 0;
					_dataWater[i + 1] = 0;
					_dataWater[i + 2] = 0;
				}
			}
			proj_x -= 0.5f;
			proj_y += 0.25f;
		}
		tmp_x += 0.5f;
		tmp_y += 0.25f;
	}
	XPutImage(_dis, _win, _gc, _imageWater, 0, 0, 0, 200, _width, _height);
}


/*
**	Draw image foreground for button.
*/
void		GraphicalDisplay::setBackground(void)
{
	int		i;

	for (int x = 0; x < 200; x++)
	{
		for (int y = 0; y < 25; y++)
		{
			i = y * _greyBG->bytes_per_line + x * 4;
			_dataGrey[i] = (!y || !x ? 0 : 0xD3);
			_dataGrey[i + 1] = (!y || !x ? 0 : 0xD3);
			_dataGrey[i + 2] = (!y || !x ? 0 : 0xD3);
			_dataWhite[i] = (!y || !x ? 0 : 0xFF);
			_dataWhite[i + 1] = (!y || !x ? 0 : 0xFF);
			_dataWhite[i + 2] = (!y || !x ? 0 : 0xFF);
		}
	}
}


/*
**	Expose, redraw each button and the image.
*/
void		GraphicalDisplay::expose(void)
{
	XPutImage(_dis, _win, _gc, _imageWater, 0, 0, 0, 200, _width, _height);
	XPutImage(_dis, _win, _gc, (rise ? _whiteBG : _greyBG), 0, 0, 0, 0, 200, 25);
	XDrawString(_dis, _win, _gc, 50, 15, "Rise water", 10);
	XPutImage(_dis, _win, _gc, (rain ? _whiteBG : _greyBG), 0, 0, 200, 0, 200, 25);
	XDrawString(_dis, _win, _gc, 250, 15, "Rain water", 10);
	XPutImage(_dis, _win, _gc, (evaporate ? _whiteBG : _greyBG), 0, 0, 400, 0, 200, 25);
	XDrawString(_dis, _win, _gc, 450, 15, "Evaporate water", 15);

	// Wave
	XPutImage(_dis, _win, _gc, (sWave ? _whiteBG : _greyBG), 0, 0, 0, 25, 200, 25);
	XDrawString(_dis, _win, _gc, 50, 40, "South wave", 10);
	XPutImage(_dis, _win, _gc, (eWave ? _whiteBG : _greyBG), 0, 0, 200, 25, 200, 25);
	XDrawString(_dis, _win, _gc, 250, 40, "East wave", 9);
	XPutImage(_dis, _win, _gc, (nWave ? _whiteBG : _greyBG), 0, 0, 400, 25, 200, 25);
	XDrawString(_dis, _win, _gc, 450, 40, "North wave", 10);
	XPutImage(_dis, _win, _gc, (wWave ? _whiteBG : _greyBG), 0, 0, 600, 25, 200, 25);
	XDrawString(_dis, _win, _gc, 650, 40, "West wave", 9);
	XPutImage(_dis, _win, _gc, _greyBG, 0, 0, 800, 25, 200, 25);
	XDrawString(_dis, _win, _gc, 850, 40, "All wave", 8);
	//Fall
	XPutImage(_dis, _win, _gc, (sFall ? _whiteBG : _greyBG), 0, 0, 0, 50, 200, 25);
	XDrawString(_dis, _win, _gc, 50, 65, "South fall", 10);
	XPutImage(_dis, _win, _gc, (eFall ? _whiteBG : _greyBG), 0, 0, 200, 50, 200, 25);
	XDrawString(_dis, _win, _gc, 250, 65, "East fall", 9);
	XPutImage(_dis, _win, _gc, (nFall ? _whiteBG : _greyBG), 0, 0, 400, 50, 200, 25);
	XDrawString(_dis, _win, _gc, 450, 65, "North fall", 10);
	XPutImage(_dis, _win, _gc, (wFall ? _whiteBG : _greyBG), 0, 0, 600, 50, 200, 25);
	XDrawString(_dis, _win, _gc, 650, 65, "West fall", 9);
	XPutImage(_dis, _win, _gc, _greyBG, 0, 0, 800, 50, 200, 25);
	XDrawString(_dis, _win, _gc, 850, 65, "All Fall", 8);

	XPutImage(_dis, _win, _gc, _greyBG, 0, 0, 600, 0, 200, 25);
	XDrawString(_dis, _win, _gc, 650, 15, "Reset", 5);
	XPutImage(_dis, _win, _gc, _greyBG, 0, 0, 800, 0, 200, 25);
	XDrawString(_dis, _win, _gc, 850, 15, "Exit", 4);
	XFlush(_dis);
}


/*
**	On click check position x | y for button event.
*/
bool		GraphicalDisplay::buttonEvent(XEvent event)
{
	int		x = event.xbutton.x;
	int		y = event.xbutton.y;

	if (x < 1000 && y < 75)
	{
		if (y < 25 && x > 800 && x < 1000)
			return (true);
		if (y < 25 && x < 200)
			rise = (rise ? false : true);
		else if (y < 25 && x < 400)
			rain = (rain ? false : true);
		else if (y < 25 && x < 600)
			evaporate = (evaporate ? false : true);
		else if (y < 25 && x < 800)
		{
			rise = false;
			rain = false;
			evaporate = false;
			sWave = sFall = false;
			eWave = eFall = false;
			nWave = nFall = false;
			wWave = wFall = false;
			memcpy(_dataWater, _data, _height * _width * 4);
			_water->ClearCurMap();
		}
		else if (y < 50 && x < 200)
			sWave = (sWave ? false : true);
		else if (y < 50 && x < 400)
			eWave = (eWave ?  false: true);
		else if (y < 50 && x < 600)
			nWave = (nWave ? false: true);
		else if (y < 50 && x < 800)
			wWave = (wWave ? false : true);
		else if (y < 50 && x < 1000)
		{
			sWave = true;
			eWave = true;
			nWave = true;
			wWave = true;
		}
		else if (y < 75 && x < 200)
			sFall = (sFall ? false : true);
		else if (y < 75 && x < 400)
			eFall = (eFall ? false : true);
		else if (y < 75 && x < 600)
			nFall = (nFall ? false : true);
		else if (y < 75 && x < 800)
			wFall = (wFall ? false : true);
		else if (y < 75 && x < 1000)
		{
			sFall = true;
			eFall = true;
			nFall = true;
			wFall = true;
		}
		this->expose();
	}
	return (false);
}

/*
**		Run XQuartz in infinity loop for catch event
*/
bool		GraphicalDisplay::run(void)
{
	while (XPending(_dis))
	{
		XNextEvent(_dis, &_report);
		switch (_report.type) {
			case Expose:
				if (_report.xexpose.count == 0)
					this->expose();
				break;
			case KeyPress:
				if (XLookupKeysym(&_report.xkey, 0) == XK_q || XLookupKeysym(&_report.xkey, 0) == XK_Escape)
					return (false);
				break;
			case ButtonPress:
				if (this->buttonEvent(_report))
					return (false);
				break;
		}
	}

	if (rise)
		_water->Flood();
	if (sWave || eWave || nWave || wWave)
		_water->Waves(nWave, sWave, eWave, wWave);
	if (rain)
		_water->Rainy();
	if (evaporate)
		_water->Evapor();
	if (sFall || eFall || nFall || wFall)
		_water->DiscWorld(sFall, eFall, nFall, wFall);

	return (true);
}
