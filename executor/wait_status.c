/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 20:25:55 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:20:00 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Prints bash-like signal text (optionally) and returns shell exit status. */
static int	handle_wait_status(int status, int print_signal)
{
	if (WIFSIGNALED(status))
	{
		if (print_signal && WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (print_signal && WTERMSIG(status) == SIGQUIT)
		{
			if (WCOREDUMP(status))
				write(STDERR_FILENO, "Quit (core dumped)\n", 19);
			else
				write(STDERR_FILENO, "Quit\n", 5);
		}
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/* Waits one specific child handling EINTR interruptions transparently. */
static int	wait_one(pid_t pid, int *status)
{
	int	ret;

	ret = waitpid(pid, status, 0);
	while (ret == -1 && errno == EINTR)
		ret = waitpid(pid, status, 0);
	return (ret);
}

/* Waits for one child and converts waitpid status into shell exit status. */
int	wait_for_child(pid_t pid, t_shell *sh)
{
	int	status;
	int	ret;

	if (sh->interactive)
		set_parent_wait_signals_ignored();
	ret = wait_one(pid, &status);
	if (sh->interactive)
		interactive_signals_configuration();
	if (ret == -1)
		return (perror("waitpid"), 1);
	return (handle_wait_status(status, 1));
}

/* Waits all children and returns translated status from pipeline last pid. */
int	wait_for_children(pid_t *pids, int count, pid_t last_pid, t_shell *sh)
{
	int	status;
	int	ret;
	int	i;
	int	final_status;

	if (sh->interactive)
		set_parent_wait_signals_ignored();
	final_status = 1;
	i = 0;
	while (i < count)
	{
		ret = wait_one(pids[i], &status);
		if (ret == -1)
			final_status = (perror("waitpid"), 1);
		else if (ret == last_pid)
			final_status = handle_wait_status(status, 1);
		else
			handle_wait_status(status, 0);
		i++;
	}
	if (sh->interactive)
		interactive_signals_configuration();
	return (final_status);
}
