/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segmentation_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 20:25:22 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/23 17:29:04 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"

/// @brief processes nested quotes within a segment
/// @param token the token to add segments to
/// @param content the content to process
static void	process_nested_quotes_in_segment(t_token *token, char *content)
{
	int		i;
	int		start;
	char	quote_char;

	if (!content)
		return;
	i = 0;
	start = 0;
	while (content[i])
	{
		if (content[i] == '"' || content[i] == '\'')
		{
			quote_char = content[i];
			// Add text before quote as normal segment (only if there is text)
			if (i > start)
				add_segment_to_token(token, ft_substr(content, start, i - start), SEG_NORMAL_QUOTE);
			
			// Skip opening quote
			i++;
			start = i;
			
			// Find closing quote
			while (content[i] && content[i] != quote_char)
				i++;
			
			// Add quoted content (without quotes) - only if not empty
			if (i > start)
			{
				if (quote_char == '\'')
					add_segment_to_token(token, ft_substr(content, start, i - start), SEG_SINGLE_QUOTE);
				else if (quote_char == '"')
					add_segment_to_token(token, ft_substr(content, start, i - start), SEG_DOUBLE_QUOTE);
			}
			
			// Skip closing quote
			if (content[i] == quote_char)
				i++;
			start = i;
		}
		else
			i++;
	}
	// Add remaining text as normal segment (only if there is text)
	if (i > start)
		add_segment_to_token(token, ft_substr(content, start, i - start), SEG_NORMAL_QUOTE);
}

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
		add_segment_to_token(token, ft_substr(token->value, *start, *index
				- *start), SEG_NORMAL_QUOTE);
	
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
	
	// Add quoted content (without quotes) - only if not empty
	if (*index > *start)
	{
		char *quoted_content = ft_substr(token->value, *start, *index - *start);
		if (quoted_content)
		{
			// Check if the quoted content contains nested quotes
			if (seg_type == SEG_SINGLE_QUOTE && ft_strchr(quoted_content, '"'))
			{
				// For single-quoted content with double quotes, process the double quotes
				process_nested_quotes_in_segment(token, quoted_content);
			}
			else if (seg_type == SEG_DOUBLE_QUOTE && ft_strchr(quoted_content, '\''))
			{
				// For double-quoted content with single quotes, process the single quotes
				process_nested_quotes_in_segment(token, quoted_content);
			}
			else
			{
				// No nested quotes, add as normal segment
				add_segment_to_token(token, quoted_content, seg_type);
			}
			free(quoted_content);
		}
	}
	// Empty quotes should not create segments
	
	// Skip closing quote
	if (token->value[*index] == quote_char)
		(*index)++;
	*start = *index;
	return (true);
}
