#include"minishell.h"

void free_redirect(t_redirect *redir)
{
    if (!redir)
        return;
    free(redir->file);
    free(redir);
}

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

void free_cmds(t_cmd *cmds)
{
    t_cmd       *tmp_cmd;
    t_redirect  *tmp_redir;
    int         i;

    while (cmds)
    {
        tmp_cmd = cmds;
        cmds = cmds->next;
        if (tmp_cmd->args)
        {
            i = 0;
            while(tmp_cmd->args[i])
            {
                free(tmp_cmd->args[i]);
                i++;
            }
            free(tmp_cmd->args);
        }
        while (tmp_cmd->redirs)
        {
            tmp_redir = tmp_cmd->redirs;
            tmp_cmd->redirs = tmp_cmd->redirs->next;
            free(tmp_redir->file);
            free(tmp_redir);
        }
        free(tmp_cmd);
    }
}

void free_redirections(t_redirect *redirs)
{
    t_redirect *tmp;

    while (redirs)
    {
        tmp = redirs;
        redirs = redirs->next;
        free_redirect(tmp);
    }
}

void free_cmd(t_cmd *cmd)
{
    int i;

    if (!cmd)
        return;
    if (cmd->args)
    {
        i = 0;
        while (cmd->args[i])
            free(cmd->args[i++]);
        free(cmd->args);
    }
    free_redirections(cmd->redirs);
    free(cmd);
}
