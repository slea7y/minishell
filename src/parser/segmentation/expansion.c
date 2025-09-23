/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 21:40:17 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/09/23 16:28:48 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"
#include "../../../includes/executor.h"


char *get_exit_code(t_shell_ctx *ctx)
{
    char *result;
    
    if (!ctx)
        return (ft_strdup("0"));
    result = ft_itoa(ctx->last_exit_code);
    if (!result)
        return (ft_strdup("0"));
    return (result);
}

char *get_env_value(t_env_list *env_list, char *var_name)
{
	t_env_node *current;

	if (!env_list || !env_list->head || !var_name)
		return (ft_strdup(""));
	
	current = env_list->head;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (ft_strdup(""));
}

/// @brief expands $ with given env. loops through string,
/// checks for $. adds text before $. handles edgecase for $?, sets it to
/// @param segment
/// @param env_list
void	expand_variables_in_segment(t_segment *segment, t_shell_ctx *ctx)
{
	char	*final_str;
	int		i;
	int		start;
	char	*var_name;

	if (!segment || !segment->value)
		return ;
	final_str = ft_strdup("");
	i = 0;
	start = 0;
	while (segment->value[i])
	{
		// Handle tilde expansion (~) - only in normal and double quote segments
		if (segment->value[i] == '~' && segment->type != SEG_SINGLE_QUOTE && 
			(i == 0 || segment->value[i - 1] == ' ' || 
			segment->value[i - 1] == '\t' || segment->value[i - 1] == '\n'))
		{
			final_str = ft_strjoin(final_str, ft_substr(segment->value, start, i
						- start));
			char *home = get_env_value(ctx->env, "HOME");
			final_str = ft_strjoin(final_str, home);
			free(home);
			i++;
			start = i;
		}
		// Handle variable expansion ($) - only in normal and double quote segments
		else if (segment->value[i] == '$' && segment->type != SEG_SINGLE_QUOTE && 
			segment->value[i + 1] != '\0')
		{
			final_str = ft_strjoin(final_str, ft_substr(segment->value, start, i
						- start));
			i++;
			start = i;
			if (segment->value[i] == '?')
			{
				char *exit_str = get_exit_code(ctx);
				final_str = ft_strjoin(final_str, exit_str);
				free(exit_str);
				i++;
			}
			else
			{
				// Handle empty variable name case
				if (segment->value[i] == '\0' || segment->value[i] == ' ' || 
					segment->value[i] == '\t' || segment->value[i] == '\n' ||
					segment->value[i] == '"' || segment->value[i] == '\'')
				{
					// Empty variable name, just output the $
					final_str = ft_strjoin(final_str, ft_strdup("$"));
				}
				else
				{
					while (ft_isalnum(segment->value[i])
						|| segment->value[i] == '_')
						i++;
					var_name = ft_substr(segment->value, start, i - start);
					char *env_value = get_env_value(ctx->env, var_name);
					final_str = ft_strjoin(final_str, env_value);
					free(var_name);
					free(env_value);
				}
			}
			start = i;
		}
		else
			i++;
	}
	char *remaining = ft_substr(segment->value, start, i - start);
	final_str = ft_strjoin(final_str, remaining);
	free(remaining);
	free(segment->value);
	segment->value = final_str;
}
