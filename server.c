#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

void handle_request(const char *request, char *response)
{
    char *method, *path;
    char file_path[256];
    FILE *file;
    char *content_type;
    long file_size;

    content_type = "text/plain";

    method = strtok((char *)request, " ");
    path = strtok(NULL, " ");

    if (path == NULL)
    {
        path = "/";
    }

    if (strcmp(path, "/") == 0)
    {
        strcpy(file_path, "index.html");
    }
    else
    {

        snprintf(file_path, sizeof(file_path), ".%s", path);
    }

    file = fopen(file_path, "rb");
    if (file)
    {
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (strstr(file_path, ".html"))
        {
            content_type = "text/html";
        }
        else if (strstr(file_path, ".css"))
        {
            content_type = "text/css";
        }
        else if (strstr(file_path, ".js"))
        {
            content_type = "application/javascript";
        }

        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n", content_type, file_size);
        fread(response + strlen(response), 1, file_size, file);
        fclose(file);
    }
    else
    {

        sprintf(response, "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nNot Found");
    }
}

int main()
{
    WSADATA wsaData;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char response[BUFFER_SIZE] = {0};

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Winsock initialization failed\n");
        return 1;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET)
    {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
    {
        printf("Bind failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, 3) == SOCKET_ERROR)
    {
        printf("Listen failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port 8080\n");

    while (1)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket == INVALID_SOCKET)
        {
            printf("Accept failed\n");
            closesocket(server_fd);
            WSACleanup();
            return 1;
        }

        memset(buffer, 0, sizeof(buffer));
        recv(new_socket, buffer, sizeof(buffer) - 1, 0);
        printf("Request received:\n%s\n", buffer);

        handle_request(buffer, response);
        send(new_socket, response, strlen(response), 0);

        closesocket(new_socket);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
