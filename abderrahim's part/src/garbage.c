#include"minishell.h"

int main(int ac, char **av)
{
    t_token *tokens;
    //int i = 1;

    if (ac < 2)
    {
        printf("Usage: %s \"command to parse\"\n", av[0]);
        printf("Example: %s \"echo hello | grep test\"\n", av[0]);
        return (0);
    }

    // while (i < ac)
    // {
    //     printf("\nParsing: '%s'\n", av[i]);
    //     tokens = lexer(av[i]);
    //     print_tokens(tokens);
    //     free_tokens(tokens);
    //     i++;
    // }
    // Step 1: Lexing (convert input string to tokens)
    
    // Step 2: Parsing (convert tokens to structured commands)
    t_cmd *cmds = parser(tokens);
    
    // Step 3: Debug print parsed commands
    print_parsed_cmds(cmds);

    // Step 4: Free memory
    free_tokens(tokens);
    //free_cmds(cmds);
    return (0);
}