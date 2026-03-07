/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_argv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:24:50 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_text(char **dst, char *text, int protect_ws)
{
	char	ch[2];
	int		i;
	char	*tmp;

	i = 0;
	ch[1] = '\0';
	while (text[i])
	{
		ch[0] = text[i];
		if (protect_ws && ft_isspace(ch[0]))
			ch[0] = '\1';
		tmp = ft_strjoin(*dst, ch);
		if (tmp == NULL)
			return (1);
		free(*dst);
		*dst = tmp;
		i++;
	}
	return (0);
}

static int	expand_status_dollar(char **dst, int *i, t_argv_exp *ctx)
{
	char	*status;
	int		ret;

	status = ft_itoa(ctx->sh->last_status);
	if (status == NULL)
		return (1);
	*i += 2;
	ret = append_text(dst, status, ctx->quote == '"');
	free(status);
	return (ret);
}

static int	expand_dollar(char **dst, char *raw, int *i, t_argv_exp *ctx)
{
	char	*key;
	char	*value;
	int		start;

	if (raw[*i + 1] == '?')
		return (expand_status_dollar(dst, i, ctx));
	if (ft_isdigit(raw[*i + 1]))
		return (*i += 2, 0);
	if (!ft_isalpha(raw[*i + 1]) && raw[*i + 1] != '_')
		return ((*i)++, append_text(dst, "$", ctx->quote != 0));
	start = ++(*i);
	while (ft_isalnum(raw[*i]) || raw[*i] == '_')
		(*i)++;
	key = ft_substr(raw, (unsigned int)start, (size_t)(*i - start));
	if (key == NULL)
		return (1);
	value = env_get(ctx->sh->env, key);
	if (value == NULL && ctx->sh->use_default_path
		&& ft_strcmp(key, "PATH") == 0)
		value = "/usr/local/sbin:/usr/local/bin:/usr/sbin:"
			"/usr/bin:/sbin:/bin";
	free(key);
	if (value == NULL)
		return (0);
	return (append_text(dst, value, ctx->quote == '"'));
}

static int	expand_one(char **expanded, char *raw, int *i, t_argv_exp *ctx)
{
	char	quote;

	quote = ctx->quote;
	if ((raw[*i] == '\'' || raw[*i] == '"')
		&& (quote == 0 || quote == raw[*i]))
	{
		ctx->quote = raw[(*i)++] * (quote == 0);
		return (0);
	}
	if (raw[*i] == '$' && quote != '\'')
	{
		return (expand_dollar(expanded, raw, i, ctx));
	}
	return (append_text(expanded, (char []){raw[(*i)++], '\0'}, quote != 0));
}

char	*expand_word_argv_value(char *raw, t_shell *sh)
{
	t_argv_exp	ctx;
	char		*expanded;
	int			i;

	expanded = ft_strdup("");
	if (expanded == NULL)
		return (NULL);
	i = 0;
	ctx.sh = sh;
	ctx.quote = 0;
	while (raw[i])
	{
		if (expand_one(&expanded, raw, &i, &ctx) != 0)
			return (free(expanded), NULL);
	}
	return (expanded);
}
