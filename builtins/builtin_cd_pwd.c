/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_pwd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 16:10:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/06 18:05:20 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Joins logical PWD and relative target in broken-physical-cwd situations. */
static char	*join_logical_pwd(char *pwd, char *target)
{
	char	*tmp;
	char	*joined;

	if (pwd == NULL || *pwd == '\0')
		return (ft_strdup(target));
	tmp = ft_strjoin(pwd, "/");
	if (tmp == NULL)
		return (NULL);
	joined = ft_strjoin(tmp, target);
	free(tmp);
	return (joined);
}

/* Stores relative logical PWD using previous PWD chain and requested target. */
static int	set_relative_pwd(t_shell *sh, char *oldpwd, char *target)
{
	char	*newpwd;

	if (oldpwd && oldpwd[0] != '/')
		newpwd = join_logical_pwd(oldpwd, target);
	else
		newpwd = ft_strdup(target);
	if (newpwd == NULL)
		return (1);
	if (env_set(&sh->env, "PWD", newpwd) != 0)
		return (free(newpwd), 1);
	free(newpwd);
	return (0);
}

/* Prints bash-like warning after leaving an unreachable current directory. */
static void	print_cwd_warning(char *oldpwd)
{
	if (oldpwd == NULL || oldpwd[0] != '/')
		return ;
	ft_putstr_fd("chdir: error retrieving current directory: ", STDERR_FILENO);
	ft_putstr_fd("getcwd: cannot access parent directories: ", STDERR_FILENO);
	ft_putendl_fd("No such file or directory", STDERR_FILENO);
}

/* Updates PWD/OLDPWD after successful chdir with physical and logical modes. */
int	cd_update_pwd_after_cd(t_shell *sh, char *target, int had_cwd_before)
{
	char	cwd[4096];
	char	*oldpwd;

	oldpwd = env_get(sh->env, "PWD");
	if (oldpwd && env_set(&sh->env, "OLDPWD", oldpwd) != 0)
		return (1);
	if (oldpwd && oldpwd[0] != '/' && target[0] != '/')
		return (set_relative_pwd(sh, oldpwd, target));
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return (env_set(&sh->env, "PWD", cwd));
	if (had_cwd_before || target[0] == '/')
		return (0);
	print_cwd_warning(oldpwd);
	return (set_relative_pwd(sh, oldpwd, target));
}

/* Prints directory for cd '-' preferring physical cwd when available. */
void	cd_print_target(char *target)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_putendl_fd(cwd, STDOUT_FILENO);
	else
		ft_putendl_fd(target, STDOUT_FILENO);
}
