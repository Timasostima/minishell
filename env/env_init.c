/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:07:24 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/04 14:07:24 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Creates one env node from a KEY=VALUE source string. */
static t_env	*env_new_node(char *entry)
{
	t_env	*node;
	char	*equal;
	size_t	key_len;

	node = malloc(sizeof(t_env));
	if (node == NULL)
		return (NULL);
	equal = ft_strchr(entry, '=');
	if (equal == NULL)
	{
		node->key = ft_strdup(entry);
		node->value = NULL;
	}
	else
	{
		key_len = (size_t)(equal - entry);
		node->key = ft_substr(entry, 0, key_len);
		node->value = ft_strdup(equal + 1);
	}
	if (node->key == NULL || (equal && node->value == NULL))
		return (free(node->key), free(node->value), free(node), NULL);
	node->next = NULL;
	return (node);
}

/* Appends a node at the end of the environment linked list. */
static void	env_add_back(t_env **env, t_env *new_node)
{
	t_env	*current;

	if (*env == NULL)
	{
		*env = new_node;
		return ;
	}
	current = *env;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

/* Builds shell-owned environment list by copying the startup envp array. */
t_env	*env_init(char **envp)
{
	t_env	*env;
	t_env	*new_node;
	int		i;

	env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		new_node = env_new_node(envp[i]);
		if (new_node == NULL)
			return (env_clear(&env), NULL);
		env_add_back(&env, new_node);
		i++;
	}
	return (env);
}
