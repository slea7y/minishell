/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokanisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 17:25:10 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/23 16:03:25 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/parser.h"
#include "../../../includes/syntax_validation.h"

/// @brief goes through the prompt and checks for different chars for token types
/// @param token_list The token list to populate
static void	split_tokenisation(t_token_list *token_list)
{
	int		i;

	i = 0;
	while (token_list->prompt[i])
	{
		while (token_list->prompt[i] && check_space(token_list->prompt[i]))
			i++;
		if (!token_list->prompt[i])
			break;
		if (check_empty_quote(token_list, i))
			i = handle_empty_quotes(token_list, i);
		else if (check_redirection(token_list, i))
			i = handle_redirection(token_list, i);
		else if (token_list->prompt[i] == '|')
			i = handle_pipe(token_list, i);
		else if (token_list->prompt[i] == '&')
			i = handle_ampersand(token_list, i);
		else
			i = handle_word_master(token_list, i);
		if (i == -1)  // Error handling
		{
			token_list->found_error = 1;
			break;
		}
	}
}

/// @brief Initialize and populate token list from input
/// @param prompt Input string to parse
/// @return Initialized token list or NULL on error
t_token_list	*start_lexer(char *prompt)
{
	t_token_list	*token_list;

	if (!prompt)
		return (NULL);
	token_list = initialise_token_list(prompt);
	if (!token_list)
		return (NULL);
	
	// Validate quotes before tokenization
	if (!validate_quotes(token_list))
	{
		token_list->found_error = 1;
		free(token_list);
		return (NULL);
	}
	
	split_tokenisation(token_list);
	if (token_list->found_error)
	{
		// Clean up token list on error
		free_token_list(token_list);
		return (NULL);
	}
	
	// Validate overall syntax after tokenization
	if (!validate_syntax(token_list))
	{
		token_list->found_error = 1;
		free_token_list(token_list);
		return (NULL);
	}
	
	return (token_list);
}
