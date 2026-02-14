/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:06:04 by ibenaven          #+#    #+#             */
/*   Updated: 2026/02/14 13:36:29 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_split(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
		i++;
	while (i--)
		free(array[i]);
	free(array);
	return (NULL);
}
