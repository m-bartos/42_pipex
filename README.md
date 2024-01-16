# 42_pipex
The purpose of this project is to discover in detail pipe UNIX mechanism by writing my own "piping" program in C.

## Assignment
* The program is able to handle multiple pipes.
  * This: `./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2` will behave like `< file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2`
* The program supports `<<` and `>>` when first parameter is `'here_doc'`.
  * This: `./pipex here_doc LIMITER cmd cmd1 file` will behave like `cmd << LIMITER | cmd1 >> file`

### Examples
* `./pipex infile "ls -l" "wc -l" outfile` will behave like `< infile ls -l | wc -l > outfile`
* `./pipex infile "grep a1" "wc -w" outfile` will behave like `< infile grep a1 | wc -w > outfile`
* `./pipex here_doc EOF cat "wc -l" outfile` will behave like `cat << EOF | wc -l >> outfile`

## How to use the program
1. `git clone git@github.com:m-bartos/42_pipex.git`
2. `make`
3. Play with the program. If you run just `./pipex`, program will show you how the right format of input should look like.

**You have to create file1 before executing the program!**

## What I learned during this project
* I learned more about pipes in Unix terminal
* I learned about how multiprocessing and pipes in C work
* I learned about redirections of STD_IN and STD_OUT
* I applied various new functions, including pipe, fork, dup2, execve, perror, exit, etc.
* I had to handle several mallocs and ensured accurate freeing of allocated memory

## Thanks and kudos
Acknowledgements to CodeVault's YouTube playlist on [Unix Processes in C](https://www.youtube.com/playlist?list=PLfqABt5AS4FkW5mOn2Tn9ZZLLDwA3kZUY) for the valuable insights.
