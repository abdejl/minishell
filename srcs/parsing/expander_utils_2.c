/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:08:18 by abjellal          #+#    #+#             */
/*   Updated: 2025/07/09 10:08:18 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_expandable(char c)
{
    return (ft_isalnum(c) || c == '_' || c == '?');
}

void	process_quote(t_expand_state *state)
{
	if (state->p > state->segment_start)
		append_str_node(state->list,
			ft_substr(state->segment_start, 0, state->p - state->segment_start));
	if (*state->p == '\'')
		state->in_s_quotes = !state->in_s_quotes;
	else
		state->in_d_quotes = !state->in_d_quotes;
	state->segment_start = state->p + 1;
}

void	process_dollar(t_expand_state *state)
{
	if (state->p > state->segment_start)
		append_str_node(state->list,
			ft_substr(state->segment_start, 0, state->p - state->segment_start));
	append_str_node(state->list, get_var_value(&state->p, state->shell));
	state->segment_start = state->p;
}

void	init_expand_state(t_expand_state *st, char *arg, t_shell *sh)
{
	st->list = gc_malloc(sizeof(t_str_list *));
	*st->list = NULL;
	st->p = arg;
	st->segment_start = arg;
	st->in_s_quotes = 0;
	st->in_d_quotes = 0;
	st->shell = sh;
}

char	*expand_and_join(char *arg, t_shell *shell)
{
	t_expand_state	st;
	char			*result;

	init_expand_state(&st, arg, shell);
	while (*st.p)
	{
		if ((*st.p == '\'' && !st.in_d_quotes) || (*st.p == '\"' && !st.in_s_quotes))
			process_quote(&st);
		else if (*st.p == '$' && !st.in_s_quotes && is_expandable(*(st.p + 1)))
		{
			process_dollar(&st);
			continue ;
		}
		st.p++;
	}
//...
	if (st.p > st.segment_start)
		append_str_node(st.list, \
			ft_substr(st.segment_start, 0, st.p - st.segment_start));
	result = join_str_list(*st.list);
	free_str_list(*st.list);
	free(st.list);
	return (result);
}

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	t_str_list	*list_head;
	char		*p;
	char		*segment_start;
	char		*result;

	list_head = NULL;
	p = line;
	segment_start = line;
	while (*p)
	{
		if (*p == '$' && is_expandable(*(p + 1)))
		{
			if (p > segment_start)
				append_str_node(&list_head, ft_substr(segment_start, 0, p - segment_start));
			append_str_node(&list_head, get_var_value(&p, shell));
			segment_start = p;
			continue ;
		}
		p++;
	}
	if (p > segment_start)
		append_str_node(&list_head, ft_substr(segment_start, 0, p - segment_start));
	result = join_str_list(list_head);
	free_str_list(list_head); // Free the temporary list
	return (result);
}
