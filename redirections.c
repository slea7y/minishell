/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:20:19 by majkijew          #+#    #+#             */
/*   Updated: 2025/08/11 20:26:21 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

// ◦ < should redirect input.
// void	redi_inp()

void	handle_redirections(int red_pos, char **args, char **envp)
{
	if (ft_strncmp(args[red_pos], ">", 1) == 0)
	{
		printf("whats after %s this?: %s\n", args[red_pos], args[red_pos + 1]);

		int out_file;
		out_file = open(args[red_pos + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (!out_file)
		{
			printf("smth wrong with my file\n");
			return ;
		}
		// dup2(fd[0], STDIN_FILENO);
		dup2(out_file, STDOUT_FILENO);
		// close(fd[1]);
		// close(fd[0]);
		close(out_file);
		args[red_pos] = NULL;
		execute_external(args, envp);
		exit (1);
	}
}

void	check_for_redirections(char **args, char **envp)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], ">", 1) == 0
			|| ft_strncmp(args[i], ">>", 2) == 0
			|| ft_strncmp(args[i], "<", 1) == 0
			|| ft_strncmp(args[i], "<<", 2) == 0)
			handle_redirections(i, args, envp);
		i++;
	}
	return ;
}

// ◦ > should redirect output.
// ◦ << should be given a delimiter, then read the input until a line containing the
// delimiter is seen. However, it doesn’t have to update the history!
// ◦ >> should redirect output in append mode.

