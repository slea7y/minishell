#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "../libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
# define MAX_INPUT 1024
# define MAX_ARGS 64

void start_executor(t_cmd_list *cmds, t_env_list *env_list);


// typedef struct data_s
// {
//     char		**env;
// }	t_data;

// String and utility functions
int	detect_pipes(char **args, char **env_list);

// Environment functions
int	check_for_redirections(char **args, char **envp);
void	execute_cmd(char **args, char **env, int place);
int	check_if_var_exist(char *key, t_env_list *env);
void	change_var_value(char *key, char *new_value, t_env_list *env);
int	is_numeric(char	*str);
void	sort_env(t_env_list *env_list);
void	add_change_export(char *args, t_env_list *env);
int	is_valid_identifier(char *args);
int	ft_strcmp(char *s1, char *s2);

// Builtin functions
void	sort_env(t_env_list *env_list);
void	ft_echo(char **args);
void	ft_pwd(void);
void	ft_env(t_env_list *env_list);
void	ft_cd(char **args, t_env_list *env_list);
void	ft_exit(char **args);
void	ft_export(char **args, t_env_list *env_list);
void	ft_unset(char **args, t_env_list *env_list);

// Env and execution helpers
char	**dup_env(char **env);
// void	init_list_env(t_env_list *env_list, char **envp);
void	execute_external(char **args, char **envp);

#endif