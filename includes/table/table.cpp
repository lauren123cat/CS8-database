#include "table.h"

int Table::_serial = 0;

Table::Table() 
{
    // cout << "this shouldn't come up" << endl;
}

Table::Table(const string &name) : _name(name) 
{
    /*
    OPENS AN EXISTING TABLE
    - ctor that opens a table that already exists
    - reads info from the table (it only has field names)
    */

    read_info(); // reads from txt file & updates fieldnames vector

    fstream bin; 
    string binname = _name + ".bin"; 
    open_fileRW(bin, binname.c_str()); // create binary file
    
    FileRecord record;
    int recordcountt = 0;
    long bytes = record.read(bin, recordcountt);
    while (bytes > 0) 
    {
        recordcountt++;
        bytes = record.read(bin, recordcountt);
    }
    bin.close();
    _recordcount = recordcountt;

    // field map! 
    for (int i = 0; i < _fieldnames.size(); i++) 
    {
        _fieldmap.insert(_fieldnames.at(i), i); // [fname, 0] [lname, 1] [age, 2]
    }

    // index structure!
    for (int i = 0; i < _fieldnames.size(); i++) 
    {
        _index.push_back(MMap<string, long>()); // insert empty values into index vector
    }
    reindex();
}

Table::Table(const string &name, const vectorstr &fieldnames) : _name(name), _fieldnames(fieldnames)
{
    cout << "in ctor that makes a table" << endl;
    /*
    - ctor that creates a table out of a vector of fieldnames
    - writes fieldnames into a txt file
    - creates / adds to field map
    - creates index structure (with nothing inside yet)
    */

    // binary!
    fstream bin; 
    string binname = _name + ".bin"; 
    open_fileW(bin, binname.c_str()); 
    bin.close();

    // text!
    fstream text; 
    string textname = _name + "_fields.txt"; 
    text.seekg(0, text.end); // this lets me always write from the last index
    open_fileRW(text, textname.c_str());
    for (int i = 0; i < fieldnames.size(); i++) 
    {
        text << fieldnames.at(i) << endl;
    }
    text.close();

    // field map! 
    // cout << "inserting into field mappppppppppp: " << endl;
    for (int i = 0; i < fieldnames.size(); i++) 
    {
        // cout << fieldnames.at(i) << " at " << i << endl;
        _fieldmap.insert(fieldnames.at(i), i); // [last 0] [first 1] [dep 2] [salary 3] [year 4]
    }

    // index structure!
    for (int i = 0; i < fieldnames.size(); i++) 
    {
        _index.push_back(MMap<string, long>()); // insert empty values into index vector
    }
}

void Table::insert_into(const vectorstr &values)
{
    /*
    - inserts records into the table (that already exists)
    - writes record info into binary file
    - adds recnos to index structure
    */

    fstream bin; 
    _recordcount++;

    // binary!
    string binname = _name + ".bin"; 
    
    open_fileRW(bin, binname.c_str()); // open the file

    // writing into binary file!
    FileRecord record(values);
    bin.seekg(0, bin.end); // this lets me always write from the last index
    long recno = record.write(bin);

    vectorstr_outputter(_fieldnames);

    for (int i = 0; i < values.size(); i++) 
    {
        // cout << "inserting [" << values.at(i) << "] into index's [" << _fieldnames.at(i) << "] at " << i << endl;
        _index.at(i).insert(values.at(i), recno);
    }

    bin.close();
}

Table Table::select_all()
{
    /* 
    - name is alr defined when i'm in this function cause i construct it with the name 
    - this is all so i can update recnos. 
    */
    string filename = _name + ".bin";
    FileRecord r;
    fstream bin;
    open_fileRW(bin, filename.c_str());
    int i = 0;
    long bytes = r.read(bin, i);
    while (bytes > 0)
    {
        _recnos.push_back(i);
        i++;
        bytes = r.read(bin, i);
    }
    bin.close();
    return *this;
}

Table Table::select_columns(const vectorstr &fields)
{
    _serial++;

    Table t(_name + to_string(_serial), fields);
    fstream bin; 
    string binname = _name + ".bin"; 
    open_fileRW(bin, binname.c_str());

    int indexx;
    vector<int> fieldindexes;

    for (int i = 0; i < fields.size(); i++)
    {
        indexx = _fieldmap[fields.at(i)]; // get [] function 
        fieldindexes.push_back(indexx);
    }

    FileRecord r;
    vectorstr row;
    int i = 0;
    long bytes = r.read(bin, i); 
    while (bytes > 0)
    {
        row = r.ivostaftfr(fieldindexes);
        t.insert_into(row);
        _recnos.push_back(i);
        i++;
        bytes = r.read(bin, i);
    }
    bin.close();
    
    t._recnos = _recnos;
    return t;
}

Table Table::select(const vectorstr &fields, const string &field, const string &symbol, const string &condition)
{
    /*
    - makes a subset table based on conditions
    - relationals only
    */

    _serial++;
    Table newtable(_name + to_string(_serial), fields); // you have to add the serial number too so that it doesn't mix conditions up

    vectorl info = select_options(field, symbol, condition); // vector with all the recnos that satisfy conditions

    vector<int> fieldindex;
    int index;
    string temp;
    for (int i = 0; i < fields.size(); i++)
    {
        temp = fields.at(i);
        //cout << "temp: " << temp;
        index = _fieldmap.get(temp); // pass in field name to field map to get index of it in the field map
        fieldindex.push_back(index);
    }

    fstream bin; 
    string binname = _name + ".bin"; 
    open_fileRW(bin, binname.c_str()); 
    FileRecord person;
    vectorstr row;

    for (int i = 0; i < info.size(); i++)
    {
        long recno = info.at(i);
        person.read(bin, recno);
        row = person.ivostaftfr(fieldindex);
        newtable.insert_into(row);
    }

    bin.close();
    _recnos = info;
    return newtable;
}

Table Table::select(const vectorstr &fields, const Queue<Token*> &post)
{
    /*
    - RPN rpn1(post, *this); // passing in queue and table as arguments
    - rpn1.makesolution()
    */
    _serial++;
    Table newtable(_name + to_string(_serial), fields); 
    newtable.set_fields(fields);
    RPN rpn1(post);
    vectorl info = rpn1.make_solution(*this);
    _recnos = info;
  
    fstream bin; 
    string binname = _name + ".bin"; 
    open_fileRW(bin, binname.c_str()); 
    FileRecord person;
    vectorstr row;

    // for ivostaftfr()
    vector<int> fieldindex;
    int index;
    string temp;
    for (int i = 0; i < fields.size(); i++)
    {
        temp = fields.at(i);
        index = _fieldmap.get(temp); // pass in field name to field map to get index of it in the field map
        fieldindex.push_back(index);
    }
    for (int i = 0; i < info.size(); i++)
    {
        long recno = info.at(i);
        person.read(bin, recno);
        row = person.ivostaftfr(fieldindex);
        newtable.insert_into(row);
    }

    bin.close();
    _recnos = info;
    return newtable;
}

Table Table::select(const vectorstr &fields, const vectorstr &strings)
{
    // cout << "in select(fields,strings)!" << endl;
    vector<Token*> newtokens = tokenize(strings);
    Queue<Token*> post = shuntingyard(newtokens);
    return select(fields, post);
}

vector<Token*> Table::tokenize(const vectorstr &conditions)
{
    vector<Token*> tokens;
    for (int i = 0; i < conditions.size(); i++)
    {
        Token* current = new Token(conditions.at(i));
        tokens.push_back(current);
    }
    return tokens;
}

Queue<Token*> Table::shuntingyard(const vector<Token*> &tokens)
{
    cout << "in shuntingyard()" << endl;
    Queue<Token*> answerbox; // has final results
    Stack<Token*> holderbox; // the temporary thing

    for (int i = 0; i < tokens.size(); i++) 
    {
        Token* curr = tokens[i];
        TokenType curr_type = curr->type();

        if (curr_type == TokenType::TokenStr) 
        {
            answerbox.push(curr);
        }
        else if (curr_type == TokenType::LeftParantheses)
        {
            holderbox.push(curr);
        }
        else if (curr_type == TokenType::RightParantheses) 
        {
            while (holderbox.top()->type() != TokenType::LeftParantheses) //keep popping the stuff inside parens
            { 
                Token* top = holderbox.top();
                answerbox.push(top);
                holderbox.pop();

                Token next = *holderbox.top();
            }
            holderbox.pop(); // pop the remaining left parens
        }
        else 
        {
            while (!holderbox.empty() && curr_type <= holderbox.top()->type() && holderbox.top()->type() != TokenType::LeftParantheses) 
            {
                Token* top = holderbox.top();
                answerbox.push(top);
                holderbox.pop(); 
            }
            holderbox.push(curr);
        }
    }
    while (!holderbox.empty()) 
    {
        Token* top = holderbox.top();
        answerbox.push(top);
        holderbox.pop(); 
    }
    // cout << "queue: " << answerbox << endl;
    return answerbox;
}

vectorl Table::select_recnos()
{
    return _recnos;
}

vectorl Table::get_select_recnos()
{
    return _recnos;
}

vectorl Table::select_options(const string &field, const string &symbol, const string &condition)
{
    /*
    - returns a vector<long> of recnos that match conditions
    - using an iterator, go thru multimap of specified field
    - lower bound returns the one
    - upper bound returns the one that stops at the one after)
    - call lower bound
    */

    int indexfield = _fieldmap[field]; // find index of field 

    MMap<string, long>::Iterator begin = _index.at(indexfield).begin();
    MMap<string, long>::Iterator end = _index.at(indexfield).end();
    vectorl thevector;

    for (; begin != end; ++begin) 
    {
        MPair<string, long> temp = *(begin);

        if (symbol == "=" && temp.key == condition) 
        {
            thevector += temp.value_list;
        } 
        else if (symbol == "<" && temp.key < condition) 
        {
            thevector += temp.value_list;
        } 
        else if (symbol == ">" && temp.key > condition) 
        {
            thevector += temp.value_list;
        } 
        else if (symbol == "<=" && temp.key <= condition) 
        {
            thevector += temp.value_list; 
        } 
        else if (symbol == ">=" && temp.key >= condition) 
        {
            thevector += temp.value_list;
        }
    }
    _recnos = thevector;
    return thevector;
}

void Table::set_fields(const vectorstr &fields)
{
    _fieldnames = fields;
}

vectorstr Table::get_fields() // returns vector of field names
{
    return _fieldnames;
}

void Table::reindex()
{
    /*
    - open / read records from the binary file that already exists
    - adds records to index structure
    - makes the output look nice but not necessary for grader
    */

    vector<int> fieldindex;
    int index;

    for (int i = 0; i < _fieldnames.size(); i++)
    {
        index = _fieldmap.get(_fieldnames.at(i)); // pass in field name to field map to get index of it in the field map
        fieldindex.push_back(index);
    }

    fstream bin; 
    string binname = _name + ".bin"; 
    // cout << "binname: " << binname << endl;
    open_fileRW(bin, binname.c_str()); 
    vectorstr row; // 1 record 
    FileRecord person;

    // this is copied and pasted from filerecord (just reading from file)
    long i = 0;
    long bytes = person.read(bin, i); 
    while (bytes > 0)
    {
        row = person.ivostaftfr(fieldindex);
        for (int j = 0; j < row.size(); j++)
        {
            _index.at(j).insert(row.at(j), i);
        }
        i++;
        bytes = person.read(bin, i);
    }
    bin.close();
}

ostream& operator <<(ostream& outs, const Table& to_print)
{
    
    outs << endl << "⋆ ˚｡⋆୨୧˚. Table name: " << to_print._name << ", Record #: " << to_print._recordcount << " ୨୧˚⋆｡˚ ⋆" << endl << endl;
    outs << "record" << "\t" << "\t";
    for (int i = 0; i < to_print._fieldnames.size(); i++)
    {
        outs << to_print._fieldnames.at(i) << "\t" << "\t";
    }
    outs << endl;

    long recno = 0;
    fstream bin;
    FileRecord r;
    string binname = to_print._name + ".bin";
    open_fileRW(bin, binname.c_str());

    long recsize = r.read(bin, recno); 
    for (recno = 1; recsize > 0; recno++) 
    {
        outs << recno - 1 << "\t" << "\t";
        outs << r << endl;
        recsize = r.read(bin, recno);
    }
    outs << endl;
    return outs;
}

bool Table::empty()
{
    return false;
}

void Table::write_info(FileRecord &record)
{
    /*
    - writes field names into a txt file 
    */
    return;
}

void Table::read_info()
{
    /* 
    - reads field names from a txt file
    - updates fields vector 
    */

    // cout << "in readinfo"<< endl;

    string filename = _name + "_fields.txt";
    ifstream text(filename); // <- this is where the file opens! 
    assert(!text.fail());
    string line;
    
    while (getline(text, line)) 
    {
        _fieldnames.push_back(line);
    }
    
    text.close();
}

void Table::print_index()
{
    cout << "printing index structure:" << endl;
    for (int i = 0; i < _index.size(); i++) 
    {
        cout << _fieldnames.at(i) << endl;
        cout << _index.at(i) << endl;
    }
}

void Table::vectorstr_outputter(const vectorstr& vec) 
{
    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec.at(i) << ", ";
    }
    cout << endl;
}

void Table::vectorl_outputter(const vectorl& vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec.at(i) << ", ";
    }
    cout << endl;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

vectorl RPN::make_solution(Table& table)
{
    // there's always gonna be 2 tokenstr's below a relational or logical because he's not gonna break our code
    // the queue is already ordered by using shunting yard
    // popping from postfix queue (the buffer basically) and pushing the results to stack

    cout << "in RPN make_solution()" << endl;
    Token* current;
    Token* token1;
    Token* token2;
    vectorl solutionholder;
    int size = postfix.size();

    for (int i = 0; i < size; i++) 
    {
        current = postfix.pop(); 
        if (current->type() == TokenType::Relational) // relational 
        {
            // I AM ASSUMING THERE WILL BE 2 TOKENSTR tokens in the stack once i see relational
            token1 = stack.pop(); // condition
            token2 = stack.pop(); // fieldname
            solutionholder = table.select_options(token2->token_str(), current->token_str(), token1->token_str()); // change all to strings
            Token* finalone = new Solution(solutionholder);
            stack.push(finalone);
        }
        else if (current->type() == TokenType::LogicalOr || current->type() == TokenType::LogicalAnd) // logical
        {
            // I am assuming there will be 2 solution tokens in the stack once i see logical
            token1 = stack.pop(); 
            token2 = stack.pop(); 
            Solution* sol1 = dynamic_cast<Solution*>(token1); // token* -> solution*
            Solution* sol2 = dynamic_cast<Solution*>(token2);
            vectorl temp;

            if (current->token_str() == "and") 
            {
                temp = _intersect(sol1->get_vectorl(), sol2->get_vectorl());
            }
            else if (current->token_str() == "or")
            {
                temp = _union(sol1->get_vectorl(), sol2->get_vectorl());
            }
            solutionholder = temp;
            cout << endl;
            Token* finalone = new Solution(solutionholder);
            stack.push(finalone);
        }
        else if (current->type() == TokenType::TokenStr) // tokenstr
        {
            stack.push(current);
        }
    }
    while (!stack.empty())
    {
        postfix.push(stack.pop());
    }

    return solutionholder; 
}  

vectorl RPN::_intersect(vectorl vec1, vectorl vec2)
{
    sort(vec1.begin(), vec1.end());
    sort(vec2.begin(), vec2.end());
    vectorl vec3; 

    for (int i = 0; i < vec1.size(); ++i) 
    {
        for (int j = 0; j < vec2.size(); ++j) 
        {
            if (vec1.at(i) == vec2.at(j)) 
            {
                vec3.push_back(vec1[i]);
                break; 
            }
        }
    }
    return vec3;
}

vectorl RPN::_union(vectorl vec1, vectorl vec2)
{
    sort(vec1.begin(), vec1.end());
    sort(vec2.begin(), vec2.end());
    vectorl vec3; 

    for (int i = 0; i < vec1.size(); ++i) // i can't set vec3 = vec1 for some reason
    {
        vec3.push_back(vec1.at(i));
    }

    for (int i = 0; i < vec2.size(); i++) 
    {
        bool bad = false;
        for (int j = 0; j < vec1.size(); j++) 
        {
            if (vec2.at(i) == vec1.at(j)) 
            {
                bad = true;
                break;
            }
        }
        if (!bad) 
        {
            vec3.push_back(vec2.at(i));
        }
    }
    return vec3;
}
