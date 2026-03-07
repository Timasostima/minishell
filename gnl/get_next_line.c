/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 11:47:20 by ibenaven          #+#    #+#             */
/*   Updated: 2025/04/22 16:30:20 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	**gnl_get_temp_buffer_ptr(void)
{
	static char	*temp_buffer;

	return (&temp_buffer);
}

static char	*read_file(char *temp_buffer, int fd)
{
	int		bytes_read;
	char	*read_buffer;

	read_buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (read_buffer == NULL)
		return (free(temp_buffer), NULL);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, read_buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(read_buffer), free(temp_buffer), NULL);
		if (bytes_read == 0)
			break ;
		read_buffer[bytes_read] = '\0';
		temp_buffer = gnl_combined_buffers(temp_buffer, read_buffer);
		if (temp_buffer == NULL)
			return (NULL);
		if (ft_strchr(temp_buffer, '\n'))
			break ;
	}
	free(read_buffer);
	return (temp_buffer);
}

static char	*extract_line(char *temp_buffer)
{
	int		i;
	char	*line;

	if (temp_buffer == NULL || temp_buffer[0] == '\0')
		return (NULL);
	i = 0;
	while (temp_buffer[i] && temp_buffer[i] != '\n')
		i++;
	if (temp_buffer[i] == '\n')
		i++;
	line = ft_calloc(i + 1, sizeof(char));
	if (line == NULL)
		return (NULL);
	i = 0;
	while (temp_buffer[i] && temp_buffer[i] != '\n')
	{
		line[i] = temp_buffer[i];
		i++;
	}
	if (temp_buffer[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*reset_buffer(char *temp_buffer)
{
	int		i;
	int		j;
	int		temp_buffer_len;
	char	*new_buffer;

	if (temp_buffer == NULL)
		return (NULL);
	i = 0;
	while (temp_buffer[i] && temp_buffer[i] != '\n')
		i++;
	if (temp_buffer[i] == '\0')
		return (free(temp_buffer), NULL);
	i++;
	if (temp_buffer[i] == '\0')
		return (free(temp_buffer), NULL);
	temp_buffer_len = ft_strlen(temp_buffer + i);
	new_buffer = ft_calloc(temp_buffer_len + 1, sizeof(char));
	if (new_buffer == NULL)
		return (free(temp_buffer), NULL);
	j = 0;
	while (temp_buffer[i] != '\0')
		new_buffer[j++] = temp_buffer[i++];
	new_buffer[j] = '\0';
	free(temp_buffer);
	return (new_buffer);
}

char	*get_next_line(int fd)
{
	char		**temp_buffer;
	char		*line;

	temp_buffer = gnl_get_temp_buffer_ptr();
	if (fd < 0 || fd > MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	if (*temp_buffer == NULL)
		*temp_buffer = ft_calloc(1, sizeof(char));
	if (*temp_buffer == NULL)
		return (NULL);
	if (ft_strchr(*temp_buffer, '\n') == NULL)
		*temp_buffer = read_file(*temp_buffer, fd);
	if (*temp_buffer == NULL)
		return (NULL);
	if ((*temp_buffer)[0] == '\0')
		return (free(*temp_buffer), *temp_buffer = NULL, NULL);
	line = extract_line(*temp_buffer);
	if (line == NULL)
		return (free(*temp_buffer), *temp_buffer = NULL, NULL);
	*temp_buffer = reset_buffer(*temp_buffer);
	return (line);
}
