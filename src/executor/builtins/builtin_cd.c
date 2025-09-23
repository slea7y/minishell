/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:16:52 by majkijew          #+#    #+#             */
/*   Updated: 2025/09/22 22:56:37 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/executor.h"
#include "../../../includes/parser.h"

static int	go_home(char *prev_pwd, t_env_list *env)
{
	char	*home;

	home = get_env_value(env, "HOME");
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		free(prev_pwd);
		return (1);
	}
	if (chdir(home) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		free(home);
		free(prev_pwd);
		return (1);
	}
	change_var_value("OLDPWD", prev_pwd, env);
	change_var_value("PWD", home, env);
	free(home);
	free(prev_pwd);
	return (0);
}

static int	go_dest(t_cmd_node *cmd, char *prev_pwd, t_env_list *env)
{
	char	*curr;
	int		ret;

	if (cmd->cmd[1][0] == '/' || ft_strcmp(cmd->cmd[1], "..") == 0 || 
		ft_strcmp(cmd->cmd[1], ".") == 0)
		curr = ft_strdup(cmd->cmd[1]);
	else
	{
		curr = getcwd(NULL, 0);
		if (!curr)
		{
			free(prev_pwd);
			return (1);
		}
		char *tmp = ft_strjoin(curr, "/");
		free(curr);
		if (!tmp)
		{
			free(prev_pwd);
			return (1);
		}
		curr = ft_strjoin(tmp, cmd->cmd[1]);
		free(tmp);
		if (!curr)
		{
			free(prev_pwd);
			return (1);
		}
	}

	ret = chdir(curr);
	if (ret != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(cmd->cmd[1], 2);
		ft_putstr_fd("\n", 2);
		free(curr);
		free(prev_pwd);
		return (1);
	}
	change_var_value("OLDPWD", prev_pwd, env);
	change_var_value("PWD", curr, env);
	free(curr);
	free(prev_pwd);
	return (0);
}

int	ft_cd(t_cmd_node *cmd, t_env_list *env)
{
	char	*prev_pwd;

	prev_pwd = getcwd(NULL, 0);
	if (!prev_pwd)
	{
		perror("cd");
		return (1);
	}
	if (!cmd->cmd[1] || (ft_strcmp(cmd->cmd[1], "~") == 0))
		return (go_home(prev_pwd, env));
	else if (ft_strcmp(cmd->cmd[1], "-") == 0)
	{
		char *oldpwd = get_env_value(env, "OLDPWD");
		if (oldpwd[0] == '\0')
		{
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
			free(prev_pwd);
			return (1);
		}
		ft_putstr_fd(oldpwd, 1);
		ft_putstr_fd("\n", 1);
		if (chdir(oldpwd) != 0)
		{
			ft_putstr_fd("cd: ", 2);
			ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(oldpwd, 2);
			ft_putstr_fd("\n", 2);
			free(oldpwd);
			free(prev_pwd);
			return (1);
		}
		change_var_value("OLDPWD", prev_pwd, env);
		change_var_value("PWD", oldpwd, env);
		free(oldpwd);
		free(prev_pwd);
		return (0);
	}
	else if (ft_strcmp(cmd->cmd[1], ".") == 0)
	{
		free(prev_pwd);
		return (0);
	}
	return (go_dest(cmd, prev_pwd, env));
}