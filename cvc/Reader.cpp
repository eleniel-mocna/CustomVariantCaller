#include "Reader.h";
#include <string>;
#include <fstream>;
#include <iostream>;
#include <vector>;
#include "Read.h";
using namespace std;

char HEADER_CHAR = '@';

Reader::Reader(string file_name)
{
	myReadFile.open(file_name);
	open = true;
	line_index = 0;
	skipHeader();
	cout << "Opened file " << file_name << '\n';

}
Reader::~Reader()
{
	myReadFile.close();
}
string Reader::getLine()
{
	/* Return the next line or empty string if there are no lines to read.*/
	if (open)
	{
		lastLine = currentLine;
		if (!getline(myReadFile, currentLine, '\n'))
		{
			open = false;
		}

		++line_index;
		return lastLine;
	}
	else
	{
		return "";
	}

}
Read Reader::getRead()
{
	string qname;
	size_t flag;
	string rname;
	unsigned int pos;
	size_t mapq;
	string cigar;
	string rnext;
	unsigned int pnext;
	unsigned int tlen;
	string seq;
	string qual;
	vector<string> splitted = splitString(getLine(), '\t');

	/*return Read(splitted[0],
				splitted[0])*/
}
vector<string> Reader::splitString(string input, char delimeter)
{
	vector<string> ret;
	string current = "";
	for (size_t i = 0; i < input.length(); i++)
	{
		if (input[i]==delimeter)
		{
			ret.push_back(current);
			current = "";
		}
		else
		{
			current.push_back(input[i]);
		}
	}
	return ret;
}

void Reader::skipHeader()
{
	do
	{
		getline(myReadFile, currentLine);
		cout << currentLine << "HEADER" << '\n';
	} while (currentLine[0] == HEADER_CHAR);
}
