/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 19:32:19 by maja              #+#    #+#             */
/*   Updated: 2025/09/22 00:17:04 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/executor.h"
#include "../../includes/parser.h"

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

static void execute_builtin(t_cmd_node *cmd, t_env_list *env_list)
{
    // Handle redirections for builtins
    int stdin_backup = -1;
    int stdout_backup = -1;

    if (cmd->files)
    {
        t_file_node *file = cmd->files->head;
        while (file)
        {
            int fd;
            if (file->redir_type == INFILE)
            {
                stdin_backup = dup(STDIN_FILENO);
                fd = open(file->filename, O_RDONLY);
                if (fd == -1)
                {
                    perror("open");
                    return;
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (file->redir_type == OUTFILE)
            {
                stdout_backup = dup(STDOUT_FILENO);
                fd = open(file->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1)
                {
                    perror("open");
                    return;
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (file->redir_type == OUTFILE_APPEND)
            {
                stdout_backup = dup(STDOUT_FILENO);
                fd = open(file->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd == -1)
                {
                    perror("open");
                    return;
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            file = file->next;
        }
    }

    // Execute the builtin
    if (ft_strcmp(cmd->cmd[0], "echo") == 0)
        ft_echo(cmd->cmd);
    else if (ft_strcmp(cmd->cmd[0], "cd") == 0)
        ft_cd(cmd->cmd, env_list);
    else if (ft_strcmp(cmd->cmd[0], "pwd") == 0)
        ft_pwd();
    else if (ft_strcmp(cmd->cmd[0], "export") == 0)
        ft_export(cmd->cmd, env_list);
    else if (ft_strcmp(cmd->cmd[0], "unset") == 0)
        ft_unset(cmd->cmd, env_list);
    else if (ft_strcmp(cmd->cmd[0], "env") == 0)
        ft_env(env_list);
    else if (ft_strcmp(cmd->cmd[0], "exit") == 0)
        ft_exit(cmd->cmd);

    // Restore original stdin/stdout if they were changed
    if (stdin_backup != -1)
    {
        dup2(stdin_backup, STDIN_FILENO);
        close(stdin_backup);
    }
    if (stdout_backup != -1)
    {
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdout_backup);
    }
}

static void execute_single_command(t_cmd_node *cmd, t_env_list *env_list)
{
    pid_t pid;
    char *cmd_path;

    if (is_builtin(cmd->cmd[0]))
    {
        execute_builtin(cmd, env_list);
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
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
                file = file->next;
            }
        }

        // Execute command
        cmd_path = find_command_path(cmd->cmd[0], env_list);
        if (!cmd_path)
        {
            fprintf(stderr, "Command not found: %s\n", cmd->cmd[0]);
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
                exit(1);
            ft_strlcpy(tmp, current->key, ft_strlen(current->key) + 1);
            ft_strlcat(tmp, "=", ft_strlen(current->key) + 2);
            ft_strlcat(tmp, current->value, ft_strlen(current->key) + ft_strlen(current->value) + 2);
            envp[i++] = tmp;
            current = current->next;
        }
        envp[i] = NULL;

        execve(cmd_path, cmd->cmd, envp);
        perror("execve");
        exit(127);
    }
    else  // Parent process
    {
        int status;
        waitpid(pid, &status, 0);
    }
}

void start_executor(t_cmd_list *cmds, t_env_list *env_list)
{
    t_cmd_node *current;
    int pipe_fd[2];
    int prev_pipe_read = -1;

    if (!cmds || !cmds->head)
        return;

    current = cmds->head;
    while (current)
    {
        if (current->next)  // If there's a next command, we need a pipe
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                return;
            }
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return;
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

            // Execute the command
            execute_single_command(current, env_list);
            exit(0);
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
    while (wait(NULL) > 0)
        ;
}