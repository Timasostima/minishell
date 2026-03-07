/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_state.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:10:00 by ibenaven          #+#    #+#             */
/*   Updated: 2025/04/22 17:10:00 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*gnl_combined_buffers(char *temp_buffer, char *read_buffer)
{
	char	*temp;

	temp = ft_strjoin(temp_buffer, read_buffer);
	if (temp == NULL)
	{
		free(temp_buffer);
		free(read_buffer);
		return (NULL);
	}
	free(temp_buffer);
	return (temp);
}

void	gnl_clear(void)
{
	char	**temp_buffer;

	temp_buffer = gnl_get_temp_buffer_ptr();
	free(*temp_buffer);
	*temp_buffer = NULL;
}
