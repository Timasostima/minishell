/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:53:36 by ibenaven          #+#    #+#             */
/*   Updated: 2026/02/14 14:05:15 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_cmd_path(char **split_paths, char *cmd)
{
	char	*path;
	char	*dup_path;
	char	*path_slash;
	int	i;


	if (cmd == NULL || cmd[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	if (split_paths == NULL)
	{
		path = ft_strjoin("./", cmd);
		if (access(path, X_OK) != 0)
			return (free(path), NULL);	
		return (path);
	}
	i = 0;
	while (split_paths[i] != NULL)
	{
		dup_path = ft_strdup(split_paths[i]);
		path_slash = ft_strjoin(dup_path, "/");
		free (dup_path);
		path = ft_strjoin(path_slash, cmd);
		free (path_slash);
		if (access(path, X_OK) == 0)
			return (path);
		free (path);
		i++;
	}
	return (NULL);
}
