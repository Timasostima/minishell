/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:24:10 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_text(char **dst, char *text)
{
	char	*tmp;

	tmp = ft_strjoin(*dst, text);
	if (tmp == NULL)
		return (1);
	free(*dst);
	*dst = tmp;
	return (0);
}

static int	expand_status_dollar(char **dst, int *i, int status)
{
	char	*code;
	int		ret;

	code = ft_itoa(status);
	if (code == NULL)
		return (1);
	*i += 2;
	ret = append_text(dst, code);
	free(code);
	return (ret);
}

static int	expand_dollar(char **dst, char *raw, int *i, t_shell *sh)
{
	char	*key;
	char	*value;
	int		start;

	if (raw[*i + 1] == '?')
		return (expand_status_dollar(dst, i, sh->last_status));
	if (ft_isdigit(raw[*i + 1]))
		return (*i += 2, 0);
	if (!ft_isalpha(raw[*i + 1]) && raw[*i + 1] != '_')
		return ((*i)++, append_text(dst, "$"));
	start = ++(*i);
	while (ft_isalnum(raw[*i]) || raw[*i] == '_')
		(*i)++;
	key = ft_substr(raw, (unsigned int)start, (size_t)(*i - start));
	if (key == NULL)
		return (1);
	value = env_get(sh->env, key);
	if (value == NULL && sh->use_default_path && ft_strcmp(key, "PATH") == 0)
		value = "/usr/local/sbin:/usr/local/bin:/usr/sbin:"
			"/usr/bin:/sbin:/bin";
	free(key);
	if (value == NULL)
		return (0);
	return (append_text(dst, value));
}

static int	is_quote_char(char c, char quote)
{
	return ((c == '\'' || c == '"') && (quote == 0 || quote == c));
}

char	*expand_word_value(char *raw, t_shell *sh, int allow_vars)
{
	char	*expanded;
	char	quote;
	int		i;

	expanded = ft_strdup("");
	if (expanded == NULL)
		return (NULL);
	quote = 0;
	i = 0;
	while (raw[i])
	{
		if (is_quote_char(raw[i], quote))
		{
			quote = raw[i] * (quote == 0);
			i++;
		}
		else if (raw[i] == '$' && quote != '\'' && allow_vars)
		{
			if (expand_dollar(&expanded, raw, &i, sh) != 0)
				return (free(expanded), NULL);
		}
		else if (append_text(&expanded, (char []){raw[i++], '\0'}) != 0)
			return (free(expanded), NULL);
	}
	return (expanded);
}
