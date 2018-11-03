//Giovanni Bruno Travassos de Carvalho - 11506849 - Trabalho LP2 - 01/10/2018
#include "requisicao.h"
#include "monitor.h"
#define PORTA 7777
using namespace std;

void ProcessoCliente(int cliente, string ip, monitor* Log);

void ProcessoCliente(int cliente, string ip, monitor* Log){
    char buffer[2048];
    recv(cliente, buffer, sizeof(buffer),0);
    string req = buffer;
    Request x(req);

    printf("[CLIENTE %s] Solicitando:\n%s\n", ip.c_str(), req.c_str());

    Log->writeLog("[CLIENTE " + ip + "] Solicitando:\n" + req + "\n");

    string answer = x.GetAnswer();

    const char *output = answer.c_str();
    send(cliente, output, answer.size(), 0);

    printf("[CLIENTE %s]Recebendo Resposta:\n%s\n", ip.c_str(), answer.c_str());
    Log->writeLog("Cliente " + ip + " Recebido:\n" + answer + "\n");

    printf("[CLIENTE %s] DESCONECTANDO.\n", ip.c_str());
    Log->writeLog("Client " + ip + " desconectando.\n");

    close(cliente);
}


int main(int argc, char *argv[]){
    monitor *Log = new monitor("log.txt");

	int socketfd;
    struct sockaddr_in serv_addr, cli_addr;
    cout << "[SERVIDOR] Usando porta: " << PORTA << ".\n";
    // criando servidor
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd < 0){
        cerr << "[ERRO] Nao foi possivel criar o socket.\n";
        Log->writeLog("[ERRO] Socket nao foi criado, servidor nao foi inicializado!\n");
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORTA);

    if(bind(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        cerr << "[ERRO] Bind nao configurado.\n";
        Log->writeLog("[ERRO]Bind nao configurada, servidor nao inicilizado!\n");
        exit(1);
    }

    listen(socketfd, 0);

    while(1){
        cout << "[SERVIDOR] Esperando por Clientes.\n";
        socklen_t clilen = sizeof(cli_addr);
        int cliente = accept(socketfd, (struct sockaddr *) &cli_addr, &clilen);

        char str[1048];
        inet_ntop(AF_INET, &(cli_addr.sin_addr), str, INET_ADDRSTRLEN);
        string ip = str;

        Log->writeLog("Novo Cliente conectado. [IP: " + ip + "]\n");
        cout << "[SERVIDOR] Cliente " << ip << " conenctado!\n";

        thread ThreadCliente(ProcessoCliente, cliente, ip, Log);
        ThreadCliente.detach();
    }

    delete Log;
    return 0;
}
