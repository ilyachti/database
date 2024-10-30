//server.cpp  сервер



 


#include "base.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>

#define PORT 3556
#define MAX_CONNECTIONS 5

int main() {
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    library db;
    std::string fout("db.txt");
    db.addfromfile(fout);

    int server_fd, new_socket;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation error");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Binding error");
        return -1;
    }

    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        perror("Listening error");
        return -1;
    }

    fd_set readfds;
    int max_sd, activity, sd;
    int client_socket[MAX_CONNECTIONS] = { 0 };

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (int i = 0; i < MAX_CONNECTIONS; i++) {
            sd = client_socket[i];
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            printf("select error");
        }

        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                perror("Acceptance error");
                return -1;
            }

            for (int i = 0; i < MAX_CONNECTIONS; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CONNECTIONS; i++) {
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds)) {
                std::string query;
                char buffer[1024] = { 0 };

                read(sd, buffer, 1024);
                query = buffer;

                int result = db.query(query);

                if (result == 0) {
                    printf("OK\n");
                    ifstream file("to_send.txt");
                    while (file.good()) {
                        file.read(buffer, sizeof(buffer));
                        send(sd, buffer, file.gcount(), 0);
                    }
                }
            

                close(sd);
                client_socket[i] = 0;
            }
        }
    }

    return 0;
}



























//#include "base.h"
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <unistd.h>
//#include <thread>
//#include <vector>
//
//#define PORT 8091
//#define MAX_CONNECTIONS 5
//
//void handleClient(int client_socket) {
//    library db;
//    string fout("db.txt");
//    db.addfromfile(fout);
//    string query;
//    char buffer[1024] = { 0 };
//    read(client_socket, buffer, 1024);
//    query = buffer;
//    int result = db.readq(query);
//
//    if (result == 0) {
//        ifstream file("to_send.txt");
//        while (file.good()) {
//            file.read(buffer, sizeof(buffer));
//            send(client_socket, buffer, file.gcount(), 0);
//        }
//    }
//
//    close(client_socket);
//}
//
//int main() {
//    int server_fd, new_socket;
//    struct sockaddr_in address;
//    int addrlen = sizeof(address);
//
//    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//        perror("socket failed");
//        exit(EXIT_FAILURE);
//    }
//
//    address.sin_family = AF_INET;
//    address.sin_addr.s_addr = INADDR_ANY;
//    address.sin_port = htons(PORT);
//
//    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
//        perror("bind failed");
//        exit(EXIT_FAILURE);
//    }
//
//    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
//        perror("listen");
//        exit(EXIT_FAILURE);
//    }
//
//    std::vector<std::thread> clientThreads;
//
//    while (true) {
//        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*) & addrlen)) < 0) {
//            perror("accept");
//            exit(EXIT_FAILURE);
//        }
//
//        std::thread clientThread(handleClient, new_socket);
//        clientThreads.push_back(std::move(clientThread));
//    }
//
//    for (auto& thread : clientThreads) {
//        thread.join();
//    }
//
//    return 0;
//}


















































//#include "base.h"
//
//#include <stdio.h>
//#include <errno.h>
//#include <stdlib.h>
//#include <string.h>
//#include <ctype.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>
//#include "../sqlhard/all.h"
//
//#define PORT    5555
//#define BUFLEN  4096
//
//int   read_from_client(int fd, char* buf);
//void  write_to_client(int fd, char* buf, DB* db);
//
//
//int  main(void) {
//    int     i, err, opt = 1;
//    int     sock, new_sock;
//    fd_set  active_set, read_set;
//    struct  sockaddr_in  addr;
//    struct  sockaddr_in  client;
//    char    buf[BUFLEN];
//    socklen_t  size;
//
//    sock = socket(PF_INET, SOCK_STREAM, 0);
//    if (sock < 0) {
//        perror("server: cannot create socket");
//        exit(EXIT_FAILURE);
//    }
//
//    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
//
//    addr.sin_family = AF_INET;
//    addr.sin_port = htons(PORT);
//    addr.sin_addr.s_addr = htonl(INADDR_ANY);
//
//    err = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
//
//    if (err < 0) {
//        perror("server: cannot bind socket");
//        exit(EXIT_FAILURE);
//    }
//
//    err = listen(sock, 5);
//    if (err < 0) {
//        perror("server: listen queue failure");
//        exit(EXIT_FAILURE);
//    }
//
//    FD_ZERO(&active_set);
//    FD_SET(sock, &active_set);
//
//    std::cout << "server started\n";
//    std::cout << "server socket: " << sock << "\n";
//
//    std::string fn = "../sqlhard/dbs/my_insert.db";
//    DB db(fn);
//
//    while (1) {
//        read_set = active_set;
//        if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0) {
//            perror("server: select failure");
//            exit(EXIT_FAILURE);
//        }
//
//        for (i = 0; i < FD_SETSIZE; i++) {
//            if (FD_ISSET(i, &read_set)) {
//                if (i == sock) {
//                    size = sizeof(client);
//                    new_sock = accept(sock, (struct sockaddr*)&client, &size);
//                    if (new_sock < 0) {
//                        perror("server: accept failure");
//                        exit(EXIT_FAILURE);
//                    }
//                    fprintf(stdout, "server: connect from host %s, port %hu.\n",
//                        inet_ntoa(client.sin_addr),
//                        ntohs(client.sin_port));
//                    FD_SET(new_sock, &active_set);
//                }
//                else {
//                    err = read_from_client(i, buf);
//                    if (err < 0) {
//                        close(i);
//                        FD_CLR(i, &active_set);
//                    }
//                    else {
//                        if (strstr(buf, "stop")) {
//                            close(i);
//                            FD_CLR(i, &active_set);
//                        }
//                        else {
//                            write_to_client(i, buf, &db);
//                        }
//                    }
//                }
//            }
//        }
//    }
//}
//
//
//
//int  read_from_client(int fd, char* buf) {
//    int  nbytes;
//    nbytes = read(fd, buf, BUFLEN);
//
//    if (nbytes < 0) {
//        perror("server: read failure");
//        return -1;
//    }
//    else if (nbytes == 0) {
//        return -1;
//    }
//    else {
//        fprintf(stdout, "server got message: %s\n", buf);
//        return 0;
//    }
//}
//
//
//
//void  write_to_client(int fd, char* buf, library* db) {
//    int  nbytes;
//    std::string sql(buf);
//
//    std::cout << "parsing sql: " << sql << "\n";
//
//    std::stringstream ss;
//    db->parse_select_sql(sql, ss);
//
//    std::string s = ss.str();
//    uint32_t size = htonl(s.size());
//
//    nbytes = write(fd, &size, sizeof(uint32_t));
//    if (nbytes < 0) {
//        perror("write size");
//        return;
//    }
//
//    nbytes = write(fd, s.c_str(), s.size());
//    if (nbytes < 0) {
//        perror("write data");
//        return;
//    }
//
//}







//#include "base.h" 
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <unistd.h>
//
//#define PORT 8090
//#define MAX_CONNECTIONS 5
//
//int main() {
//    int server_fd, new_socket;
//    struct sockaddr_in address;
//    int addrlen = sizeof(address);
//
//
//
//    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//        perror("socket failed");
//        exit(EXIT_FAILURE);
//    }
//
//    address.sin_family = AF_INET;
//    address.sin_addr.s_addr = INADDR_ANY;
//    address.sin_port = htons(PORT);
//
//
//    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
//        perror("bind failed");
//        exit(EXIT_FAILURE);
//    }
//
//
//    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
//        perror("listen");
//        exit(EXIT_FAILURE);
//    }
//
//
//    while (true) {
//        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
//            perror("accept");
//            exit(EXIT_FAILURE);
//        }
//
//
//        library db;
//        string fout("db.txt");
//        db.addfromfile(fout);
//        string query;
//        char buffer[1024] = { 0 };
//        read(new_socket, buffer, 1024);
//        query = buffer;
//        int result = db.readq(query);
//
//        if (result == 0) {
//
//            ifstream file("to_send.txt");
//            while (file.good()) {
//                file.read(buffer, sizeof(buffer));
//                send(new_socket, buffer, file.gcount(), 0);
//            }
//
//        }
//
//        close(new_socket);
//    }
//    return 0;
//}

