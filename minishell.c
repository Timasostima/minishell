/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:53:36 by ibenaven          #+#    #+#             */
/*   Updated: 2026/02/07 23:59:57 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*input;
	const char	*prompt = "minishell$ ";
	bool	is_running;

	is_running = true;
	while (is_running)
	{
		input = readline(prompt);
		if (strlen(input) > 0)
			add_history(input);
		printf("%s\n", input);
		free(input);
	}
	return (0);
}
