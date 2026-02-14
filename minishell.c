/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaven <ibenaven@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:53:36 by ibenaven          #+#    #+#             */
/*   Updated: 2026/02/14 17:36:36 by ibenaven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	const char	*prompt = "minishell$ ";
	bool	is_running;
	char	*path;
	char	*cmd_path;
	char	**split_paths;
	char	**split_argvs;
	pid_t	pid;
	int	status;

	(void)argc;
	(void)argv;
	is_running = true;
	while (is_running)
	{
		input = readline(prompt);
		if (input == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		if (input[0] == '\0')
		{
			free(input);
			continue;
		}
		add_history(input);
		if (ft_strcmp(input, "exit") == 0)
			break;
		pid = fork();
		if (pid == -1)
		{
			perror("fork() error");
			free (input);
			rl_clear_history();
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			split_argvs = ft_split(input, ' ');
			path = getenv("PATH");
			split_paths = ft_split(path, ':');
			cmd_path = build_cmd_path(split_paths, split_argvs[0]);	
			execve(cmd_path, split_argvs, envp);
			free_split(split_argvs);
			free_split(split_paths);
			free (input);
			rl_clear_history();
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("%jd\n", (intmax_t) pid);
			waitpid(pid, &status, 0);
			free (input);
		}	
	}
	rl_clear_history();
	return (0);
}
