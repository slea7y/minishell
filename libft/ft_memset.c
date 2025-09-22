/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:11:59 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/05/27 17:58:08 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			count;
	unsigned char	*bb;

	bb = b;
	count = 0;
	while (len != 0)
	{
		bb[count] = (unsigned char)c;
		len--;
		count++;
	}
	return (b);
}
