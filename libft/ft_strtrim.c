/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 22:16:43 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/05/27 18:01:59 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_fronttrim(char const *s1, char const *set)
{
	int	index_s1;
	int	index_set;
	int	found;

	index_s1 = 0;
	while (s1[index_s1])
	{
		found = 0;
		index_set = 0;
		while (set[index_set] && !found)
		{
			if (s1[index_s1] == set[index_set])
				found = 1;
			index_set++;
		}
		if (!found)
			return (index_s1);
		index_s1++;
	}
	return (index_s1);
}

static int	ft_backtrim(char const *s1, char const *set, int index_s1_front)
{
	int	index_s1;
	int	index_set;
	int	found;

	index_s1 = ft_strlen(s1) - 1;
	while (index_s1_front <= index_s1)
	{
		found = 0;
		index_set = 0;
		while (set[index_set] && !found)
		{
			if (s1[index_s1] == set[index_set])
				found = 1;
			index_set++;
		}
		if (!found)
			return (index_s1);
		index_s1--;
	}
	return (index_s1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		index_s1_front;
	int		index_s1_back;
	char	*dst;
	int		index_dst;

	index_dst = 0;
	if (!s1 || !set)
		return (NULL);
	index_s1_front = ft_fronttrim(s1, set);
	index_s1_back = ft_backtrim(s1, set, index_s1_front);
	if (index_s1_front > index_s1_back)
		return (ft_strdup(""));
	dst = (char *)malloc(index_s1_back - index_s1_front + 2);
	if (!dst)
		return (NULL);
	while (index_s1_front <= index_s1_back)
	{
		dst[index_dst] = s1[index_s1_front];
		index_dst++;
		index_s1_front++;
	}
	dst[index_dst] = '\0';
	return (dst);
}
