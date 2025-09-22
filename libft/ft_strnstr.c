/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:40:32 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/05/29 19:14:55 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	idx;
	int		len_ndl;
	int		chk;

	idx = 0;
	len_ndl = ft_strlen(needle);
	if (len_ndl == 0)
		return ((char *)haystack);
	while (haystack[idx] && idx < len)
	{
		chk = 0;
		while (idx + chk < len && haystack[idx + chk] == needle[chk]
			&& chk < len_ndl)
		{
			chk++;
		}
		if (chk == len_ndl)
			return ((char *)(haystack + idx));
		chk = 0;
		idx++;
	}
	return (NULL);
}
