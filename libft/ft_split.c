/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 10:35:39 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/21 21:36:23 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_word_count(const char *s, char c)
{
	int	i;
	int	word_count;

	word_count = 0;
	i = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		if (s[i])
			word_count++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (word_count);
}

static char	**ft_put_word(char const *s, int word_index, char **dst,
		int *indices)
{
	int	char_index;
	int	start_word;
	int	end_word;

	start_word = indices[0];
	end_word = indices[1];
	char_index = 0;
	dst[word_index] = (char *)malloc(sizeof(char) * (end_word - start_word
				+ 1));
	if (!dst[word_index])
		return (NULL);
	while (start_word < end_word)
	{
		dst[word_index][char_index] = s[start_word];
		char_index++;
		start_word++;
	}
	dst[word_index][char_index] = '\0';
	return (dst);
}

static char	**ft_strsplit(char const *s, char c, char **dst, int word_count)
{
	int	i;
	int	start_word;
	int	word_index;
	int	indices[2];

	word_index = 0;
	i = 0;
	while (s[i] != 0 && word_count > word_index)
	{
		while (s[i] == c)
			i++;
		start_word = i;
		while (s[i] != c && s[i] != '\0')
			i++;
		indices[0] = start_word;
		indices[1] = i;
		if (!ft_put_word(s, word_index, dst, indices))
		{
			while (word_index > 0)
				free(dst[--word_index]);
			return (NULL);
		}
		word_index++;
	}
	return (dst);
}

char	**ft_split(char const *s, char c)
{
	char	**dst;
	int		word_count;

	if (!s)
		return (NULL);
	word_count = ft_word_count(s, c);
	dst = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!dst)
		return (NULL);
	if (!ft_strsplit(s, c, dst, word_count))
	{
		free(dst);
		return (NULL);
	}
	dst[word_count] = NULL;
	return (dst);
}
