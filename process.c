/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 20:03:59 by majkijew          #+#    #+#             */
/*   Updated: 2025/08/11 20:27:48 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <unistd.h>
// #include <errno.h>
// #include "../../libft/libft.h"
// void execute_external(char **args, t_env_list *env_list)

// char *find_full_path(t_env_list *env_list)

// void execute_external(char **args, char **env_list)
// {
// 	pid_t	pid_child;
// 	pid_t	pid_parent;
// 	pid_t	pid_idk;
// 	pid_t	pid_what;
// 	(void)env_list;
// 	(void)args;
// 	pid_child = fork();
// 	pid_what = fork();
// 	pid_parent = fork();
// 	pid_idk = fork();
// 	printf("%d\n", pid_idk);
// 	printf("%d\n", pid_child);
// 	printf("%d\n", pid_parent);
// 	if (pid_child < 0)
// 	{
// 		perror("fork");
// 		exit(1);
// 	}
// 	if (pid_child == 0)
// 		printf("idk it should be a child yaaa\n");
// 		//child proces
// 	// else if (pid_child > 4000)
// 	if (pid_parent > 4000)
// 		printf("idk it should be a parent yaaa\n");
// 	else
// 		printf("wtf\n");
// 	int kids = 0;
// 	while(wait(NULL) != -1 || errno != ECHILD)
// 	{
// 		kids++;
// 		printf("the kidsss\n");
// 	}
// 	// printf("how many kids%d\n", kids);
// 		//partent process
// }

char	**find_full_path(void)
{
	char	*path_val;
	char	**paths;

	path_val = getenv("PATH");
	if (!path_val)
		return (NULL);
	paths = ft_split(path_val, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

void	execute_cmd(char **args, char **env)
{
	char	**paths;
	char	*path_to_cmd;
	char	*real_path;
	int		i;

	i = 0;
	paths = find_full_path();
	while (paths[i])
	{
		path_to_cmd = malloc((ft_strlen(args[0]) + 1) + (ft_strlen(paths[i]) + 1));
		if (!path_to_cmd)
			return ;
		// path_to_cmd = ft_strjoin(path_to_cmd, paths[i]);
		// path_to_cmd = ft_strjoin(args[0], "/");
		path_to_cmd = ft_strjoin(paths[i], "/");
		path_to_cmd = ft_strjoin(path_to_cmd, args[0]);
		// printf("%s\n", path_to_cmd);
		if (access(path_to_cmd, X_OK) == 0)
		{
			real_path = path_to_cmd;
			execve(path_to_cmd, &args[0], env);
			return ;
		}
		free(path_to_cmd);
		i++;
	}
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(args[0], 2);
	ft_putstr_fd("\n", 2);
	exit(127);
}

void	execute_external(char **args, char **env)
{
	pid_t	pid_try;
	// pid_t	parent_id;

	pid_try = fork();
	// printf("%d\n", pid_try);
	if (pid_try < 0)
	{
		perror("fork");
		return ;
	}
	if (pid_try == 0)
	{
		check_for_redirections(args, env);
		execute_cmd(args, env);
		return ;
	}
	else
	{
		waitpid(pid_try, NULL, 0);
		// printf("hello its ur pops\n");
		return ;
	}
}
