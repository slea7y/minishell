/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 21:40:17 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/22 15:01:22 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"
#include "../../../includes/executor.h"


char *get_exit_code(void)
{
	return("");
}

char *get_env_value(t_env_list *env_list, char *var_name)
{
	(void) env_list;
	if (ft_strcmp("USER", var_name ) == 0)
		return("tdietz-r");
	if (ft_strcmp("pwd", var_name)== 0)
		return("/Users/tdietz-r/Documents/m3/minishell/src/parser");
	if (ft_strcmp("FOO", var_name) == 0)
		return("hello world this is a test");
	return("");
}

/// @brief expands $ with given env. loops through string,
/// checks for $. adds text before $. handles edgecase for $?, sets it to
/// @param segment
/// @param env_list
void	expand_variables_in_segment(t_segment *segment, t_env_list *env_list)
{
	char	*final_str;
	int		i;
	int		start;
	char	*var_name;

	if (!segment || !segment->value || segment->type == SEG_SINGLE_QUOTE)
		return ;
	final_str = ft_strdup("");
	i = 0;
	start = 0;
	while (segment->value[i])
	{
		if (segment->value[i] == '$' && segment->value[i + 1] != '\0')
		{
			final_str = ft_strjoin(final_str, ft_substr(segment->value, start, i
						- start));
			i++;
			start = i;
			if (segment->value[i] == '?')
			{
				final_str = ft_strjoin(final_str, get_exit_code());
				i++;
			}
			else
			{
				while (ft_isalnum(segment->value[i])
					|| segment->value[i] == '_')
					i++;
				var_name = ft_substr(segment->value, start, i - start);
				final_str = ft_strjoin(final_str, get_env_value(env_list,
							var_name));
			}
			start = i;
		}
		else
			i++;
	}
	final_str = ft_strjoin(final_str, ft_substr(segment->value, start, i
				- start));
	segment->value = final_str;
}
