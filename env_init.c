/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:30:21 by majkijew          #+#    #+#             */
/*   Updated: 2025/08/08 18:37:14 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"

void	sort_env(t_env_list *env_list)
{
	t_env_node	*head;
	t_env_node	*ptr;
	char		*tmp_key;
	char		*tmp_value;

	head = env_list->head;
	ptr = head;
	if (!env_list || !env_list->head)
		return ;
	while (ptr && ptr->next)
	{
		if (strcmp(ptr->key, ptr->next->key) > 0)
		{
			tmp_key = ptr->key;
			ptr->key = ptr->next->key;
			ptr->next->key = tmp_key;
			tmp_value = ptr->value;
			ptr->value = ptr->next->value;
			ptr->next->value = tmp_value;
			ptr = head;
		}
		else
			ptr = ptr->next;
	}
}

char	*extract_key(const char	*env)
{
	char	*key;
	int		i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	key = malloc(sizeof(char) * (i + 1));
	if (!key)
		return (NULL);
	ft_strlcpy(key, env, i + 1);
	return (key);
}

char	*extract_value(const char *env)
{
	char	*break_point;

	break_point = ft_strchr(env, '=');
	if (!break_point)
		return (NULL);
	return (ft_strdup(break_point + 1));
}

void	add_node(t_env_list *env_list, t_env_node	*init)
{
	if (env_list->head == NULL)
		env_list->head = init;
	else
		env_list->tail->next = init;
	env_list->tail = init;
	env_list->size++;
}

void	init_list_env(t_env_list *env_list, char **env)
{
	t_env_node	*init;
	char		*value;
	char		*key;
	int			i;

	i = 0;
	env_list->head = NULL;
	env_list->tail = NULL;
	env_list->size = 0;
	while (env[i])
	{
		key = extract_key(env[i]);
		value = extract_value(env[i]);
		init = malloc(sizeof(t_env_node));
		if (!init)
		{
			printf("malloc failed in init_list_env()\n");
			return ;
		}
		init->key = key;
		init->value = value;
		init->next = NULL;
		add_node(env_list, init);
		i++;
	}
}
