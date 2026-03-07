/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:29:30 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_heredoc_handler(int signo)
{
	(void)signo;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
}

void	set_heredoc_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = sigint_heredoc_handler;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	restore_heredoc_signals(int interactive)
{
	if (interactive)
		interactive_signals_configuration();
	else
		set_child_signals_default();
}
