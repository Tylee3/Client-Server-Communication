# Client Server Communication

## Problem Statement
This program uses a client–server communication model using TCP sockets. The client requests a text file from the server, and the server sends the contents of that file back. It also handles errors when a file is missing or is too long (<255 characters).

## Describe the Solution
The server listens on port 9001 and waits for incoming client connections. When a client connects, it sends the requested filename, and the server either returns the file’s contents or an appropriate error message. The client connects to the server, sends the filename, and prints whatever message it receives back.

## Pros
1. Easy to understand and use
2. Precise error handling for invalid or large files

## Cons
1. Only supports one client connection at a time
3. No implementation of graceful shutdown
