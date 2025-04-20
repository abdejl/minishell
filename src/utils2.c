#include"minishell.h"

void free_tokens(t_token *tokens)
{
    t_token *tmp;
    
    while (tokens != NULL)
    {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->value);
        free(tmp);
    }
}

char *extract_word(char **str)
{
    char *start = *str;
    char *end = start;
    
    while (*end && !is_white_space(*end) && !is_operator(end) && !is_quote(end))
        end++;
    
    char *word = ft_strndup(start, end - start);
    *str = end;
    return (word);
}

char *extract_operator(char **str)
{
    char *op = NULL;
    
    if (**str == '>' && *(*str + 1) == '>')
    {
        op = ft_strdup(">>");
        *str += 2;
    }
    else if (**str == '<' && *(*str + 1) == '<')
    {
        op = ft_strdup("<<");
        *str += 2;
    }
    else
    {
        op = ft_strndup(*str, 1);
        (*str)++;
    }
    return (op);
}

int get_operator_type(char *op)
{
    if (*op == '|')
        return (TOKEN_PIPE);
    if (*op == '>')
        return (op[1] == '>' ? TOKEN_APPEND : TOKEN_REDIR_OUT);
    if (*op == '<')
        return (op[1] == '<' ? TOKEN_HEREDOC : TOKEN_REDIR_IN);
    return (TOKEN_WORD);
}

void add_token(t_token **token_list, char *value, int type)
{
    t_token *new_token;
    t_token *last;

    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return;
    
    new_token->value = ft_strdup(value);
    new_token->type = type;
    new_token->next = NULL;
    if (*token_list == NULL)
    {
        *token_list = new_token;
    }
    else
    {
        last = *token_list;
        while (last->next != NULL)
            last = last->next;
        last->next = new_token;
    }
}

char    *ft_strndup(const char *s, size_t n)
{
    char    *dup;
    size_t  len;
    size_t  i;

    len = 0;
    while (s[len] && len < n)
        len++;
    
    dup = (char *)malloc(len + 1);
    if (!dup)
        return (NULL);
    
    i = 0;
    while (i < len)
    {
        dup[i] = s[i];
        i++;
    }
    dup[len] = '\0';
    
    return (dup);
}

void *ft_memcpy(void *dst, const void *src, size_t n)
{
    unsigned char *d = dst;
    const unsigned char *s = src;
    while (n--) *d++ = *s++;
    return dst;
}

// Minimal ft_strdup
char *ft_strdup(const char *s1)
{
    size_t len = 0;
    while (s1[len]) len++;
    char *dup = malloc(len + 1);
    if (!dup) return NULL;
    for (size_t i = 0; i <= len; i++) dup[i] = s1[i];
    return dup;
}