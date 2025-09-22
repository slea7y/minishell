/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_struct.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 23:07:36 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/21 22:14:19 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"

/// @brief fills in the value of the linked list segment. called by add_token
/// @param value
/// @param token_type
/// @return
static t_token	*create_new_token(char *value, t_token_type token_type)
{
	t_token	*new_token;

	new_token = ft_calloc(sizeof(t_token), 1);
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value);
	if (!new_token->value)
		return (NULL);
	new_token->token = token_type;
	new_token->next = NULL;
	new_token->segment_list = NULL;
	return (new_token);
}

/// @brief creates new linked listsegment and connects it to the meta_struct
/// @param token_list
/// @param value token
/// @param token_type
void	add_token(t_token_list *token_list, char *value,
		t_token_type token_type)
{
	t_token	*new_token;

	if (!token_list || !value)
		return ;
	new_token = create_new_token(value, token_type);
	if (!token_list->head)
	{
		token_list->head = new_token;
		token_list->tail = new_token;
	}
	else
	{
		token_list->tail->next = new_token;
		token_list->tail = new_token;
	}
	token_list->size++;
}

/// @brief checks the values,copys token snippet from prompt calls add_token
/// @param token_list
/// @param start start of token snippet from prompt
/// @param end end pf token snippet from prompt
/// @param token_type predefined type, from function where found_token is called

void	found_token(t_token_list *token_list, int start, int end,
		t_token_type token_type)
{
	char	*value;

	if (!token_list || !token_list->prompt)
		return ;
	if (start < 0 || end < start)
		return ;
	value = ft_substr(token_list->prompt, start, (end - start));
	if (!value)
		return ;
	add_token(token_list, value, token_type);
}
