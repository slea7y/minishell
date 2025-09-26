/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:16:52 by majkijew          #+#    #+#             */
/*   Updated: 2025/09/26 01:55:27 by maja             ###   ########.fr       */
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
		ft_putstr_fd("minishell: cd: ", 2);
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

static char	*expand_tilde(char *path, t_env_list *env)
{
	char	*home;
	char	*result;

	if (!path || path[0] != '~')
		return (ft_strdup(path));
	
	// Handle ~ (just tilde)
	if (path[1] == '\0' || path[1] == '/')
	{
		home = get_env_value(env, "HOME");
		if (!home || home[0] == '\0')
		{
			if (home)
				free(home);
			return (ft_strdup(path)); // Return original if HOME not set
		}
		if (path[1] == '\0')
		{
			result = ft_strdup(home);
			free(home);
			return (result);
		}
		// Handle ~/something
		result = ft_strjoin(home, &path[1]); // path[1] is '/'
		free(home);
		return (result);
	}
	
	// Handle ~username (not implemented in most shells, but we can handle it)
	// For now, just return the original path
	return (ft_strdup(path));
}

static int	go_dest_with_path(char *path, char *prev_pwd, t_env_list *env)
{
	char	*curr;
	int		ret;

	if (path[0] == '/' || ft_strcmp(path, "..") == 0 || ft_strcmp(path, ".") == 0)
		curr = ft_strdup(path);
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
		curr = ft_strjoin(tmp, path);
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
		ft_putstr_fd(path, 2);
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
	char	*expanded_path;

	prev_pwd = getcwd(NULL, 0);
	if (!prev_pwd)
	{
		perror("cd");
		return (1);
	}
	if (!cmd->cmd[1] || (ft_strcmp(cmd->cmd[1], "~") == 0))
		return (go_home(prev_pwd, env));
	// Check for too many arguments
	if (cmd->cmd[2] != NULL)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		free(prev_pwd);
		return (1);
	}
	else if (ft_strcmp(cmd->cmd[1], "-") == 0)
	{
		char *oldpwd = get_env_value(env, "OLDPWD");
		if (!oldpwd || oldpwd[0] == '\0')
		{
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
			free(prev_pwd);
			if (oldpwd)
				free(oldpwd);
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
	
	// Handle tilde expansion for other paths
	expanded_path = expand_tilde(cmd->cmd[1], env);
	if (!expanded_path)
	{
		free(prev_pwd);
		return (1);
	}
	
	// Now use the expanded path
	int ret = go_dest_with_path(expanded_path, prev_pwd, env);
	free(expanded_path);
	return (ret);
}