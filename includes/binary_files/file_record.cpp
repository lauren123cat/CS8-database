#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

#include "file_record.h"

long FileRecord::write(fstream &outs)
{
    // r.write(f); //take the Record r and write it into file f
    //  and return the record number of this Record

    // write to the end of the file.
    outs.seekg(0, outs.end);

    long pos = outs.tellp(); //retrieves the current position of the file pointer

    outs.write(&_record[0][0], sizeof(_record));

    return pos / sizeof(_record); //record number
}

long FileRecord::read(fstream &ins, long recno)
{
    /* summary:
    - reading a file that is already open
    - recno = record number
    - pos = address of where we need to be
    - start at recno and read the size of 2d array amount 
    */
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos = recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    //ins.read(&_record[0], sizeof(_record))

    ins.read(&_record[0][0], sizeof(_record)); // (char*)

    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD: 
    // _record[ins.gcount()] = '\0';
    return ins.gcount();
}
ostream& operator<<(ostream& outs, const FileRecord& r)
{
    for (int i = 0; i < r.MAX + 1 || r._record[i][0] == '\0'; i++) 
    {
        outs << r._record[i] << "\t" << "\t";
    }
    return outs;
}

void FileRecord::set2null()
{
    for (int i = 0; i < MAX + 1; i++)
    {
        for (int j = 0; j < MAX + 1; j++)
        {
            _record[i][j] = '\0';
        }
    }
}

vector<string> FileRecord::ivostaftfr(vector<int> indexes)
/* passing in an empty vector, adding the recnos from the file to the vector
setting the vector to row, which is one person's record
*/
{
    vector<string> luis_barkeshli;
    for (int i = 0; i < indexes.size(); i++)
    {
        luis_barkeshli.push_back(_record[indexes[i]]); 
    }
    return luis_barkeshli;
}