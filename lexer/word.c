/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 10:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 17:45:10 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Returns 1 when lexer must stop word scan at current character. */
static int	stop_word_scan(char c, char quote)
{
	if (quote != 0)
		return (0);
	if (ft_isspace(c))
		return (1);
	return (c == '|' || c == '<' || c == '>');
}

/* Scans one word range and validates quote pairing while setting quote flag. */
static int	scan_word_end(char *input, int start, int *quoted, int *end)
{
	char	quote;

	quote = 0;
	*quoted = 0;
	*end = start;
	while (input[*end])
	{
		if (stop_word_scan(input[*end], quote))
			break ;
		if (input[*end] == '\'' || input[*end] == '"')
		{
			*quoted = 1;
			if (quote == 0)
				quote = input[*end];
			else if (quote == input[*end])
				quote = 0;
		}
		(*end)++;
	}
	if (quote != 0)
		return (0);
	return (1);
}

/* Extracts one WORD token preserving quote chars for expansion stage. */
char	*lexer_extract_word(char *input, int *i, int *quoted)
{
	char	*word;
	int		end;

	if (!scan_word_end(input, *i, quoted, &end))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n",
			STDERR_FILENO);
		return (NULL);
	}
	word = ft_substr(input, (unsigned int)*i, (size_t)(end - *i));
	if (word == NULL)
		return (NULL);
	*i = end;
	return (word);
}
