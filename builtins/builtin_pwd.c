/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 12:58:07 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/04 12:58:07 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Prints bash-like pwd failure message for unreachable physical cwd. */
static int	pwd_error(void)
{
	ft_putendl_fd("pwd: error retrieving current directory: "
		"getcwd: cannot access parent directories: "
		"No such file or directory", STDERR_FILENO);
	return (1);
}

/* Prints current working directory and returns shell-compatible status. */
int	builtin_pwd(char **argv, t_shell *sh)
{
	char	cwd[4096];
	char	*logical_pwd;

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		logical_pwd = env_get(sh->env, "PWD");
		if (logical_pwd && logical_pwd[0] != '/')
		{
			ft_putendl_fd(logical_pwd, STDOUT_FILENO);
			return (0);
		}
		return (pwd_error());
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}
