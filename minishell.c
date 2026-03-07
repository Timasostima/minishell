/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:53:36 by ibenaven          #+#    #+#             */
/*   Updated: 2026/03/01 20:17:05 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Sets one env variable only when it does not already exist in shell env. */
static int	set_var_if_missing(t_shell *sh, char *key, char *value)
{
	if (env_get(sh->env, key) != NULL)
		return (0);
	return (env_set(&sh->env, key, value));
}

/* Creates a minimal shell env when startup env is empty or missing PWD. */
static int	ensure_minimal_env(t_shell *sh)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (1);
	if (set_var_if_missing(sh, "PWD", cwd) != 0)
		return (1);
	return (0);
}

/* Initializes shell core state and bootstraps editable environment storage. */
static int	init_shell_state(t_shell *sh, char *envp[])
{
	sh->last_status = 0;
	sh->interactive = isatty(STDIN_FILENO);
	sh->should_exit = 0;
	sh->env = env_init(envp);
	if (envp && envp[0] && sh->env == NULL)
		return (1);
	sh->use_default_path = (env_get(sh->env, "PATH") == NULL);
	if (ensure_minimal_env(sh) != 0)
		return (env_clear(&sh->env), 1);
	if (env_inc_shlvl(&sh->env) != 0)
		return (env_clear(&sh->env), 1);
	return (0);
}

/* Initializes shell state and delegates execution to the run loop. */
int	main(int argc, char *argv[], char *envp[])
{
	t_shell	sh;

	(void)argc;
	(void)argv;
	if (init_shell_state(&sh, envp) != 0)
		return (ft_putstr_fd("minishell: env init failed\n", STDERR_FILENO), 1);
	if (sh.interactive)
		interactive_signals_configuration();
	sh.last_status = run_shell(&sh);
	rl_clear_history();
	env_clear(&sh.env);
	return (sh.last_status);
}
