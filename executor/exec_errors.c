/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:26:01 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:20:00 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Releases temporary arrays and path strings built for one external command. */
static void	free_exec_data(char **paths, char *cmd_path, char **envp)
{
	if (paths)
		ft_free_split(paths);
	if (cmd_path)
		free(cmd_path);
	if (envp)
		ft_free_split(envp);
}

/* Maps execve failure errno/state to the shell-compatible command exit code. */
static int	get_exec_error_code(char *cmd_path)
{
	struct stat	st;

	if (cmd_path && stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
		return (126);
	if (errno == EACCES || errno == EISDIR || errno == ENOEXEC)
		return (126);
	if (errno == ENOENT)
		return (127);
	return (1);
}

/* Prints shell-like diagnostic text for one failed external command launch. */
static void	print_exec_error(char *cmd_name, char *cmd_path, int exit_code,
		int path_missing)
{
	struct stat	st;

	if (exit_code == 127 && cmd_name && !ft_strchr(cmd_name, '/'))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		if (path_missing)
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		else
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else if (cmd_path && stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd_name);
	}
}

/* Launches one external command and handles cleanup and diagnostics on fail. */
static int	launch_external_cmd(t_cmd *cmd, t_shell *sh, char **paths,
		int path_missing)
{
	char	*cmd_path;
	char	**envp;
	int		exit_code;

	cmd_path = build_cmd_path(paths, cmd->argv[0]);
	if (cmd_path == NULL)
		return (free_exec_data(paths, NULL, NULL),
			print_exec_error(cmd->argv[0], NULL, 127, path_missing), 127);
	envp = env_to_envp(sh->env);
	if (envp == NULL)
		return (free_exec_data(paths, cmd_path, NULL), 1);
	execve(cmd_path, cmd->argv, envp);
	exit_code = get_exec_error_code(cmd_path);
	print_exec_error(cmd->argv[0], cmd_path, exit_code, path_missing);
	free_exec_data(paths, cmd_path, envp);
	return (exit_code);
}

/* Executes external command and returns code if execve fails in child. */
int	execute_external_cmd(t_cmd *cmd, t_shell *sh)
{
	char	*path;
	char	**paths;
	int		path_missing;

	path = env_get(sh->env, "PATH");
	path_missing = (path == NULL);
	if (path == NULL && sh->use_default_path)
	{
		path = "/usr/local/sbin:/usr/local/bin:/usr/sbin:"
			"/usr/bin:/sbin:/bin";
		path_missing = 0;
	}
	paths = NULL;
	if (path != NULL)
		paths = ft_split(path, ':');
	if (path != NULL && paths == NULL)
		return (1);
	return (launch_external_cmd(cmd, sh, paths, path_missing));
}
