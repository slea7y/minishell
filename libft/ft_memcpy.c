/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:46:28 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/05/27 17:58:33 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned const char	*nsrc;
	unsigned char		*ndst;

	nsrc = src;
	ndst = dst;
	if (!dst && !src)
		return (NULL);
	while (n--)
		ndst[n] = nsrc[n];
	return (dst);
}
