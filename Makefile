# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmartin <mmartin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/02/03 13:39:05 by mmartin           #+#    #+#              #
#    Updated: 2015/05/18 17:07:48 by mmartin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS		=	-Wall -Wextra -Werror -O3

CC			=	g++

INC			=	-I includes -I /usr/X11/include

DOBJ		=	obj/

NAME		=	mod1

LIB			=	-L /usr/X11/lib -lX11

SRC			=	srcs/main.cpp					\
				srcs/Liquid.class.cpp			\
				srcs/Scenario.class.cpp			\
				srcs/GraphicalDisplay.class.cpp	\
				srcs/Map.class.cpp				\
				srcs/Water.class.cpp			\

OBJ			=	$(patsubst %.cpp, $(DOBJ)%.o, $(SRC))

DEPS		=	$(patsubst %.cpp, $(DOBJ)%.d, $(SRC))

DEPENDS		=	-MT $@ -MD -MP -MF $(subst .o,.d,$@)

all:		$(NAME)

$(NAME):	$(OBJ)
	@echo "\033[32m$(CC) \033[33m$(CFLAGS) \033[36m-c $< -o $@ $(LIB)\033[0m"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIB)
	@echo "\033[33m"Compilation of $(NAME) : "\033[32m"Success"\033[0m"

-include		$(OBJ:.o=.d)

$(DOBJ)%.o:		%.cpp
	@mkdir -p $(DOBJ)
	@mkdir -p $(DOBJ)/srcs
	@echo "\033[32m$(CC) \033[33m$(CFLAGS) \033[35m$(DEPENDS) \033[36m-c $< -o $@ $(INC)\033[0m"
	@$(CC) $(CFLAGS) $(DEPENDS) -c $< -o $@ $(INC)

clean:
	@/bin/rm -rf $(DOBJ)
	@echo "\033[31m"Objects of $(NAME) : deleted"\033[0m"

fclean:			clean
	@/bin/rm -f $(NAME)
	@/bin/rm -rf $(NAME).dSYM
	@echo "\033[31m"$(NAME) : deleted"\033[0m"

re:				fclean all

.PHONY:			all clean fclean re
