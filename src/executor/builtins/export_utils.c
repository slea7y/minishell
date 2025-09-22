/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:28:25 by majkijew          #+#    #+#             */
/*   Updated: 2025/09/22 00:33:29 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/executor.h"

/// @brief // Valid Identifier Rules (POSIX-compatible)
// Must start with a letter or underscore (A-Z, a-z, or _)
// May contain only letters, digits, or underscores after the first character
// Cannot contain special characters, such as -, +, !, @, ., =, /, etc.
// In the context of export, the name part (before =) must follow these rules
/// @param args TO DO FOR THE PARSER <<<----------------------------------------
/// @return	valid = 1 || false = 0
int	is_valid_identifier(char *args)
{
	int	i;

	if (!args || !args[0])
		return (0);
	if (!ft_isalpha(args[0]) && args[0] != '_')
		return (0);
	i = 0;
	while (args[i] != '=' && args[i])
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	check_if_var_exist(char *key, t_env_list *env)
{
	t_env_node	*current;

	if (!key || !env)
		return (0);
	current = env->head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

void	add_new_var(char *key, char *value, t_env_list *env)
{
	t_env_node	*new;

	if (!key || !value || !env)
		return ;
	new = malloc(sizeof(t_env_node));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	if (!env->head)
	{
		env->head = new;
		env->tail = new;
	}
	else
	{
		env->tail->next = new;
		env->tail = new;
	}
	env->size++;
}

void	change_var_value(char *key, char *new_value, t_env_list *env)
{
	t_env_node	*current;

	if (!key || !env || !new_value)
		return ;
	current = env->head;
	while (current)
	{
		if (ft_strcmp(key, current->key) == 0)
		{
			free(current->value);
			current->value = strdup(new_value);
			return ;
		}
		current = current->next;
	}
}

void	add_change_export(char *args, t_env_list *env)
{
	char	*break_point;
	char	*key;
	char	*value;

	break_point = ft_strchr(args, '=');
	if (!break_point)
		return ;
	key = ft_substr(args, 0, break_point - args);
	value = ft_strdup(break_point + 1);
	if (!check_if_var_exist(key, env))
		add_new_var(key, value, env);
	else
		change_var_value(key, value, env);
	free(key);
	free(value);
}
