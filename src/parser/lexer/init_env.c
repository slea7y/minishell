/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 19:13:10 by maja              #+#    #+#             */
/*   Updated: 2025/09/22 20:52:57 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
// #include "../../includes/parser.h"
#include "../../../includes/executor.h"

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
			// printf("malloc failed in init_list_env()\n");  // Commented out for tester
			return ;
		}
		init->key = key;
		init->value = value;
		init->next = NULL;
		add_node(env_list, init);
		i++;
	}
}

void free_env_list(t_env_list *env_list)
{
    t_env_node *current;
    t_env_node *next;

    if (!env_list)
        return;
    current = env_list->head;
    while (current)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
    free(env_list);
}