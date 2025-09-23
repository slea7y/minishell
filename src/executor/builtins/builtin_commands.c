/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 20:08:44 by majkijew          #+#    #+#             */
/*   Updated: 2025/09/23 16:18:26 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/executor.h"

int	ft_echo(t_cmd_node *cmd)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	// Handle -n flag (only if it's the first argument and nothing else)
	if (cmd->cmd[i] && ft_strcmp(cmd->cmd[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	while (cmd->cmd[i])
	{
		ft_putstr_fd(cmd->cmd[i], 1);
		i++;
		if (cmd->cmd[i])
			ft_putstr_fd(" ", 1);
	}
	if (!n_flag)
		ft_putstr_fd("\n", 1);
	return (0);
}

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putstr_fd("pwd: error retrieving current directory\n", 2);
		return (1);
	}
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	free(pwd);
	return (0);
}

int	ft_env(t_env_list *envp)
{
	t_env_node	*current;

	if (!envp)
		return (1);
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
	return (0);
}

int	ft_exit(t_cmd_node *cmd)
{
	int	exit_code;

	exit_code = 0;
	if (!cmd->cmd[1])
		exit(exit_code);
	
	if (!is_numeric(cmd->cmd[1]))
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		exit(255);
	}
	exit_code = ft_atoi(cmd->cmd[1]);
	if (cmd->cmd[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	// Ensure exit code is in valid range (0-255)
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code = 256 + exit_code;
	exit(exit_code);
}

int	ft_export(t_cmd_node *cmd, t_env_list *env)
{
	int			i;
	int			ret;

	ret = 0;
	if (!cmd->cmd[1])
	{
		sort_env(env);
		return (ft_env(env));
	}

	i = 1;
	while (cmd->cmd[i])
	{
		if (!is_valid_identifier(cmd->cmd[i]))
		{
			ft_putstr_fd("export: ", 2);
			ft_putstr_fd(cmd->cmd[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			ret = 1;
		}
		else
			add_change_export(cmd->cmd[i], env);
		i++;
	}
	return (ret);
}

// int	ft_unset(t_cmd_node *cmd, t_env_list *env)
// {
// 	int			i;
// 	int			ret;

// 	ret = 0;
// 	if (!cmd->cmd[1])
// 		return (0);

// 	i = 1;
// 	while (cmd->cmd[i])
// 	{
// 		if (!is_valid_identifier(cmd->cmd[i]))
// 		{
// 			ft_putstr_fd("unset: ", 2);
// 			ft_putstr_fd(cmd->cmd[i], 2);
// 			ft_putstr_fd(": not a valid identifier\n", 2);
// 			ret = 1;
// 		}
// 		else if (check_if_var_exist(cmd->cmd[i], env))
// 			remove_env_var(cmd->cmd[i], env);
// 		i++;
// 	}
// 	return (ret);
// }