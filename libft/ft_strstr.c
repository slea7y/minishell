/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:40:32 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/05/29 19:14:51 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	int	idx;
	int	len_ndl;
	int	chk;

	chk = 0;
	idx = -1;
	len_ndl = 0;
	ft_strlen(needle);
	if (needle[idx] == '\0')
		return ((char *)haystack);
	while (haystack[idx++])
	{
		if (haystack[idx] == needle[chk])
		{
			while (haystack[idx] == needle[chk] && chk != len_ndl)
			{
				idx++;
				chk++;
			}
			if (chk == len_ndl)
				return ((char *)(haystack + len_ndl - 1));
		}
	}
	return (NULL);
}
