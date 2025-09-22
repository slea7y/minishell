/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:43 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/05/27 17:59:05 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_n_len(int n)
{
	int	n_len;

	n_len = 0;
	if (n < 0)
		n_len++;
	while (n != 0)
	{
		n = n / 10;
		n_len++;
	}
	return (n_len);
}

char	*ft_itoa(int n)
{
	int		n_len;
	char	*dst;
	long	num;

	if (n == 0)
		return (ft_strdup("0"));
	n_len = ft_n_len(n);
	dst = (char *)malloc(n_len + 1);
	if (!dst)
		return (NULL);
	dst[n_len] = '\0';
	num = n;
	if (num < 0)
	{
		dst[0] = '-';
		num = num * -1;
	}
	while (num > 0)
	{
		n_len--;
		dst[n_len] = ((num % 10) + '0');
		num = num / 10;
	}
	return (dst);
}
