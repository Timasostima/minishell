/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 20:26:08 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:20:00 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Returns 1 when line contains only space characters and should be skipped. */
static int	input_is_blank(char *input)
{
	int	i;

	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	return (input[i] == '\0');
}

/* Parses one input line into token and command tables used by executor core. */
static int	parse_input_line(char *input, t_token **tokens, t_cmd **cmds)
{
	*tokens = lexer_tokenize(input);
	if (*tokens == NULL)
		return (2);
	*cmds = parser_parse_tokens(*tokens);
	if (*cmds == NULL)
		return (1);
	return (0);
}

/* Runs parent builtin with temporary stdio redirections and restores stdio. */
static int	run_parent_builtin_redirs(t_cmd *cmd, t_shell *sh)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0)
	{
		if (saved_stdin >= 0)
			close(saved_stdin);
		if (saved_stdout >= 0)
			close(saved_stdout);
		return (perror("dup"), 1);
	}
	status = apply_redirections(cmd->redirs);
	if (status == 0)
		status = exec_builtin(cmd->argv, sh);
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		perror("dup2");
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		perror("dup2");
	close(saved_stdin);
	close(saved_stdout);
	return (status);
}

/* Executes parent-only builtin in-process when command is a single node. */
static int	maybe_exec_parent_builtin(t_cmd *cmds, t_shell *sh)
{
	if (cmds->next != NULL)
		return (-1);
	if (cmds->argv == NULL || cmds->argv[0] == NULL)
		return (-1);
	if (!is_parent_builtin(cmds->argv[0]))
		return (-1);
	return (run_parent_builtin_redirs(cmds, sh));
}

/* Executes one input line through lexer+parser and command-table executor. */
int	execute_input_line(char *input, t_shell *sh)
{
	t_token	*tokens;
	t_cmd	*cmds;
	int		status;

	if (input_is_blank(input))
		return (0);
	tokens = NULL;
	cmds = NULL;
	status = parse_input_line(input, &tokens, &cmds);
	if (status != 0)
		return (parser_free_cmds(&cmds), lexer_free_tokens(&tokens), status);
	status = expand_cmd_table(cmds, sh);
	if (status == 0)
		status = prepare_heredocs(cmds, sh);
	if (status == 0)
		status = update_underscore_var(cmds, sh);
	if (status == 0)
		status = maybe_exec_parent_builtin(cmds, sh);
	if (status < 0)
		status = execute_cmd_table(cmds, sh);
	return (close_heredoc_fds(cmds), parser_free_cmds(&cmds),
		lexer_free_tokens(&tokens), status);
}
