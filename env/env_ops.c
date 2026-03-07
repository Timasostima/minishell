/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:08:45 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/04 14:08:45 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Allocates one env node by duplicating provided key and optional value. */
static t_env	*create_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (new_node == NULL)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (new_node->key == NULL)
		return (free(new_node), NULL);
	new_node->value = NULL;
	if (value != NULL)
	{
		new_node->value = ft_strdup(value);
		if (new_node->value == NULL)
			return (free(new_node->key), free(new_node), NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

/* Finds and returns env node by key or NULL when key is not present. */
static t_env	*find_env_node(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

/* Returns pointer to value for an env key, or NULL when key is missing. */
char	*env_get(t_env *env, char *key)
{
	t_env	*node;

	if (key == NULL)
		return (NULL);
	node = find_env_node(env, key);
	if (node == NULL)
		return (NULL);
	return (node->value);
}

/* Creates or replaces one environment key with the provided value string. */
int	env_set(t_env **env, char *key, char *value)
{
	t_env	*node;
	t_env	*new_node;

	if (env == NULL || key == NULL || key[0] == '\0')
		return (1);
	node = find_env_node(*env, key);
	if (node)
	{
		free(node->value);
		node->value = NULL;
		if (value != NULL)
			node->value = ft_strdup(value);
		return ((value != NULL && node->value == NULL));
	}
	new_node = create_env_node(key, value);
	if (new_node == NULL)
		return (1);
	if (*env == NULL)
		return (*env = new_node, 0);
	node = *env;
	while (node->next)
		node = node->next;
	node->next = new_node;
	return (0);
}

/* Removes one environment key from the linked list if it exists. */
int	env_unset(t_env **env, char *key)
{
	t_env	*current;
	t_env	*previous;

	if (env == NULL || *env == NULL || key == NULL)
		return (0);
	previous = NULL;
	current = *env;
	while (current && ft_strcmp(current->key, key) != 0)
	{
		previous = current;
		current = current->next;
	}
	if (current == NULL)
		return (0);
	if (previous == NULL)
		*env = current->next;
	else
		previous->next = current->next;
	free(current->key);
	free(current->value);
	free(current);
	return (0);
}
