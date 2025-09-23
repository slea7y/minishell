/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segmentation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 20:13:13 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/23 17:20:19 by tdietz-r         ###   ########.fr       */
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
	if (!type)
		return (NULL);
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
// 	$VAR
// asd asd asd
// / @brief splits given token into segments. loop through given token check char
// / for quote and adds quoted section with calling handle_quted_content.
// / @param token
void	dissect_token(t_token *token)
{
	int	i;
	int	start;

	if (!token || !token->value)
		return ;
	// printf("  dissect_token called for: %s\n", token->value); // debug
	i = 0;
	start = 0;
	while (token->value[i])
	{
		if (handle_quoted_content(token, &i, &start))
			continue ;
		i++;
	}
	// Add remaining text as normal segment (only if there is text)
	if (i > start)
		add_segment_to_token(token, ft_substr(token->value, start, i - start),
			SEG_NORMAL_QUOTE);
}

// static void	recursive_dissector(t_token *token, int i, int start)
// {
// 	int	next_i;
// 	int	next_start;

// 	if (!token->value[i])
// 	{
// 		if (i > start)
// 			add_segment_to_token(token, ft_substr(token->value, start, i
// 					- start), SEG_NORMAL_QUOTE);
// 		return ;
// 	}
// 	next_i = i;
// 	next_start = start;
// 	if (handle_quoted_content(token, &next_i, &next_start))
// 		recursive_dissector(token, next_i, next_start);
// 	else
// 		recursive_dissector(token, i + 1, start);
// }

// void	dissect_token(t_token *token)
// {
// 	if (!token || !token->value)
// 		return ;
// 	recursive_dissector(token, 0, 0);
// }

/// @brief strips quotes from segments and joins them to final token
/// @param token
static void	strip_quotes_from_segments(t_token *token)
{
	t_segment	*current_seg;

	if (!token || !token->segment_list || !token->segment_list->head)
		return ;
	current_seg = token->segment_list->head;
	while (current_seg)
	{
		// Quotes are already stripped in segmentation_handler.c
		// This function is kept for potential future use
		current_seg = current_seg->next;
	}
}

/// @brief loops through the segments and joins the segments to the final token
/// @param token
void	assemble_final_token(t_token *token)
{
	t_segment	*current_seg;
	char		*new_value;

	if (!token || !token->segment_list || !token->segment_list->head)
		return ;
	strip_quotes_from_segments(token);
	new_value = ft_strdup("");
	current_seg = token->segment_list->head;
	while (current_seg)
	{
		if (current_seg->value && ft_strlen(current_seg->value) > 0)
			new_value = ft_strjoin(new_value, current_seg->value);
		current_seg = current_seg->next;
	}
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
