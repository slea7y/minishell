/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:13:57 by majkijew          #+#    #+#             */
/*   Updated: 2025/09/27 00:32:48 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// • Implement pipes (| character). The output of each command in the pipeline is
// connected to the input of the next command via a pipe.

#include "../../../includes/minishell.h"
#include "../../../includes/executor.h"

int	handle_one_pipe(char **args, char **env_list)
{
	int fd[2];
	pid_t id1, id2;
	int i = 0;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	while (args[i] && ft_strcmp(args[i], "|") != 0)
		i++;
	if (!args[i] || !args[i + 1])
	{
		ft_putstr_fd("minishell: parse error near `|'\n", 2);
		close(fd[0]);
		close(fd[1]);
		return (1);
	}
	args[i] = NULL; 
	char **cmd1 = args;
	char **cmd2 = &args[i + 1];

	id1 = fork();
	if (id1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);  // Redirect stdout to pipe write end
		close(fd[0]);  // Close read end
		close(fd[1]);  // Close write end (after dup2)
		check_for_redirections(cmd1, env_list);
		execute_cmd(cmd1, env_list, 0);
		exit(1); // This should never be reached as execute_cmd calls exit()
	}
	id2 = fork();
	if (id2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);   // Redirect stdin to pipe read end
		close(fd[0]);  // Close read end (after dup2)
		close(fd[1]);  // Close write end
		check_for_redirections(cmd2, env_list);
		execute_cmd(cmd2, env_list, 0);
		exit(1); // This should never be reached as execute_cmd calls exit()
	}

	close(fd[0]);
	close(fd[1]);
	
	int status1, status2;
	waitpid(id1, &status1, 0);
	waitpid(id2, &status2, 0);
	
	// Return the exit code of the last command in the pipe
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	else if (WIFSIGNALED(status2))
		return (128 + WTERMSIG(status2));
	else
		return (1);
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
// 			// printf("debug: forking failed i guess :((\n");  // Commented out for tester
// 			exit (1);
// 		}
// 		if (pidx == 0)
// 		{
// 			// printf("do i ever get here??\n");  // Commented out for tester
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
// 			// printf("ktore wejscie %i\nco za komeda %s\n", i, args[i]);  // Commented out for tester
// 			execute_cmd(args, env_list, i);
// 		}
// 		else
// 		{
// 			// printf("popsss\n");  // Commented out for tester
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



int	handle_multiple_pipes(char **args, char **env_list)
{
	pid_t	pidx;
	int		**pipes;
	int		i;
	int		j;
	int		last_exit_code = 0;
	pid_t	*pids = NULL;
	int		pid_count = 0;
	int		pid_index = 0;
	int		pipe_count = 0;

	// Count commands first
	i = 0;
	while (args[i])
	{
		j = i;
		while (args[j] && strcmp(args[j], "|") != 0)
			j++;
		pid_count++;
		i = j + 1;
	}
	pipe_count = pid_count - 1; // Number of pipes = commands - 1
	pids = malloc(sizeof(pid_t) * pid_count);
	if (!pids)
		return (1);
	pipes = malloc(sizeof(int*) * pipe_count);
	if (!pipes)
	{
		free(pids);
		return (1);
	}
	i = 0;
	while (i < pipe_count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			j = 0;
			while (j < i)
			{
				free(pipes[j]);
				j++;
			}
			free(pipes);
			free(pids);
			return (1);
		}
		i++;
	}

	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipes[i]) == -1)
		{
			j = 0;
			while (j <= i)
			{
				free(pipes[j]);
				j++;
			}
			free(pipes);
			free(pids);
			return (1);
		}
		i++;
	}

	// Execute commands
	i = 0;
	pid_index = 0;
	while (args[i])
	{
		j = i;
		while (args[j] && strcmp(args[j], "|") != 0)
			j++;
		if (args[j])
			args[j] = NULL;

		pidx = fork();
		if (pidx < 0)
		{
			j = 0;
			while (j < pipe_count)
			{
				free(pipes[j]);
				j++;
			}
			free(pipes);
			free(pids);
			return (1);
		}
		if (pidx == 0)
		{
			// Child process
			// Connect stdin from previous pipe (if not first command)
			if (pid_index > 0)
				dup2(pipes[pid_index - 1][0], STDIN_FILENO);
			
			// Connect stdout to next pipe (if not last command)
			if (pid_index < pipe_count)
				dup2(pipes[pid_index][1], STDOUT_FILENO);
			
			j = 0;
			while (j < pipe_count)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				j++;
			}
			
			// Handle redirections after pipe setup (redirections override pipes)
			check_for_redirections(args, env_list);
			
			execute_cmd(args, env_list, i);
			exit(1); // This should never be reached as execute_cmd calls exit()
		}
		else
		{
			// Parent process
			pids[pid_index++] = pidx;
		}
		i = j + 1;
	}

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}

	int status;
	i = 0;
	while (i < pid_count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			last_exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_exit_code = 128 + WTERMSIG(status);
		else
			last_exit_code = 1;
		i++;
	}

	i = 0;
	while (i < pipe_count)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
	free(pids);
	return (last_exit_code);
}

int	handle_pipes(int pipes_count, char **args, char **env_list)
{
	if (pipes_count == 1)
		return (handle_one_pipe(args, env_list));
	else
		return (handle_multiple_pipes(args, env_list));
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
		return (handle_pipes(pipes_count, args, env_list));
	}
	return (0);
}
