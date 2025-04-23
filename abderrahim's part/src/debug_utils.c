#include "minishell.h"

void print_parsed_cmds(t_cmd *cmds)
{
    int cmd_num = 0;
    while (cmds) {
        printf("--- Command %d ---\n", cmd_num++);

        // Print arguments (e.g., ["echo", "hello"])
        printf("Args: ");
        int i = 0;
        while (cmds->args[i])
        {
            printf("[%s] ", cmds->args[i]);
            i++;
        }
        printf("\n");

        // Print redirections (e.g., > out.txt)
        t_redirect *redir = cmds->redirs;
        while (redir) {
            const char *type_str = 
                (redir->type == TOKEN_REDIR_IN) ? "<" :
                (redir->type == TOKEN_REDIR_OUT) ? ">" :
                (redir->type == TOKEN_APPEND) ? ">>" : "<<";
            printf("Redirect: %s %s\n", type_str, redir->file);
            redir = redir->next;
        }

        cmds = cmds->next;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s \"command\"\n", argv[0]);
        return 1;
    }

    // Step 1: Lexing (convert input string to tokens)
    t_token *tokens = lexer(argv[1]);
    
    // Step 2: Parsing (convert tokens to structured commands)
    t_cmd *cmds = parser(tokens);
    
    // Step 3: Debug print parsed commands
    print_parsed_cmds(cmds);

    // Step 4: Free memory
    free_tokens(tokens);
    free_cmds(cmds);
    return 0;
}