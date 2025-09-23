/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 19:21:01 by maja              #+#    #+#             */
/*   Updated: 2025/09/23 16:03:48 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/parser.h"
#include "../../../includes/heredoc.h"

static t_cmd_node *create_cmd_node(t_token *token)
{
    t_cmd_node *node;

    node = malloc(sizeof(t_cmd_node));
    if (!node)
        return (NULL);
    node->next = NULL;
    node->cmd_type = (token->token == TOKEN_BUILTIN) ? BUILTIN : CMD;
    node->cmd = malloc(sizeof(char *) * 2);
    if (!node->cmd)
    {
        free(node);
        return (NULL);
    }
    node->cmd[0] = ft_strdup(token->value);
    node->cmd[1] = NULL;
    node->files = NULL;
    return (node);
}

static void add_to_cmd_list(t_cmd_list *list, t_cmd_node *node)
{
    if (!list->head)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
}

static int add_argument(t_cmd_node *cmd_node, char *arg)
{
    int     i;
    char    **new_cmd;

    i = 0;
    while (cmd_node->cmd[i])
        i++;
    new_cmd = malloc(sizeof(char *) * (i + 2));
    if (!new_cmd)
        return (0);
    i = 0;
    while (cmd_node->cmd[i])
    {
        new_cmd[i] = cmd_node->cmd[i];
        i++;
    }
    new_cmd[i] = ft_strdup(arg);
    if (!new_cmd[i])
    {
        free(new_cmd);
        return (0);
    }
    new_cmd[i + 1] = NULL;
    free(cmd_node->cmd);
    cmd_node->cmd = new_cmd;
    return (1);
}

static void add_redirection(t_cmd_node *cmd_node, t_token *token, t_token *next)
{
    t_file_node *file;
    char        *filename;

    if (!next || !next->value)
        return;
    if (!cmd_node->files)
    {
        cmd_node->files = malloc(sizeof(t_file_list));
        if (!cmd_node->files)
            return;
        cmd_node->files->head = NULL;
        cmd_node->files->tail = NULL;
        cmd_node->files->size = 0;
    }

    file = malloc(sizeof(t_file_node));
    if (!file)
        return;
    
    // Handle heredoc specially
    if (token->token == TOKEN_HEREDOC)
    {
        filename = handle_heredoc(next->value);
        if (!filename)
        {
            free(file);
            return;
        }
        file->filename = filename;
        file->redir_type = HEREDOC;
    }
    else
    {
        file->filename = ft_strdup(next->value);
        if (token->token == TOKEN_INFILE)
            file->redir_type = INFILE;
        else if (token->token == TOKEN_OUTFILE)
            file->redir_type = OUTFILE;
        else if (token->token == TOKEN_APPEND)
            file->redir_type = OUTFILE_APPEND;
        else
            file->redir_type = HEREDOC;
    }
    file->next = NULL;

    if (!cmd_node->files->head)
    {
        cmd_node->files->head = file;
        cmd_node->files->tail = file;
    }
    else
    {
        cmd_node->files->tail->next = file;
        cmd_node->files->tail = file;
    }
    cmd_node->files->size++;
}

t_cmd_list *start_parser(t_token_list *tokens, t_shell_ctx *ctx)
{
    t_cmd_list  *cmd_list;
    t_cmd_node  *current_cmd;
    t_token     *token;
    (void)ctx;  // Will use later for variable expansion

    if (!tokens || !tokens->head)
        return (NULL);

    cmd_list = malloc(sizeof(t_cmd_list));
    if (!cmd_list)
        return (NULL);
    cmd_list->head = NULL;
    cmd_list->tail = NULL;
    cmd_list->size = 0;
    cmd_list->syntax_error = 0;

    current_cmd = NULL;
    token = tokens->head;
    while (token)
    {
        if (token->token == TOKEN_WORD || token->token == TOKEN_BUILTIN)
        {
            if (!current_cmd)
            {
                current_cmd = create_cmd_node(token);
                if (!current_cmd)
                {
                    cmd_list->syntax_error = 1;
                    break;
                }
                add_to_cmd_list(cmd_list, current_cmd);
            }
            else if (!add_argument(current_cmd, token->value))
            {
                cmd_list->syntax_error = 1;
                break;
            }
        }
        else if (token->token == TOKEN_PIPE)
        {
            if (!current_cmd || !token->next)
            {
                cmd_list->syntax_error = 1;
                break;
            }
            current_cmd = NULL;
        }
        else if (token->token >= TOKEN_OUTFILE && token->token <= TOKEN_HEREDOC)
        {
            if (!current_cmd || !token->next)
            {
                cmd_list->syntax_error = 1;
                break;
            }
            add_redirection(current_cmd, token, token->next);
            token = token->next;  // Skip the filename token
        }
        token = token->next;
    }

    return (cmd_list);
}
// {
//     t_cmd_list *cmd_list;
//     t_cmd_node *current_cmd;
//     t_token *token;
//     (void)env_list;

//     if (!tokens || !tokens->head)
//         return (NULL);
    
// }