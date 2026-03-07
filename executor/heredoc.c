/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:27:30 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_heredoc_line(int interactive, char *prompt)
{
	char	*line;
	size_t	len;

	if (interactive)
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	line = get_next_line(STDIN_FILENO);
	if (line == NULL)
		return (NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

/* Prints warning when heredoc ends on EOF before finding the delimiter. */
static void	print_heredoc_eof_warning(char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document at line 1 ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

static int	collect_one_heredoc(t_redir *redir, t_shell *sh, int interactive)
{
	int		pipefd[2];
	char	*line;
	char	*prompt;

	prompt = "";
	if (interactive)
		prompt = "> ";
	if (pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	while (1)
	{
		line = read_heredoc_line(interactive, prompt);
		if (g_signal == SIGINT)
			return (free(line), close(pipefd[0]), close(pipefd[1]), 130);
		if (line == NULL)
			return (print_heredoc_eof_warning(redir->target), close(pipefd[1]),
				redir->heredoc_fd = pipefd[0], 0);
		if (ft_strcmp(line, redir->target) == 0)
			return (free(line), close(pipefd[1]), redir->heredoc_fd = pipefd[0],
				0);
		if (write_heredoc_expanded_line(pipefd[1], line, redir, sh) != 0)
			return (free(line), close(pipefd[0]), close(pipefd[1]), 1);
		free(line);
	}
}

static int	collect_cmd_heredocs(t_cmd *cmd, t_shell *sh, int interactive)
{
	t_redir	*redir;
	int		status;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			status = collect_one_heredoc(redir, sh, interactive);
			if (status != 0)
				return (status);
		}
		redir = redir->next;
	}
	return (0);
}

int	prepare_heredocs(t_cmd *cmds, t_shell *sh)
{
	t_cmd	*head;
	int		status;

	head = cmds;
	g_signal = 0;
	set_heredoc_signals();
	while (cmds)
	{
		status = collect_cmd_heredocs(cmds, sh, sh->interactive);
		if (status != 0)
		{
			close_heredoc_fds(head);
			restore_heredoc_signals(sh->interactive);
			return (status);
		}
		cmds = cmds->next;
	}
	restore_heredoc_signals(sh->interactive);
	return (0);
}
