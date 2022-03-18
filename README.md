# Doc on 42 project ft_irc

If you need somewhere to start, I have summarized my research for the project in the folder notes.

I have also created a very simple server/client program, to familiarize myself with sockets, polling, and non-blocking code.

In the folder server_test run make, which will create 2 executables: server and client.

run ./server <port> with the port of your choosing (ex: 6667).

in a separate terminal tab/window, run ./client <port> with the same port as the server.

Enter a username, and you can send messages to the server.
  
You can open other terminals and add other users to the server (by running ./client on the terminal instance) to communicate between terminals.
