/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:52:16 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/05/27 17:58:43 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*ndst;
	unsigned const char	*nsrc;
	int					i;

	if ((dst == NULL && src == NULL))
		return (NULL);
	ndst = (unsigned char *)dst;
	nsrc = (const unsigned char *)src;
	if (nsrc < ndst && nsrc + len > ndst)
	{
		i = len;
		while (i > 0)
		{
			i--;
			ndst[i] = nsrc[i];
		}
	}
	else
	{
		while (len--)
			*ndst++ = *nsrc++;
	}
	return (dst);
}
