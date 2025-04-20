#include"minishell.h"

t_token *lexer(char *input)
{
    t_token *tokens = NULL;
    
    while (*input)
    {
        if (is_white_space(*input))
            skip_white_spaces(&input);
        else if (is_quote(input))
            add_token(&tokens, extract_quoted_string(&input), TOKEN_WORD);
        else if (is_operator(input))
            add_token(&tokens, extract_operator(&input), get_operator_type(input));
        else
            add_token(&tokens, extract_word(&input), TOKEN_WORD);
        
        if (!*input)
            break;
    }
    return (tokens);
}

void print_tokens(t_token *tokens)
{
    int i = 0;
    while (tokens)
    {
        printf("Token %d: [%s]\n", i++, tokens->value);
        tokens = tokens->next;
    }
}

int main(int ac, char **av)
{
    t_token *tokens;
    int i = 1;

    if (ac < 2)
    {
        printf("Usage: %s \"command to parse\"\n", av[0]);
        printf("Example: %s \"echo hello | grep test\"\n", av[0]);
        return (0);
    }

    while (i < ac)
    {
        printf("\nParsing: '%s'\n", av[i]);
        tokens = lexer(av[i]);
        print_tokens(tokens);
        free_tokens(tokens);
        i++;
    }
    return (0);
}
