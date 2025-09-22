/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 22:42:56 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/21 22:12:57 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"

/// @brief identifies the quote type from a empty
/// quote passes the char and following char to found_token
/// @param token_list
/// @param i position of first char after quotes incremented before passing
int	handle_empty_quotes(t_token_list *token_list, int i)
{
	char			quote_char;
	t_token_type	token_type;

	quote_char = token_list->prompt[i];
	if (quote_char == '"')
		token_type = TOKEN_DOUBLE_QUOTES;
	else
		token_type = TOKEN_SINGLE_QUOTES;
	found_token(token_list, i, i + 1, token_type);
	return (i + 2);
}

/// @brief defines token_type of redirection if Append >>,
///			outfile >,Heredoc << or Infile <. calls found_token
/// @param token_list
/// @param i
/// @return one or two increments from the given i
///	depending if its a one or two char redirection
int	handle_redirection(t_token_list *token_list, int i)
{
	int	start;

	start = i;
	if (token_list->prompt[i] == '>')
	{
		if (token_list->prompt[i + 1] == '>')
			return (found_token(token_list, start, i + 2, TOKEN_APPEND), i + 2);
		else
			return (found_token(token_list, start, i + 1, TOKEN_OUTFILE), i
				+ 1);
	}
	else if (token_list->prompt[i] == '<')
	{
		if (token_list->prompt[i + 1] == '<')
			return (found_token(token_list, start, i + 2, TOKEN_HEREDOC), i
				+ 2);
		else
			return (found_token(token_list, start, i + 1, TOKEN_INFILE), i + 1);
	}
	return (i);
}

/// @brief handles pipe token/ calls found_token with token_type pipe
/// @param token_list
/// @param i
/// @return current i position in prompt plus one
int	handle_pipe(t_token_list *token_list, int i)
{
	found_token(token_list, i, i + 1, TOKEN_PIPE);
	return (i + 1);
}

/// @brief called by handle_word_master. returns the end of the quote
/// @param token_list
/// @param i
/// @return position of next same quote
// int	handle_word_in_quotes(t_token_list *token_list, int i)
// {
// 	char	quote_type;

// 	quote_type = token_list->prompt[i];
// 	i++;
// 	while (token_list->prompt[i] && token_list->prompt[i] != quote_type)
// 		i++;
// 	if (token_list->prompt[i] == quote_type)
// 		return (i);
// 	if (!token_list->prompt[i])
// 	{
// 		token_list->found_error = 1;
// 		ft_putstr_fd("teeestminishell: syntax error: unclosed quotes\n", 2);
// 		return (i);
// 	}
// 	return (i);
// }
int	handle_word_in_quotes(t_token_list *token_list, int i)
{
	char	quote_type;

	printf("DEBUG: handle_word_in_quotes called with i=%d, char='%c'\n", i,
		token_list->prompt[i]);
	quote_type = token_list->prompt[i];
	printf("DEBUG: quote_type='%c'\n", quote_type);
	i++;
	printf("DEBUG: starting search at i=%d\n", i);
	while (token_list->prompt[i] && token_list->prompt[i] != quote_type)
	{
		printf("DEBUG: checking i=%d, char='%c'\n", i, token_list->prompt[i]);
		i++;
	}
	printf("DEBUG: loop ended at i=%d, char='%c'\n", i,
		token_list->prompt[i] ? token_list->prompt[i] : '\0');
	if (token_list->prompt[i] == quote_type)
	{
		printf("DEBUG: Found closing quote at i=%d\n", i);
		return (i + 1);
	}
	if (!token_list->prompt[i])
	{
		printf("DEBUG: ERROR- Reached end of string without closing quote. Handle_word_in_quotes\n");
		token_list->found_error = 1;
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		return (i);
	}
	return (i);
}

// static int	recursive_word_finder(t_token_list *token_list, int i)
// {
// 	int	next_i;

// 	if (!token_list->prompt[i] || check_space(token_list->prompt[i])
// 		|| token_list->prompt[i] == '|' || check_redirection(token_list, i))
// 		return (i);
// 	if (check_quote(token_list, i))
// 		next_i = handle_word_in_quotes(token_list, i);
// 	else
// 		next_i = i + 1;
// 	return (recursive_word_finder(token_list, next_i));
// }

// int	handle_word_master(t_token_list *token_list, int i)
// {
// 	int	start;
// 	int	end;

// 	start = i;
// 	end = recursive_word_finder(token_list, i);
// 	found_token(token_list, start, end, TOKEN_WORD);
// 	return (end + 1);
// }

/// @brief loops trough the prompt until a token split char is detected
/// If there is quotes, it will take them as a whole token
/// @param token_list
/// @param i
/// @return position of next char after current token
int	handle_word_master(t_token_list *token_list, int i)
{
	int	start;

	start = i;
	while (token_list->prompt[i] && !check_space(token_list->prompt[i])
		&& token_list->prompt[i] != '|' && !check_redirection(token_list, i)
		&& token_list->prompt[i] != '&')
	{
		if (check_quote(token_list, i))
			i = handle_word_in_quotes(token_list, i);
		else
			i++;
	}
	found_token(token_list, start, i, TOKEN_WORD);
	return (i + 1);
}

/// @brief handles ampersand token & calls found_token with token_type ampersand
/// @param token_list
/// @param i
/// @return current i position in prompt plus two (for &&)
int	handle_ampersand(t_token_list *token_list, int i)
{
	found_token(token_list, i, i + 1, TOKEN_AMPERSAND);
	return (i + 1);
}
