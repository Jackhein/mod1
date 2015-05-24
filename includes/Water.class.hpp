/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Water.class.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/13 11:17:55 by tbalea            #+#    #+#             */
/*   Updated: 2015/03/20 13:50:32 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WATER_CLASS_HPP
# define WATER_CLASS_HPP

# include "Scenario.class.hpp"

class Water: public Scenario {
	public:
		//Function
		//	Constructor
		Water(float ** Map, unsigned int sizeX, unsigned int sizeY);

		//	Destructor
		~Water(void);

		//	Setter
		//		No setter

		//	Getter
		//		No getter
	private:
		//Function
		//	Constructor
		Water(void);

		//	Copy
		Water(const Water & water);
		Water & operator=(const Water & water);
};

#endif
