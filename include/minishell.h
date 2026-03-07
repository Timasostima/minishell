/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkulivar <tkulivar@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:53:36 by tkulivar          #+#    #+#             */
/*   Updated: 2026/03/05 18:20:00 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <math.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include "get_next_line.h"
# include "libft.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	int					quoted;
	struct s_token		*next;
}	t_token;

typedef struct s_redir
{
	t_redir_type			type;
	char					*target;
	int						target_quoted;
	int						heredoc_quoted;
	int						heredoc_fd;
	struct s_redir			*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	int				is_builtin;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	int				last_status;
	int				interactive;
	int				should_exit;
	int				use_default_path;
	struct s_env	*env;
}	t_shell;

typedef struct s_argv_exp
{
	t_shell	*sh;
	char	quote;
}	t_argv_exp;

extern volatile sig_atomic_t	g_signal;

/* ************************************************************************** */
/*                                   Run                                      */
/* ************************************************************************** */

char	*read_interactive_line(void);
char	*read_non_interactive_line(void);
int		run_shell(t_shell *sh);

/* ************************************************************************** */
/*                                  Lexer                                     */
/* ************************************************************************** */

t_token	*lexer_tokenize(char *input);
t_token	*lexer_new_token(t_token_type type, char *value, int quoted);
char	*lexer_extract_word(char *input, int *i, int *quoted);
int		lexer_validate_syntax(t_token *tokens);
void	lexer_add_back(t_token **tokens, t_token *new_token);
void	lexer_free_tokens(t_token **tokens);

/* ************************************************************************** */
/*                                  Parser                                    */
/* ************************************************************************** */

t_cmd	*parser_parse_tokens(t_token *tokens);
int		parser_token_is_redir(int type);
int		parser_add_redir_from_tokens(t_redir **redirs,
			t_token *op, t_token *arg);
void	parser_free_cmds(t_cmd **cmds);

/* ************************************************************************** */
/*                                    Env                                     */
/* ************************************************************************** */

int		env_inc_shlvl(t_env **env);
int		env_set(t_env **env, char *key, char *value);
int		env_unset(t_env **env, char *key);
char	*env_get(t_env *env, char *key);
char	**env_to_envp(t_env *env);
t_env	*env_init(char **envp);
void	env_clear(t_env **env);

/* ************************************************************************** */
/*                                 Builtins                                   */
/* ************************************************************************** */

int		is_builtin(char *cmd);
int		is_parent_builtin(char *cmd);
int		exec_builtin(char **argv, t_shell *sh);
int		builtin_echo(char **argv);
int		builtin_pwd(char **argv, t_shell *sh);
int		builtin_env(char **argv, t_shell *sh);
int		builtin_cd(char **argv, t_shell *sh);
int		builtin_export(char **argv, t_shell *sh);
int		builtin_unset(char **argv, t_shell *sh);
int		builtin_exit(char **argv, t_shell *sh);
int		print_export_sorted(t_shell *sh);
int		cd_update_pwd_after_cd(t_shell *sh, char *target, int had_cwd_before);
void	cd_print_target(char *target);

/* ************************************************************************** */
/*                                 Executor                                   */
/* ************************************************************************** */

int		execute_input_line(char *input, t_shell *sh);
int		wait_for_child(pid_t pid, t_shell *sh);
int		wait_for_children(pid_t *pids, int count, pid_t last_pid,
			t_shell *sh);
int		execute_cmd_table(t_cmd *cmds, t_shell *sh);
int		execute_external_cmd(t_cmd *cmd, t_shell *sh);
int		expand_cmd_table(t_cmd *cmds, t_shell *sh);
int		expand_argv_words(t_cmd *cmd, t_shell *sh);
int		prepare_heredocs(t_cmd *cmds, t_shell *sh);
int		update_underscore_var(t_cmd *cmds, t_shell *sh);
int		apply_redirections(t_redir *redirs);
char	*expand_heredoc_line(char *raw, t_shell *sh);
int		write_heredoc_expanded_line(int fd, char *line,
			t_redir *redir, t_shell *sh);
char	*expand_word_value(char *raw, t_shell *sh, int allow_vars);
char	*expand_word_argv_value(char *raw, t_shell *sh);
int		cmd_has_input_redir(t_cmd *cmd);
int		cmd_has_output_redir(t_cmd *cmd);
char	*build_cmd_path(char **split_paths, char *cmd);
void	close_heredoc_fds(t_cmd *cmds);
pid_t	spawn_command(t_cmd *cmd, t_shell *sh, int prev_read,
			int pipefd[2]);
int		launch_pipeline(t_cmd *cmds, t_shell *sh, pid_t *pids,
			pid_t *last_pid);

/* ************************************************************************** */
/*                                  Signals                                   */
/* ************************************************************************** */

void	set_heredoc_signals(void);
void	restore_heredoc_signals(int interactive);
void	interactive_signals_configuration(void);
void	set_child_signals_default(void);
void	set_parent_wait_signals_ignored(void);

#endif
