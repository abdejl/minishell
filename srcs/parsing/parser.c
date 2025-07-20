/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 10:07:05 by abjellal          #+#    #+#             */
/*   Updated: 2025/07/09 10:07:05 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static t_cmd	*finalize_parsing(t_cmd *list, t_cmd *current, int expect_cmd)
// {
//     if (expect_cmd && list)
//     {
//         print_syntax_error("newline");
//         free_cmds(list);
//         free_cmd(current);
//         return (NULL);
//     }
//     cmd_add_back(&list, current);
//     return (list);
// }

static int	is_separator(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_PIPE || token->type == TOKEN_SEMICOLON
		|| token->type == TOKEN_AMPERSAND);
}

static int	handle_word_or_redir(t_token **tok, t_cmd *cmd)
{
	t_token_type	type;

	type = (*tok)->type;
	if (type == TOKEN_WORD)
		add_arg(cmd, (*tok)->value);
	else
	{
		if (!(*tok)->next || (*tok)->next->type != TOKEN_WORD)
		{
			if ((*tok)->next)
				print_syntax_error((*tok)->next->value);
			else
				print_syntax_error("newline");
			return (0);
		}
		add_redirect(cmd, type, (*tok)->next->value);
		*tok = (*tok)->next;
	}
	return (1);
}

static int	process_separator(t_token *tok, t_cmd **list, t_cmd **curr)
{
	if (tok->type == TOKEN_PIPE)
		(*curr)->pipe_out = 1;
	(*curr)->background = (tok->type == TOKEN_AMPERSAND);
	cmd_add_back(list, *curr);
	*curr = new_command();
	return (1);
}

static int	process_token_loop(t_token **tokens, t_cmd **cmd_list,
		t_cmd **current_cmd, int *expect_cmd)
{
	if ((*tokens)->type == TOKEN_ERROR)
	{
		print_syntax_error((*tokens)->value);
		free_cmd(*current_cmd);
		return (0);
	}
	if (is_separator(*tokens))
	{
		if (*expect_cmd)
		{
			print_syntax_error((*tokens)->value);
			free_cmd(*current_cmd);
			return (0);
		}
		process_separator(*tokens, cmd_list, current_cmd);
		*expect_cmd = 1;
	}
	else if (!handle_word_or_redir(tokens, *current_cmd))
	{
		return (free_cmds(*cmd_list), free_cmd(*current_cmd), 0);
	}
	else
		*expect_cmd = 0;
	return (1);
}

static int	is_variable_assignment(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (str[i] == '=');
}

t_cmd	*parser(t_token *tokens, t_shell *shell)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	int		expect_cmd;

	(void)shell;
	cmd_list = NULL;
	if (!tokens)
		return (NULL);
	current_cmd = new_command();
	expect_cmd = 1;
	while (tokens)
	{
        if (tokens->type == TOKEN_WORD && is_variable_assignment(tokens->value)
			&& !cmd_list && !current_cmd->args)
		{
			// This is a standalone assignment, handle it as a built-in
			add_arg(current_cmd, ft_strdup("export")); // Pretend it's an export command
			add_arg(current_cmd, ft_strdup(tokens->value));
			cmd_add_back(&cmd_list, current_cmd);
			current_cmd = new_command();
			tokens = tokens->next;
			continue ;
		}
		if (!process_token_loop(&tokens, &cmd_list, &current_cmd, &expect_cmd))
			return (NULL);
		tokens = tokens->next;
	}
	cmd_add_back(&cmd_list, current_cmd);
	return (cmd_list);
}
