#ifndef REQUISICAO_H
#define REQUISICAO_H

#include "monitor.h"

class requisicao
{
private:
	string Command;
	string File;
	string FileType;

	bool CheckCmd();
	bool CheckFile();
	string getTime();
	string getType();
	string getContent();
public:
	Request(string FullRequest);
	string GetAnswer();
};

#endif // REQUISICAO_H
