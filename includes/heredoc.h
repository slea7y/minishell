#ifndef HEREDOC_H
# define HEREDOC_H

# include "parser.h"

/// @brief handles heredoc redirection
/// @param delimiter the heredoc delimiter
/// @return filename of temp file or NULL on error
char	*handle_heredoc(char *delimiter);

/// @brief cleans up heredoc temp files
/// @param filename temp file to clean up
void	cleanup_heredoc(char *filename);

#endif
