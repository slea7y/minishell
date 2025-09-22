
#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>

/// testing
# include "../libft/libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include "minishell.h"

// int								main(int argc, char **argv);
// t_env_list				*init_env_list(void);

typedef struct s_token			t_token;
typedef struct s_segment		t_segment;
typedef struct s_segment_list	t_segment_list;
typedef struct s_token_list		t_token_list;

// typedef struct s_env_list
// {
// 	char						*key;
// 	char						*value;
// 	struct s_env_list			*next;
// }								t_env_list;

/// tester end

/// @brief enum struct for token types
typedef enum e_token_type
{
	TOKEN_WORD,          // 0 regular command
	TOKEN_BUILTIN,       // 1 Builtin function
	TOKEN_OUTFILE,       // 2 >
	TOKEN_APPEND,        // 3 >>
	TOKEN_INFILE,        // 4 <
	TOKEN_HEREDOC,       // 5 <<
	TOKEN_PIPE,          // 6 |
	TOKEN_AMPERSAND,     // 7 &
	TOKEN_SINGLE_QUOTES, // 8 '' only empty ones
	TOKEN_DOUBLE_QUOTES, // 9 "" only empty ones
}								t_token_type;

/// @brief enum struct for quote type
typedef enum e_seg_quote_type
{
	SEG_SINGLE_QUOTE, // Segment enbedded single quotes ''. no expansion
	SEG_DOUBLE_QUOTE, // Segment enbedded double quotes "". expansion
	SEG_NORMAL_QUOTE, // Segment enbedded in no  quotes   . expansion
}								t_seg_quote_type;

/// meta struct for tokens
typedef struct s_token_list
{
	t_token *head; // points to first t_token
	t_token *tail; // points to last t_token
	ssize_t size;  // amount of tokens
	char *prompt;  // whole prompt
	int							found_error;
}								t_token_list;

/// @brief
typedef struct s_token
{
	t_token_type token;   // 0-9 Word, Builtin, etc...
	char *value;          // "echo", ""hello world""
	struct s_token *next; // pointer to next token or NULL
	t_segment_list				*segment_list;
	// segs list of tokens NULL if not seg or no segs
}								t_token;

typedef struct s_segment
{
	char						*value;
	t_seg_quote_type			type;
	struct s_segment			*next;
}								t_segment;

typedef struct s_segment_list
{
	t_segment					*head;
	t_segment					*tail;
	ssize_t						size;
}								t_segment_list;


//from maja ==env
void	init_list_env(t_env_list *env_list, char **envp);
void	free_env_list(t_env_list *env_list);
t_cmd_list *start_parser(t_token_list *tokens, t_shell_ctx *ctx);


bool							check_builtin(t_token_list *token_list,
									int start, int i);
bool							check_space(char c);
bool							check_empty_quote(t_token_list *token_list,
									int i);
bool							check_quote(t_token_list *token_list, int i);
bool							check_redirection(t_token_list *token_list,
									int i);
bool							check_token_split(t_token_list *token_list,
									int i);

int								handle_empty_quotes(t_token_list *token_list,
									int i);
int								handle_redirection(t_token_list *token_list,
									int i);
int								handle_pipe(t_token_list *token_list, int i);
int								handle_word_in_quotes(t_token_list *token_list,
									int i);
int								handle_word_master(t_token_list *token_list,
									int i);

t_token_list					*initialise_token_list(char *prompt);

t_token_list					*start_lexer(char *prompt);

void							add_token(t_token_list *token_list, char *value,
									t_token_type token_type);
void							add_segment_to_token(t_token *token,
									char *value, t_seg_quote_type type);
void							found_token(t_token_list *token_list, int start,
									int end, t_token_type token_type);

void							start_segmentation(t_token_list *tokens,
									t_shell_ctx *ctx);
void							dissect_token(t_token *token);
void							expand_variables_in_segment(t_segment *segment,
									t_shell_ctx *ctx);
void							assemble_final_token(t_token *token);
bool							handle_quoted_content(t_token *token,
									int *index, int *start);
int								handle_ampersand(t_token_list *token_list,
									int i);

t_segment_list					*init_segment_list(void);

char							*get_env_value(t_env_list *env_list,
									char *var_name);
char							*get_exit_code(t_shell_ctx *ctx);

// int								ft_strcmp(const char *s1, const char *s2);

#endif