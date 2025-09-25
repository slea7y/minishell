/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segmentation_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 20:25:22 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/25 01:51:24 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"


/// @brief handles quoted content and strips quotes from segments
/// @param index position while looping
/// @param start start of current segment
/// @return true if quote was handled, false otherwise
bool	handle_quoted_content(t_token *token, int *index, int *start)
{
	char				quote_char;
	t_seg_quote_type	seg_type;

	quote_char = token->value[*index];
	if (quote_char != '\'' && quote_char != '"')
		return (false);
	
	// Add text before quote as normal segment (only if there is text)
	if (*index > *start)
	{
		char *before_quote = ft_substr(token->value, *start, *index - *start);
		if (before_quote)
		{
			add_segment_to_token(token, before_quote, SEG_NORMAL_QUOTE);
			free(before_quote);
		}
	}
	
	// Determine quote type
	if (quote_char == '\'')
		seg_type = SEG_SINGLE_QUOTE;
	else if (quote_char == '"')
		seg_type = SEG_DOUBLE_QUOTE;
	
	// Skip opening quote
	(*index)++;
	*start = *index;
	
	// Find closing quote
	while (token->value[*index] && token->value[*index] != quote_char)
		(*index)++;
	
	// Add quoted content (including empty quotes)
	if (*index > *start)
	{
		char *quoted_content = ft_substr(token->value, *start, *index - *start);
		if (quoted_content)
		{
			// For single quotes: everything is literal (no expansion or nested processing)
			if (seg_type == SEG_SINGLE_QUOTE)
			{
				add_segment_to_token(token, quoted_content, seg_type);
			}
			// For double quotes: add as normal segment
			else if (seg_type == SEG_DOUBLE_QUOTE)
			{
				add_segment_to_token(token, quoted_content, seg_type);
			}
			free(quoted_content);
		}
	}
	else
	{
		// Handle empty quotes by creating empty segment
		add_segment_to_token(token, ft_strdup(""), seg_type);
	}
	
	// Skip closing quote
	if (token->value[*index] == quote_char)
		(*index)++;
	*start = *index;
	return (true);
}
