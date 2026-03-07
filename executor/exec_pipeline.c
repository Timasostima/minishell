/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:23:20 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Counts number of command nodes in the parsed pipeline list. */
static int	count_commands(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

/* Waits already-spawned children after launch errors to avoid zombies. */
static void	wait_spawned_children(pid_t *pids, int created, t_shell *sh)
{
	if (created > 0)
		wait_for_children(pids, created, pids[created - 1], sh);
}

/* Executes parsed command table by spawning and waiting full pipeline. */
int	execute_cmd_table(t_cmd *cmds, t_shell *sh)
{
	pid_t	*pids;
	pid_t	last_pid;
	int		count;
	int		created;

	count = count_commands(cmds);
	if (count == 0)
		return (0);
	pids = ft_calloc((size_t)count, sizeof(pid_t));
	if (pids == NULL)
		return (1);
	last_pid = -1;
	created = launch_pipeline(cmds, sh, pids, &last_pid);
	if (created < 0)
	{
		created = -created - 1;
		wait_spawned_children(pids, created, sh);
		return (free(pids), 1);
	}
	count = wait_for_children(pids, created, last_pid, sh);
	free(pids);
	return (count);
}
