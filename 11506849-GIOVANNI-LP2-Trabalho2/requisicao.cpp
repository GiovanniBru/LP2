#include "requisicao.h"

Request::Request(string FullRequest)
{
	string line = "";
	string cmd = "";
	string file = "";
	string type = "";


	for(unsigned int i = 0; FullRequest[i] != '\n'; i++) // Get first line of the complete request
		line += FullRequest[i];

	unsigned int i = 0;
	for(; line[i] != ' '; i++) // Get the command, first element of the request's first line
		cmd += line[i];

	i += 2;
	for(; line[i] != ' '; i++) // Get the file name, second element of the request's first line
		file += line[i];

	this->Command = cmd;
	this->File = file;


	// Now preparing to get file's format/type

	reverse(file.begin(), file.end()); // Reversing file name to read backwards

	for(unsigned int j = 0; file[j] != '.'; j++) // Read until find a dot
		type += file[j];

	reverse(type.begin(), type.end()); // Reversing the result should give the file format/type

	this->FileType = type;
}

bool Request::CheckCmd() // Returns true if the command is valid
{
	if(this->Command == "GET")
		return true;
	else
		return false;
}

bool Request::CheckFile()  // Returns false if the file does not exist
{
	string filePath = "files/" + this->File;
	ifstream input(filePath.c_str());
	if(!input)
		return false;
	else
		return true;
}

string Request::getTime()
{
	const string WeekDay[]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	const string MonthName[]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	time_t Time = time(NULL);
	struct tm *aTime = localtime(&Time);

	int day = aTime->tm_mday;
	int wday = aTime->tm_wday;
	int month = aTime->tm_mon;
	int year = aTime->tm_year + 1900;
	int hour = aTime->tm_hour;
	int min = aTime->tm_min;
	int sec = aTime->tm_sec;

	return WeekDay[wday] + ", " + to_string(day) + " " + MonthName[month] + " " + to_string(year) + " " + to_string(hour) + ":" + to_string(min) + ":" + to_string(sec) + " BRT";
}

string Request::getType()
{
	if(this->FileType == "html")
		return "text/html; charset=utf-8";
	else if(this->FileType == "jpg")
		return "image/jpeg";
}

string Request::getContent()
{
	ifstream input;
	string fPath = "files/" + this->File;
	string line, fBuffer = "";

	input.open(fPath, std::ios::binary);
	while(getline(input, line))
		fBuffer += line + "\n";
	input.close();

	return fBuffer;
}

string Request::GetAnswer()
{
    string ret = "";
    string Content;

	if(this->CheckCmd()){
    	if(this->CheckFile()){
    		Content = this->getContent();

    		ret = "HTTP/1.1 200 OK\n"
    			  "Date: " + this->getTime() + "\n"
    			  "Server: DeepShadow/1.0.2\n"
				  "Content-Length: " + to_string(Content.size()) + "\n"
				  "Content-Type: " + this->getType() + "\n"
				  "Connection: Closed\n\n" + Content;

    	}
    	else{
    		Content = "<!DOCTYPE html>\n"
  					  "<html>\n"
 					  "<head>\n"
 					  "<title>Not Found</title>\n"
 					  "</head>\n"
 					  "<body>\n"
 					  "<H1><b>ERROR 404: NOT FOUND</b></H1>\n"
					  "</body>\n"
					  "</html>\n";

    		ret = "HTTP/1.1 404 Not Found\n"
      			  "Date: " + this->getTime() + "\n"
      			  "Server: DeepShadow/1.0.2\n"
				  "Content-Length: " + to_string(Content.size()) + "\n"
  				  "Content-Type: text/html; charset=iso-8859-1\n"
				  "Connection: Closed\n\n" + Content;
    	}
    }
	else{
		Content = "<!DOCTYPE html>\n"
				  "<html>\n"
				  "<head>\n"
				  "<title>Bad Request</title>\n"
				  "</head>\n"
				  "<body>\n"
				  "<H1><b>ERROR 400: BAD REQUEST</b></H1>\n"
				  "</body>\n"
				  "</html>\n";

		ret = "HTTP/1.1 400 Bad Request\n"
    	      "Date: " + this->getTime() + "\n"
    	      "Server: DeepShadow/1.0.2\n"
			  "Content-Length: " + to_string(Content.size()) + "\n"
    	  	  "Content-Type: text/html; charset=iso-8859-1\n"
			  "Connection: Closed\n\n" + Content;
	}

	return ret;
}
