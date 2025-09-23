/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:40:00 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/23 15:40:00 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../../includes/parser.h"
#include "../../../includes/heredoc.h"
#include <fcntl.h>
#include <unistd.h>

/// @brief creates a temporary file for heredoc content
/// @param delimiter the heredoc delimiter (unused but kept for future use)
/// @return filename of created temp file or NULL on error
static char	*create_heredoc_temp_file(char *delimiter)
{
	char	*temp_filename;
	int		fd;
	int		counter;

	(void)delimiter;  // Suppress unused parameter warning
	counter = 0;
	temp_filename = ft_strjoin("/tmp/heredoc_", ft_itoa(counter));
	if (!temp_filename)
		return (NULL);
	fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	while (fd == -1 && counter < 1000)
	{
		free(temp_filename);
		counter++;
		temp_filename = ft_strjoin("/tmp/heredoc_", ft_itoa(counter));
		if (!temp_filename)
			return (NULL);
		fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	}
	if (fd == -1)
	{
		free(temp_filename);
		return (NULL);
	}
	close(fd);
	return (temp_filename);
}

/// @brief reads input until delimiter is found
/// @param delimiter the heredoc delimiter
/// @param temp_filename filename of temp file
/// @return 0 on success, -1 on error
static int	read_until_delimiter(char *delimiter, char *temp_filename)
{
	char	*line;
	int		fd;
	int		delimiter_len;

	fd = open(temp_filename, O_WRONLY | O_APPEND);
	if (fd == -1)
		return (-1);
	delimiter_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
	if (ft_strncmp(line, delimiter, delimiter_len) == 0 && 
		(int)ft_strlen(line) == delimiter_len)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (0);
}

/// @brief handles heredoc redirection
/// @param delimiter the heredoc delimiter
/// @return filename of temp file or NULL on error
char	*handle_heredoc(char *delimiter)
{
	char	*temp_filename;

	if (!delimiter)
		return (NULL);
	temp_filename = create_heredoc_temp_file(delimiter);
	if (!temp_filename)
		return (NULL);
	if (read_until_delimiter(delimiter, temp_filename) == -1)
	{
		unlink(temp_filename);
		free(temp_filename);
		return (NULL);
	}
	return (temp_filename);
}

/// @brief cleans up heredoc temp files
/// @param filename temp file to clean up
void	cleanup_heredoc(char *filename)
{
	if (filename)
	{
		unlink(filename);
		free(filename);
	}
}
