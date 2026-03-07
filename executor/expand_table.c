/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_table.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:20:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 18:26:10 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_redirs(t_redir *redirs, t_shell *sh)
{
	char	*new_target;
	char	*raw_target;

	while (redirs)
	{
		raw_target = redirs->target;
		new_target = expand_word_value(redirs->target, sh,
				redirs->type != REDIR_HEREDOC);
		if (new_target == NULL)
			return (1);
		if (redirs->type != REDIR_HEREDOC && !redirs->target_quoted
			&& new_target[0] == '\0')
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(raw_target, STDERR_FILENO);
			ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
			free(new_target);
			return (1);
		}
		free(redirs->target);
		redirs->target = new_target;
		redirs = redirs->next;
	}
	return (0);
}

int	expand_cmd_table(t_cmd *cmds, t_shell *sh)
{
	while (cmds)
	{
		if (expand_argv_words(cmds, sh) != 0)
			return (1);
		cmds->is_builtin = (cmds->argv && cmds->argv[0]
				&& is_builtin(cmds->argv[0]));
		if (expand_redirs(cmds->redirs, sh) != 0)
			return (1);
		cmds = cmds->next;
	}
	return (0);
}
