/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:00:00 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/01/27 10:00:00 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"
#include "../../../includes/heredoc.h"

static char	*get_exit_code_value(t_shell_ctx *ctx)
{
	int		exit_code;
	char	*exit_str;

	exit_code = 0;
	if (ctx)
		exit_code = ctx->last_exit_code;
	exit_str = ft_itoa(exit_code);
	return (exit_str);
}

static char	*get_variable_value(char *var_name, t_shell_ctx *ctx)
{
	t_env_list	*env_list;
	char		*value;

	env_list = NULL;
	if (ctx)
		env_list = ctx->env;
	value = get_env_value(env_list, var_name);
	return (value);
}

static int	extract_variable_name(char *line, int *i, char **var_name)
{
	int	start;

	start = *i;
	while (ft_isalnum(line[*i]) || line[*i] == '_')
		(*i)++;
	*var_name = ft_substr(line, start, *i - start);
	return (*var_name != NULL);
}

static char	*expand_dollar_sequence(char *line, int *i, t_shell_ctx *ctx)
{
	char	*result;
	char	*var_name;

	(*i)++;
	if (line[*i] == '?')
	{
		(*i)++;
		return (get_exit_code_value(ctx));
	}
	if (!extract_variable_name(line, i, &var_name))
		return (ft_strdup(""));
	result = get_variable_value(var_name, ctx);
	free(var_name);
	return (result);
}

static char	*append_segment(char *result, char *line, int start, int end)
{
	char	*segment;
	char	*temp;

	segment = ft_substr(line, start, end - start);
	if (!segment)
		return (result);
	temp = ft_strjoin(result, segment);
	free(result);
	free(segment);
	return (temp);
}

char	*expand_line_variables(char *line, t_shell_ctx *ctx)
{
	char	*result;
	int		i;
	int		start;
	char	*expanded;

	if (!line)
		return (ft_strdup(""));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	start = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] != '\0')
		{
			result = append_segment(result, line, start, i);
			expanded = expand_dollar_sequence(line, &i, ctx);
			if (expanded)
			{
				char *temp = ft_strjoin(result, expanded);
				free(result);
				free(expanded);
				result = temp;
			}
			start = i;
		}
		else
			i++;
	}
	if (i > start)
		result = append_segment(result, line, start, i);
	return (result);
}
