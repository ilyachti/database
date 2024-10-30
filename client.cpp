//client.cpp   клиент


#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <vector>
#include <sys/select.h>


#define PORT 3556

int main() {
    
    struct sockaddr_in serv_addr;
    int valread;
    char buffer[1024] = { 0 };


    std::cout << "Enter the number of the query file (1, 2, or 3): ";
    int file_number;
    std::cin >> file_number;
    while (1) {

    std::string query_filename = "query" + std::to_string(file_number) + ".txt";
    std::ifstream query_file(query_filename);
    

        if (!query_file.is_open()) {
            std::cerr << "Failed to open query file" << std::endl;
            return -1;
        }

        std::string query;
        std::vector<std::string> query_list;
        while (query_file.good()) {
            std::getline(query_file, query);
            query_list.push_back(query);
        }

        fd_set readfds;

        for (const auto& query : query_list) {
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                std::cerr << "Socket creation error" << std::endl;
                return -1;
            }

            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(PORT);

            if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
                std::cerr << "Invalid address/ Address not supported" << std::endl;
                return -1;
            }

            if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
                std::cerr << "Connection Failed" << std::endl;
                return -1;
            }

            send(sock, query.c_str(), query.length(), 0);
            std::cout << "Query sent: " << query << std::endl;

            FD_ZERO(&readfds);
            FD_SET(sock, &readfds);

            int activity = select(sock + 1, &readfds, NULL, NULL, NULL);
            if ((activity < 0) && (errno != EINTR)) {
                printf("select error");
            }

            if (FD_ISSET(sock, &readfds)) {
                std::string filename("received" + std::to_string(file_number) + ".txt");
                std::ofstream outfile(filename);

                while ((valread = read(sock, buffer, 5)) > 0) {
                    outfile.write(buffer, valread);
                }

                if (valread < 0) {
                    std::cerr << "Error reading from socket" << std::endl;
                }
            }

            close(sock);

           
            sleep(1);
           
        }
    }

    return 0;
}

























//#include <iostream>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <unistd.h>
//#include <cstring>
//#include <fstream>
//
//
//#define PORT 8091
//int main() {
//    int sock = 0, valread;
//    struct sockaddr_in serv_addr;
//
//    char buffer[1024] = { 0 };
//
//    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//        std::cerr << "Socket creation error" << std::endl;
//        return -1;
//    }
//
//    serv_addr.sin_family = AF_INET;
//    serv_addr.sin_port = htons(PORT);
//
//    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
//        std::cerr << "Invalid address/ Address not supported" << std::endl;
//        return -1;
//    }
//
//    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
//        std::cerr << "Connection Failed" << std::endl;
//        return -1;
//    }
//
//    std::ifstream query_file("query.txt");
//    if (!query_file.is_open()) {
//        std::cerr << "Failed to open query file" << std::endl;
//        return -1;
//    }
//
//    std::string query;
//    std::getline(query_file, query);
//
//    send(sock, query.c_str(), query.length(), 0);
//    std::cout << "Query sent" << std::endl;
//
//    std::string filename("received.txt");
//    std::ofstream outfile(filename);
//
//    while ((valread = read(sock, buffer, 1024)) > 0) {
//        outfile.write(buffer, valread);
//    }
//
//    if (valread < 0) {
//        std::cerr << "Error reading from socket" << std::endl;
//    }
//
//    close(sock);
//
//    return 0;
//}


































//#include <stdio.h>
//#include <string.h>
//#include <errno.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <iostream>
//
//#define  SERVER_PORT     5555
//#define  SERVER_HOST    "127.0.0.1"
//#define  BUFLEN          4096
//
//int  write_to_server(int fd);
//void write_to_serverLong(int fd);
//int  read_from_server(int fd);
//
//
//int main(int argc, char* argv[]) {
//    // if (argc == 1) {
//    //     std::cout << "need filename for requests\n";
//    //     return;
//    // }
//
//    // std::string fn = 
//
//    int err;
//    int sock;
//    struct sockaddr_in server_addr;
//    struct hostent* hostinfo;
//
//    hostinfo = gethostbyname(SERVER_HOST);
//    if (hostinfo == NULL) {
//        fprintf(stderr, "unknown host %s.\n", SERVER_HOST);
//        exit(EXIT_FAILURE);
//    }
//
//    server_addr.sin_family = PF_INET;
//    server_addr.sin_port = htons(SERVER_PORT);
//    server_addr.sin_addr = *(struct in_addr*)hostinfo->h_addr;
//
//    sock = socket(PF_INET, SOCK_STREAM, 0);
//    if (sock < 0) {
//        perror("client: socket was not created");
//        exit(EXIT_FAILURE);
//    }
//
//    err = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
//    if (err < 0) {
//        perror("client: connect failure");
//        exit(EXIT_FAILURE);
//    }
//    fprintf(stdout, "connection is ready\n");
//
//    while (1) {
//        fprintf(stdout, "enter sql query: ");
//        //if (write_to_server(sock) < 0) break;
//
//        for (int i = 0; i < 10; ++i) {
//            fprintf(stdout, "query send: ");
//            write_to_serverLong(sock);
//            if (read_from_server(sock) < 0) break;
//            sleep(1);
//        }
//    }
//
//    fprintf(stdout, "the end of life :(\n");
//
//    close(sock);
//    exit(EXIT_SUCCESS);
//}
//
//
//
//int write_to_server(int fd) {
//
//    // char buf[] = "select id from maps where id < 10 or id > 30";
//
//    // nbytes = write(fd, buf, strlen(buf) + 1);
//    // if (nbytes < 0) { perror("write"); return -1; }
//    // if (strstr(buf, "stop")) return -1;
//
//    int nbytes;
//    char buf[BUFLEN];
//
//    if (fgets(buf, BUFLEN, stdin) == nullptr) {
//        printf("error\n");
//    }
//
//    buf[strlen(buf) - 1] = 0;
//
//    nbytes = write(fd, buf, strlen(buf) + 1);
//    if (nbytes < 0) { perror("write"); return -1; }
//    if (strstr(buf, "stop")) return -1;
//
//    return 0;
//}
//
//void write_to_serverLong(int fd) {
//
//    int nbytes;
//    char buf[] = "select id from maps where id < 10 or id > 30";
//    fprintf(stdout, "%s", buf);
//    nbytes = write(fd, buf, strlen(buf) + 1);
//
//}
//
//
//int read_from_server(int fd) {
//    uint32_t size;
//    int nbytes;
//    nbytes = read(fd, &size, sizeof(uint32_t));
//
//    if (nbytes < 0) {
//        perror("read size");
//        return -1;
//    }
//    if (nbytes == 0) {
//        std::cout << "server closed connection\n";
//        return -1;
//    }
//
//    size = ntohl(size);
//
//    char buf[size + 1];
//
//    nbytes = read(fd, buf, size);
//
//    if (nbytes < 0) {
//        perror("read data");
//        return -1;
//    }
//    if (nbytes == 0) {
//        std::cout << "server closed connection\n";
//        return -1;
//    }
//
//    buf[nbytes] = '\0';
//
//    std::cout << "\nserver reply: " << buf << "\n";
//    return 0;
//}







//#include <iostream>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <unistd.h>
//#include <cstring>
//#include <fstream>
//
//
//#define PORT 8090
//
//int main() {
//    int sock = 0, valread;
//    struct sockaddr_in serv_addr;
//
//
//
//    char buffer[1024] = { 0 };
//
//
//    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//        std::cerr << "Socket creation error" << std::endl;
//        return -1;
//    }
//
//    serv_addr.sin_family = AF_INET;
//    serv_addr.sin_port = htons(PORT);
//
//
//    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
//        std::cerr << "Invalid address/ Address not supported" << std::endl;
//        return -1;
//    }
//
//
//    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
//        std::cerr << "Connection Failed" << std::endl;
//        return -1;
//    }
//
//    std::ifstream query_file("query.txt");
//    if (!query_file.is_open()) {
//        std::cerr << "Failed to open query file" << std::endl;
//        return -1;
//    }
//
//
//    std::string query;
//    std::getline(query_file, query);
//
//
//    send(sock, query.c_str(), query.length(), 0);
//    std::cout << "Query sent" << std::endl;
//
//    std::string filename("received.txt");
//    std::ofstream outfile(filename);
//
//    while ((valread = read(sock, buffer, 1024)) > 0) {
//        outfile.write(buffer, valread);
//    }
//
//    if (valread < 0) {
//        std::cerr << "Error reading from socket" << std::endl;
//    }
//
//    close(sock);
//
//    return 0;
//}
