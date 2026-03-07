/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   underscore.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/06 11:09:10 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Returns command lookup PATH value honoring startup fallback toggle state. */
static char	*get_lookup_path(t_shell *sh)
{
	char	*path;

	path = env_get(sh->env, "PATH");
	if (path == NULL && sh->use_default_path)
		return ("/usr/local/sbin:/usr/local/bin:/usr/sbin:"
			"/usr/bin:/sbin:/bin");
	return (path);
}

/* Returns first executable command node from parsed pipeline list. */
static t_cmd	*get_ref_cmd(t_cmd *cmds)
{
	while (cmds && (cmds->argv == NULL || cmds->argv[0] == NULL))
		cmds = cmds->next;
	return (cmds);
}

/* Resolves command to absolute executable path using env PATH or fallback. */
static char	*resolve_cmd_path(t_shell *sh, char *cmd)
{
	char		*path;
	char		**paths;
	char		*resolved;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = get_lookup_path(sh);
	if (path == NULL)
		return (NULL);
	paths = ft_split(path, ':');
	if (paths == NULL)
		return (NULL);
	resolved = build_cmd_path(paths, cmd);
	ft_free_split(paths);
	return (resolved);
}

/* Builds '_' value from command argv and command path resolution. */
static char	*build_underscore_value(t_shell *sh, char **argv)
{
	int		argc;
	char	*value;

	argc = 0;
	while (argv[argc])
		argc++;
	if (argc > 1)
		return (ft_strdup(argv[argc - 1]));
	value = resolve_cmd_path(sh, argv[0]);
	if (value != NULL)
		return (value);
	return (ft_strdup(argv[0]));
}

/* Updates '_' in shell env before command execution. */
int	update_underscore_var(t_cmd *cmds, t_shell *sh)
{
	t_cmd	*ref;
	char	*value;

	ref = get_ref_cmd(cmds);
	if (ref == NULL || ref->argv == NULL || ref->argv[0] == NULL)
		return (0);
	value = build_underscore_value(sh, ref->argv);
	if (value == NULL)
		return (1);
	if (env_set(&sh->env, "_", value) != 0)
		return (free(value), 1);
	free(value);
	return (0);
}
