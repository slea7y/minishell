/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:13:57 by majkijew          #+#    #+#             */
/*   Updated: 2025/09/22 00:23:26 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// • Implement pipes (| character). The output of each command in the pipeline is
// connected to the input of the next command via a pipe.

#include "../../../includes/minishell.h"
#include "../../../includes/executor.h"

void	handle_one_pipe(char **args, char **env_list)
{
	int fd[2];
	pid_t id1, id2;
	int i = 0;

	if (pipe(fd) == -1)
	{
		ft_putstr_fd("debug: handle_one_pipe() failed creating the pipe\n", 2);
		perror("pipe");
		return;
	}
	// znajdź pozycję pipe
	while (args[i] && ft_strcmp(args[i], "|") != 0)
		i++;
	if (!args[i] || !args[i + 1])
	{
		ft_putstr_fd("minishell: parse error near `|'\n", 2);
		return;
	}
	args[i] = NULL; // odcinamy pierwsze polecenie
	char **cmd1 = args;
	char **cmd2 = &args[i + 1];

	id1 = fork();
	if (id1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execute_cmd(cmd1, env_list, 0);
		exit(1);
	}
	// printf ("do i even get here?\n");
	id2 = fork();
	if (id2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		close(fd[0]);
		execute_cmd(cmd2, env_list, 0);
		exit(1);
	}

	close(fd[0]);
	close(fd[1]);
	waitpid(id1, NULL, 0);
	waitpid(id2, NULL, 0);
}

// void handle_multiple_pipes(char **args, char **env)
// {
//     int pipefd[2];
//     int in_fd = 0; // początkowo STDIN
//     int i = 0;

//     while (args[i])
//     {
//         // znajdź koniec obecnej komendy
//         int j = i;
//         while (args[j] && strcmp(args[j], "|") != 0)
//             j++;

//         // odetnij potok
//         if (args[j])
//             args[j] = NULL;

//         // stwórz nowy pipe jeśli nie ostatnia komenda
//         if (args[j])
//             pipe(pipefd);

//         pid_t pid = fork();
//         if (pid == 0)
//         {
//             dup2(in_fd, STDIN_FILENO);
//             if (args[j]) // jeśli nie ostatnia komenda
//                 dup2(pipefd[1], STDOUT_FILENO);

//             if (in_fd != 0) close(in_fd);
//             if (args[j]) { close(pipefd[0]); close(pipefd[1]); }

//             execute_cmd(args, env, i); // tylko ten fragment args
//             exit(0);
//         }
//         else
//         {
//             waitpid(pid, NULL, 0);
//             if (in_fd != 0) close(in_fd);
//             if (args[j])
//             {
//                 close(pipefd[1]);
//                 in_fd = pipefd[0];
//             }
//             i = j + 1; // przejdź do następnej komendy po "|"
//         }
//     }
// }

// void	handle_multiple_pipes(int pipes_count, char **args, char **env_list)
// {
// 	pid_t	pidx;
// 	int		fd[2];
// 	int		i = 0;
// 	int		first_entry;
// 	(void)pipes_count;

// 	first_entry = 1;
// 	if (pipe(fd) == -1)
// 	{
// 		ft_putstr_fd("debug: handle_multiple_pipes failed creating the pipe\n", 2);
// 		// perror("pipe");
// 		return ;
// 	}
// 	// cmd1 | cdm2 | cdm3 | cmd4
// 	// while musi breaknac przed ostanim argumentem bo jego drukujemy do standard outputu
// 	// while (i < ac)
// 	while (i <= 6)
// 	{
// 		if ((ft_strcmp(args[i], "|") == 0) && args[i])
// 			i++;
// 		pidx = fork();
// 		if (pidx < 0)
// 		{
// 			printf("debug: forking failed i guess :((\n");
// 			exit (1);
// 		}
// 		if (pidx == 0)
// 		{
// 			printf("do i ever get here??\n");
// 			if (first_entry)
// 			{
// 				// tutaj musze skopiowac z normalnego imputu a nie naszych fd
// 				dup2(fd[1], STDOUT_FILENO);
// 			}
// 			else
// 			{
// 				dup2(fd[0], STDIN_FILENO);
// 				dup2(fd[1], STDOUT_FILENO);
// 				// tu robie calle pomiedzy fd
// 			}
// 			close(fd[0]);
// 			close(fd[1]);
// 			first_entry = 0;
// 			printf("ktore wejscie %i\nco za komeda %s\n", i, args[i]);
// 			execute_cmd(args, env_list, i);
// 		}
// 		else
// 		{
// 			printf("popsss\n");
// 			waitpid(pidx, NULL, 0);
// 			return ;
// 		}
// 		i++;
// 	}
// 	if (pidx == 0)
// 	{
// 		dup2(fd[0], STDIN_FILENO);
// 		close(fd[0]);
// 		close(fd[1]);
// 		execute_cmd(args, env_list, i + 1);
// 		return ;
// 	}
// 	//for each cmd i have to creat its own forkkk
// }



void	handle_multiple_pipes(char **args, char **env_list)
{
	pid_t	pidx;
	int		fd[2];
	int		i;
	int		j;
	int		first_entry;

	i = 0;
	first_entry = 1;
	while (args[i])
	{
		j = i;
		while (args[j] && strcmp(args[j], "|") != 0)
			j++;
		if (args[j])
			args[j] = NULL;
		if (args[j])
		{
			if (pipe(fd) == 0)
			{
				ft_putstr_fd("debug: handle_multiple_pipes failed creating the pipe\n", 2);
				exit (1);
			}
		}
		pidx = fork();
		if (pidx < 0)
		{
			printf("debug: forking failed i guess :((\n");
			exit (1);
		}
		if (pidx == 0)
		{
			printf("do i ever get here??\n");
			if (first_entry)
			{
				// tutaj musze skopiowac z normalnego imputu a nie naszych fd
				dup2(fd[1], STDOUT_FILENO);
			}
			else
			{
				dup2(fd[0], STDIN_FILENO);
				dup2(fd[1], STDOUT_FILENO);
				// tu robie calle pomiedzy fd
			}
			close(fd[0]);
			close(fd[1]);
			first_entry = 0;
			printf("ktore wejscie %i\nco za komeda %s\n", i, args[i]);
			execute_cmd(args, env_list, i);
		}
		else
		{
			printf("popsss\n");
			waitpid(pidx, NULL, 0);
			return ;
		}
		i = j + 1;
	}
}

void	handle_pipes(int pipes_count, char **args, char **env_list)
{
	if (pipes_count == 1)
	{
		handle_one_pipe(args, env_list);
		return ;
	}
	else
	{
		printf("multiple pipessss???\n");
		handle_multiple_pipes(args, env_list);
		return ;
	}
}

int	detect_pipes(char **args, char **env_list)
{
	int	i;
	int	pipes_count;

	i = 0;
	pipes_count = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
		{
			pipes_count++;
			// printf("we have pipes!\n");
		}
		i++;
	}
	if (pipes_count >= 1)
	{
		// printf("pipe!\n");
		handle_pipes(pipes_count, args, env_list);
		return (1);
	}
	return (0);
}
