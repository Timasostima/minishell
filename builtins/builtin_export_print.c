/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 15:50:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/04 15:50:00 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Counts env entries for export display array allocation. */
static int	env_count_entries(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (ft_strcmp(env->key, "_") != 0)
			count++;
		env = env->next;
	}
	return (count);
}

/* Copies env node pointers into a heap array for sorting. */
static t_env	**env_to_array(t_env *env, int *size)
{
	t_env	**array;
	int		i;

	*size = env_count_entries(env);
	array = malloc(sizeof(t_env *) * (*size));
	if (array == NULL && *size > 0)
		return (NULL);
	i = 0;
	while (env)
	{
		if (ft_strcmp(env->key, "_") != 0)
		{
			array[i] = env;
			i++;
		}
		env = env->next;
	}
	return (array);
}

/* Sorts env array by key using in-place bubble sort for small data. */
static void	sort_env_array(t_env **array, int size)
{
	int		i;
	t_env	*tmp;

	while (size > 1)
	{
		i = 0;
		while (i < size - 1)
		{
			if (ft_strcmp(array[i]->key, array[i + 1]->key) > 0)
			{
				tmp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = tmp;
			}
			i++;
		}
		size--;
	}
}

/* Prints export value escaping shell-sensitive characters inside quotes. */
static void	print_export_value(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == '"' || value[i] == '\\'
			|| value[i] == '$' || value[i] == '`')
			write(STDOUT_FILENO, "\\", 1);
		ft_putchar_fd(value[i], STDOUT_FILENO);
		i++;
	}
}

/* Prints sorted export list in bash-like "declare -x" format. */
int	print_export_sorted(t_shell *sh)
{
	t_env	**array;
	int		i;
	int		size;

	array = env_to_array(sh->env, &size);
	if (array == NULL && size > 0)
		return (1);
	sort_env_array(array, size);
	i = 0;
	while (i < size)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(array[i]->key, STDOUT_FILENO);
		if (array[i]->value != NULL)
		{
			write(STDOUT_FILENO, "=\"", 2);
			print_export_value(array[i]->value);
			write(STDOUT_FILENO, "\"", 1);
		}
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	free(array);
	return (0);
}
