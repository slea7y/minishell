/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 21:57:51 by majkijew          #+#    #+#             */
/*   Updated: 2025/09/22 17:22:51 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/executor.h"

static int	unset_var(char *var_name, t_env_list *env)
{
	if (!check_if_var_exist(var_name, env))
		return (0);
	remove_env_var(var_name, env);
	return (0);
}

int	ft_unset(t_cmd_node *cmd, t_env_list *env)
{
	int	i;
	int	ret;

	ret = 0;
	if (!cmd->cmd[1])
		return (0);

	i = 1;
	while (cmd->cmd[i])
	{
		if (!is_valid_identifier(cmd->cmd[i]))
		{
			ft_putstr_fd("unset: ", 2);
			ft_putstr_fd(cmd->cmd[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			ret = 1;
		}
		else
			unset_var(cmd->cmd[i], env);
		i++;
	}
	return (ret);
}