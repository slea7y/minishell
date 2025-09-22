/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 22:43:15 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/22 15:00:39 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"
#include "../../../includes/executor.h"

/// @brief checks if  snippet is a buildint fct.  uppercase -> lower case
/// @param token_list
/// @param start
/// @param i
/// @return true is builint, false just a word lel
bool	check_builtin(t_token_list *token_list, int start, int i)
{
	char		*word;
	int			j;
	char	*builtins[] = {"echo", "cd", "pwd", "export", "unset", "env",
			"exit", NULL};

	j = 0;
	word = ft_substr(token_list->prompt, start, (i - start));
	if (!word)
		return (false);
	while (word[j])
	{
		if (word[j] >= 65 && word[j] <= 90)
			word[j] += 32;
		j++;
	}
	j = 0;
	while (builtins[j])
	{
		if (ft_strcmp(word, builtins[j]) == 0)
			return (true);
		j++;
	}
	return (false);
}

/// @brief checks if the given char is space
/// @param c
/// @return true for whitespace false for non whitespace char
bool	check_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\r'
		|| c == '\f')
		return (true);
	return (false);
}


//!!!!!!!!!!!!! needs update to fix edgecase with ""hello
/// @brief checks if current and following char are of the same quote_types
/// @param token_list and i for index
/// @return true for quote, false for no quote
bool	check_empty_quote(t_token_list *token_list, int i)
{
	if (token_list->prompt[i] == '"' && token_list->prompt[i + 1] == '"')
		return (true);
	if (token_list->prompt[i] == '\'' && token_list->prompt[i + 1] == '\'')
		return (true);
	return (false);
}

/// @brief checks if given char is quote
/// @param token_list and i for index
/// @return true for quote, false for no quote
bool	check_quote(t_token_list *token_list, int i)
{
	if (token_list->prompt[i] == '"' || token_list->prompt[i] == '\'')
		return (true);
	return (false);
}

/// @brief checks if current char is a redirection
/// @param token_list
/// @param i
/// @return true or false for case
bool	check_redirection(t_token_list *token_list, int i)
{
	if (token_list->prompt[i] && (token_list->prompt[i] == '<'
			|| token_list->prompt[i] == '>'))
		return (true);
	return (false);
}

/// @brief checks if current char is a token separation char
/// @param token_list
/// @param i
/// @return false if it is not a separation char
/// true if it is a separation char
bool	check_token_split(t_token_list *token_list, int i)
{
	if (token_list->prompt[i] && !check_space(token_list->prompt[i])
		&& token_list->prompt[i] != '|' && token_list->prompt[i] != '>'
		&& token_list->prompt[i] != '<')
		return (false);
	else
		return (true);
}
