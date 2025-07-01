#include "minishell.h"

t_cmd *parser(t_token *tokens)
{
    t_cmd *cmd_list = NULL;
    t_cmd *current = new_command();
    int syntax_error = 0;

    if (!current)
        return NULL;
    while (tokens && !syntax_error)
    {
        if (tokens->type == TOKEN_ERROR) {
            print_syntax_error(tokens->value);
            syntax_error = 1;
        }
        else if (is_pipe(tokens))
            syntax_error = !handle_pipe(&tokens, &cmd_list, &current);
        else if (is_redirect(tokens))
            syntax_error = !handle_redirect(&tokens, &cmd_list, current);
        else {
            add_arg(current, tokens->value);
            tokens = tokens->next;
        }
    }
    if (syntax_error) {
        free_cmds(cmd_list);
        free_cmd(current);
        return NULL;
    }
    cmd_add_back(&cmd_list, current);
    return cmd_list;
}

