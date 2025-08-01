/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brbaazi <brbaazi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:45:18 by brbaazi           #+#    #+#             */
/*   Updated: 2025/07/18 20:45:18 by brbaazi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	executor(t_shell *shell, t_cmd *cmd_list)
{
	int		status;
	t_cmd	*segment_head;
	t_cmd	*runner;
	t_cmd	*next_segment_head;

	if (process_heredocs(cmd_list, shell) == -1)
		return (1);
	status = 0;
	segment_head = cmd_list;
	while (segment_head)
	{
		runner = segment_head;
		while (runner->pipe_out)
			runner = runner->next;
		next_segment_head = runner->next;
		runner->next = NULL;
		if (segment_head->pipe_out)
			status = execute_pipeline(shell, segment_head);
		else
			status = handle_cmd(shell, segment_head);
		runner->next = next_segment_head;
		segment_head = next_segment_head;
	}
	shell->exit_status = status;
	return (status);
}
