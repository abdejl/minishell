#include "minishell.h"

int executor(t_shell *shell, t_cmd *cmd_list)
{
	int status = 0;

	if (cmd_list && cmd_list->pipe_out)
		status = execute_pipeline(shell, cmd_list);
	else
	{
		while (cmd_list)
		{
			status = handle_cmd(shell, cmd_list);
			cmd_list = cmd_list->next;
		}
	}
	shell->exit_status = status;
	return (status);
}
