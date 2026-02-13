NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

LIBFT = libft
LIBFT_LIB = $(LIBFT)/libft.a

FILES = minishell

SRCS = $(addsuffix .c, $(FILES))

OBJS = $(addprefix ./objs/, $(addsuffix .o, $(FILES)))

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) -lreadline -o $(NAME)

objs/%.o: %.c minishell.h
	mkdir -p objs
	$(CC) $(CFLAGS) -I $(LIBFT) -c $< -o $@

$(LIBFT_LIB): 
	if [ ! -d $(LIBFT) ]; then \
		echo "Cloning libft..."; \
		mkdir -p $(LIBFT) &&\
		cd $(LIBFT) && \
		git clone --depth=1 -n https://github.com/Timasostima/42.git && \
		cd 42 && \
		git sparse-checkout set cursus/Libft && \
		git checkout && \
		cd ../.. && \
		cp -r $(LIBFT)/42/cursus/Libft/* $(LIBFT)/ && \
		rm -rf $(LIBFT)/42; \
	fi
	make -C $(LIBFT)

clean:
	$(RM) -r objs/
	make -C $(LIBFT) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT) fclean

erase_all: fclean
	$(RM) -r $(LIBFT)

re: fclean all

.PHONY: all clean fclean re erase_all
