#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "../libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
# define MAX_INPUT 1024
# define MAX_ARGS 64

int start_executor(t_cmd_list *cmds, t_shell_ctx *ctx);


// typedef struct data_s
// {
//     char		**env;
// }	t_data;

// String and utility functions
int	detect_pipes(char **args, char **env_list);
int	handle_pipes(int pipes_count, char **args, char **env_list);
int	handle_one_pipe(char **args, char **env_list);
int	handle_multiple_pipes(char **args, char **env_list);

// Environment functions
int	check_for_redirections(char **args, char **envp);
void	execute_cmd(char **args, char **env, int place);
int	execute_external(char **args, char **env);
int	check_if_var_exist(char *key, t_env_list *env);
void	change_var_value(char *key, char *new_value, t_env_list *env);
int	is_numeric(char	*str);
void	sort_env(t_env_list *env_list);
void	add_change_export(char *args, t_env_list *env);
int	is_valid_identifier(char *args);
void	remove_env_var(char *key, t_env_list *env);
int	ft_strcmp(char *s1, char *s2);

// Builtin functions
void	sort_env(t_env_list *env_list);
int     ft_echo(t_cmd_node *cmd);
int 	ft_pwd(void);
// int		ft_env(t_env_list *env_list);
int     ft_env(t_cmd_node *cmd, t_env_list *envp);
int		ft_cd(t_cmd_node *cmd, t_env_list *env_list);
int		ft_exit(t_cmd_node *cmd);
int		ft_export(t_cmd_node *cmd, t_env_list *env_list);
int		ft_unset(t_cmd_node *cmd, t_env_list *env_list);

// Env and execution helpers
char	**dup_env(char **env);
// void	init_list_env(t_env_list *env_list, char **envp);

#endif