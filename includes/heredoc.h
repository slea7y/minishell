#ifndef HEREDOC_H
# define HEREDOC_H

# include "parser.h"

/// @brief expands variables in a line (central function)
/// @param line the line to expand
/// @param ctx shell context for variable expansion
/// @return expanded line or NULL on error
char	*expand_line_variables(char *line, t_shell_ctx *ctx);

/// @brief reads a line from file descriptor (get_next_line implementation)
/// @param fd file descriptor to read from
/// @return line read or NULL on error/EOF
char	*get_next_line(int fd);

/// @brief cleans up line reader static buffer
void	cleanup_line_reader(void);

/// @brief handles heredoc redirection (main function)
/// @param delimiter the heredoc delimiter
/// @param delimiter_quoted whether the delimiter is quoted
/// @param ctx shell context for variable expansion
/// @return filename of temp file or NULL on error
char	*handle_heredoc(char *delimiter, int delimiter_quoted, t_shell_ctx *ctx);

#endif
