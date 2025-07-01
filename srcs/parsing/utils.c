/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:05:27 by abjellal          #+#    #+#             */
/*   Updated: 2025/04/25 10:11:07 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_white_space(char c)
{
    return ((c >= 9 && c <= 13) || c == 32);
}

void skip_white_spaces(char **str)
{
    while (**str && is_white_space(**str))
        (*str)++;
}

int is_quote(char *str)
{
    if (*str == 34 || *str == 39)
        return (1);
    return (0);
}

char *extract_quoted_string(char **input)
{
    char    *result;
    char    *start;
    int     length;
    char    quote_type;

    if (!input || !*input || !is_quote(*input))
        return (NULL); 
    quote_type = **input;
    (*input)++;
    start = *input;
    while (**input && **input != quote_type)
    {
        if (**input == '\\' && *(*input + 1))
            (*input)++;
        (*input)++;
    }
    if (**input != quote_type)
    {
        write(2, "Error: Unclosed quote\n",23);
        return (NULL);
    }
    length = *input - start;
    result = malloc(length + 1);
    if (!result)
        return (NULL);
    ft_memcpy(result, start, length);
    result[length] = '\0';
    (*input)++;
    return (result);
}

int is_operator(char *str)
{
    if (!str)
        return (0);
    if (*str == '<' || *str == '>' || *str == '|')
        return (1);
    return (0);
}
