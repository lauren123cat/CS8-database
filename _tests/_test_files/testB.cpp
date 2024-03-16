#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include "../../includes/sql/sql.h"
using namespace std;

bool test_stub(bool debug = false)
{
  const vector<string> command_list = {

    //****************************************************************************
    // CREATE AND INSERT
    //****************************************************************************
    //---- employee table ----------
    "make table employee fields last, first, dep",
    "insert into employee values Blow, Joe, CS",
    "insert into employee values Johnson, \"Jimmy\", Chemistry",
    "insert into employee values Yang, Bo, CS",
    "insert into employee values \"Jackson\", Billy, Math",
    "insert into employee values Johnson, Billy, \"Phys Ed\"",
    "insert into employee values \"Van Gogh\", \"Jim Bob\", \"Phys Ed\"",
    "select * from employee",
    //----- student table ----------
    "make table student fields fname, lname, major, age",
    "insert into student values Flo, Yao, CS, 20",
    "insert into student values \"Flo\", \"Jackson\", Math, 21",
    "insert into student values Calvin, Woo, Physics, 22",
    "insert into student values \"Anna Grace\", \"Del Rio\", CS, 22",
    "select * from student",
    //****************************************************************************
    // SIMPLE SELECT
    //****************************************************************************
    "select * from student",
    //------- simple strings -------------------
    "select * from student where lname = Jackson",
    //----- quoted strings ---------------------
    "select * from student where lname = \"Del Rio\"",
    //-------- non-existing string ------------------
    "select * from student where lname = \"Does Not Exist\"",
    //****************************************************************************
    // RELATIONAL OPERATORS:
    //****************************************************************************
    //.................
    //:Greater Than :
    //.................
    "select * from student where lname > Yang",
    //. . . . . . . . . . . . . (Greater Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
    "select * from student where age > 50",
    //. . . . . . . . . . . . . (Greater than: last item) . . . . . . . . . . . . . . . . . .
    "select * from student where age > 53",
    //. . . . . . . . . . . . . (Greater Than: Passed last item) . . . . . . . . . . . . . . . . . . . . .
    "select * from student where age > 54",
    //.................
    //:Greater Equal :
    //.................
    "select * from student where lname >= Yang",
    //. . . . . . (Greater Equal non-existing: ) . . . . . . . . . . .
    "select * from employee where last >= Jack",
    //.................
    //:Less Than :
    //.................
    //. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
    "select * from student where lname < Jackson",
    //. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . . . . . . . .
    "select * from student where age < 20",
    //. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . . . . . . . . . . . . . .
    "select * from student where age < 19",
    //.................
    //:Less Equal :
    //.................
    "select * from student where lname <= Smith",
    //. . . . . . (Less Equal non-existing: ) . . . . . . . . . . .
    "select * from employee where last <= Peach",
    //****************************************************************************
    // LOGICAL OPERATORS
    //****************************************************************************
    //.................
    //:AND :
    //.................
    "select * from student where fname = \"Flo\" and lname = \"Yao\"",
    //.................
    //:OR :
    //.................
    "select * from student where fname = Flo or lname = Jackson",
    //.................
    //:OR AND :
    //.................
    "select * from student where fname = Flo or major = CS and age <= 23",
    //.................
    //:AND OR AND :
    //.................
    "select * from student where age <30 and major=CS or major = Physics and lname = Jackson",
    //.................
    //:OR AND OR :
    //.................
    "select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson"
  };
  const int MAKE_TABLE_COMMANDS = 34;
  SQL sql;
  Table t;
     for (int i = 0; i < MAKE_TABLE_COMMANDS; i++)
     {
          cout << ">" << command_list[i] << endl;
          sql.command(command_list[i]);
          cout << endl;
     }

     cout << endl << endl;
     for (int i = MAKE_TABLE_COMMANDS; i < command_list.size(); i++)
     {
          cout << "\n>" << command_list[i] << endl;
          if (debug)
               cout<< sql.command(command_list[i])<<endl;
          else
               t = sql.command(command_list[i]);
          cout << "FINAL ONE: " << command_list[i] << endl << t << endl;
          cout << "basic_test: records selected: "<<sql.select_recnos() << endl;
     }
  return true;
}


// /*00*/     "make table employee fields  last,       first,         dep,      salary, year",
// /*01*/     "insert into employee values Blow,       Joe,           CS,       100000, 2018",
// /*02*/     "insert into employee values Blow,       JoAnn,         Physics,  200000, 2016",
// /*03*/     "insert into employee values Johnson,    Jack,          HR,       150000, 2014",
// /*04*/     "insert into employee values Johnson,    \"Jimmy\",     Chemistry,140000, 2018",

// /*05*/     "make table student fields  fname,          lname,    major,    age",
// /*06*/     "insert into student values Flo,            Yao, 	Art, 	20",
// /*07*/     "insert into student values Bo, 		     Yang, 	CS, 		28",
// /*08*/     "insert into student values \"Sammuel L.\", Jackson, 	CS, 		40",
// /*09*/     "insert into student values \"Billy\",	     Jackson, 	Math,	27",
// /*10*/     "insert into student values \"Mary Ann\",   Davis,	Math,	30",

// /*11*/     "select * from employee",
// /*12*/     "select last, first, age from employee",
// /*13*/     "select last from employee",
// /*14*/     "select * from employee where last = Johnson",
// /*15*/     "select * from employee where last=Blow and major=\"JoAnn\"",

// /*16*/     "select * from student",
// /*17*/     "select * from student where (major=CS or major=Art)",
// /*18*/     "select * from student where lname>J",
// /*19*/     "select * from student where lname>J and (major=CS or major=Art)"

bool test_sql(bool debug = false)
{
  const vector<string> sanrio_list = {
    // CREATE AND INSERT
    //---- sanrio table ----------
    "make table sanrio fields name, species, color, origin_year",
    "insert into sanrio values \"Hello Kitty\", Cat, White, 1974",
    "insert into sanrio values Keroppi, Frog, Green, 1988",
    "insert into sanrio values Pompompurin, Dog, Yellow, 1996",
    "insert into sanrio values \"My Melody\", Rabbit, Pink, 1975",
    "insert into sanrio values Gudetama, Egg, Yellow, 2013",
    //----- merch table ----------
    "make table merch fields item, character, category, price",
    "insert into merch values Plushie, Kitty, Toy, 25",
    "insert into merch values Keychain, Pompompurin, Accessory, 10",
    "insert into merch values Notebook, Keroppi, Stationery, 8",
    "insert into merch values Bag, \"My Melody\", Fashion, 40",
    "insert into merch values Mug, Gudetama, Kitchenware, 15",

    // SIMPLE SELECT
    "select * from sanrio",
    "select * from sanrio where species = Cat",
    "select * from sanrio where name = \"My Melody\"",
    "select * from merch where category = Toy",

    // RELATIONAL OPERATORS
    // Greater Than
    "select * from sanrio where origin_year > 1990",
    // Greater Equal
    "select * from merch where price >= 20",
    // Less Than
    "select * from sanrio where origin_year < 1980",
    // Less Equal
    "select * from merch where price <= 12",

    // LOGICAL OPERATORS
    // AND
    "select * from merch where category = Toy and price = 25",
    // OR
    "select * from sanrio where species = Frog or color = Pink",
    // OR AND
    "select * from merch where category = Accessory or price < 20 and character = Gudetama",
    // AND OR AND
    "select * from sanrio where color = Yellow and species = Dog or species = Egg and name = Gudetama",
    // OR AND OR
    "select * from merch where item = Bag or character = Keroppi and price < 30 or category = Kitchenware"
  };

    const int TABLE_COMMANDS = 34;
    SQL sanriosql;
    Table t;
  
    sanriosql.command(sanrio_list[0]);

    for (int i = 0; i < sanrio_list.size(); i++)
    {
      sanriosql.command(sanrio_list[i]);
    }

    for(int i = 0; i < sanrio_list.size(); i++)
    {
      sanriosql.command(sanrio_list[i]); 
    }

     cout << "----- END TEST --------" << endl;


  return true;
}

bool test_specific(bool debug){
  if(debug){
    SQL sql; 
    SQL sql2; 
    const vector<string> command_list = {

    /*00*/     "make table employee fields  last,       first,         dep,      salary, year",
    /*01*/     "insert into employee values Blow,       Joe,           CS,       100000, 2018",
    /*02*/     "insert into employee values Blow,       JoAnn,         Physics,  200000, 2016",
    /*03*/     "insert into employee values Johnson,    Jack,          HR,       150000, 2014",
    /*04*/     "insert into employee values Johnson,    \"Jimmy\",     Chemistry,140000, 2018"}; 

    const vector<string> command_list2 = {
      /*05*/     "make table student fields  fname,          lname,    major,    age",
    /*06*/     "insert into student values Flo,            Yao, 	Art, 	20",
    /*07*/     "insert into student values Bo, 		     Yang, 	CS, 		28",
    /*08*/     "insert into student values \"Sammuel L.\", Jackson, 	CS, 		40",
    /*09*/     "insert into student values \"Billy\",	     Jackson, 	Math,	27",
    /*10*/     "insert into student values \"Mary Ann\",   Davis,	Math,	30"
    }; 

    cout<<"specific test\n"; 
     Table t;
     cout << ">" << command_list[0] << endl;
     sql.command(command_list[0]);
     cout << "basic_test: table created." << endl<<endl;

     for (int i = 0; i < command_list.size(); i++)
     {
          //cout << ">" << command_list[i] << endl;
          sql.command(command_list[i]);
          //cout << endl;
     }

     for(int i = 0; i < command_list2.size(); i++){
      sql2.command(command_list2[i]); 
     }

     sql2.command("select * from student where age <30 and major=CS or major = Physics and lname = Jackson"); // <<endl; 
  }
  return true; 
}

// TEST(TEST_STUB, TestStub) {
//   EXPECT_EQ(1, test_stub(true));
// }

TEST(TEST_SQL, TestSql) {
  EXPECT_EQ(1, test_sql(true));
}

// TEST(TEST_SPECIFIC, testSpecific){
//   EXPECT_EQ(1, test_specific(true)); 
// }





int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

