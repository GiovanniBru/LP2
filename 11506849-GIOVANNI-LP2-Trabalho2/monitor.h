#ifndef MONITOR_H
#define MONITOR_H
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <thread>
#include <mutex>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
using namespace std;

class monitor{
	mutex m;
	string logPath;
public:
	monitor(string FilePath);
	void writeLog(string Message);
};

#endif // MONITOR_H
