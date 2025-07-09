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

static void	handle_word_concatenation(char **input, t_token **list, t_shell *shell)
{
	char	*current_word;
	char	*extracted_part;

	current_word = NULL;
	while (**input && !is_white_space(**input) && !is_operator(**input))
	{
		if (is_quote(**input))
		{
			extracted_part = extract_quoted_string(input, shell);
			current_word = str_append_and_free(current_word, extracted_part);
		}
		else
		{
			extracted_part = extract_word(input);
			current_word = str_append_and_free(current_word, extracted_part);
		}
	}
	if (current_word)
		add_token(list, current_word, TOKEN_WORD);
}

t_token	*lexer(char *input, t_shell *shell)
{
	t_token	*list;

	list = NULL;
	while (input && *input)
	{
		if (is_white_space(*input))
			skip_white_spaces(&input);
		else if (is_operator(*input))
			handle_operator_token(&input, &list);
		else
		{
			handle_word_concatenation(&input, &list, shell);
		}
	}
	return (list);
}