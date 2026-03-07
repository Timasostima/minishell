/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_launch.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:22:10 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Closes fd only when valid so error paths can use sentinel descriptors. */
static void	safe_close(int fd)
{
	if (fd != -1)
		close(fd);
}

/* Closes parent-side ends and stores read endpoint for the next command. */
static void	update_parent_fds(t_cmd *cmd, int *prev_read, int pipefd[2])
{
	safe_close(*prev_read);
	if (cmd->next != NULL)
	{
		safe_close(pipefd[1]);
		*prev_read = pipefd[0];
	}
	else
		*prev_read = -1;
}

/* Handles launch failure by closing fds and returning encoded count. */
static int	launch_error(int prev_read, int pipefd[2], int created, char *name)
{
	safe_close(prev_read);
	safe_close(pipefd[0]);
	safe_close(pipefd[1]);
	perror(name);
	return (-(created + 1));
}

/* Spawns every command node and returns child count or encoded launch error. */
int	launch_pipeline(t_cmd *cmds, t_shell *sh, pid_t *pids, pid_t *last_pid)
{
	int	prev_read;
	int	pipefd[2];
	int	i;

	prev_read = -1;
	i = 0;
	while (cmds)
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
		if (cmds->next != NULL && pipe(pipefd) == -1)
			return (launch_error(prev_read, pipefd, i, "pipe"));
		pids[i] = spawn_command(cmds, sh, prev_read, pipefd);
		if (pids[i] < 0)
			return (launch_error(prev_read, pipefd, i, "fork"));
		*last_pid = pids[i++];
		update_parent_fds(cmds, &prev_read, pipefd);
		cmds = cmds->next;
	}
	return (i);
}
