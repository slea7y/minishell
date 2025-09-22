/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 21:57:51 by majkijew          #+#    #+#             */
/*   Updated: 2025/09/22 00:22:58 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/executor.h"

void	unset_var(char **args, t_env_list *env)
{
	t_env_node	*current;
	t_env_node	*prev;

	if (!check_if_var_exist(args[1], env))
		return ;
	current = env->head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, args[1]) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				env->head = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	ft_unset(char **args, t_env_list *env)
{
	int	i;

	i = 1;
	if (args[i] == NULL)
	{
		ft_putstr_fd("unset: not enough arguments\n", 2);
		return ;
	}
	while (args[i] != NULL)
	{
		if (!is_valid_identifier(args[1]))
		{
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": invalid parameter name\n", 2);
		}
		else
		{
			unset_var(args, env);
		}
		i++;
	}
}
