/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:30:10 by abjellal          #+#    #+#             */
/*   Updated: 2025/04/23 09:57:17 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

t_token *lexer(char *input)
{
    t_token *tokens = NULL;
    
    while (*input)
    {
        if (is_white_space(*input))
            skip_white_spaces(&input);
        else if (is_quote(input))
            add_token(&tokens, extract_quoted_string(&input), TOKEN_WORD);
        else if (is_operator(input))
            add_token(&tokens, extract_operator(&input), get_operator_type(input));
        else
            add_token(&tokens, extract_word(&input), TOKEN_WORD);
        
        if (!*input)
            break;
    }
    return (tokens);
}

void print_tokens(t_token *tokens)
{
    int i = 0;
    while (tokens)
    {
        printf("Token %d: [%s]\n", i++, tokens->value);
        tokens = tokens->next;
    }
}

t_cmd *parser(t_token *tokens)
{
    t_cmd *cmd_list = NULL;
    t_cmd *current = new_command();
    
    while (tokens)
    {
        if (is_pipe(tokens))
        {
            cmd_add_back(&cmd_list, current);
            current = new_command();
            tokens = tokens->next;
        }
        else if (is_redirect(tokens))
        {
            add_redirect(current, tokens->type, tokens->next->value);
            tokens = tokens->next->next;
        }
        else
        {
            add_arg(current, tokens->value);
            tokens = tokens->next;
        }
    }
    cmd_add_back(&cmd_list, current);
    return cmd_list;
}

void print_parsed_cmds(t_cmd *cmds)
{
    int cmd_num = 0;
    while (cmds)
    {
        printf("--- Command %d ---\n", cmd_num++);
        printf("Args: ");
        int i = 0;
        while (cmds->args[i])
        {
            printf("[%s] ", cmds->args[i]);
            i++;
        }
        printf("\n");
        t_redirect *redir = cmds->redirs;
        while (redir)
        {
            const char *type_str = 
                (redir->type == TOKEN_REDIR_IN) ? "<" :
                (redir->type == TOKEN_REDIR_OUT) ? ">" :
                (redir->type == TOKEN_APPEND) ? ">>" : "<<";
            printf("Redirect: %s %s\n", type_str, redir->file);
            redir = redir->next;
            free_redirect(redir);
        }
        cmds = cmds->next;
        free_redirect(redir);
    }
}

int main(int argc, char **argv)
{
    t_token *tokens;
    t_cmd *cmds;

    if (argc < 2)
    {
        printf("Usage: %s \"command\"\n", argv[0]);
        return (1);
    }
    tokens = lexer(argv[1]);
    if (!tokens)
    {
        printf("Lexer error!\n");
        return (1);
    }
    cmds = parser(tokens);
    if (!cmds)
    {
        free_tokens(tokens);
        printf("Parser error!\n");
        return (1);
    }
    print_parsed_cmds(cmds);
    free_tokens(tokens);
    free_cmds(cmds);
    return (0);
}
