/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:28:10 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_hd_text(char **dst, char *text)
{
	char	*tmp;

	tmp = ft_strjoin(*dst, text);
	if (tmp == NULL)
		return (1);
	free(*dst);
	*dst = tmp;
	return (0);
}

static int	append_hd_env_value(char **dst, char *raw, int *i, t_shell *sh)
{
	char	*key;
	char	*value;
	int		start;

	start = *i;
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
	return (append_hd_text(dst, value));
}

static int	expand_hd_dollar(char **dst, char *raw, int *i, t_shell *sh)
{
	char	*status_txt;
	int		ret;

	if (raw[*i + 1] == '?')
	{
		status_txt = ft_itoa(sh->last_status);
		if (status_txt == NULL)
			return (1);
		*i += 2;
		ret = append_hd_text(dst, status_txt);
		free(status_txt);
		return (ret);
	}
	if (ft_isdigit(raw[*i + 1]))
		return (*i += 2, 0);
	if (!ft_isalpha(raw[*i + 1]) && raw[*i + 1] != '_')
		return ((*i)++, append_hd_text(dst, "$"));
	(*i)++;
	return (append_hd_env_value(dst, raw, i, sh));
}

char	*expand_heredoc_line(char *raw, t_shell *sh)
{
	char	*expanded;
	int		i;

	expanded = ft_strdup("");
	if (expanded == NULL)
		return (NULL);
	i = 0;
	while (raw[i])
	{
		if (raw[i] == '$' && expand_hd_dollar(&expanded, raw, &i, sh) != 0)
			return (free(expanded), NULL);
		else if (raw[i] != '$'
			&& append_hd_text(&expanded, (char []){raw[i++], '\0'}) != 0)
			return (free(expanded), NULL);
	}
	return (expanded);
}

int	write_heredoc_expanded_line(int fd, char *line, t_redir *redir, t_shell *sh)
{
	char	*content;
	int		ret;

	if (redir->heredoc_quoted)
		content = ft_strdup(line);
	else
		content = expand_heredoc_line(line, sh);
	if (content == NULL)
		return (1);
	ret = 0;
	if (write(fd, content, ft_strlen(content)) == -1)
		ret = 1;
	if (ret == 0 && write(fd, "\n", 1) == -1)
		ret = 1;
	free(content);
	return (ret);
}
