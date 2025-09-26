/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:20:19 by majkijew          #+#    #+#             */
/*   Updated: 2025/09/26 20:47:35 by tdietz-r         ###   ########.fr       */
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
		int in_file;

		in_file = open(args[red_pos + 1], O_RDONLY, 0644);
		if (in_file == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(args[red_pos + 1], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exit(1);
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
		if (out_file == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(args[red_pos + 1], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exit(1);
		}
		dup2(out_file, STDOUT_FILENO);
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

