#include "minishell.h"

static int	handle_quote_token(char **input, t_token **list, t_shell *shell)
{
    char	*value;
    char	*full_value;
    char	*temp;
    char	quote_str[2];

    quote_str[0] = **input; // Save the quote type (' or ")
    quote_str[1] = '\0';
    value = extract_quoted_string(input, shell);
    if (!value)
        return (0);
    temp = ft_strjoin(quote_str, value);
    if (!temp)
    {
        free(value);
        return (0);
    }
    full_value = ft_strjoin(temp, quote_str);
    free(temp);
    if (!full_value)
    {
        free(value);
        return (0);
    }
    add_token(list, full_value, TOKEN_WORD);
    free(value);
    return (1);
}

static int	handle_operator_token(char **input, t_token **list)
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

static int	handle_word_token(char **input, t_token **list)
{
	char	*word;

	word = extract_word(input);
	add_token(list, word, TOKEN_WORD);
	return (1);
}

t_token	*lexer(char *input, t_shell *shell)
{
	t_token	*list;
	int		success;

	list = NULL;
	success = 1;
	while (*input && success)
	{
		if (is_white_space(*input))
			skip_white_spaces(&input);
		else if (is_quote(*input))
			success = handle_quote_token(&input, &list, shell);
		else if (is_operator(*input))
			success = handle_operator_token(&input, &list);
		else
			success = handle_word_token(&input, &list);
	}
	if (!success)
	{
		free_tokens(list);
		return (NULL);
	}
	return (list);
}
