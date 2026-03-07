# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/20 11:37:02 by ibenaven          #+#    #+#              #
#    Updated: 2026/03/06 18:38:14 by ibenaven         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS ?= -Wall -Wextra -Werror
RM = rm -f

SHELL	:= /bin/bash

ifeq ($(DEBUG), 1)
CFLAGS += -g
endif

VG		:= valgrind
VGFLAGS	:= \
		-s \
		--tool=memcheck \
		--leak-check=full \
		--show-leak-kinds=all \
		--show-reachable=yes \
		--track-origins=yes \
		--trace-children=yes \
		--track-fds=yes \
		--suppressions=readline.supp \
		--log-file=informe

LIBFT = libft
LIBFT_A = $(LIBFT)/libft.a

GNL = gnl
GNL_A = $(GNL)/gnl.a
INCLUDE = -Iinclude -I$(LIBFT) -I$(GNL)

FILES = minishell \
		signals/signals \
		lexer/tokenize \
		lexer/token_utils \
		lexer/word \
		lexer/syntax \
		parser/parse \
		parser/redir \
		parser/utils \
		env/env_init \
		env/env_ops \
		env/env_export \
		executor/path \
		builtins/builtin_dispatcher \
		builtins/builtin_echo \
		builtins/builtin_pwd \
		builtins/builtin_env \
		builtins/builtin_cd \
		builtins/builtin_cd_pwd \
		builtins/builtin_export \
		builtins/builtin_export_print \
		builtins/builtin_unset \
		builtins/builtin_exit \
		run/interactive_mode \
		run/non_interactive_mode \
		run/run_loop \
		executor/exec_child \
		executor/pipeline_child \
		executor/pipeline_launch \
		executor/exec_pipeline \
		executor/redirections \
		executor/expand_word \
		executor/expand_word_argv \
		executor/expand_argv \
		executor/expand_table \
		executor/heredoc \
		executor/heredoc_expand \
		executor/heredoc_signals \
		executor/heredoc_utils \
		executor/underscore \
		executor/exec_errors \
		executor/wait_status

SRCS = $(addsuffix .c, $(FILES))
OBJS = $(addprefix ./objs/, $(addsuffix .o, $(FILES)))

SYM_BUILD = [BUILD]
SYM_CLONE = [CLONE]
SYM_LINK = [LINK]
SYM_CLEAN = [CLEAN]
SYM_RE = [RE]
SYM_RUN = [RUN]
SYM_NORM = [NORM]
SYM_OK = [OK]

RED := \033[91;1m
GREEN := \033[92;1m
CLEAR_COLOR := \033[0m
CYAN := \033[96;1m

all: $(NAME)

$(NAME): $(LIBFT_A) $(GNL_A) $(OBJS)
	@echo -e "$(CYAN)$(SYM_LINK) Linking $(NAME)...$(CLEAR_COLOR)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(GNL_A) -lreadline -o $(NAME)
	@echo -e "$(GREEN)$(SYM_OK) $(NAME) ready$(CLEAR_COLOR)"

objs/.build_msg:
	@echo -e "$(CYAN)$(SYM_BUILD) Compiling $(NAME) objects...$(CLEAR_COLOR)"
	@mkdir -p objs
	@touch $@

objs/%.o: %.c include/minishell.h objs/.build_msg
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(LIBFT_A):
	@if [ ! -d $(LIBFT) ]; then \
		echo -e "$(CYAN)$(SYM_CLONE) Cloning $(LIBFT)...$(CLEAR_COLOR)"; \
		mkdir -p $(LIBFT) &&\
		cd $(LIBFT) && \
		git clone --depth=1 -n https://github.com/Timasostima/42.git && \
		cd 42 && \
		git sparse-checkout set cursus/Libft && \
		git checkout && \
		cd ../.. && \
		cp -r $(LIBFT)/42/cursus/Libft/* $(LIBFT)/ && \
		rm -rf $(LIBFT)/42; \
		echo -e "$(GREEN)$(SYM_OK) $(LIBFT) cloned$(CLEAR_COLOR)"; \
	fi
	@echo -e "$(CYAN)$(SYM_BUILD) Compiling $(LIBFT)...$(CLEAR_COLOR)"
	@make -s --no-print-directory -C $(LIBFT)
	@echo -e "$(GREEN)$(SYM_OK) $(LIBFT) ready$(CLEAR_COLOR)"

$(GNL_A):
	@echo -e "$(CYAN)$(SYM_BUILD) Compiling $(GNL)...$(CLEAR_COLOR)"
	@make -s --no-print-directory -C $(GNL)
	@echo -e "$(GREEN)$(SYM_OK) $(GNL) ready$(CLEAR_COLOR)"

clean:
	@echo -e "$(RED)$(SYM_CLEAN) Cleaning project objects...$(CLEAR_COLOR)"
	@$(RM) -r objs/
	@make -s --no-print-directory -C $(LIBFT) clean
	@make -s --no-print-directory -C $(GNL) clean
	@echo -e "$(GREEN)$(SYM_OK) clean done$(CLEAR_COLOR)"

fclean: clean
	@echo -e \
		"$(RED)$(SYM_CLEAN) Removing binaries and libraries...$(CLEAR_COLOR)"
	@$(RM) $(NAME)
	@make -s --no-print-directory -C $(LIBFT) fclean
	@make -s --no-print-directory -C $(GNL) fclean
	@echo -e "$(GREEN)$(SYM_OK) fclean done$(CLEAR_COLOR)"

erase_all: fclean
	@echo -e "$(RED)$(SYM_CLEAN) Removing $(LIBFT) directory...$(CLEAR_COLOR)"
	@$(RM) -r $(LIBFT)
	@echo -e "$(GREEN)$(SYM_OK) erase_all done$(CLEAR_COLOR)"

re:
	@echo -e "$(CYAN)$(SYM_RE) Rebuilding $(NAME)...$(CLEAR_COLOR)"
	@make -s --no-print-directory fclean
	@make -s --no-print-directory all
	@echo -e "$(GREEN)$(SYM_OK) re done$(CLEAR_COLOR)"

leaks:
	@echo -e \
		"$(CYAN)$(SYM_BUILD) Preparing DEBUG build for leaks...$(CLEAR_COLOR)"
	@make -s --no-print-directory erase_all
	@make -s --no-print-directory DEBUG=1
	@echo -e "$(CYAN)$(SYM_RUN) Running valgrind...$(CLEAR_COLOR)"
	@$(VG) $(VGFLAGS) ./$(NAME)

norminette:
	@echo -e "$(CYAN)$(SYM_NORM) Running norminette...$(CLEAR_COLOR)"
	@norminette minishell.c include/minishell.h builtins env executor \
		lexer parser run signals gnl
	@echo -e "$(GREEN)$(SYM_OK) norminette done$(CLEAR_COLOR)"

.PHONY: all clean fclean re erase_all leaks norminette
