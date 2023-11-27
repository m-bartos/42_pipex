/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartos <mbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:02:15 by mbartos           #+#    #+#             */
/*   Updated: 2023/11/27 16:29:40 by mbartos          ###   ########.fr       */
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
	return(ft_split(argv, ' '));
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
	while (paths[i])
	{
		new_path = ft_strjoin(paths[i], "/");
		free(paths[i]);
		paths[i] = ft_strjoin(new_path, cmd[0]); //need array of commands as input to execve
		free(new_path);
		if(access(paths[i] , X_OK) == 0)
			break;
		i++;
	}
	// Cmd not found
	if (paths[i] == NULL)
		return (NULL);
	path = ft_strdup(paths[i]);
	free_array(paths);
	return (path);
	// need to free path in another function
}

int	execute(char *argv, char **env)
{
	char *path;
	char **cmd;
	
	path = get_path(argv, env);
	cmd = get_cmd(argv);
	execve(path, cmd, env);
	return (1); // need to handle errors from execve
}

int	pipe_fork(char *argv, char **env)
{
	int p_fd[2];
	int p_id;

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
	int fd_in;
	int fd_out;

	//error handling when !(argc >= 5)

	//argv[1] = file_in
	//argv [2 -- (argc-2)] =cmnds
	//argv[argc - 1] = file_out


	//opening the input and output file
	fd_in = open(argv[1], O_RDONLY, 0777);
	fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if(fd_in < 0 || fd_out < 0)
		perror("File error");

	//first iteration - STDIN = fd_in
	dup2(fd_in, STDIN_FILENO);
	//pipe_fork for all the cmnds
	i = 2;
	while (i < (argc - 2))
	{
		pipe_fork(argv[i], env);
		i++;
	}
	//last iteration - STDOUT = fd_out
	dup2(fd_out, STDOUT_FILENO);
	execute(argv[i], env);
}