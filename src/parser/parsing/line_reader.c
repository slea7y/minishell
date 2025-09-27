/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 02:29:06 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/27 02:29:06 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/parser.h"
#include "../../../includes/heredoc.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif

static int	append_from_read(int fd, char **buffer)
{
	char	read_buf[BUFFER_SIZE + 1];
	char	*temp;
	int		bytes_read;

	bytes_read = read(fd, read_buf, BUFFER_SIZE);
	if (bytes_read < 0)
		return (-1);
	if (bytes_read == 0)
		return (0);
	read_buf[bytes_read] = '\0';
	temp = ft_strjoin(*buffer, read_buf);
	free(*buffer);
	*buffer = temp;
	if (!*buffer)
		return (-1);
	return (1);
}

static char	*extract_line_from_buffer(char **buffer)
{
	char	*line;
	char	*temp;
	char	*newline_pos;

	newline_pos = ft_strchr(*buffer, '\n');
	if (newline_pos)
	{
		line = ft_substr(*buffer, 0, (newline_pos - *buffer) + 1);
		temp = ft_strdup(newline_pos + 1);
		free(*buffer);
		*buffer = temp;
		if (*buffer && !**buffer)
		{
			free(*buffer);
			*buffer = NULL;
		}
		return (line);
	}
	line = *buffer;
	*buffer = NULL;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer = NULL;
	int			read_status;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer)
	{
		buffer = ft_strdup("");
		if (!buffer)
			return (NULL);
	}
	read_status = 1;
	while (read_status > 0)
	{
		if (ft_strchr(buffer, '\n'))
			break ;
		read_status = append_from_read(fd, &buffer);
	}
	if (read_status < 0 || (read_status == 0 && (!buffer || !*buffer)))
	{
		if (buffer)
			free(buffer);
		buffer = NULL;
		return (NULL);
	}
	return (extract_line_from_buffer(&buffer));
}