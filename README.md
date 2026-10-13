# TCP Prisoner's Dilemma Game Server

A multi-client TCP implementation of the classic **Iterative Prisoner's Dilemma** game-theory model built in **C** using Linux POSIX sockets (`sys/socket.h`, `arpa/inet.h`). A central **Warden Server** (`warden.c`) pairs two networked **Prisoner Clients** (`prisoner_1.c`, `prisoner_2.c`), synchronizes their turn submissions, evaluates the payoff matrix, and broadcasts round verdicts in real time.

## Architecture & Networking Flow

* **Two-Client Matchmaking Barrier (`warden.c`):** Binds to `INADDR_ANY:8080` over IPv4 TCP (`AF_INET`, `SOCK_STREAM`) and blocks until both Prisoner connections are established before starting the game loop.
* **Lockstep Turn Synchronization:** For each round, the server reads simultaneous choices (`1` = Cooperate, `0` = Defect) from both client file descriptors before evaluating the outcome, preventing either player from seeing the opponent's move early.
* **Graceful Disconnect Detection:** Monitors socket `read()` and `recv()` return values for `EOF` or errors (`<= 0` bytes) when either client terminates their session (`Ctrl+C`), cleanly closing all active client and listening socket descriptors.

## Payoff Matrix

| Prisoner 1 Move | Prisoner 2 Move | Prisoner 1 Outcome | Prisoner 2 Outcome |
| :--- | :--- | :--- | :--- |
| **Cooperate (`1`)** | **Cooperate (`1`)** | `1 year in jail` | `1 year in jail` |
| **Defect (`0`)** | **Cooperate (`1`)** | `Free (0 years)` | `3 years in jail` |
| **Cooperate (`1`)** | **Defect (`0`)** | `3 years in jail` | `Free (0 years)` |
| **Defect (`0`)** | **Defect (`0`)** | `2 years in jail` | `2 years in jail` |

## Build & Run

### 1. Compile Server & Clients
```bash
gcc -O2 warden.c -o warden
gcc -O2 prisoner_1.c -o prisoner1
gcc -O2 prisoner_2.c -o prisoner2
```

### 2. Local Execution (Single Machine)
Open three terminal windows:

* **Terminal 1 (Start Warden Server):** `./warden`
* **Terminal 2 (Connect Prisoner 1):** `./prisoner1`
* **Terminal 3 (Connect Prisoner 2):** `./prisoner2`

Enter `1` (Cooperate) or `0` (Defect) in both client terminals to play each round. Press `Ctrl+C` in either client terminal to end the session.

### 3. Running Across Different Machines (LAN)
1. Find the local IPv4 address of the machine running `warden` (`ip addr` or `hostname -I`).
2. Update `#define IP "127.0.0.1"` in `prisoner_1.c` and `prisoner_2.c` to the Warden machine's IP address and recompile.