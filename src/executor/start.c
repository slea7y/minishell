/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 19:32:19 by maja              #+#    #+#             */
/*   Updated: 2025/09/26 02:20:05 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/executor.h"
#include "../../includes/parser.h"
#include <stdbool.h>

static char *find_command_path(char *cmd, t_env_list *env_list)
{
    char *path;
    char *dir;
    char *full_path;
    char **path_dirs;
    int i;

    if (cmd[0] == '/' || cmd[0] == '.')
        return (ft_strdup(cmd));

    // Find PATH in environment
    path = NULL;
    t_env_node *current = env_list->head;
    while (current)
    {
        if (ft_strcmp(current->key, "PATH") == 0)
        {
            path = current->value;
            break;
        }
        current = current->next;
    }
    if (!path)
        return (ft_strdup(cmd));

    // Split PATH and search each directory
    path_dirs = ft_split(path, ':');
    if (!path_dirs)
        return (NULL);

    i = 0;
    while (path_dirs[i])
    {
        dir = path_dirs[i];
        full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
        if (!full_path)
            continue;
        ft_strlcpy(full_path, dir, ft_strlen(dir) + 1);
        ft_strlcat(full_path, "/", ft_strlen(dir) + 2);
        ft_strlcat(full_path, cmd, ft_strlen(dir) + ft_strlen(cmd) + 2);

        if (access(full_path, X_OK) == 0)
        {
            // Free path_dirs before returning
            int j = 0;
            while (path_dirs[j])
                free(path_dirs[j++]);
            free(path_dirs);
            return (full_path);
        }
        free(full_path);
        i++;
    }

    // Free path_dirs if command not found
    i = 0;
    while (path_dirs[i])
        free(path_dirs[i++]);
    free(path_dirs);
    return (ft_strdup(cmd));
}

static int is_builtin(char *cmd)
{
    return (ft_strcmp(cmd, "echo") == 0 ||
            ft_strcmp(cmd, "cd") == 0 ||
            ft_strcmp(cmd, "pwd") == 0 ||
            ft_strcmp(cmd, "export") == 0 ||
            ft_strcmp(cmd, "unset") == 0 ||
            ft_strcmp(cmd, "env") == 0 ||
            ft_strcmp(cmd, "exit") == 0);
}

static int execute_builtin(t_cmd_node *cmd, t_env_list *env_list)
{
    // Handle redirections for builtins
    int stdin_backup = -1;
    int stdout_backup = -1;
    int exit_code = 0;

    if (cmd->files)
    {
        t_file_node *file = cmd->files->head;
        while (file)
        {
            int fd;
            if (file->redir_type == INFILE)
            {
                fd = open(file->filename, O_RDONLY);
                if (fd == -1)
                {
                    perror("open");
                    return 1;
                }
                if (stdin_backup == -1)
                    stdin_backup = dup(STDIN_FILENO);
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (file->redir_type == OUTFILE)
            {
                fd = open(file->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1)
                {
                    perror("open");
                    return 1;
                }
                if (stdout_backup == -1)
                    stdout_backup = dup(STDOUT_FILENO);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (file->redir_type == OUTFILE_APPEND)
            {
                fd = open(file->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd == -1)
                {
                    perror("open");
                    return 1;
                }
                if (stdout_backup == -1)
                    stdout_backup = dup(STDOUT_FILENO);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (file->redir_type == HEREDOC)
            {
                fd = open(file->filename, O_RDONLY);
                if (fd == -1)
                {
                    perror("open heredoc");
                    return 1;
                }
                if (stdin_backup == -1)
                    stdin_backup = dup(STDIN_FILENO);
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            file = file->next;
        }
    }

    // Execute the builtin and capture exit code
    if (ft_strcmp(cmd->cmd[0], "echo") == 0)
        exit_code = ft_echo(cmd);
    else if (ft_strcmp(cmd->cmd[0], "cd") == 0)
        exit_code = ft_cd(cmd, env_list);
    else if (ft_strcmp(cmd->cmd[0], "pwd") == 0)
        exit_code = ft_pwd();
    else if (ft_strcmp(cmd->cmd[0], "export") == 0)
        exit_code = ft_export(cmd, env_list);
    else if (ft_strcmp(cmd->cmd[0], "unset") == 0)
        exit_code = ft_unset(cmd, env_list);
    else if (ft_strcmp(cmd->cmd[0], "env") == 0)
        exit_code = ft_env(cmd, env_list);
    else if (ft_strcmp(cmd->cmd[0], "exit") == 0)
        ft_exit(cmd); // exit handles its own exit

    // Restore original stdin/stdout if they were changed
    if (stdin_backup != -1 && stdin_backup != STDIN_FILENO)
    {
        dup2(stdin_backup, STDIN_FILENO);
        close(stdin_backup);
        stdin_backup = -1;
    }
    if (stdout_backup != -1 && stdout_backup != STDOUT_FILENO)
    {
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdout_backup);
        stdout_backup = -1;
    }

    return exit_code;
}

static int execute_single_command(t_cmd_node *cmd, t_env_list *env_list)
{
    pid_t pid;
    char *cmd_path;
    int exit_code = 0;

    if (cmd->cmd_type == BUILTIN)
        return execute_builtin(cmd, env_list);

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }
    
    if (pid == 0)  // Child process
    {
        // Handle redirections if any
        if (cmd->files)
        {
            t_file_node *file = cmd->files->head;
            while (file)
            {
                int fd;
                if (file->redir_type == INFILE)
                {
                    fd = open(file->filename, O_RDONLY);
                    if (fd == -1)
                    {
                        perror("open");
                        exit(1);
                    }
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }
                else if (file->redir_type == OUTFILE)
                {
                    fd = open(file->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd == -1)
                    {
                        perror("open");
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
                else if (file->redir_type == OUTFILE_APPEND)
                {
                    fd = open(file->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (fd == -1)
                    {
                        perror("open");
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
                else if (file->redir_type == HEREDOC)
                {
                    fd = open(file->filename, O_RDONLY);
                    if (fd == -1)
                    {
                        perror("open heredoc");
                        exit(1);
                    }
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }
                file = file->next;
            }
        }

        // Execute command
        cmd_path = find_command_path(cmd->cmd[0], env_list);
        if (!cmd_path)
        {
            // fprintf(stderr, "Command not found: %s\n", cmd->cmd[0]);  // Commented out for tester
            exit(127);
        }

        // Convert env_list to char** for execve
        char **envp = malloc(sizeof(char *) * (env_list->size + 1));
        if (!envp)
            exit(1);
        
        int i = 0;
        t_env_node *current = env_list->head;
        while (current)
        {
            char *tmp = malloc(ft_strlen(current->key) + ft_strlen(current->value) + 2);
            if (!tmp)
            {
                // Clean up already allocated envp entries
                int j = 0;
                while (j < i)
                    free(envp[j++]);
                free(envp);
                exit(1);
            }
            ft_strlcpy(tmp, current->key, ft_strlen(current->key) + 1);
            ft_strlcat(tmp, "=", ft_strlen(current->key) + 2);
            ft_strlcat(tmp, current->value, ft_strlen(current->key) + ft_strlen(current->value) + 2);
            envp[i++] = tmp;
            current = current->next;
        }
        envp[i] = NULL;

        execve(cmd_path, cmd->cmd, envp);
        // Command not found or permission denied
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->cmd[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        // Free environment array before exit
        int j = 0;
        while (envp[j])
            free(envp[j++]);
        free(envp);
        free(cmd_path);
        exit(127);
    }
    else  // Parent process
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            exit_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            exit_code = 128 + WTERMSIG(status);
        else
            exit_code = 1;
    }
    return exit_code;
}

int start_executor(t_cmd_list *cmds, t_shell_ctx *ctx)
{
    t_cmd_node *current;
    int pipe_fd[2];
    int prev_pipe_read = -1;
    int exit_code = 0;
    bool has_pipes = false;

    if (!cmds || !cmds->head)
        return 0;

    // Check if we have any pipes in the command list
    current = cmds->head;
    while (current)
    {
        if (current->next)
        {
            has_pipes = true;
            break;
        }
        current = current->next;
    }

    // If it's a single builtin command without pipes, execute directly
    current = cmds->head;
    if (!has_pipes && current && is_builtin(current->cmd[0]))
    {
        exit_code = execute_builtin(current, ctx->env);
        return exit_code;
    }

    // Handle piped commands
    current = cmds->head;
    while (current)
    {
        if (current->next)  // If there's a next command, we need a pipe
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                return 1;
            }
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return 1;
        }

        if (pid == 0)  // Child process
        {
            // Set up pipe input from previous command if it exists
            if (prev_pipe_read != -1)
            {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }

            // Set up pipe output for next command if it exists
            if (current->next)
            {
                close(pipe_fd[0]);  // Close read end
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            // In pipes, execute builtins as external commands to avoid double execution
            if (is_builtin(current->cmd[0]))
            {
                // Handle redirections for builtin in child
                if (current->files)
                {
                    t_file_node *file = current->files->head;
                    while (file)
                    {
                        int fd;
                        if (file->redir_type == INFILE)
                        {
                            fd = open(file->filename, O_RDONLY);
                            if (fd == -1)
                            {
                                perror("open");
                                exit(1);
                            }
                            dup2(fd, STDIN_FILENO);
                            close(fd);
                        }
                        else if (file->redir_type == OUTFILE)
                        {
                            fd = open(file->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                            if (fd == -1)
                            {
                                perror("open");
                                exit(1);
                            }
                            dup2(fd, STDOUT_FILENO);
                            close(fd);
                        }
                        else if (file->redir_type == OUTFILE_APPEND)
                        {
                            fd = open(file->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
                            if (fd == -1)
                            {
                                perror("open");
                                exit(1);
                            }
                            dup2(fd, STDOUT_FILENO);
                            close(fd);
                        }
                        else if (file->redir_type == HEREDOC)
                        {
                            fd = open(file->filename, O_RDONLY);
                            if (fd == -1)
                            {
                                perror("open heredoc");
                                exit(1);
                            }
                            dup2(fd, STDIN_FILENO);
                            close(fd);
                        }
                        file = file->next;
                    }
                }
                
                // Execute builtin directly in child
                if (ft_strcmp(current->cmd[0], "echo") == 0)
                    exit_code = ft_echo(current);
                else if (ft_strcmp(current->cmd[0], "cd") == 0)
                    exit_code = ft_cd(current, ctx->env);
                else if (ft_strcmp(current->cmd[0], "pwd") == 0)
                    exit_code = ft_pwd();
                else if (ft_strcmp(current->cmd[0], "export") == 0)
                    exit_code = ft_export(current, ctx->env);
                else if (ft_strcmp(current->cmd[0], "unset") == 0)
                    exit_code = ft_unset(current, ctx->env);
                else if (ft_strcmp(current->cmd[0], "env") == 0)
                    exit_code = ft_env(current , ctx->env);
                else if (ft_strcmp(current->cmd[0], "exit") == 0)
                    ft_exit(current);
                exit(exit_code);
            }
            else
            {
                // Execute external command
                exit_code = execute_single_command(current, ctx->env);
                exit(exit_code);
            }
        }

        // Parent process
        if (prev_pipe_read != -1)
            close(prev_pipe_read);

        if (current->next)
        {
            close(pipe_fd[1]);  // Close write end
            prev_pipe_read = pipe_fd[0];  // Save read end for next iteration
        }

        current = current->next;
    }

    // Wait for all child processes
    int status;
    int last_exit_code = 0;
    while (wait(&status) > 0)
    {
        if (WIFEXITED(status))
            last_exit_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            last_exit_code = 128 + WTERMSIG(status);
        else
            last_exit_code = 1;
    }
    return last_exit_code;
}