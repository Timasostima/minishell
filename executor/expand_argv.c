/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:25:30 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unprotect_quoted_spaces(char *value)
{
	int	i;

	i = 0;
	while (value && value[i])
	{
		if (value[i] == '\1')
			value[i] = ' ';
		i++;
	}
}

static int	append_split_fields(char **new_argv, int *write_i, char *value)
{
	int		i;
	int		start;
	char	*field;

	i = 0;
	while (value[i])
	{
		while (value[i] && ft_isspace(value[i]))
			i++;
		if (!value[i])
			break ;
		start = i;
		while (value[i] && !ft_isspace(value[i]))
			i++;
		field = ft_substr(value, (unsigned int)start, (size_t)(i - start));
		if (field == NULL)
			return (1);
		unprotect_quoted_spaces(field);
		new_argv[*write_i] = field;
		(*write_i)++;
	}
	return (0);
}

static int	append_expanded_arg(char *raw, t_shell *sh, char **new_argv, int *i)
{
	char	*expanded;

	expanded = expand_word_argv_value(raw, sh);
	if (expanded == NULL)
		return (1);
	if (expanded[0] == '\0' && !ft_strchr(raw, '\'') && !ft_strchr(raw, '"'))
		free(expanded);
	else if (expanded[0] == '\0')
		new_argv[(*i)++] = expanded;
	else
	{
		if (append_split_fields(new_argv, i, expanded) != 0)
			return (free(expanded), 1);
		free(expanded);
	}
	return (0);
}

static int	fill_expanded_argv(t_cmd *cmd, t_shell *sh, char **new_argv)
{
	int	read_i;
	int	write_i;

	read_i = 0;
	write_i = 0;
	while (cmd->argv && cmd->argv[read_i])
	{
		if (append_expanded_arg(cmd->argv[read_i], sh,
				new_argv, &write_i) != 0)
			return (1);
		free(cmd->argv[read_i]);
		read_i++;
	}
	new_argv[write_i] = NULL;
	return (0);
}

int	expand_argv_words(t_cmd *cmd, t_shell *sh)
{
	char	**new_argv;
	char	*tmp;
	int		max_slots;
	int		i;

	max_slots = 1;
	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		tmp = expand_word_argv_value(cmd->argv[i], sh);
		if (tmp == NULL)
			return (1);
		max_slots += (int)ft_strlen(tmp) + 1;
		free(tmp);
		i++;
	}
	new_argv = ft_calloc((size_t)max_slots, sizeof(char *));
	if (new_argv == NULL)
		return (1);
	if (fill_expanded_argv(cmd, sh, new_argv) != 0)
		return (ft_free_split(new_argv), 1);
	free(cmd->argv);
	cmd->argv = new_argv;
	return (0);
}
