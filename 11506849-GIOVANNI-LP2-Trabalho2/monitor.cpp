#include "monitor.h"

monitor::monitor(string FilePath)
{
	this->logPath = FilePath;
}

void monitor::writeLog(string Message)
{
	std::ofstream output;

	this->m.lock();  // Entering critic session

	output.open(this->logPath.c_str(), std::ios::out|std::ios::app);
	output << Message;
	output.close();

	this->m.unlock();  // Exit critic session
}
