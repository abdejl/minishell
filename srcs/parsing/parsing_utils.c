/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:07:17 by abjellal          #+#    #+#             */
/*   Updated: 2025/07/09 10:07:17 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *handle_operator(char **input, t_token *tokens)
{
    char *op = NULL;
    
    if (!input || !*input || !**input)
        return NULL;

    if ((**input == '>' && *(*input + 1) == '>' && *(*input + 2) == '>') ||
        (**input == '<' && *(*input + 1) == '<' && *(*input + 2) == '<')) {
        op = ft_strndup(*input, 3);
        *input += 3;
        add_token(&tokens, op, TOKEN_ERROR);
        free(op);
        return tokens;
    }
    else if ((**input == '>' && *(*input + 1) == '>') ||
             (**input == '<' && *(*input + 1) == '<')) {
        op = ft_strndup(*input, 2);
        *input += 2;
        add_token(&tokens, op, get_operator_type(op));
        free(op);
    }
    else {
        op = ft_strndup(*input, 1);
        (*input)++;
        add_token(&tokens, op, get_operator_type(op));
        free(op);
    }
    return tokens;
}

t_token	*handle_word(char **input, t_token *tokens)
{
	char	*word;

	word = extract_word(input);
	if (!word)
	{
		free_tokens(tokens);
		return (NULL);
	}
	add_token(&tokens, word, TOKEN_WORD);
	free(word);
	return (tokens);
}

int handle_pipe(t_token **tokens, t_cmd **cmd_list, t_cmd **current) 
{
    if (!*cmd_list && !(*current)->args && !(*current)->redirs) {
        print_syntax_error("|");
        return 0;
    }
    cmd_add_back(cmd_list, *current);
    *current = new_command();
    if (!*current) {
        free_cmds(*cmd_list);
        return 0;
    }
    *tokens = (*tokens)->next;
    return 1;
}

int handle_redirect(t_token **tokens, t_cmd **cmd_list, t_cmd *current)
{
    if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD) {
        print_syntax_error((*tokens)->next ? (*tokens)->next->value : NULL);
        return 0;
    }
    if (!add_redirect(current, (*tokens)->type, (*tokens)->next->value)) {
        free_cmds(*cmd_list);
        free_cmd(current);
        return 0;
    }
    *tokens = (*tokens)->next->next;
    return 1;
}

void print_syntax_error(char *token)
{
    ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
    if (token)
        ft_putstr_fd(token, STDERR_FILENO);
    else
        ft_putstr_fd("newline", STDERR_FILENO);
    ft_putstr_fd("'\n", STDERR_FILENO);
}

t_token	*handle_quoted_string(char **input, t_token *tokens)
{
	char	*quoted_content;

	t_shell	shell;
	shell.exit_status = 0;
	shell.env_list = NULL;
	quoted_content = extract_quoted_string(input, &shell);
	if (!quoted_content)
	{
		free_tokens(tokens);
		return (NULL);
	}
	add_token(&tokens, quoted_content, TOKEN_WORD);
	return (tokens);
}
