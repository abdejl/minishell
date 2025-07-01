// srcs/main.c
#include "minishell.h"

void minishell_loop(t_shell *shell)
{
    char    *input;
    t_token *tokens;
    t_cmd   *cmd_list;

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break; // Ctrl+D
        if (*input)
            add_history(input);
        tokens = lexer(input);
        cmd_list = parser(tokens);
        executor(shell, cmd_list);
        free(input);
        free_tokens(tokens);
        free_cmds(cmd_list);
    }
}

void minishell_exec_line(t_shell *shell, char *line)
{
    t_token *tokens;
    t_cmd   *cmd_list;

    tokens = lexer(line);
    cmd_list = parser(tokens);
    executor(shell, cmd_list);
    free_tokens(tokens);
    free_cmds(cmd_list);
}

void minishell_exec_script(t_shell *shell, char *input)
{
    char **lines = ft_split(input, ';'); // You must have a proper ft_split
    int i = 0;

    while (lines && lines[i])
    {
        minishell_exec_line(shell, lines[i]);
        free(lines[i]);
        i++;
    }
    free(lines);
}

int main(int ac, char **av, char **envp)
{
    t_shell shell;

    shell.env_list = init_env_list(envp);
    shell.exit_status = 0;
    set_signal_parent();

    // Check for -c flag
    if (ac == 3 && strcmp(av[1], "-c") == 0)
    {
        minishell_exec_script(&shell, av[2]);
        ft_free_env_list(shell.env_list);
        return (shell.exit_status); // Return proper exit status
    }

    minishell_loop(&shell);
    ft_free_env_list(shell.env_list);
    return (shell.exit_status);
}
