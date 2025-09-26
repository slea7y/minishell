#ifndef SYNTAX_VALIDATION_H
# define SYNTAX_VALIDATION_H

# include "parser.h"

/// @brief validates pipe syntax in token list
/// @param tokens token list to validate
/// @return true if valid, false if syntax error
bool	validate_pipes(t_token_list *tokens);

/// @brief validates redirection syntax
/// @param tokens token list to validate
/// @return true if valid, false if syntax error
bool	validate_redirections(t_token_list *tokens);

/// @brief validates that redirections don't appear at the end of input
/// @param tokens token list to validate
/// @return true if valid, false if syntax error
bool	validate_redirection_end(t_token_list *tokens);

/// @brief validates for invalid characters and malformed redirections
/// @param tokens token list to validate
/// @return true if valid, false if syntax error
bool	validate_invalid_chars(t_token_list *tokens);

/// @brief validates overall syntax of token list
/// @param tokens token list to validate
/// @return true if valid, false if syntax error
bool	validate_syntax(t_token_list *tokens);

#endif
