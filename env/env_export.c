/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:10:11 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/04 14:10:11 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Counts environment entries that should be exported through execve envp. */
static int	env_count_exported(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->value != NULL)
			count++;
		env = env->next;
	}
	return (count);
}

/* Builds one KEY=VALUE environment line for execve array format. */
static char	*build_env_line(t_env *entry)
{
	char	*line;
	char	*tmp;

	tmp = ft_strjoin(entry->key, "=");
	if (tmp == NULL)
		return (NULL);
	line = ft_strjoin(tmp, entry->value);
	free(tmp);
	return (line);
}

/* Converts linked-list environment to a NULL-terminated execve-ready array. */
char	**env_to_envp(t_env *env)
{
	char	**envp;
	int		i;

	envp = ft_calloc(env_count_exported(env) + 1, sizeof(char *));
	if (envp == NULL)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value != NULL)
		{
			envp[i] = build_env_line(env);
			if (envp[i] == NULL)
				return (ft_free_split(envp), NULL);
			i++;
		}
		env = env->next;
	}
	return (envp);
}

/* Frees entire environment linked list and sets list head to NULL. */
void	env_clear(t_env **env)
{
	t_env	*current;
	t_env	*next;

	if (env == NULL)
		return ;
	current = *env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	*env = NULL;
}

/* Increments SHLVL variable in shell environment at shell startup. */
int	env_inc_shlvl(t_env **env)
{
	char	*value;
	char	*new_value;
	int		level;

	value = env_get(*env, "SHLVL");
	level = 0;
	if (value != NULL)
		level = ft_atoi(value);
	level++;
	new_value = ft_itoa(level);
	if (new_value == NULL)
		return (1);
	if (env_set(env, "SHLVL", new_value) != 0)
		return (free(new_value), 1);
	free(new_value);
	return (0);
}
