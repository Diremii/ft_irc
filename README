*This project has been created as part of the 42 curriculum by humontas, ttremel*

# ft_irc — Internet Relay Chat

A custom IRC server written in C++98, compatible with HexChat, 
implementing the core operator commands of the IRC protocol (RFC 1459).

## Description

ft_irc implements the core commands for a functional IRC server (RFC 1459). 
Clients can connect and, once authenticated, chat in public channels or 
via private messages. Users have access to channel modes, allowing them 
to configure their channels as needed. This project also includes a bot 
named 'ShallowRed' — a reference to Deep Blue — featuring a Tic Tac Toe 
game, as well as DCC file transfer support.

## Instructions

### Server
```Bash
# Build
make

# Run
./ircserv <port> <password>
```

### Bot
```Bash
# Build
cd bot && make

# Run
./bot <hostname> <port> <password>
```

### Connect with HexChat
1. Open HexChat → Network List → Add
2. Edit the network, set server to `<server_ip>/<port>`
3. Enter the server password in the password field.

If your connection details are correct, HexChat will automatically handle the authentication commands.

### Connect with netcat
```bash
nc -C <server_ip> <port>
PASS <password>
NICK <nickname>
USER <username>
```

## Command Reference

### General

| Command | Description |
|---|---|
| `PASS <password>` | Authenticate with the server |
| `NICK <nickname>` | Set or change your nickname |
| `USER <username>` | Set your username (required for registration) |
| `QUIT [:<reason>]` | Disconnect from the server |
| `PRIVMSG <target> :<message>` | Send a message to a user or channel |
| `JOIN <#channel> [password]` | Join a channel |
| `PART <#channel> [:<reason>]` | Leave a channel |
 
> Commands requiring registration: `PASS`, `NICK`, and `USER` must be completed before using any other command.

### MODE Flags
 
| Flag | Description |
|---|---|
| `+o / -o <nick>` | Grant or revoke operator status |
| `+i / -i` | Toggle invite-only mode |
| `+t / -t` | Restrict topic changes to operators |
| `+k / -k <password>` | Set or remove channel password |
| `+l / -l <limit>` | Set or remove user limit |
 
### DCC File Transfer
 
```
DCC SEND <nick> <filename> <port> <filesize>
```
 
### Bot Commands
 
| Command | Description |
|---|---|
| `!help` | Display available bot commands |
| `!play` | Start a Tic Tac Toe game |
| `!move <position>` | Make a move (1-9) |
 
The bot (`ShallowRed`) auto-joins `#general` on startup and joins any channel it is invited to. Games time out after 60 seconds of inactivity.

> `ShallowRed` can only play with one person per channel at a time, but you can also challenge him via private message.
 
## Resources

- [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [HexChat IRC Client](https://hexchat.github.io/)
- AI was used as a learning aid for debugging, understanding edge cases in socket programming, and reviewing code for potential crashes. All code was written and understood by the authors.