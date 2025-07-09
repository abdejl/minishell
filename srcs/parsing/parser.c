/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 10:07:05 by abjellal          #+#    #+#             */
/*   Updated: 2025/07/09 10:07:05 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_separator(t_token *token)
{
    if (!token)
        return (0);
    return (token->type == TOKEN_PIPE || token->type == TOKEN_SEMICOLON
        || token->type == TOKEN_AMPERSAND);
}

static int	handle_word_or_redir(t_token **tok, t_cmd *cmd)
{
    t_token_type	type;

    type = (*tok)->type;
    if (type == TOKEN_WORD)
        add_arg(cmd, (*tok)->value);
    else
    {
        if (!(*tok)->next || (*tok)->next->type != TOKEN_WORD)
        {
            print_syntax_error((*tok)->next ? (*tok)->next->value : "newline");
            return (0);
        }
        add_redirect(cmd, type, (*tok)->next->value);
        *tok = (*tok)->next;
    }
    return (1);
}

static t_cmd	*finalize_parsing(t_cmd *list, t_cmd *current, int expect_cmd)
{
    if (expect_cmd && list)
    {
        print_syntax_error("newline");
        free_cmds(list);
        free_cmd(current);
        return (NULL);
    }
    cmd_add_back(&list, current);
    return (list);
}

t_cmd	*parser(t_token *tokens, t_shell *shell)
{
    t_cmd	*cmd_list;
    t_cmd	*current_cmd;
    int		expect_cmd;

    (void)shell;
    cmd_list = NULL;
    if (!tokens)
        return (NULL);
    current_cmd = new_command();
    expect_cmd = 1;
    while (tokens)
    {
        if (tokens->type == TOKEN_ERROR)
            free_cmds(cmd_list);
            return (print_syntax_error(tokens->value), free_cmd(current_cmd), NULL);
        if (is_separator(tokens))
        {
            if (expect_cmd)
                free_cmds(cmd_list);
                return (print_syntax_error(tokens->value), free_cmd(current_cmd), NULL);
            // if (tokens->type == TOKEN_PIPE)
            //     current_cmd->pipe_out = 1;
            current_cmd->background = (tokens->type == TOKEN_AMPERSAND);
            cmd_add_back(&cmd_list, current_cmd);
            current_cmd = new_command();
            expect_cmd = 1;
        }
        else if (!handle_word_or_redir(&tokens, current_cmd))
            return (free_cmds(cmd_list), free_cmd(current_cmd), NULL);
        else
            expect_cmd = 0;
        tokens = tokens->next;
    }
    return (finalize_parsing(cmd_list, current_cmd, expect_cmd));
}
