# Minimalistic HTTP Server in C

A minimalistic HTTP server implemented in C that mimics basic Express.js functionality. This server supports static file serving and simple routing. It is designed to be a lightweight example of how to handle HTTP requests and serve files using sockets on Windows.

## Features

- Serve static files (HTML, CSS, JS) from the server directory.
- Handle basic routes: `/hello` and `/bye`.
- Respond with "Hello, World!" and "Goodbye!" for the respective routes.
- Returns a 404 Not Found response for unrecognized routes.

## Prerequisites

- Windows operating system.
- MinGW or similar toolchain for compiling C code.
- Basic knowledge of HTTP and socket programming.

## Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/your-repository.git
cd your-repository
