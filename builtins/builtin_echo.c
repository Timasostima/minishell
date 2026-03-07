/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 12:57:29 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/04 12:57:29 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Returns 1 only for valid -n echo flags like -n, -nn, -nnnn. */
static int	is_n_flag(char *arg)
{
	int	i;

	if (arg == NULL || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

/* Prints arguments like bash echo and handles repeated -n options. */
int	builtin_echo(char **argv)
{
	int	i;
	int	print_newline;

	i = 1;
	print_newline = 1;
	while (argv[i] && is_n_flag(argv[i]))
	{
		print_newline = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (print_newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
