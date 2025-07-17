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
		return (ft_strdup(addition));
	if (!addition)
		return (original);
	new_str = ft_strjoin(original, addition);
	if (original)
		free(original);
	return (new_str);
}

static void	handle_word_concatenation(char **input, t_token **list, t_shell *shell)
{
	char	*current_word;
	char	*extracted_part;
	char	*temp_input;
	int		len;

	(void)shell;
	current_word = ft_strdup("");
	while (**input && !is_white_space(**input) && !is_operator(**input))
	{
		if (is_quote(**input))
		{
			temp_input = *input;
			len = 0;
			char quote_type = temp_input[len++];
			while(temp_input[len] && temp_input[len] != quote_type)
				len++;
			if (temp_input[len] == quote_type)
				len++;
			extracted_part = ft_strndup(temp_input, len);
			*input += len;
		}
		else
		{
			extracted_part = extract_word(input);
		}
		current_word = str_append_and_free(current_word, extracted_part);
	}
	if (current_word && *current_word)
		add_token(list, current_word, TOKEN_WORD);
	else
		free(current_word);
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
