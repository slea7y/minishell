/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:52:42 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/21 22:23:10 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"


/// @brief initialise the token list like the name said lol
/// @param prompt combined string of arguments
/// @return 
t_token_list	*initialise_token_list(char *prompt)
{
	t_token_list	*token_list;

	token_list = ft_calloc(1, sizeof(t_token_list));
	if (!token_list)
		return (NULL);
	token_list->prompt = ft_strdup(prompt);
	if (!token_list->prompt)
		return (NULL);
	return (token_list);
}
