/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segmentation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 20:13:13 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/27 04:25:20 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"

/// @brief creates a single segment and fills it with the given values
/// @param value
/// @param type
/// @return pointer t_segment struct
static t_segment	*create_and_fill_segment(char *value, t_seg_quote_type type)
{
	t_segment	*segment;

	if (!value)
		return (NULL);
	segment = (t_segment *)malloc(sizeof(t_segment));
	if (!segment)
		return (NULL);
	if (!value)
		return (NULL);
	segment->value = ft_strdup(value);
	segment->type = type;
	segment->next = NULL;
	if (!segment->value)
	{
		free(segment);
		return (NULL);
	}
	return (segment);
}

/// @brief calls create_and_fill_segment to create segment,
/// then adds it to segment_list.called the first time initialises the struct.
/// @param token
/// @param value
/// @param type

void	add_segment_to_token(t_token *token, char *value, t_seg_quote_type type)
{
	t_segment	*new_seg;

	if (!token)
		return ;
	if (!token->segment_list)
	{
		token->segment_list = init_segment_list();
		if (!token->segment_list)
			return ;
	}
	new_seg = create_and_fill_segment(value, type);
	if (!new_seg)
		return ;
	if (!token->segment_list->head)
		token->segment_list->head = new_seg;
	else
		token->segment_list->tail->next = new_seg;
	token->segment_list->tail = new_seg;
	token->segment_list->size++;
}

void dissect_token(t_token *token)
{
	int i;
	int start;
	char quote_char;

	if (!token || !token->value)
		return;
	i = 0;
	start = 0;
	while (token->value[i])
	{
		if (token->value[i] == '\'' || token->value[i] == '"')
		{
			if (i > start)
				add_segment_to_token(token, ft_substr(token->value, start, i - start), SEG_NORMAL_QUOTE);
			
			quote_char = token->value[i];
			start = i + 1; 
			i++;
			while (token->value[i] && token->value[i] != quote_char)
				i++;
						if (quote_char == '\'')
				add_segment_to_token(token, ft_substr(token->value, start, i - start), SEG_SINGLE_QUOTE);
			else
				add_segment_to_token(token, ft_substr(token->value, start, i - start), SEG_DOUBLE_QUOTE);

			if (token->value[i])
				i++;
			start = i;
		}
		else
			i++;
	}
	if (i > start)
		add_segment_to_token(token, ft_substr(token->value, start, i - start), SEG_NORMAL_QUOTE);
}


void assemble_final_token(t_token *token)
{
	t_segment   *current_seg;
	char		*new_value;
	char		*temp;

	if (!token || !token->segment_list || !token->segment_list->head)
		return;
	
	new_value = ft_strdup("");
	current_seg = token->segment_list->head;
	while (current_seg)
	{
		if (current_seg->value)
		{
			// Nutze eine temporäre Variable, um ft_strjoin sicher zu verwenden
			temp = ft_strjoin(new_value, current_seg->value);
			free(new_value);
			new_value = temp;
		}
		current_seg = current_seg->next;
	}

	// Ersetze den alten Token-Wert (mit Anführungszeichen) durch den neuen (ohne)
	free(token->value);
	token->value = new_value;
}

/// @brief loop through t_token_list. segment tokens if possible.
/// loop through segmenten_token_list if somethg to expand.
/// reconstruct tokens that are split through parsing
/// @param tokens
/// @param env_list
void	start_segmentation(t_token_list *tokens, t_shell_ctx *ctx)
{
	t_token		*current_token;
	t_segment	*current_seg;

	if (!tokens || !tokens->head || !ctx)
	{
		return ;
	}
	current_token = tokens->head;
	while (current_token)
	{
		dissect_token(current_token);
		if (current_token->segment_list)
		{
			current_seg = current_token->segment_list->head;
			while (current_seg)
			{
				expand_variables_in_segment(current_seg, ctx);
				current_seg = current_seg->next;
			}
		}
		assemble_final_token(current_token);
		current_token = current_token->next;
	}
}
