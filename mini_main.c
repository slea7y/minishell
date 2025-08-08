/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:25:54 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/08/08 22:19:05 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"
#include "../../libft/libft.h"

 #include <string.h> //for str tok

int	main(int ac, char **av, char **envp)
{
	char	line[MAX_INPUT];
	char	*args[MAX_ARGS];
	int		running = 1;
	t_env_list env_list;
	(void)ac;
	(void)av;

	init_list_env(&env_list, envp);
	// enviroment = dup_env(envp);
	while (running)
	{
		printf("minishell> ");
		if (!fgets(line, sizeof(line), stdin)) //obv this and strtok are just helper func for now
			break ;
		line[strcspn(line, "\n")] = 0;
		int i = 0;
		char *token = strtok(line, " ");
		while (token && i < MAX_ARGS - 1)
		{
			args[i++] = token;
			token = strtok(NULL, " ");
		}
		args[i] = NULL;
		if (i == 0)
			continue ;
		if (ft_strncmp(args[0], "exit", 4) == 0)
			ft_exit(args);
		else if (ft_strncmp(args[0], "env", 3) == 0)
			ft_env(&env_list);
		else if (ft_strncmp(args[0], "echo", 4) == 0)
			ft_echo(args);
		else if (ft_strncmp(args[0], "cd", 2) == 0)
			ft_cd(args, &env_list);
		else if (ft_strncmp(args[0], "pwd", 3) == 0)
			ft_pwd();
		else if (ft_strncmp(args[0], "export", 6) == 0)
			ft_export(args, &env_list);
		else if (ft_strncmp(args[0], "unset", 6) == 0)
			ft_unset(args, &env_list);
		else
			printf("minishell: command not found: %s\n", args[0]);
	}
	printf("Koniec minishell\n");
	return 0;
}
