/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:37:25 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/04 15:37:25 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

/* Prints exit builtin numeric argument failure with shell-like message. */
static int	exit_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	return (2);
}

/* Parses unsigned digits with overflow check against provided maximum limit. */
static int	parse_exit_digits(char *arg, int *i, unsigned long limit,
		unsigned long *acc)
{
	*acc = 0;
	if (!ft_isdigit(arg[*i]))
		return (0);
	while (ft_isdigit(arg[*i]))
	{
		if (*acc > limit / 10 || (*acc == limit / 10
				&& (unsigned long)(arg[*i] - '0') > limit % 10))
			return (0);
		*acc = *acc * 10 + (unsigned long)(arg[*i] - '0');
		(*i)++;
	}
	return (1);
}

/* Skips spaces in exit argument so quoted numeric values match bash parsing. */
static int	skip_spaces(char *arg, int i)
{
	while (arg[i] && ft_isspace((unsigned char)arg[i]))
		i++;
	return (i);
}

/* Parses exit numeric argument and rejects overflow outside long range. */
static int	parse_exit_code(char *arg, long *code)
{
	unsigned long	acc;
	unsigned long	limit;
	int				i;
	int				sign;

	i = skip_spaces(arg, 0);
	sign = 1;
	if (arg[i] == '+' || arg[i] == '-')
		sign = (arg[i++] != '-');
	limit = (unsigned long)LONG_MAX + (sign == 0);
	if (!parse_exit_digits(arg, &i, limit, &acc))
		return (0);
	i = skip_spaces(arg, i);
	if (arg[i] != '\0')
		return (0);
	if (sign)
		*code = (long)acc;
	else if (acc == (unsigned long)LONG_MAX + 1)
		*code = LONG_MIN;
	else
		*code = -(long)acc;
	return (1);
}

/* Implements exit builtin status rules and parent loop termination flagging. */
int	builtin_exit(char **argv, t_shell *sh)
{
	long	code;

	if (sh->interactive)
		write(STDOUT_FILENO, "exit\n", 5);
	if (argv[1] == NULL)
	{
		sh->should_exit = 1;
		return (sh->last_status);
	}
	if (!parse_exit_code(argv[1], &code))
	{
		sh->should_exit = 1;
		return (exit_numeric_error(argv[1]));
	}
	if (argv[2] != NULL)
		return (ft_putstr_fd("minishell: exit: too many arguments\n",
				STDERR_FILENO), 1);
	sh->should_exit = 1;
	return ((unsigned char)code);
}
