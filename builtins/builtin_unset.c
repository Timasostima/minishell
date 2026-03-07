/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:35:56 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/04 15:35:56 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Validates unset identifier as shell name without '=' separator chars. */
static int	is_valid_unset_key(char *arg)
{
	int	i;

	if (arg == NULL || arg[0] == '\0')
		return (0);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/* Removes each provided key from shell environment when identifier is valid. */
int	builtin_unset(char **argv, t_shell *sh)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (is_valid_unset_key(argv[i]))
		{
			if (ft_strcmp(argv[i], "PATH") == 0)
				sh->use_default_path = 0;
			env_unset(&sh->env, argv[i]);
		}
		i++;
	}
	return (0);
}
