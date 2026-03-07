/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_loop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 20:14:48 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/04 11:58:08 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Selects the proper input reader depending on shell interaction mode. */
static char	*read_input_line(int interactive)
{
	if (interactive)
		return (read_interactive_line());
	return (read_non_interactive_line());
}

/* Executes one line and returns 1 only when shell loop must terminate. */
static int	handle_shell_line(t_shell *sh, char *input)
{
	if (input[0] == '\0')
		return (free(input), 0);
	if (sh->interactive)
		add_history(input);
	sh->last_status = execute_input_line(input, sh);
	free(input);
	return (sh->should_exit);
}

/* Runs the shell read-execute loop and returns the final shell status. */
int	run_shell(t_shell *sh)
{
	char	*input;

	while (1)
	{
		input = read_input_line(sh->interactive);
		if (input == NULL)
		{
			if (sh->interactive)
				write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (handle_shell_line(sh, input))
			break ;
	}
	return (sh->last_status);
}
