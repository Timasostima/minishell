/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:21:00 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Exits a child after cleaning copied resources for clean leak reports. */
static void	exit_child_process(t_shell *sh, int status)
{
	env_clear(&sh->env);
	gnl_clear();
	rl_clear_history();
	exit(status);
}

/* Wires child stdin/stdout to pipes unless command redirections override it. */
static int	setup_child_fds(t_cmd *cmd, int prev_read, int pipefd[2])
{
	if (prev_read != -1 && !cmd_has_input_redir(cmd)
		&& dup2(prev_read, STDIN_FILENO) == -1)
		return (perror("dup2"), 1);
	if (cmd->next != NULL && !cmd_has_output_redir(cmd)
		&& dup2(pipefd[1], STDOUT_FILENO) == -1)
		return (perror("dup2"), 1);
	if (prev_read != -1)
		close(prev_read);
	if (cmd->next != NULL)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (0);
}

/* Runs builtin or external command and returns the exit status for child. */
static int	run_child_command(t_cmd *cmd, t_shell *sh)
{
	if (cmd->argv == NULL || cmd->argv[0] == NULL)
		return (0);
	if (cmd->is_builtin)
		return (exec_builtin(cmd->argv, sh));
	return (execute_external_cmd(cmd, sh));
}

/* Forks one pipeline command and executes child branch with default signals. */
pid_t	spawn_command(t_cmd *cmd, t_shell *sh, int prev_read, int pipefd[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		set_child_signals_default();
		status = setup_child_fds(cmd, prev_read, pipefd);
		if (status == 0)
			status = apply_redirections(cmd->redirs);
		if (status == 0)
			status = run_child_command(cmd, sh);
		exit_child_process(sh, status);
	}
	return (pid);
}
