/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:13:40 by ibenaven          #+#    #+#             */
/*   Updated: 2025/04/18 01:59:50 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(int argc, char *argv[])
{
	int		fd;
	int		count;
	char	*line;

	count = 0;
	if (argc != 2)
		return (printf("%s\n", "Error: Not enough arguments!"));
	fd = open(argv[1], O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		count++;
		printf("\n[%d]:%s\n", count, line);
		free(line);
		line = NULL;
	}
	if (fd != -1)
		close (fd);
	return (0);
}
