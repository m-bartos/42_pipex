# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbartos <mbartos@student.42prague.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/21 10:59:06 by mbartos           #+#    #+#              #
#    Updated: 2023/11/24 17:24:06 by mbartos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BOLD =	\033[1m
GREEN =	\033[32m
RED =	\033[31m
BCYAN =	\033[96m

#name
NAME = 		pipex

#compiler
CC =		cc
CFLAGS =	-Wall -Wextra -Werror

#libft
LIBFT_PATH =	libft/
LIBFT_NAME =	libft.a
LIBFT =			$(LIBFT_PATH)$(LIBFT_NAME)

#Includes
INC =	-I ./libft

#sources
SRC_PATH =	src/
SRC = 	pipex.c
SRCS =	$(addprefix $(SRC_PATH), $(SRC))

#objects
OBJ_PATH =	obj/
OBJ =		$(SRC:.c=.o)
OBJS =		$(addprefix $(OBJ_PATH), $(OBJ))


all: $(LIBFT) $(NAME)

$(OBJ_PATH)%.o :$(SRC_PATH)%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(OBJS): $(OBJ_PATH)

$(OBJ_PATH):
	@mkdir $(OBJ_PATH)

$(LIBFT):
	@echo "$(BOLD)$(BCYAN)[ Making libft... ]"
	@make -sC $(LIBFT_PATH)

$(NAME): $(OBJS)
	@echo "$(BOLD)$(BCYAN)[ Compiling... ]"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(INC)
	@echo "$(BOLD)$(GREEN)[ Program ready! ]"

clean:
	@rm -Rf $(OBJ_PATH)
	@make clean -sC $(LIBFT_PATH)
	@echo "$(BOLD)$(RED)[ Obj files deleted ]"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIBFT_PATH)$(LIBFT_NAME)
	@echo "$(BOLD)$(RED)[ Program deleted ]"

re: fclean all

.PHONY: all re clean fclean