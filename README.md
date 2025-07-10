bdelt logic dial lexer kamlo 7it 3ndi mochkil fach kandkhel had command: echo "kjdhfhkd"skdjfhhj""

katwli 4 dail tokens blast 2: echo "ksdjhfkjdhf"jksdfjkh""
=== Command ===
Args: [echo] ["ksdjhfkjdhf"] [jksdfjkh] [""]

ila makanch espace wla pipe makhasch command tsplita

<<<<<<< HEAD
test 




modifications


This is the new function that reads a SINGLE heredoc and returns the fd.
static int	read_heredoc_input(t_redirect *redir)

This is the main pre-processing function to call from your executor.
int	process_heredocs(t_cmd *cmd_list)

else if (redir->type == TOKEN_HEREDOC)
		{
			New logic: just dup the fd we already created
			dup2(redir->heredoc_fd, STDIN_FILENO);
			close(redir->heredoc_fd);
		}