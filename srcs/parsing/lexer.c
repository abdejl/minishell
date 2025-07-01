#include "minishell.h"

t_token *lexer(char *input)
{
    t_token *tokens = NULL;

    if (!input)
        return NULL;
    while (*input)
    {
        if (is_white_space(*input))
            skip_white_spaces(&input);
        else if (is_quote(input))
            tokens = handle_quoted_string(&input, tokens);
        else if (is_operator(input))
            tokens = handle_operator(&input, tokens);
        else
            tokens = handle_word(&input, tokens);
        if (!tokens || !*input)
            break;
    }
    return tokens;
}