/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:24:14 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/06 16:10:00 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Prints cd builtin message to stderr using shell-like error text. */
static int	cd_error(char *msg)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	return (1);
}

/* Prints chdir failure including target path for bash-like diagnostic text. */
static int	cd_target_error(char *target)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(target, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	return (1);
}

/* Resolves cd target path from argv and shell environment variables. */
static char	*resolve_cd_target(char **argv, t_shell *sh, int *print_new_pwd)
{
	char	*target;

	*print_new_pwd = 0;
	if (argv[1] && argv[2])
		return (cd_error("too many arguments"), NULL);
	if (argv[1] == NULL)
	{
		target = env_get(sh->env, "HOME");
		if (target == NULL)
			return (cd_error("HOME not set"), NULL);
		return (target);
	}
	if (ft_strcmp(argv[1], "-") == 0)
	{
		*print_new_pwd = 1;
		target = env_get(sh->env, "OLDPWD");
		if (target == NULL)
			return (cd_error("OLDPWD not set"), NULL);
		return (target);
	}
	return (argv[1]);
}

/* Builds relative logical target when current PWD is already relative. */
static char	*build_logical_target(t_shell *sh, char *target)
{
	char	*oldpwd;
	char	*tmp;
	char	*joined;

	oldpwd = env_get(sh->env, "PWD");
	if (oldpwd == NULL || oldpwd[0] == '/' || target[0] == '/')
		return (NULL);
	tmp = ft_strjoin(oldpwd, "/");
	if (tmp == NULL)
		return (NULL);
	joined = ft_strjoin(tmp, target);
	free(tmp);
	return (joined);
}

/* Implements cd builtin with HOME/OLDPWD handling and env updates. */
int	builtin_cd(char **argv, t_shell *sh)
{
	char	*target;
	char	*logical_target;
	char	cwd[4096];
	int		print_new_pwd;
	int		had_cwd_before;

	target = resolve_cd_target(argv, sh, &print_new_pwd);
	if (target == NULL)
		return (1);
	logical_target = build_logical_target(sh, target);
	had_cwd_before = (getcwd(cwd, sizeof(cwd)) != NULL);
	if ((logical_target != NULL && chdir(logical_target) != 0)
		|| (logical_target == NULL && chdir(target) != 0))
		return (free(logical_target), cd_target_error(target));
	if (cd_update_pwd_after_cd(sh, target, had_cwd_before) != 0)
		return (free(logical_target), 1);
	if (print_new_pwd)
		cd_print_target(target);
	free(logical_target);
	return (0);
}
