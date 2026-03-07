/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 12:25:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 17:51:35 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Maps lexer redirection token type to parser redirection enum value. */
static t_redir_type	get_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	if (type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	return (REDIR_HEREDOC);
}

/* Creates one redirection node and duplicates target token string value. */
static t_redir	*parser_new_redir(t_redir_type type, char *target, int quoted)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (redir == NULL)
		return (NULL);
	redir->type = type;
	redir->target = ft_strdup(target);
	if (redir->target == NULL)
		return (free(redir), NULL);
	redir->target_quoted = quoted;
	redir->heredoc_quoted = 0;
	if (type == REDIR_HEREDOC)
		redir->heredoc_quoted = quoted;
	redir->heredoc_fd = -1;
	redir->next = NULL;
	return (redir);
}

/* Appends one redirection node at the end of command redirection list. */
static void	parser_add_redir_back(t_redir **redirs, t_redir *new_redir)
{
	t_redir	*current;

	if (*redirs == NULL)
	{
		*redirs = new_redir;
		return ;
	}
	current = *redirs;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

/* Creates and appends redirection node from operator and target tokens. */
int	parser_add_redir_from_tokens(t_redir **redirs, t_token *op, t_token *arg)
{
	t_redir	*new_redir;

	if (op == NULL || arg == NULL || arg->type != TOKEN_WORD)
		return (0);
	new_redir = parser_new_redir(get_redir_type(op->type),
			arg->value, arg->quoted);
	if (new_redir == NULL)
		return (0);
	parser_add_redir_back(redirs, new_redir);
	return (1);
}
