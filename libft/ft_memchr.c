/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:38:06 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/05/29 19:15:07 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ns;
	size_t			idx;
	unsigned char	cc;

	cc = (unsigned char)c;
	idx = 0;
	ns = (unsigned char *)s;
	while (idx < n && ns[idx] != cc)
		idx++;
	if (idx < n && ns[idx] == cc)
		return ((unsigned char *)s + idx);
	return (NULL);
}
