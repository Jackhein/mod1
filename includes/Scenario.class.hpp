/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scenario.class.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/17 18:50:42 by tbalea            #+#    #+#             */
/*   Updated: 2015/05/19 12:10:23 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENARIO_CLASS_HPP
# define SCENARIO_CLASS_HPP

# include "Liquid.class.hpp"

class Scenario: public Liquid {
	public:
		//Function
		//	Constructor
		Scenario(float ** Map, unsigned int sizeX, unsigned int sizeY, int type);

		//	Destructor
		~Scenario(void);

		//	Setter
		//		No setter

		//	Getter
		//		No getter

		//	Scenario
		void Rainy(void);
		void Waves(bool n, bool s, bool e, bool w);
		void Flood(void);
		void Evapor(void);
		void DiscWorld(bool n, bool s, bool e, bool w);

	protected:
		//Function
		//	Constructor
		Scenario(void);

		//	Copy
		Scenario(const Scenario & scenario);
		Scenario & operator=(const Scenario & scenario);

		//	Wave function
		float WavesMin(bool n, bool s, bool e, bool w);

		//	Fall function
		float FallsMin(bool n, bool s, bool e, bool w);
};

#endif
