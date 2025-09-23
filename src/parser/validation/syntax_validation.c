/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:46:46 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/23 15:46:46 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"
#include "../../../includes/syntax_validation.h"

/// @brief validates pipe syntax in token list
/// @param tokens token list to validate
/// @return true if valid, false if syntax error
bool	validate_pipes(t_token_list *tokens)
{
	t_token	*current;
	bool	expecting_cmd;

	if (!tokens || !tokens->head)
		return (false);
	current = tokens->head;
	expecting_cmd = true;
	while (current)
	{
		if (current->token == TOKEN_PIPE)
		{
			if (expecting_cmd)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
				return (false);
			}
			expecting_cmd = true;
		}
		else if (current->token == TOKEN_WORD || current->token == TOKEN_BUILTIN)
		{
			expecting_cmd = false;
		}
		current = current->next;
	}
	if (expecting_cmd)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (false);
	}
	return (true);
}

/// @brief validates redirection syntax
/// @param tokens token list to validate
/// @return true if valid, false if syntax error
bool	validate_redirections(t_token_list *tokens)
{
	t_token	*current;

	if (!tokens || !tokens->head)
		return (false);
	current = tokens->head;
	while (current)
	{
		if (current->token >= TOKEN_OUTFILE && current->token <= TOKEN_HEREDOC)
		{
			if (!current->next || (current->next->token != TOKEN_WORD && 
				current->next->token != TOKEN_BUILTIN))
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
				if (current->token == TOKEN_OUTFILE)
					ft_putstr_fd(">", 2);
				else if (current->token == TOKEN_APPEND)
					ft_putstr_fd(">>", 2);
				else if (current->token == TOKEN_INFILE)
					ft_putstr_fd("<", 2);
				else if (current->token == TOKEN_HEREDOC)
					ft_putstr_fd("<<", 2);
				ft_putstr_fd("'\n", 2);
				return (false);
			}
		}
		current = current->next;
	}
	return (true);
}

/// @brief validates overall syntax of token list
/// @param tokens token list to validate
/// @return true if valid, false if syntax error
bool	validate_syntax(t_token_list *tokens)
{
	if (!tokens || !tokens->head)
		return (false);
	if (!validate_quotes(tokens))
		return (false);
	if (!validate_pipes(tokens))
		return (false);
	if (!validate_redirections(tokens))
		return (false);
	return (true);
}
