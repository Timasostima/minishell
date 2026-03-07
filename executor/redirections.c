/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:26:50 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Returns 1 when command contains one input source redirection operator. */
int	cmd_has_input_redir(t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
			return (1);
		redir = redir->next;
	}
	return (0);
}

/* Returns 1 when command contains one output target redirection operator. */
int	cmd_has_output_redir(t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
			return (1);
		redir = redir->next;
	}
	return (0);
}

/* Opens one redirection target and returns its file descriptor or failure. */
static int	open_redir_fd(t_redir *redir)
{
	if (redir->type == REDIR_IN)
		return (open(redir->target, O_RDONLY));
	if (redir->type == REDIR_OUT)
		return (open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (redir->type == REDIR_APPEND)
		return (open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644));
	if (redir->heredoc_fd < 0)
		return (-2);
	return (redir->heredoc_fd);
}

/* Duplicates one opened redirection fd and closes the original descriptor. */
static int	dup_redir_fd(t_redir *redir, int fd)
{
	int	target_fd;

	target_fd = STDOUT_FILENO;
	if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		target_fd = STDIN_FILENO;
	if (dup2(fd, target_fd) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		return (close(fd), perror("dup2"), 1);
	}
	close(fd);
	if (redir->type == REDIR_HEREDOC)
		redir->heredoc_fd = -1;
	return (0);
}

/* Applies command redirections in order using open, dup2 and close. */
int	apply_redirections(t_redir *redirs)
{
	int	fd;

	while (redirs)
	{
		fd = open_redir_fd(redirs);
		if (fd == -2)
		{
			ft_putstr_fd("minishell: invalid heredoc input\n", STDERR_FILENO);
			return (1);
		}
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			return (perror(redirs->target), 1);
		}
		if (dup_redir_fd(redirs, fd) != 0)
			return (1);
		redirs = redirs->next;
	}
	return (0);
}
