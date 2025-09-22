/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segmentation_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 20:25:22 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/21 22:17:22 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"

/// @brief checks if char is quote. adds text before latest char with
/// first add_segment_to_token. Checks if is singe or double quote. 
///Loops through quoted section. adds token. sets pointer to pos after quote
/// @param token
/// @param index position while looping
/// @param start start of current segment
/// @return
bool	handle_quoted_content(t_token *token, int *index, int *start)
{
	char				quote_char;
	t_seg_quote_type	seg_type;

	quote_char = token->value[*index];
	if (quote_char != '\'' && quote_char != '"')
		return (false);
	if (*index > *start)
		add_segment_to_token(token, ft_substr(token->value, *start, *index
				- *start), SEG_NORMAL_QUOTE);
	if (quote_char == '\'')
		seg_type = SEG_SINGLE_QUOTE;
	else if (quote_char == '"')
		seg_type = SEG_DOUBLE_QUOTE;
	(*index)++;
	*start = *index;
	while (token->value[*index] && token->value[*index] != quote_char)
		(*index)++;
	if (*index > *start)
		add_segment_to_token(token, ft_substr(token->value, *start, *index
				- *start), seg_type);
	if (token->value[*index] == quote_char)
		(*index)++;
	*start = *index;
	return (true);
}
