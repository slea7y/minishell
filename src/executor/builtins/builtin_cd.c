/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:16:52 by majkijew          #+#    #+#             */
/*   Updated: 2025/09/22 00:22:42 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/executor.h"

///FOR FUTURE MAJA OR TIMO: there is this one
// case where we can use cd in our pipes but idk yet how it works

/// @brief Changes the current working directory of the shell.
/// Updates the environment variables PWD and OLDPWD accordingly.
/// Handles cases such as `cd`, `cd ~`, `cd .`, or `cd <path>`.
/// @param args Command arguments.
/// args[0] is "cd", args[1] is the optional target directory.
/// If args[1] is NULL or "~", goes to the home directory.
/// @param env Pointer to the environment variables list (t_env_list),
/// allowing updates to PWD and OLDPWD after a directory change.

char	*go_home(char *prev_pwd, t_env_list *env)
{
	char	*home;

	(void)prev_pwd;
	home = getenv("HOME");
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", 1);
		return (NULL);
	}
	change_var_value("PWD", home, env);
	change_var_value("OLDPWD", prev_pwd, env);
	return (home);
}

char	*go_dest(char **args, char *prev_pwd, t_env_list *env)
{
	char	*dest;
	char	*curr;
	char	*cwd;
	char	*slash;

	cwd = getcwd(NULL, 0);
	if (ft_strcmp(args[1], "..") == 0)
	{
		slash = ft_strrchr(cwd, '/');
		curr = malloc(slash - cwd + 1);
		if (!curr)
			return (NULL);
		ft_strlcpy(curr, cwd, (slash - cwd + 1));
	}
	else
	{
		cwd = ft_strjoin(cwd, "/");
		curr = ft_strjoin(cwd, args[1]);
		free(cwd);
	}
	change_var_value("PWD", curr, env);
	free(curr);
	change_var_value("OLDPWD", prev_pwd, env);
	dest = args[1];
	return (dest);
}

void	ft_cd(char **args, t_env_list *env)
{
	char	*prev_pwd;
	char	*go;

	prev_pwd = getcwd(NULL, 0);
	if (!prev_pwd)
	{
		perror("getcwd");
		return ;
	}
	if (args[1] && ft_strcmp(args[1], ".") == 0)
		return ;
	else if (args[1] == NULL || (ft_strcmp(args[1], "~") == 0))
		go = go_home(prev_pwd, env);
	else
		go = go_dest(args, prev_pwd, env);
	if (chdir(go) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd("\n", 2);
	}
}
