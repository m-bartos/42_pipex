# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbartos <mbartos@student.42prague.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/21 10:59:06 by mbartos           #+#    #+#              #
#    Updated: 2023/11/29 14:11:27 by mbartos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BOLD =	\033[1m
GREEN =	\033[32m
RED =	\033[31m
BCYAN =	\033[96m

#name
NAME =			pipex
NAME_BONUS =	pipex_bonus

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
SRC = 	pipex.c pipex_utils.c
SRCS =	$(addprefix $(SRC_PATH), $(SRC))

#sources BONUS
SRC_PATH_BONUS =	src_bonus/
SRC_BONUS = 	pipex_bonus.c
SRCS_BONUS =	$(addprefix $(SRC_PATH_BONUS), $(SRC_BONUS))

#objects
OBJ_PATH =	obj/
OBJ =		$(SRC:.c=.o)
OBJS =		$(addprefix $(OBJ_PATH), $(OBJ))

#objects BONUS
OBJ_PATH_BONUS =	obj_bonus/
OBJ_BONUS =			$(SRC_BONUS:.c=.o)
OBJS_BONUS =		$(addprefix $(OBJ_PATH_BONUS), $(OBJ_BONUS))

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

bonus: all

$(OBJ_PATH_BONUS)%.o :$(SRC_PATH_BONUS)%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)
	
$(OBJS_BONUS): $(OBJ_PATH_BONUS)

$(OBJ_PATH_BONUS):
	@mkdir $(OBJ_PATH_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
	@echo "$(BOLD)$(BCYAN)[ Compiling... ]"
	@$(CC) $(CFLAGS) -o $(NAME_BONUS) $(OBJS_BONUS) $(LIBFT) $(INC)
	@echo "$(BOLD)$(GREEN)[ Program ready! ]"

clean:
	@rm -Rf $(OBJ_PATH)
	@rm -Rf $(OBJ_PATH_BONUS)
	@make clean -sC $(LIBFT_PATH)
	@echo "$(BOLD)$(RED)[ Obj files deleted ]"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIBFT_PATH)$(LIBFT_NAME)
	@echo "$(BOLD)$(RED)[ Program deleted ]"

re: fclean all

.PHONY: all, bonus, re, clean, fclean