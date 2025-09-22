/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:52:42 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/21 22:22:13 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"


/// @brief does what it looks like lol. initialises the segment_list
/// @param
/// @return
t_segment_list	*init_segment_list(void)
{
	t_segment_list	*list;

	list = (t_segment_list *)malloc(sizeof(t_segment_list));
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}