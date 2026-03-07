/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 12:58:37 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/04 12:58:37 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Prints environment entries with '=' and rejects extra arguments for now. */
int	builtin_env(char **argv, t_shell *sh)
{
	t_env	*current;

	if (argv[1] != NULL)
		return (1);
	current = sh->env;
	while (current)
	{
		if (current->value != NULL)
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			write(STDOUT_FILENO, "=", 1);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			write(STDOUT_FILENO, "\n", 1);
		}
		current = current->next;
	}
	return (0);
}
