/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:00:00 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/01/27 10:00:00 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"
#include "../../../includes/heredoc.h"
#include <fcntl.h>
#include <unistd.h>

static char	*get_temp_filename(int counter)
{
	char	*num_str;
	char	*filename;

	num_str = ft_itoa(counter);
	if (!num_str)
		return (NULL);
	filename = ft_strjoin("/tmp/heredoc_", num_str);
	free(num_str);
	return (filename);
}

static char	*create_temp_file_iterative(char *delimiter)
{
	char	*temp_filename;
	int		fd;
	int		counter;

	(void)delimiter;
	counter = 0;
	while (counter < 1000)
	{
		temp_filename = get_temp_filename(counter);
		if (!temp_filename)
			return (NULL);
		fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd != -1)
		{
			close(fd);
			return (temp_filename);
		}
		free(temp_filename);
		counter++;
	}
	return (NULL);
}

static char	*read_line_interactive(void)
{
	return (readline("> "));
}

static char	*read_line_non_interactive(void)
{
	return (get_next_line(STDIN_FILENO));
}

static char	*read_heredoc_line(int is_interactive)
{
	if (is_interactive)
		return (read_line_interactive());
	return (read_line_non_interactive());
}

static int	write_line_to_file(int fd, char *line, int delimiter_quoted, 
		t_shell_ctx *ctx)
{
	char	*expanded_line;

	if (delimiter_quoted)
		expanded_line = ft_strdup(line);
	else
		expanded_line = expand_line_variables(line, ctx);
	if (!expanded_line)
		return (-1);
	write(fd, expanded_line, ft_strlen(expanded_line));
	write(fd, "\n", 1);
	free(expanded_line);
	return (0);
}

static int	process_heredoc_line(char *line, char *delimiter, int fd,
	int delimiter_quoted, t_shell_ctx *ctx)
{
int		line_len;
int		is_delimiter;
size_t	delimiter_len;

line_len = ft_strlen(line);
delimiter_len = ft_strlen(delimiter);
is_delimiter = 0;
if (line_len > 0 && line[line_len - 1] == '\n')
{
	if ((size_t)(line_len - 1) == delimiter_len && \
		ft_strncmp(line, delimiter, delimiter_len) == 0)
		is_delimiter = 1;
}
else
{
	if ((size_t)line_len == delimiter_len && \
		ft_strncmp(line, delimiter, delimiter_len) == 0)
		is_delimiter = 1;
}
if (is_delimiter)
	return (1);
return (write_line_to_file(fd, line, delimiter_quoted, ctx));
}


char	*handle_heredoc(char *delimiter, int delimiter_quoted, t_shell_ctx *ctx)
{
	char	*temp_filename;
	int		fd;
	char	*line;
	int		is_interactive;

	if (!delimiter)
		return (NULL);
	temp_filename = create_temp_file_iterative(delimiter);
	if (!temp_filename)
		return (NULL);
	fd = open(temp_filename, O_WRONLY | O_APPEND);
	if (fd == -1)
	{
		free(temp_filename);
		return (NULL);
	}
	is_interactive = isatty(STDIN_FILENO);
	while (1)
	{
		line = read_heredoc_line(is_interactive);
		if (!line)
			break;
		if (process_heredoc_line(line, delimiter, fd, delimiter_quoted, ctx))
		{
			free(line);
			break;
		}
		free(line);
	}
	close(fd);
	return (temp_filename);
}
