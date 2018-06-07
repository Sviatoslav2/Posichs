#include <iostream>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <netdb.h>

#include <arpa/inet.h>

#include <vector>



void ERROR(const std::string &merror){

    std::string message("[client] ERROR "+merror);
    std::cout<<message.c_str()<<std::endl;
    exit(1);

}

void login(const std::string &m){
    printf("[client] %s \n", m.c_str());
}

bool check_ops(const std::string &ops, const std::vector<std::string> Vector){
    bool key = false;
    for (int i = 0; i < Vector.size(); i++){
        key = ops.compare(Vector.at((unsigned)i))==0;
        if(key){
            return key;
        }
    }
    return key;
}



void CorectorOfConsol(int position,char *simbols[]){
    std::vector <std::string> ops = {"d", "t","h", "m" };
    if (position < 3) {

        ERROR("requires hostname and command");

    }

    if(!check_ops(simbols[2], ops)){
        fprintf(stderr, "[client] ERROR, undefined command\n");
        exit(1);
    }
};

void Connect_to_the_server(struct sockaddr_in serv_addr,int sockfd){
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        ERROR("connecting to server");
    else
        login("Has connected to server");
}

void Send_mesege_to_server(int FD,char buf[]){
    if (write(FD, buf, strlen(buf)) < 0) {
        ERROR("writing to socket...");
    }
}




void Read_server_response(int FD,char buf[]){
    if (read(FD, buf, 255) < 0) {
        ERROR("reading from socket");
    }
}


struct hostent *IsHostExist(char *ch){
    auto server = gethostbyname(ch);
    if (server == NULL) {
        ERROR("no such host");
    }
    return server;
}

int main(int argc, char *argv[]) {

    struct sockaddr_in serv_addr;

    char buf[256];
    CorectorOfConsol(argc,argv);

    int portNumber = 5002;

    
    auto FD = socket(AF_INET, SOCK_STREAM, 0);



    if (FD < 0) {

        ERROR("opening socket");

    }



    auto server = IsHostExist(argv[1]);



    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, (size_t)server->h_length);

    serv_addr.sin_port = htons(portNumber);


    Connect_to_the_server(serv_addr,FD);




    bzero(buf,256);




    strcpy(buf, argv[2]);



    for(int i = 3; i < argc; i++){
        strcat(buf, " ");
        strcat (buf, argv[i]);
    }

    Send_mesege_to_server(FD,buf);
    bzero(buf,256);
    Read_server_response(FD,buf);
    printf("[server] %s\n", buf);
    return 0;
}