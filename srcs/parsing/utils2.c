/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:30:31 by abjellal          #+#    #+#             */
/*   Updated: 2025/04/25 10:12:09 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *extract_word(char **str)
{
    char *start;
    char *end;
    char *word;

    if (!str || !*str)
        return (NULL);
    start = *str;
    end = start;
    while (*end && !is_white_space(*end) && !is_operator(end) && !is_quote(end))
        end++;
    word = ft_strndup(start, end - start);
    if (!word)
        return (NULL);
    *str = end;
    return (word);
}

char *extract_operator(char **str)
{
    char *op = NULL;
    
    if (!str || !*str || !**str)
        return (NULL);
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
    if (!op)
        return (NULL);
    return (op);
}

int get_operator_type(char *op)
{
    if (!op || !*op)
        return TOKEN_ERROR;
    if (ft_strcmp(op, "|") == 0)
        return TOKEN_PIPE;
    if (ft_strcmp(op, ">") == 0)
        return TOKEN_REDIR_OUT;
    if (ft_strcmp(op, ">>") == 0)
        return TOKEN_APPEND;
    if (ft_strcmp(op, "<") == 0)
        return TOKEN_REDIR_IN;
    if (ft_strcmp(op, "<<") == 0)
        return TOKEN_HEREDOC;
    return TOKEN_ERROR;
}

void add_token(t_token **token_list, char *value, int type)
{
    t_token *new_token;
    t_token *last;

    if (!value)
        return;
    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return;
    new_token->value = ft_strdup(value);
    if (!new_token->value)
    {
        free(new_token);
        return;
    }
    new_token->type = type;
    new_token->next = NULL;
    if (*token_list == NULL)
        *token_list = new_token;
    else
    {
        last = *token_list;
        while (last->next != NULL)
            last = last->next;
        last->next = new_token;
    }
}
