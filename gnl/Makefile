# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/09 13:40:34 by ibenaven          #+#    #+#              #
#    Updated: 2025/08/19 20:27:57 by ibenaven         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= gnl.a

INC := get_next_line.h  

ARGS ?=

CFLAGS	:= -Wextra -Wall -Werror 

C_FILES	:= get_next_line.c \
		get_next_line_utils.c \
		get_next_line_state.c \

O_FILES := $(C_FILES:%.c=%.o)

all: $(NAME) 

$(NAME): $(O_FILES) 
	ar rcs $(NAME) $(O_FILES)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(O_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

test:
	$(CC) main.c $(NAME) $(INC) -o test
	./test
	rm -f test

sanitize:
	$(CC) -fsanitize=address -g main.c $(NAME) $(INC) -o test
	./test

debug:
	$(CC) -g main.c $(NAME) $(INC)  -o test
	valgrind --leak-check=full --show-leak-kinds=all -s ./test $(ARGS)
	rm -f test

.PHONY: all clean fclean re test sanitize debug
