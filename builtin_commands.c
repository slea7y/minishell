/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 20:08:44 by majkijew          #+#    #+#             */
/*   Updated: 2025/08/09 18:15:10 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

// • Handle $? which should expand to the exit status of the most recently
// executed foreground pipeline.

void	ft_echo(char **args)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (args[i] && (ft_strncmp(args[i], "-n", 2) == 0))
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		i++;
		if (args[i])
			ft_putstr_fd(" ", 1);
	}
	if (!n_flag)
		ft_putstr_fd("\n", 1);
}

void	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putstr_fd("pwd: error retrieving current directory\n", 1);
		return ;
	}
	ft_putstr_fd(pwd, 1);
	free(pwd);
	ft_putstr_fd("\n", 1);
}

void	ft_env(t_env_list *envp)
{
	t_env_node	*current;

	current = envp->head;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->key, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(current->value, 1);
			ft_putstr_fd("\n", 1);
		}
		current = current->next;
	}
}

void	ft_exit(char **args)
{
	int	exit_code;

	exit_code = 0;
	if (args[1] == 0)
		exit (exit_code);
	else
	{
		if (!is_numeric(args[1]))
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			exit(255);
		}
		exit_code = ft_atoi(args[1]);
	}
	exit(exit_code);
}

void	ft_export(char **args, t_env_list *env)
{
	int	i;

	if (args[1] == NULL)
	{
		sort_env(env);
		ft_env(env);
		return ;
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": not an identifier: ", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("\n", 2);
		}
		else
			add_change_export(args[i], env);
		i++;
	}
}
