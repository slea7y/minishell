/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 17:15:36 by maja              #+#    #+#             */
/*   Updated: 2025/09/21 22:43:51 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "includes/parser.h"
#include "includes/executor.h"
// #include "includes/.h"
// #include "includes/executor.h"
// #include "minishell.h"

// #include "minishell.h"
// #include "executor.h"
void    free_file_list(t_file_list *file_list)
{
    t_file_node *current;
    t_file_node *next;
    
    if(!file_list)
        return ;
    current = file_list->head;
    while (current)
    {
        next = current->next;
        if (current->filename)
            free(current->filename);
        free(current);
        current = next;
    }
    free(file_list);
}

void	free_cmd_list(t_cmd_list *cmds_list)
{
    t_cmd_node *current;
    t_cmd_node *next;
    int			i;
    if (!cmds_list)
        return ;
    current = cmds_list->head;
    while (current)
    {
        next = current->next;
        if (current->cmd)
        {
			i = 0;
			while (current->cmd[i])
			{
				free(current->cmd[i]);
				i++;
			}
			free(current->cmd);
        }
		if (current->files)
			free_file_list(current->files);
        free(current);
        current = next;
    }
    free(cmds_list);
}

void	free_token_list(t_token_list *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return ;
	current = tokens->head;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	free(tokens->prompt);
	free(tokens);
}

int main(int argc, char **argv, char **envp)
{
    char        *input;
    t_env_list  env_list;
    t_token_list  *tokens;
    t_cmd_list  *cmds;

    (void)argc;
    (void)argv;

    setup_signals();
    init_list_env(&env_list, envp);
    if (env_list.head == NULL)
    {
        perror("env_list init failed");
        return (1);
    }
    while (1)
    {
        input = readline("minishell$ ");
        if (!input)  // Handle Ctrl+D (EOF)
        {
            printf("\nexit\n");
            break;
        }
        if (*input == '\0')  // Handle empty input
        {
            free(input);
            continue;
        }
        add_history(input);
        // --- LEXER ---
        tokens = start_lexer(input);
        if (!tokens || tokens->found_error)
        {
            fprintf(stderr, "Lexing error\n");
            free_token_list(tokens);
            free(input);
            continue ;
        }

        // --- SEGMENTATION ---
        start_segmentation(tokens, &env_list);

        // --- PARSER ---
        cmds = start_parser(tokens, &env_list);
        if (!cmds || cmds->syntax_error)
        {
            fprintf(stderr, "Parsing error\n");
            free_cmd_list(cmds);
            free_token_list(tokens);
            free(input);
            continue ;
        }

        // --- EXECUTOR ---
        start_executor(cmds, &env_list);

        // --- CLEANUP ---
        free_cmd_list(cmds);
        free_token_list(tokens);
        free(input);
    }

    // TODO: free env_list when done
    free_env_list(&env_list);
    return (0);
}
