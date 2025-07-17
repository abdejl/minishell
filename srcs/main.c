#include "minishell.h"

// void print_cmd_list(t_cmd *cmd_list) {
//     t_cmd *cmd = cmd_list;
//     int i;

//     while (cmd) {
//         printf("=== Command ===\n");

//         // Arguments
//         printf("Args: ");
//         if (cmd->args) {
//             for (i = 0; cmd->args[i]; i++) {
//                 printf("[%s] ", cmd->args[i]);
//             }
//         } else {
//             printf("(no args)");
//         }
//         printf("\n");

//         // Pipe
//         printf("Pipe out: %s\n", cmd->pipe_out ? "Yes" : "No");

//         // Background
//         printf("Background: %s\n", cmd->background ? "Yes" : "No");

//         // Redirections
//         printf("Redirections:\n");
//         t_redirect *redir = cmd->redirs;
//         while (redir) {
//             // Assuming `t_redirect` has fields like type and file
//             printf("  Type: %d, File: %s\n", redir->type, redir->file); // Modify based on your struct
//             redir = redir->next;
//         }

//         cmd = cmd->next;
//     }
// }

static void	minishell_exec_line(char *line, t_shell *shell)
{
	t_token	*tokens;
	t_cmd	*cmd_list;

	if (line && *line)
		add_history(line);
	tokens = lexer(line, shell);
	cmd_list = parser(tokens, shell);
	if (cmd_list)
	{
		 //print_cmd_list(cmd_list);
		expander(cmd_list, shell);
		executor(shell, cmd_list);
		free_cmds(cmd_list);
	}
	free_tokens(tokens);
}

void	minishell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		set_signal_parent();
		input = readline("minishell> ");
		if (!input)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		minishell_exec_line(input, shell);
		free(input);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	shell.env_list = init_env_list(envp);
	shell.exit_status = 0;
	minishell_loop(&shell);
	rl_clear_history();
	ft_free_env_list(shell.env_list);
	return (shell.exit_status);
}