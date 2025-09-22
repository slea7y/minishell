/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:05:06 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/05/27 18:01:49 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	cc;
	int				len;

	len = 0;
	cc = (unsigned char)c;
	while (s[len])
		len++;
	if (c == '\0')
		return ((char *)(s + len));
	while (len >= 0)
	{
		if (s[len] == cc)
			return ((char *)(s + len));
		len--;
	}
	if (s[0] == cc)
		return ((char *)s);
	return (NULL);
}
