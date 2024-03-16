#include "sql.h"

SQL::SQL()
{
    cout << "in sql ctor!" << endl;
}


// the big function. takes in the big big command line.
// sends string to parser and gets the parse tree (mmap)
// calls run_command to execute query
Table SQL::command(const string& str)
{
    cout << "input command: " << str << endl;
    cout << endl << endl;
    char s[300];
    strcpy(s, str.c_str());
    Parser parse(s);
    mmap_ss ptree = parse.parse_tree();
    
    string var1 = "table_name";
    string var2 = "condition";
    string var3 = "fields";
    string var4 = "command";
    string var5 = "cols";
    string var6 = "values";
    cout << "ptree lookup: " << endl;
    ptree.print_lookup();
    
    // cout<<"map: "<<endl; 
    // cout<<ptree<<endl; 

    assert(ptree.contains(var4));
    vectorstr command = ptree.get(var4);
    
    if (command.at(0) == "select")
    {
        cout << "selecting from table: " << endl;
        
        assert(ptree.contains(var1)); // table name
        vectorstr tablename = ptree.get(var1); 
        Table tadpole(tablename.at(0)); // go into existing table

        cout << "existing table looks like: " << tadpole << endl;

        assert(ptree.contains(var3)); // fields
        vectorstr fields = ptree.get(var3);
        
        if (ptree.contains(var2)) // conditions
        {
            vectorstr conditions = ptree.get(var2);
            if (fields.at(0) == "*")
            {
                fields = tadpole.get_fields(); // * -> field names
            }
            cout << "normal select" << endl;
            _table = tadpole.select(fields, conditions); // normal select
            _srecnos = tadpole._recnos;
            cout << endl << "!!!!! final one: " << str << endl << _table << endl;
            return _table;
        }
        else // no conditions (done)
        {
            cout << "no conditions" << endl;
            if (fields.at(0) == "*")
            {
                // cout << "field is a star" << endl;
                _table = tadpole.select_all();
                _srecnos = tadpole._recnos;
                cout << endl << "!!!!! final one: " << str << endl << _table << endl;
                return _table;
            }
            else 
            {
                cout << "select columns" << endl;
                _table = tadpole.select_columns(fields);
                _srecnos = tadpole._recnos;
                cout << endl << "!!!!! final one: " << str << endl << _table << endl;
                return _table;
            }
        }
    }
    else if (command.at(0) == "make" || command.at(0) == "create")
    {
        cout << "MAKING A NEW TABLE!: " << command.at(0) << endl;
        assert(ptree.contains(var1)); // table name
        vectorstr tablename = ptree.get(var1); 

        if (ptree.contains(var5)) // cols
        {
            vectorstr cols = ptree.get(var5);
            Table tadpole(tablename.at(0), cols);
            cout << _table << endl;
            _table = tadpole;
            return _table;
        }
        else 
        {
            cout << "there are no columns, this should not come up" << endl;
            Table tadpole(tablename.at(0));
            _table = tadpole;
            cout << _table << endl;
            return _table;
        }
    }
    else if (command.at(0) == "insert")
    {
        cout << "inserting into a table: " << endl;

        assert(ptree.contains(var1)); // table name
        vectorstr tablename = ptree.get(var1); 
        Table tadpole(tablename.at(0)); // go into existing table

        assert(ptree.contains(var6)); // values
        vectorstr values = ptree.get(var6); 

        cout << "table_name: " << tablename.at(0) << endl;
       
        tadpole.insert_into(values);
        _table = tadpole;
        // cout << "after insert: " << endl << _table << endl;
        return _table;
    }
    return _table;
}

vectorl SQL::select_recnos()
{
    return _srecnos;
}