# Low Latency Network Application (C++)

## Description
A simple client-server chat system built using C++ socket programming and multithreading.
Designed to demonstrate low-latency real-time communication.

## Features
- TCP client-server communication
- Multithreaded server
- Instant message echo
- Multiple clients supported

## Build
g++ server.cpp -o server -pthread  
g++ client.cpp -o client -pthread  

## Run
Terminal 1: ./server  
Terminal 2: ./client  

## Tech Stack
- C++
- POSIX Sockets
- Multithreading

## Future Improvements
- Broadcast messages
- UDP version
- Encryption
- Latency measurement
