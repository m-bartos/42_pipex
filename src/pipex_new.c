/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartos <mbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:02:15 by mbartos           #+#    #+#             */
/*   Updated: 2023/11/28 11:44:08 by mbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	free_array(char **array)
{
	int	i;

	i = 0;
	while (*(array + i))
	{
		free(*(array + i));
		i++;
	}
	free(array);
	return (1);
}

char	**get_cmd(char	*argv)
{
	return (ft_split(argv, ' '));
}

char	*get_path(char *argv, char **env)
{
	char	**cmd;
	char	*path;
	char	*new_path;
	char	**paths;
	int		i;

	cmd = get_cmd(argv);
	path = NULL;
	while (*env && path == NULL)
	{
		path = ft_strnstr(*env, "PATH=", 5);
		env++;
	}
	new_path = ft_substr(path, 5, (ft_strlen(path) - 5));
	paths = ft_split(new_path, ':');
	free(new_path);
	i = 0;
	while (paths[i] != NULL)
	{
		new_path = ft_strjoin(paths[i], "/");
		free(paths[i]);
		paths[i] = ft_strjoin(new_path, cmd[0]);
		free(new_path);
		if(access(paths[i] , X_OK) == 0)
			break ;
		i++;
	}
	if (paths[i] == NULL)
		path = NULL;
	else
		path = ft_strdup(paths[i]);
	return (free_array(paths), path);
}

int	execute(char *argv, char **env)
{
	char	*path;
	char	**cmd;

	path = get_path(argv, env);
	cmd = get_cmd(argv);
	if (path == NULL)
	{
		ft_putstr_fd("Command not found\n", 2);
		exit(1);
	}
	execve(path, cmd, env);
	exit(1);
}

int	pipe_fork(char *argv, char **env)
{
	int	p_fd[2];
	int	p_id;

	if (pipe(p_fd) == -1)
		perror("Pipe error");
	p_id = fork();
	if (p_id == -1)
	{
		perror("Fork error");
		return (1);
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
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	int	i;
	int	fd_in;
	int	fd_out;

	if(argc < 5)
	{
		ft_putstr_fd("Format should be: ./pipex file1 cmd1 cmd2 ... cmdn file2", 1);
		return (1);
	}
	fd_in = open(argv[1], O_RDONLY, 0777);
	fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if(fd_in < 0 || fd_out < 0)
	{
		perror("File error");
		exit (0);
	}
	dup2(fd_in, STDIN_FILENO);
	i = 2;
	while (i < (argc - 2))
	{
		pipe_fork(argv[i], env);
		i++;
	}
	dup2(fd_out, STDOUT_FILENO);
	execute(argv[i], env);
}