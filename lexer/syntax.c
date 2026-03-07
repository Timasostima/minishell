/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 10:25:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 17:42:35 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Returns 1 when token type is any redirection operator token. */
static int	is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

/* Prints lexer syntax error near token text and returns failure status. */
static int	syntax_error(char *near)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd(near, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	return (0);
}

/* Validates basic operator sequence grammar on the produced token list. */
int	lexer_validate_syntax(t_token *tokens)
{
	t_token	*prev;
	t_token	*cur;

	prev = NULL;
	cur = tokens;
	while (cur)
	{
		if (cur->type == TOKEN_PIPE
			&& (prev == NULL || prev->type == TOKEN_PIPE))
			return (syntax_error("|"));
		if (cur->type == TOKEN_PIPE && cur->next == NULL)
			return (syntax_error("newline"));
		if (is_redir_token(cur->type) && cur->next == NULL)
			return (syntax_error("newline"));
		if (is_redir_token(cur->type) && cur->next->type != TOKEN_WORD)
			return (syntax_error(cur->next->value));
		prev = cur;
		cur = cur->next;
	}
	return (1);
}
