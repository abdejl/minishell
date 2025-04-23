/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:30:31 by abjellal          #+#    #+#             */
/*   Updated: 2025/04/22 14:13:15 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

char *extract_word(char **str)
{
    char *start = *str;
    char *end = start;
    
    while (*end && !is_white_space(*end) && !is_operator(end) && !is_quote(end))
        end++;
    
    char *word = ft_strndup(start, end - start);
    *str = end;
    return (word);
}

char *extract_operator(char **str)
{
    char *op = NULL;
    
    if (**str == '>' && *(*str + 1) == '>')
    {
        op = ft_strdup(">>");
        *str += 2;
    }
    else if (**str == '<' && *(*str + 1) == '<')
    {
        op = ft_strdup("<<");
        *str += 2;
    }
    else
    {
        op = ft_strndup(*str, 1);
        (*str)++;
    }
    return (op);
}

int get_operator_type(char *op)
{
    if (*op == '|')
        return (TOKEN_PIPE);
    if (*op == '>')
    {
        if (op[1] == '>')
            return (TOKEN_APPEND);
        else
            return (TOKEN_REDIR_OUT);
    }
    if (*op == '<')
    {
        if (op[1] == '<')
            return (TOKEN_HEREDOC);
        else
            return (TOKEN_REDIR_IN);
    }
    return (TOKEN_WORD);
}

void add_token(t_token **token_list, char *value, int type)
{
    t_token *new_token;
    t_token *last;

    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return;
    
    new_token->value = ft_strdup(value);
    new_token->type = type;
    new_token->next = NULL;
    if (*token_list == NULL)
    {
        *token_list = new_token;
    }
    else
    {
        last = *token_list;
        while (last->next != NULL)
            last = last->next;
        last->next = new_token;
    }
}
