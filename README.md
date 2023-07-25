    THE UDP SERVER AND CLIENT

Introduction

    This project has a UDP server and client. The UDP server starts with a port.
The UDP client starts with a message, server IP and port.

Build

    Firstly clone the git repo.

      git clone https://github.com/htl1126/udp_server_client.git

    Compile the source files

      make all

Run the server and client

    Launch the server with IP and port

      ./server 4567

    Use the client with message(should not include whitespaces), server IP and port (we use localhost here)

      ./client test_message localhost 4567


Discussion

    I follow the instructions in this post to avoid listening from 0.0.0.0.

      https://stackoverflow.com/questions/16518920/why-the-first-client-sees-to-have-source-ip-of-0-0-0-0

    The exponentially incremented backoff waiting time is implemented. However, I cannot reproduce the scenario
    that the client has to wait and send another message due to connection issue or disconnected servers. I haven't
    figured out a way to know whether a UDP message has been sent successfully.

      https://stackoverflow.com/questions/33208263/udp-sendto-never-fails

