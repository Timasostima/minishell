*This project has been created as part of the 42 curriculum by ibenaven and tkulivar.*

# Minishell

## Description
`minishell` is a simplified Unix shell written in C, inspired by Bash, for the mandatory part of the 42 minishell project.

The main objective is to understand and implement the core mechanics of a shell:
- parsing user input into executable commands
- process creation and synchronization (`fork`, `execve`, `waitpid`)
- file descriptor management (pipes and redirections)
- environment and exit status handling
- interactive signal behavior

This implementation focuses on mandatory subject compliance and Bash-like behavior within the required scope:
- interactive prompt and command history
- command execution with `PATH`, relative paths, and absolute paths
- pipes and redirections (`<`, `>`, `>>`, `<<`)
- environment expansion (`$VAR`) and last status expansion (`$?`)
- signal handling for `Ctrl-C`, `Ctrl-D`, and `Ctrl-\` in interactive mode
- required builtins: `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

## Architecture & Design Decisions
### Core Data Model
The shell is structured around a linked-command table:
- `t_token`: lexer output (`WORD`, `|`, `<`, `>`, `<<`, `>>`)
- `t_redir`: redirection list attached to each command
- `t_cmd`: command node with `argv`, redirections, builtin flag, and pipeline `next`

This keeps parsing and execution decoupled and makes pipeline execution predictable.

### Processing Pipeline
Input is handled in explicit phases:
1. Lexing into tokens.
2. Parsing tokens into a command table.
3. Expansion of command arguments and heredoc content.
4. Execution with redirections, builtins, and pipelines.

This modular flow simplifies debugging and helps isolate behavior by stage.

### Environment Management
Environment state is stored in a custom linked list (`t_env`) instead of mutating `envp` directly.
This enables:
- controlled `export` / `unset` behavior
- consistent `SHLVL` updates
- dynamic export to `char **` only when launching external commands

### Signal Strategy
Signal handling is split by context:
- interactive parent shell
- child process before `execve`
- parent while waiting for children
- heredoc input mode

This mirrors Bash behavior more closely and avoids mixing signal concerns across unrelated execution states.

### Scope
Only the mandatory part is implemented. Bonus features (`&&`, `||`, parentheses, wildcards) are intentionally out of scope in this repository.

## Instructions
Prerequisites:
- `cc`
- `make`
- GNU Readline development library (`readline`)

Build:
```bash
make
```

Run:
```bash
./minishell
```

Useful Makefile rules:
```bash
make clean
make fclean
make re
```

## Resources
Classical references used for shell behavior and system calls:
- Bash Reference Manual: https://www.gnu.org/software/bash/manual/
- POSIX Shell Command Language: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html
- Bash bugs: https://lists.gnu.org/r/bug-bash/2022-06/msg00088.html
- Linux man pages: `readline(3)`, `execve(2)`, `fork(2)`, `waitpid(2)`, `dup2(2)`, `pipe(2)`, `signal(7)`

AI usage:
- AI assistance was used for technical explanations, edge-case analysis, and refactoring discussions.
- AI assistance was also used to draft documentation and help organize manual validation workflows.
- All suggestions were reviewed, adapted, and validated against the subject constraints and expected Bash behavior before integration.
