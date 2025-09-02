
# Database Management via Multithreaded Socket-Based Architecture

This project is a client-server application for managing a film database. It was developed to explore key concepts in software development, including inter-process communication, multithreading, and network programming.

## Project Overview

The application consists of two main programs: a **client** and a **server**. The client is responsible for interacting with the user, taking search and evaluation requests for a film database, and transmitting them to the server. The server receives these requests, processes them, and sends back the results to the client.

The core features and architectural principles of this project are:

  * **Multithreaded Server:** The server is designed to handle multiple client requests simultaneously. For each new client connection, the server creates a dedicated thread to process the request, ensuring concurrent operation. The `thread.c` module contains the logic for these individual client-handling threads.
  * **Socket Communication:** Communication between the client and server is handled via **sockets**, a crucial change from the previous version which used named pipes (FIFOs). This allows for more flexible and robust communication, including over a network.
  * **Synchronized Access:** To maintain data integrity, especially when multiple clients are modifying the database, the server uses a **mutex** to synchronize access to shared resources. While the design aims to allow multiple clients to read simultaneously, any write operation (like updating a film's rating) must be done exclusively to prevent data corruption.
  * **Traceability and Version Control:** The project uses Git for version control. Commits and issues are linked to specific functional requirements (HLRs - High-Level Requirements) to ensure full traceability throughout the development process. Git tags and releases are used to mark the completion of major development phases.

## File Structure

The project repository is organized into several key directories:

  * `applications/`: Contains the executable programs once compiled.
  * `client/`: Source code for the client application.
      * `src/`: Contains `comm.c` (communication module) and `recherche.c` (search module).
  * `comm/server/`: Source code for the server's communication module.
  * `scripts/`: Shell scripts for compiling and running the application.
      * `compile`: Script to compile the source code.
      * `run_all_client`: Script to simulate multiple client connections.
      * `valgrind`: Script for memory leak detection.
  * `serveur/`: Source code for the server application.
      * `src/`: Contains `serveur.c`, the main server program, as well as `thread.c`, which handles individual client connections.
  * `cote.tsv`: A TSV (Tab-Separated Values) file representing the ratings database.
  * `resultat.tsv`: A TSV file to store the search results.

-----

## How to Run the Application

### Prerequisites

  * A C compiler (e.g., GCC)
  * The `pthread` library for multithreading
  * The project's database files (not included, but available from the course's Moodle page)

### Compilation

Navigate to the `scripts/` directory and run the `compile` script. This will compile all necessary source files and create the executable programs.

```bash
cd scripts/
./compile
```

### Running the Server

To start the server, execute the `run_serveur` script from the `scripts/` directory.

```bash
cd scripts/
./run_serveur
```

The server will start listening for incoming client connections on port `10001` at `127.0.0.1` (localhost).

### Running a Client

To run a single client, use one of the `run_client` scripts.

```bash
cd scripts/
./run_client
```

### Running Multiple Clients

To test the multithreading capabilities of the server, use the `run_all_client` script to launch multiple client instances simultaneously.

```bash
cd scripts/
./run_all_client
```

This will demonstrate how the server handles and processes requests from several clients concurrently.
