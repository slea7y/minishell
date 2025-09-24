/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:20:19 by majkijew          #+#    #+#             */
/*   Updated: 2025/09/24 14:49:36 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/executor.h"

// ◦ < should redirect input.
// void	redi_inp()

void	handle_redirections(int red_pos, char **args, char **envp)
{
	(void)envp; // envp not used in this function
	if (ft_strncmp(args[red_pos], "<", 1) == 0)
	{
		// printf("whats after %s this?: %s\n", args[red_pos], args[red_pos + 1]);  // Commented out for tester
		int in_file;

		in_file = open(args[red_pos + 1], O_RDONLY, 0644);
		if (!in_file)
		{
			// printf("smth wrong with my file\n");  // Commented out for tester
			return ; //correct exit code
		}
		dup2(in_file, STDIN_FILENO);
		close(in_file);
		args[red_pos] = NULL;
		return ;
	}
	if (ft_strncmp(args[red_pos], ">", 1) == 0)
	{
		int out_file;
		if (ft_strncmp(args[red_pos], ">>", 2) == 0)
			out_file = open(args[red_pos + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			out_file = open(args[red_pos + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (!out_file)
		{
			// printf("smth wrong with my file\n");  // Commented out for tester
			return ;
		}
		// dup2(fd[0], STDIN_FILENO);
		// printf("does it get here even??\n");  // Commented out for tester
		dup2(out_file, STDOUT_FILENO);
		// close(fd[1]);
		// close(fd[0]);
		close(out_file);
		args[red_pos] = NULL;
		return ;
	}
	// Redirection handled, let normal execution flow continue
	return ;
}

int	check_for_redirections(char **args, char **envp)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], ">", 1) == 0
			|| ft_strncmp(args[i], ">>", 2) == 0
			|| ft_strncmp(args[i], "<", 1) == 0
			|| ft_strncmp(args[i], "<<", 2) == 0)
		{
			handle_redirections(i, args, envp);
			return (1); // Return 1 to indicate redirections were found and handled
		}
		i++;
	}
	return (0); // Return 0 to indicate no redirections found
}

// ◦ > should redirect output.
// ◦ << should be given a delimiter, then read the input until a line containing the
// delimiter is seen. However, it doesn’t have to update the history!
// ◦ >> should redirect output in append mode.

