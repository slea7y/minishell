/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 17:15:36 by maja              #+#    #+#             */
/*   Updated: 2025/09/26 21:58:12 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "includes/parser.h"
#include "includes/executor.h"
#include "includes/heredoc.h"
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
        {
            // Clean up heredoc temp files
            if (current->redir_type == HEREDOC)
                cleanup_heredoc(current->filename);
            else
                free(current->filename);
        }
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

void	process_single_command(char *command, t_shell_ctx *ctx)
{
	t_token_list	*tokens;
	t_cmd_list		*cmds;
	int				last_status;

	// --- LEXER ---
	tokens = start_lexer(command);
	if (!tokens || tokens->found_error)
	{
		ctx->last_exit_code = 2;
		free_token_list(tokens);
		return;
	}

	// --- SEGMENTATION ---
	start_segmentation(tokens, ctx);

	// --- PARSER ---
	cmds = start_parser(tokens, ctx);
	if (!cmds || cmds->syntax_error)
	{
		ctx->last_exit_code = 2;  
		free_cmd_list(cmds);
		free_token_list(tokens);
		return;
	}

	// --- EXECUTOR ---
	last_status = start_executor(cmds, ctx);
	ctx->last_exit_code = last_status;

	// --- CLEANUP ---
	if (cmds)
		free_cmd_list(cmds);
	if (tokens)
		free_token_list(tokens);
}

int main(int argc, char **argv, char **envp)
{
    char        *input;
    t_shell_ctx ctx;
    t_env_list  env_list;
    t_token_list  *tokens;
    t_cmd_list  *cmds;
    int         last_status;

    (void)argc;
    (void)argv;

    setup_signals();
    init_list_env(&env_list, envp);
    if (env_list.head == NULL)
    {
        perror("env_list init failed");
        return (1);
    }
    
    // Initialize shell context
    ctx.env = &env_list;
    ctx.last_exit_code = 0;
    while (1)
    {
        // Check if input is from terminal or from tester
        if (isatty(fileno(stdin)))
            input = readline("minishell$ ");
        else
        {
            // For tester - read without prompt
            char buffer[1000];
            int bytes_read = read(fileno(stdin), buffer, 999);
            if (bytes_read <= 0)
                break;
            buffer[bytes_read] = '\0';
            // Remove newline if present
            if (bytes_read > 0 && buffer[bytes_read - 1] == '\n')
                buffer[bytes_read - 1] = '\0';
            input = malloc(bytes_read + 1);
            if (input)
                ft_strlcpy(input, buffer, bytes_read + 1);
            
            if (input && ft_strchr(input, '\n'))
            {
                char *first_line = input;
                char *first_newline = ft_strchr(input, '\n');
                if (first_newline)
                {
                    *first_newline = '\0';  // Temporarily null-terminate first line
                    if (ft_strnstr(first_line, "<<", ft_strlen(first_line)))
                    {
                        *first_newline = '\n';  // Restore newline
                    }
                    else
                    {
                        *first_newline = '\n';
                        char **lines = ft_split(input, '\n');
                        if (lines)
                        {
                            int i = 0;
                            while (lines[i])
                            {
                                if (lines[i][0] != '\0')  // Skip empty lines
                                {
                                    process_single_command(lines[i], &ctx);
                                    if (ctx.last_exit_code == 2)
                                        break;
                                }
                                i++;
                            }
                            i = 0;
                            while (lines[i])
                                free(lines[i++]);
                            free(lines);
                        }
                        free(input);
                        continue; 
                    }
                }
            }
        }
        
        if (!input)  // Handle Ctrl+D (EOF)
        {
            // printf("\nexit\n");  // Commented out for tester
            break;
        }
        if (*input == '\0')  // Handle empty input
        {
            free(input);
            continue;
        }
        // Only add to history if input is from terminal
        if (isatty(fileno(stdin)))
            add_history(input);
        char *processed_input = preprocess_heredoc_input(input, &ctx);
        if (processed_input)
        {
            free(input);
            input = processed_input;
        }

        // --- LEXER ---
        tokens = start_lexer(input);
        if (!tokens || tokens->found_error)
        {
            ctx.last_exit_code = 2;  // Syntax error exit code
            free_token_list(tokens);
            free(input);
            continue ;
        }

        // --- SEGMENTATION ---
        start_segmentation(tokens, &ctx);

        // --- PARSER ---
        cmds = start_parser(tokens, &ctx);
        if (!cmds || cmds->syntax_error)
        {
            // fprintf(stderr, "Parsing error\n");  // Commented out for tester
            ctx.last_exit_code = 2;  // Syntax error exit code
            free_cmd_list(cmds);
            free_token_list(tokens);
            free(input);
            continue ;
        }

        // --- EXECUTOR ---
        last_status = start_executor(cmds, &ctx);
        ctx.last_exit_code = last_status;

        // --- CLEANUP ---
        // Temporarily comment out cleanup to test
        if (cmds)
            free_cmd_list(cmds);
        if (tokens)
            free_token_list(tokens);
        if (input)
            free(input);
    }

    // Free env_list when done
    // free_env_list(&env_list);  // Temporarily commented out to debug segfault
    return (ctx.last_exit_code);
}
