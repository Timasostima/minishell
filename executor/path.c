/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:53:36 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/04 11:58:18 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Builds a command path candidate from one PATH directory and command name. */
static char	*join_path_cmd(char *dir, char *cmd)
{
	char	*path_slash;
	char	*path;

	path_slash = ft_strjoin(dir, "/");
	if (path_slash == NULL)
		return (NULL);
	path = ft_strjoin(path_slash, cmd);
	free(path_slash);
	return (path);
}

/* Resolves command path when PATH is unavailable using current directory. */
static char	*build_relative_cmd(char *cmd)
{
	char	*path;

	path = ft_strjoin("./", cmd);
	if (path == NULL)
		return (NULL);
	if (access(path, X_OK) != 0)
		return (free(path), NULL);
	return (path);
}

/* Searches executable command in PATH directories and returns full path. */
static char	*find_cmd_in_paths(char **split_paths, char *cmd)
{
	char	*path;
	int		i;

	i = 0;
	while (split_paths[i] != NULL)
	{
		path = join_path_cmd(split_paths[i], cmd);
		if (path == NULL)
			return (NULL);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

/* Resolves external command path using slash-path or PATH directories. */
char	*build_cmd_path(char **split_paths, char *cmd)
{
	if (cmd == NULL || cmd[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	if (split_paths == NULL)
		return (build_relative_cmd(cmd));
	return (find_cmd_in_paths(split_paths, cmd));
}
