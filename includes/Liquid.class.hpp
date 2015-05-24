/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Liquid.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/17 18:50:31 by tbalea            #+#    #+#             */
/*   Updated: 2015/05/22 11:59:24 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIQUID_CLASS_HPP
# define LIQUID_CLASS_HPP

# include "Map.class.hpp"
# include "mod1.h"
# include <cmath>
# include <ctime>
# include <cstdlib>

extern const double PI;
extern const double g_tab_pi[];
extern const double g_tab_halfpi[];

class Liquid {
	public:
		//Function
		//	Constructor
		Liquid(float ** Map, unsigned int sizeX, unsigned int sizeY, int type);

		//	Destructor
		~Liquid(void);

		//	Setter
		//		No setter

		//	Getter
		t_liquid ** getCurMap(void);

		//	Clear
		void ClearCurMap(void);

		//	Flowing
		void Flow(void);
	protected:
		//Function
		//	Constructor
		Liquid(void);

		//	Copy
		Liquid(const Liquid & liquid);
		Liquid & operator=(const Liquid & liquid);

		//	Calcul flow
		void Drop(void);
		void DropNew(unsigned int x, unsigned int y, float dir, float speed);
		void Speed(void);
		void Transfert(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
		void NewDir(unsigned int x, unsigned int y, float dir, float speed);
		void NewSpd(unsigned int x, unsigned int y, float dir, float spd, float odir);

		//Attribute
		//	Map Data
		bool _w;
		bool _s;
		bool _e;
		bool _n;
		float ** _Map;
		unsigned int _sizeX;
		unsigned int _sizeY;
		t_liquid ** _CurMap;
		int	_type;
};

#endif
