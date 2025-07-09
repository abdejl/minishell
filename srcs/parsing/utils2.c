/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:30:31 by abjellal          #+#    #+#             */
/*   Updated: 2025/07/09 10:00:20 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_word(char **str)
{
	char	*start;
	char	*word;
	int		len;

	start = *str;
	len = 0;
	while (start[len] && !is_white_space(start[len]) && !is_operator(start[len])
		&& !is_quote(start[len]))
	{
		if (start[len] == '\\' && start[len + 1])
			len++;
		len++;
	}
	word = ft_strndup(start, len);
	*str += len;
	return (word);
}

char	*extract_operator(char **str)
{
	const char	*ops[] = {">>", "<<", "|", ">", "<", ";", "&", NULL};
	int			i;

	i = 0;
	while (ops[i])
	{
		if (ft_strncmp(*str, ops[i], ft_strlen(ops[i])) == 0)
		{
			*str += ft_strlen(ops[i]);
			return (ft_strdup(ops[i]));
		}
		i++;
	}
	return (NULL);
}

int	handle_operator_token(char **input, t_token **list)
{
	char	*op;

	op = extract_operator(input);
	if (op)
	{
		add_token(list, op, get_operator_type(op));
		return (1);
	}
	op = ft_strndup(*input, 1);
	add_token(list, op, TOKEN_ERROR);
	(*input)++;
	return (1);
}

int	get_operator_type(char *op)
{
	if (ft_strcmp(op, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(op, ";") == 0)
		return (TOKEN_SEMICOLON);
	if (ft_strcmp(op, "&") == 0)
		return (TOKEN_AMPERSAND);
	if (ft_strcmp(op, ">") == 0)
		return (TOKEN_REDIR_OUT);
	if (ft_strcmp(op, ">>") == 0)
		return (TOKEN_APPEND);
	if (ft_strcmp(op, "<") == 0)
		return (TOKEN_REDIR_IN);
	if (ft_strcmp(op, "<<") == 0)
		return (TOKEN_HEREDOC);
	return (TOKEN_ERROR);
}

void	add_token(t_token **token_list, char *value, t_token_type type)
{
	t_token	*new_token;
	t_token	*last;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->value = value;
	new_token->type = type;
	new_token->next = NULL;
	if (!*token_list)
		*token_list = new_token;
	else
	{
		last = *token_list;
		while (last->next)
			last = last->next;
		last->next = new_token;
	}
}
