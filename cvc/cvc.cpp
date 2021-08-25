#include <string>
#include <fstream>
#include <iostream>
#include "Reader.h"


using namespace std;

int main()
{
    string line;
    Reader fileReader("Text.txt");
    for (size_t i = 0; i < 20; i++)
    {
        cout << i;
        line = fileReader.getLine();
        cout << line << '\n';

    }
    return 0;
}