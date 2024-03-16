#ifndef FILERECORD_H
#define FILERECORD_H
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>

using namespace std;

class FileRecord
{
public:
    //when you construct a Record, it's either empty or it
    //  contains a word
    FileRecord()
    {
        set2null();
        _record[0][0] = '\0';
    }

    FileRecord(vector<string> stringvec)
    {
        set2null();
        for (int i = 0; i < stringvec.size(); i++)
        {
            strncpy(_record[i], stringvec[i].c_str(), MAX);
        }
    }

    FileRecord(string s)
    {
        set2null();
        strncpy(_record[0], s.c_str(), MAX);
    }

    long write(fstream& outs);              //returns the record number
    long read(fstream& ins, long recno);    //returns the number of bytes 
                                            //      read = MAX, or zero if
                                            //      read passed the end of file

    vector<string> ivostaftfr(vector<int> indexes); // insert vector of strings that are from the file record

    friend ostream& operator<<(ostream& outs, const FileRecord& r);
    static const int MAX = 100;
    char _record[MAX + 1][MAX + 1];

private:
    int recno = -1; // index in the file
    void set2null();
};

#endif