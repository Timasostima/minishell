/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 12:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 17:50:10 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Returns 1 when token type corresponds to one redirection operator token. */
int	parser_token_is_redir(int type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

/* Frees the complete linked list of redirection nodes. */
static void	parser_free_redirs(t_redir **redirs)
{
	t_redir	*current;
	t_redir	*next;

	if (redirs == NULL)
		return ;
	current = *redirs;
	while (current)
	{
		next = current->next;
		if (current->heredoc_fd >= 0)
			close(current->heredoc_fd);
		free(current->target);
		free(current);
		current = next;
	}
	*redirs = NULL;
}

/* Frees command list with argv arrays and associated redirection lists. */
void	parser_free_cmds(t_cmd **cmds)
{
	t_cmd	*current;
	t_cmd	*next;

	if (cmds == NULL)
		return ;
	current = *cmds;
	while (current)
	{
		next = current->next;
		if (current->argv)
			ft_free_split(current->argv);
		parser_free_redirs(&current->redirs);
		free(current);
		current = next;
	}
	*cmds = NULL;
}
