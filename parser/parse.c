/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 12:30:00 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/05 17:53:05 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Returns token pointer at next pipe token or NULL when command is last one. */
static t_token	*find_next_pipe(t_token *token)
{
	while (token && token->type != TOKEN_PIPE)
		token = token->next;
	return (token);
}

/* Counts command argv words between command start token and next pipe token. */
static int	count_cmd_words(t_token *start, t_token *end)
{
	int	count;

	count = 0;
	while (start && start != end)
	{
		if (parser_token_is_redir(start->type))
			start = start->next;
		else if (start->type == TOKEN_WORD)
			count++;
		start = start->next;
	}
	return (count);
}

/* Fills one command node argv and redirection data from token range. */
static int	fill_cmd_from_tokens(t_cmd *cmd, t_token *start, t_token *end)
{
	int	idx;

	cmd->argv = ft_calloc((size_t)count_cmd_words(start, end) + 1,
			sizeof(char *));
	if (cmd->argv == NULL)
		return (0);
	idx = 0;
	while (start && start != end)
	{
		if (parser_token_is_redir(start->type))
		{
			if (!start->next || !parser_add_redir_from_tokens(&cmd->redirs,
					start, start->next))
				return (0);
			start = start->next;
		}
		else if (start->type == TOKEN_WORD)
		{
			cmd->argv[idx] = ft_strdup(start->value);
			if (cmd->argv[idx++] == NULL)
				return (0);
		}
		start = start->next;
	}
	return (1);
}

/* Allocates, appends and fills one command node from token range. */
static t_cmd	*append_cmd(t_cmd **cmds, t_cmd *last,
		t_token *start, t_token *end)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (cmd == NULL)
		return (NULL);
	if (*cmds == NULL)
		*cmds = cmd;
	else
		last->next = cmd;
	if (!fill_cmd_from_tokens(cmd, start, end))
		return (parser_free_cmds(cmds), NULL);
	cmd->is_builtin = (cmd->argv && cmd->argv[0]
			&& is_builtin(cmd->argv[0]));
	return (cmd);
}

/* Parses validated token list into pipeline-linked command table structure. */
t_cmd	*parser_parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*last;
	t_token	*pipe_token;

	cmds = NULL;
	last = NULL;
	while (tokens)
	{
		pipe_token = find_next_pipe(tokens);
		last = append_cmd(&cmds, last, tokens, pipe_token);
		if (last == NULL)
			return (NULL);
		if (pipe_token == NULL)
			break ;
		tokens = pipe_token->next;
	}
	return (cmds);
}
