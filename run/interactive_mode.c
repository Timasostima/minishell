/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 20:14:09 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/01 20:14:09 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Reads one command line from the interactive prompt using readline. */
char	*read_interactive_line(void)
{
	return (readline("minishell$ "));
}
