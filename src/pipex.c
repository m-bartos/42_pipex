/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartos <mbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:02:15 by mbartos           #+#    #+#             */
/*   Updated: 2023/11/29 14:55:46 by mbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute(char *argv, char **env)
{
	char	*path;
	char	**cmd;

	path = get_path(argv, env);
	cmd = ft_split(argv, ' ');
	if (path == NULL)
	{
		ft_putstr_fd("Command not found\n", 2);
		exit(1);
	}
	if (execve(path, cmd, env) == -1)
	{
		perror("Error execve");
		exit(1);
	}
}

void	pipe_fork(char *argv, char **env)
{
	int	p_fd[2];
	int	p_id;

	if (pipe(p_fd) == -1)
	{
		perror("Pipe error");
		return ;
	}
	p_id = fork();
	if (p_id == -1)
	{
		perror("Fork error");
		return ;
	}
	if (p_id == 0)
	{
		close(p_fd[0]);
		dup2(p_fd[1], STDOUT_FILENO);
		execute(argv, env);
	}
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
	}
}

int	open_files(int argc, char **argv, int *fd_in, int *fd_out)
{
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")) == 0 \
		&& ft_strlen("here_doc") == ft_strlen(argv[1]) && argc > 5)
	{
		ft_read_heredoc(argv[2]);
		*fd_in = open("here_doc", O_RDONLY, 0444);
		*fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
		return (3);
	}
	else
	{
		*fd_in = open(argv[1], O_RDONLY, 0444);
		*fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		return (2);
	}
}

int	main(int argc, char **argv, char **env)
{
	int	i;
	int	fd_in;
	int	fd_out;

	if (argc < 5)
	{
		ft_putstr_fd("Error format: ./pipex file1 cmd1 .. cmdn file2\n", 2);
		ft_putstr_fd("./pipex here_doc LIMITER cmd1 .. cmdn file2\n", 2);
		return (1);
	}
	i = open_files(argc, argv, &fd_in, &fd_out);
	if (fd_in < 0 || fd_out < 0)
	{
		perror("File error");
		return (1);
	}
	dup2(fd_in, STDIN_FILENO);
	while (i < (argc - 2))
		pipe_fork(argv[i++], env);
	unlink("here_doc");
	dup2(fd_out, STDOUT_FILENO);
	execute(argv[i], env);
}
