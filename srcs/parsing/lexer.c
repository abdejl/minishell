/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:06:48 by abjellal          #+#    #+#             */
/*   Updated: 2025/07/09 10:06:48 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*str_append_and_free(char *original, char *addition)
{
	char	*new_str;

	if (!original)
		return (addition);
	if (!addition)
		return (original);
	new_str = ft_strjoin(original, addition);
	free(original);
	free(addition);
	return (new_str);
}

static char	*process_quoted_segment(char **input)
{
	char	*temp_input;
	int		len;
	char	quote_type;
	char	*part;

	temp_input = *input;
	len = 0;
	quote_type = temp_input[len++];
	while (temp_input[len] && temp_input[len] != quote_type)
		len++;
	if (temp_input[len] != quote_type)
	{
		print_error("minishell: ", NULL, "unclosed quote\n");
		return (NULL);
	}
	len++;
	part = ft_strndup(temp_input, len);
	*input += len;
	return (part);
}

static int	handle_word_token(char **input, t_token **list, t_shell *shell)
{
	char	*current_word;
	char	*part;

	(void)shell;
	current_word = ft_strdup("");
	if (!current_word)
		return (0);
	while (**input && !is_white_space(**input) && !is_operator(**input))
	{
		if (is_quote(**input))
		{
			part = process_quoted_segment(input);
			if (!part)
			{
				return (free(current_word), 0);
			}
		}
		else
			part = extract_word(input);
		current_word = str_append_and_free(current_word, part);
		if (!current_word)
			return (0);
	}
	add_token(list, current_word, TOKEN_WORD);
	return (1);
}

t_token	*lexer(char *input, t_shell *shell)
{
	t_token	*list;
	int		success;

	list = NULL;
	success = 1;
	while (input && *input && success)
	{
		if (is_white_space(*input))
			skip_white_spaces(&input);
		else if (is_operator(*input))
			success = handle_operator_token(&input, &list);
		else
			success = handle_word_token(&input, &list, shell);
	}
	if (!success)
	{
		free_tokens(list);
		return (NULL);
	}
	return (list);
}
