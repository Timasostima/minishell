/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 10:30:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 17:43:50 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Returns 1 when the char starts an operator token in shell lexer context. */
static int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/* Resolves operator token type and consumed length at current position. */
static void	set_operator_meta(char *input, int i, t_token_type *type, int *len)
{
	*type = TOKEN_PIPE;
	*len = 1;
	if (input[i] == '<' && input[i + 1] == '<')
		*type = TOKEN_HEREDOC;
	else if (input[i] == '<')
		*type = TOKEN_REDIR_IN;
	else if (input[i] == '>' && input[i + 1] == '>')
		*type = TOKEN_REDIR_APPEND;
	else if (input[i] == '>')
		*type = TOKEN_REDIR_OUT;
	if (*type == TOKEN_HEREDOC || *type == TOKEN_REDIR_APPEND)
		*len = 2;
}

/* Creates and appends one operator token and updates current input index. */
static int	add_operator_token(t_token **tokens, char *input, int *i)
{
	t_token_type	type;
	t_token			*new_token;
	char			*value;
	int				len;

	set_operator_meta(input, *i, &type, &len);
	value = ft_substr(input, *i, len);
	if (value == NULL)
		return (0);
	new_token = lexer_new_token(type, value, 0);
	if (new_token == NULL)
		return (free(value), 0);
	lexer_add_back(tokens, new_token);
	*i += len;
	return (1);
}

/* Creates and appends one WORD token parsed from current input position. */
static int	add_word_token(t_token **tokens, char *input, int *i)
{
	t_token	*new_token;
	char	*word;
	int		quoted;

	word = lexer_extract_word(input, i, &quoted);
	if (word == NULL)
		return (0);
	new_token = lexer_new_token(TOKEN_WORD, word, quoted);
	if (new_token == NULL)
		return (free(word), 0);
	lexer_add_back(tokens, new_token);
	return (1);
}

/* Tokenizes one input command line into lexer token list with syntax checks. */
t_token	*lexer_tokenize(char *input)
{
	t_token	*tokens;
	int		i;

	if (input == NULL)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (is_operator_char(input[i]))
		{
			if (!add_operator_token(&tokens, input, &i))
				return (lexer_free_tokens(&tokens), NULL);
		}
		else if (!add_word_token(&tokens, input, &i))
			return (lexer_free_tokens(&tokens), NULL);
	}
	if (!lexer_validate_syntax(tokens))
		return (lexer_free_tokens(&tokens), NULL);
	return (tokens);
}
