/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 10:15:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 17:41:10 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Allocates one lexer token node with provided type, value and quoted state. */
t_token	*lexer_new_token(t_token_type type, char *value, int quoted)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->type = type;
	token->value = value;
	token->quoted = quoted;
	token->next = NULL;
	return (token);
}

/* Appends a token node to the end of the lexer token linked list. */
void	lexer_add_back(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (new_token == NULL)
		return ;
	if (*tokens == NULL)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/* Frees the complete lexer token list and clears the list pointer. */
void	lexer_free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	if (tokens == NULL)
		return ;
	current = *tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	*tokens = NULL;
}
