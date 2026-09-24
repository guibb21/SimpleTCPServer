# TCP_Server.c — TCP Reverse Shell in C

A minimal TCP server written in C that spawns an interactive shell session for each incoming connection.

## How it works

The server binds to port 8080 and listens for incoming TCP connections. When a client connects, it forks a child process and uses `dup2()` to redirect stdin, stdout, and stderr to the client socket. The child then executes `/bin/sh`, giving the client a fully interactive shell session. The parent process immediately closes its copy of the client socket and returns to accepting new connections.

`SIGCHLD` is set to `SIG_IGN` so zombie processes are automatically reaped when children exit. `SO_REUSEADDR` is set to allow restarting the server immediately after closing without waiting for the OS to release the port.

## Concepts explored

- TCP socket lifecycle: `socket()`, `bind()`, `listen()`, `accept()`
- Process forking with `fork()` and clean parent/child separation
- File descriptor redirection with `dup2()`
- Signal handling with `signal()`
- Socket options with `setsockopt()`

## Usage

**Compile:**
```
gcc TCP_Server.c -o TCP_Server
```

**Run the server:**
```
./TCP_Server
```

**Connect from another terminal or device:**
```
nc 127.0.0.1 8080
```

Replace `127.0.0.1` with the server's local IP (`ip a`) to connect from another device on the same network.

## Environment

Developed and tested on Linux (Ubuntu). Requires GCC and netcat (`nc`) for testing.
