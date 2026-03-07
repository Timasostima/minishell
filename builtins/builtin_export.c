/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:34:26 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:20:00 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Validates export key chars for plain KEY, KEY=VALUE or KEY+=VALUE syntax. */
static int	is_valid_export_key(char *arg, int key_end)
{
	int	i;

	if (arg == NULL || key_end <= 0)
		return (0);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	i = 1;
	while (i < key_end)
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/* Extracts export metadata for KEY, KEY=VALUE and KEY+=VALUE forms. */
static void	parse_export_arg(char *arg, int *key_end, char **value, int *append)
{
	char	*equal;

	*value = NULL;
	*append = 0;
	*key_end = (int)ft_strlen(arg);
	equal = ft_strchr(arg, '=');
	if (equal == NULL)
		return ;
	*value = equal + 1;
	*key_end = (int)(equal - arg);
	if (*key_end > 0 && arg[*key_end - 1] == '+')
	{
		*append = 1;
		(*key_end)--;
	}
}

/* Appends VALUE to KEY current value (or sets KEY to VALUE when missing). */
static int	append_export_value(char *key, char *value, t_shell *sh)
{
	char	*current;
	char	*joined;
	int		status;

	current = env_get(sh->env, key);
	if (current == NULL)
		return (env_set(&sh->env, key, value));
	joined = ft_strjoin(current, value);
	if (joined == NULL)
		return (1);
	status = env_set(&sh->env, key, joined);
	free(joined);
	return (status);
}

/* Applies one export argument as key-only, assignment or append assignment. */
static int	apply_export_arg(char *arg, t_shell *sh)
{
	char	*key;
	char	*value;
	int		key_end;
	int		append;
	int		status;

	parse_export_arg(arg, &key_end, &value, &append);
	if (!is_valid_export_key(arg, key_end))
		return (ft_putstr_fd("minishell: export: `", STDERR_FILENO),
			ft_putstr_fd(arg, STDERR_FILENO),
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO), 1);
	if (value == NULL)
		return (env_set(&sh->env, arg, env_get(sh->env, arg)));
	key = ft_substr(arg, 0, (size_t)key_end);
	if (key == NULL)
		return (1);
	if (append)
		status = append_export_value(key, value, sh);
	else
		status = env_set(&sh->env, key, value);
	free(key);
	return (status);
}

/* Implements export builtin by mutating shell environment map entries. */
int	builtin_export(char **argv, t_shell *sh)
{
	int	i;
	int	status;

	if (argv[1] == NULL)
		return (print_export_sorted(sh));
	status = 0;
	i = 1;
	while (argv[i])
	{
		if (apply_export_arg(argv[i], sh) != 0)
			status = 1;
		i++;
	}
	return (status);
}
