/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 12:31:40 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/04 12:31:40 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Checks whether command name matches one of minishell builtin identifiers. */
int	is_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

/* Checks if command must be executed in parent shell process context. */
int	is_parent_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	return (ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

/* Dispatches builtin implementation and returns shell-compatible status. */
int	exec_builtin(char **argv, t_shell *sh)
{
	if (argv == NULL || argv[0] == NULL)
		return (1);
	if (ft_strcmp(argv[0], "echo") == 0)
		return (builtin_echo(argv));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (builtin_cd(argv, sh));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (builtin_pwd(argv, sh));
	if (ft_strcmp(argv[0], "export") == 0)
		return (builtin_export(argv, sh));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (builtin_unset(argv, sh));
	if (ft_strcmp(argv[0], "env") == 0)
		return (builtin_env(argv, sh));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (builtin_exit(argv, sh));
	return (1);
}
