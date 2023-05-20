# include <iostream>
# include <cstring>
# include <string> 
# include <vector>
# include <cstdio>
# include <cstdlib>
# include <iomanip>
# include <stdexcept>
# include <cmath>
# include <map>
# include <iomanip>
# include <sstream>
# include <cctype>
# include <exception>
# include <string.h>

using namespace std;

struct Information // �C��token����� 
{
  string token_name ;
  string token_type ; // id(identifier) con(constant) sign
};

struct Reg
{
  string token_name ;
  string def_type ; 
  bool is_function ;

  vector<Information> token_data ; // �s��id function���@����� 
};

int gLine ; // ��������e���ĴX��
bool glast_is_error ; // �W�謰���~�T�� gline = 1�}�l 

class Exception // �]�w���~�T�� ���U�ؿ��~�~�� 
{
public:
  string mError_Message;
  Exception() {  }

  Exception( string str ) {
    mError_Message = str;
  } // Exception()
}; //  class Expception

class Lexical_Error : public Exception // ���k���~ (�ڥ��S�o�r 
{ 
  
public:

  Lexical_Error( string token ) {

    stringstream ss;
    ss << gLine;
    string line = ss.str();
    mError_Message = "> Line " + line + " : " + mError_Message +
      "unrecognized token with first char : '" + token + "'\n";

  } // Lexical_Error()

  Lexical_Error( char token ) { 
    stringstream ss;
    ss << gLine;
    string line = ss.str();
    mError_Message = "> Line " + line + " : " + mError_Message +
      "unrecognized token with first char : '" + token + "'\n";

  } // Lexical_Error()


};

class Syntactic_Error : public Exception // �y�k���~
{   

public:

  Syntactic_Error( string token ) {

    stringstream ss;
    ss << gLine;
    string line = ss.str();
    mError_Message = "> Line " + line + " : " + mError_Message +
      "unexpected token : '" + token + "'\n";

  } // Syntactic_Error()

  Syntactic_Error( char token ) {

    stringstream ss;
    ss << gLine;
    string line = ss.str();
    mError_Message = "> Line " + line + " : " + mError_Message +
      "unexpected token : '" + token + "'\n";

  } // Syntactic_Error()

};

class Semantic_error : public Exception // �y�q���~ (���w�q 
{ 

public:

  Semantic_error( string token ) {

    stringstream ss;
    ss << gLine;
    string line = ss.str();
    mError_Message = "> Line " + line + " : " + mError_Message +
      "undefined identifier : '" + token + "'\n";

  } // Semantic_error()

  Semantic_error( char token ) {

    stringstream ss;
    ss << gLine;
    string line = ss.str();
    mError_Message = "> Line " + line + " : " + mError_Message +
      "undefined identifier : '" + token + "'\n";

  } // Semantic_error()

};

class Divid_Zero : public Exception // ��0 
{  

public:

  Divid_Zero() {
    mError_Message = "> Error\n";
  } // Divid_Zero()

};

class Done : public Exception // �������� 
{ 

public:

  Done() {
    mError_Message = "> Our-C exited ...";

  } // Done()

};

class Other : public Exception
{
  
};

class Data 
{
public:
  vector<Information> mtoken_line ;  // �N�S���D��token��J  
  vector<Reg> mreg_list ;            // �w�q�L��token
  vector<Reg> mreg_list_temp ;       // ����Ȯɩw�q��token 
  bool mrun ;                        // �O�_���b���� 
  bool mis_def ;                     // ����O�w�q
  bool mis_function ;                // �O�_��function 
  bool mend_with_brackets ;          // function while �H"}"����
  bool mis_divid ;                   // �o���O���k �p��0  

  void Remove_White_Space() // ���L�Ҧ��Ů檽��Ū��D�Ů�r������
  {
    while ( cin.peek() == ' ' || cin.peek() == '\t' )
    {
      cin.get() ;
    } // while
  } // Remove_White_Space()

  void Skip_White_Space( char &ch ) // ���L�Ҧ��Ů檽��Ū��D�Ů�r������A�æ^�ǸӦr�� 
  {
    while ( ch == ' ' || ch == '\t' ) 
    {
      ch = cin.get() ;
    } // while 
  } // Skip_White_Space()
 
  void Change_type( Information &token )
  {
    for ( int i = 0 ; i < mreg_list.size() ; i++ )
    {
      if ( mreg_list[i].token_name == token.token_name )
      {
        if ( mreg_list[i].def_type == "int" || mreg_list[i].def_type == "float" )
        {
          token.token_type = "con" ;
        } // if
        else if ( mreg_list[i].def_type == "char" || mreg_list[i].def_type == "string" )
        {
          token.token_type = "id" ;
        } // else if
      } // if
    } // for 

    for ( int i = 0 ; i < mreg_list_temp.size() ; i++ )
    {
      if ( mreg_list_temp[i].token_name == token.token_name ) 
      {
        if ( mreg_list_temp[i].def_type == "int" || mreg_list_temp[i].def_type == "float" )
        {
          token.token_type = "con" ;
        } // if
        else if ( mreg_list_temp[i].def_type == "char" || mreg_list_temp[i].def_type == "string" )
        {
          token.token_type = "id" ;
        } // else if
      } // if
    } // for
  } // Change_type()  

  Information Get_Token() // �}�lŪ�J�r������token�æ^��
  {         // �p�G�O�r���}�Y �᭱�i�H�O�r���B�Ʀr�B���u
            // �p�G�O�Ʀr�}�Y ���᭱�i�H���Ʀr�M. .�}�Y+�Ʀr�]�����Ʀr 
            // �S��Ÿ� > < = ���� 
            // ��l���h�����}�Y ����error
            
    char temp ;                  // �ȦsŪ�J���r��
    char peek ;                  // �Ȧs�U�ӭnŪ�J���r�� �s��P�_�ɻݭn�Ψ� ex: ����     
    bool decimal_point = false ; // �Ʀr�O�_�w�g��p���I �p���I�̦h�@�� ex: 1.2.3 (error)
    bool frist_isdot = false ;   // �Ĥ@�Ӧr����"." �᭱���Ʀr���� 0.xx ex: .9 == 0.9  
    bool frist_char = true ;     // �O�_���Ĥ@�Ӧr��
    bool finish_cut = false ;    // �O�_��������token 
    Information token ;     
    
    while ( !finish_cut ) // ���X�@��token�N���� 
    {
      temp = cin.get() ;
      if ( frist_char ) Skip_White_Space( temp ) ;
          
      if ( frist_char ) // �|���M�w�Ĥ@�Ӧr�������� 
      {
        if ( isalpha( temp ) ) // ���r���r�� 
        {
          token.token_name = temp ;
          token.token_type = "id" ;
          frist_char = false ; 
        } // if
        else if ( isdigit( temp ) ) // ���r���Ʀr 
        {
          decimal_point = false ;
          token.token_name = temp ;
          token.token_type = "con" ;
          frist_char = false ;
        } // else if
        else // ���r���Ÿ� 
        {
          if ( temp == '/' ) // �i�ର���� peek�U�@�Ӧp�G�]�O'/'�N���88
          {
            peek = cin.peek() ;
            if ( peek == '/' ) // �T�w������ 88 
            {
              char command_temp[128] ;
              cin.getline( command_temp, 128 ) ;
              if ( mrun )
                gLine++ ;
            } // if
            else if ( peek == '=' ) // '/='
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // else if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else
          } // if
          else if ( temp == '.' ) // �i�ର�p�� 
          {
            peek = cin.peek() ;
            if ( isdigit( peek ) ) // �T�w���p�� 
            {
              token.token_name = ".";
              token.token_type = "con" ;
              frist_isdot = true ;
              frist_char = false ; 
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '<' ) 
          {
            peek = cin.peek() ;
            if ( peek == '=' || peek == '<' ) // '<=' || '<<' 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '>' )  
          {
            peek = cin.peek() ;
            if ( peek == '=' || peek == '>' ) // '>=' || '>>'
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '=' ) 
          {
            peek = cin.peek() ;
            if ( peek == '=' ) // '=='
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '!' ) 
          {
            peek = cin.peek() ;
            if ( peek == '=' ) // '!='
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '\'' ) // ' '
          {
            if ( cin.peek() == '\n' )
              throw Lexical_Error( '\'' );
            else
            {
              token.token_name = temp ;
              token.token_type = "con" ;
              temp = cin.get() ;
              token.token_name += temp ;
            } // else

            peek = cin.peek() ;
            if ( peek != '\'' ) 
              throw Lexical_Error( '\'' );
            else
            {
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // else
          } // else if
          else if ( temp == '"' ) // " "
          {
            token.token_name = temp ;
            token.token_type = "con" ;
            while ( cin.peek() != '\n' )
            {
              temp = cin.get() ;
              token.token_name += temp ;

              if ( temp == '"' )
                return token ;
            } // while

            throw Lexical_Error( '"' );
            
          } // else if
          else if ( temp == '&' ) 
          {
            peek = cin.peek() ;
            if ( peek == '&' ) // '&&'
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '|' ) 
          {
            peek = cin.peek() ;
            if ( peek == '|' ) // '||'
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '+' ) 
          {
            peek = cin.peek() ;
            if ( peek == '+' || peek == '=' ) // '++' || '+='
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '-' ) 
          {
            peek = cin.peek() ;
            if ( peek == '-' || peek == '=' ) // '--' || '-='
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '*' ) 
          {
            peek = cin.peek() ;
            if ( peek == '=' ) // '*='
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '%' ) 
          {
            peek = cin.peek() ;
            if ( peek == '=' ) // '%='
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == ';' || temp == ',' || temp == '?' || temp == ':' || temp == '^' ||
                    temp == '(' || temp == ')' || temp == '[' || temp == ']' || temp == '{' || temp == '}' )
          {
            token.token_name = temp ;
            token.token_type = "sign" ;
            finish_cut = true ;

          } // else if 
          else if ( temp == '\n' )
          {
            if ( mrun )
            {
              gLine++ ;
            } // if
          } // else if
          else // ���k���~ �ڥ��S�o�r 
          {
            throw Lexical_Error( temp );
          } // else
        } // else
      } // if
      else // �w�M�wtoken�Ĥ@�Ӧr�������� 
      {
        if ( temp == '\n' )
        {
          cin.putback( temp ) ;
          finish_cut = true ;
        } // if
        else if ( frist_isdot ) 
        {
          if ( isdigit( temp ) )
          {
            token.token_name += temp ; 
          } // if
          else
          {
            cin.putback( temp ) ;
            frist_isdot = false ;
            finish_cut = true ;
          } // else
        } // else if
        else if ( token.token_type == "id" ) // token���r�� 
        {
          if ( isalpha( temp ) || isdigit( temp ) || temp == '_' )
          {
            token.token_name += temp ;
          } // if 
          else 
          {
            cin.putback( temp ) ;
            finish_cut = true ;
          } // else
        } // else if
        else if ( token.token_type == "con" ) // token���Ʀr
        {
          if ( isdigit( temp ) || ( temp == '.' && !decimal_point ) )
          {
            if ( temp == '.' ) 
            {
              decimal_point = true ;
              token.token_type = "con" ;
            } // if

            token.token_name += temp ;
          } // if
          else 
          {
            cin.putback( temp ) ;
            finish_cut = true ;
          } // else
        } // else if
      } // else

        
    } // while
   
    if ( token.token_name == "int" ) token.token_type = "int" ;
    else if ( token.token_name == "float" ) token.token_type = "float" ;
    else if ( token.token_name == "char" ) token.token_type = "char" ;
    else if ( token.token_name == "bool" ) token.token_type = "bool" ;
    else if ( token.token_name == "string " ) token.token_type = "string " ;
    else if ( token.token_name == "void" ) token.token_type = "void" ;
    else if ( token.token_name == "if" ) token.token_type = "if" ;
    else if ( token.token_name == "else" ) token.token_type = "else" ;
    else if ( token.token_name == "while" ) token.token_type = "while" ;
    else if ( token.token_name == "do" ) token.token_type = "do" ;
    else if ( token.token_name == "return" ) token.token_type = "return" ;
    else if ( token.token_name == "cin" || token.token_name == "cout" ) 
      token.token_type = "con" ;

    // Change_type( token ) ;
    if ( token.token_name == "/" ) 
      mis_divid = true ;
    else
      mis_divid = false ;

    if ( mis_divid )
      if ( token.token_name == "0" )
        throw Divid_Zero() ;


    return token ; 
  } // Get_Token()

  bool Same_in_Reg( string str ) // �T�{���L�w�q�L��id 
  {
    bool re = false ;
    for ( int i = 0 ; i < mreg_list.size() ; i++ )
    {
      if ( mreg_list[i].token_name == str ) re = true ;
    } // for 

    for ( int i = 0 ; i < mreg_list_temp.size() ; i++ )
    {
      if ( mreg_list_temp[i].token_name == str ) re = true ;
    } // for 

    return re ;
  } // Same_in_Reg()
 
  void Reg_sort()
  {
    Reg temp ;
    for ( int i = mreg_list.size()-1 ; i > 0 ; i-- )
    {
      for ( int j = 0 ; j <= i-1 ; j++ )
      {
        int size ;
        if ( mreg_list[j].token_name.length() >= mreg_list[j+1].token_name.length() )
          size = mreg_list[j].token_name.length() ;
        else
          size = mreg_list[j+1].token_name.length() ;

        bool stop = false ;
        for ( int k = 0 ; k < size && !stop ; k++ )
        {
          if ( mreg_list[j].token_name[k] > mreg_list[j+1].token_name[k] )
          {
            temp = mreg_list[j] ;
            mreg_list[j] = mreg_list[j+1] ;
            mreg_list[j+1] = temp ;
          } // if
          else if ( mreg_list[j].token_name[k] == mreg_list[j+1].token_name[k] )
          {
            ;
          } // else if
          else
            stop = true ;
        } // for
      } // for
    } // for
  } // Reg_sort()
  
  void User_input()
  {    
    Information token ;
    mend_with_brackets = false ; 
    mis_function = false ;    
    mis_divid = false ;

    if ( glast_is_error ) gLine = 1 ;
    else gLine = 0 ;

    mtoken_line.clear() ;
    mreg_list_temp.clear() ;
    mrun = true ;
    token = Get_Token() ;

    if ( token.token_name == "Done" )
    {
      Information token_temp1, token_temp2 ;
      token_temp1 = Get_Token() ;
      if ( token_temp1.token_name == "(" )
      {
        token_temp2 = Get_Token() ;
        if ( token_temp2.token_name == ")" )
        {
          throw Done() ;
        } // if
        else
        {
          for ( int i = 0 ; i < token_temp1.token_name.length() ; i++ )
            cin.putback( token_temp1.token_name[i] ) ;
        } // else
      } // if
    } // if
    else if ( token.token_name == "ListAllFunctions" )
    {
      cout << "> " ;
      for ( int i = 0 ; i < mreg_list.size() ; i++ )
      {
        if ( mreg_list[i].is_function )
        {
          cout << mreg_list[i].token_name << "()" << endl ;
        } // if
      } // for
   
      cout << "Statement executed ..." << endl ;
      char temp ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      throw Other() ;
    } // else if 
    else if ( token.token_name == "ListFunction" )
    {
      char temp ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      token = Get_Token() ;
       
      cout << "> " ;
      for ( int i = 0 ; i < mreg_list.size() ; i++ )
      {
        if ( mreg_list[i].token_name == token.token_name )
        {
          for ( int j = 0 ; j < mreg_list[i].token_data.size() ; j++ )
          {
            cout << mreg_list[i].token_data[j].token_name ;

            if ( j < mreg_list[i].token_data.size() - 1 )
              if ( mreg_list[i].token_data[j+1].token_name != "(" )
                cout << " " ;

            if ( j < mreg_list[i].token_data.size() - 1 )
            {
              if ( mreg_list[i].token_data[j+1].token_name == "}" )
                cout << endl ;
              else if ( mreg_list[i].token_data[j].token_name == "{" ||
                        mreg_list[i].token_data[j].token_name == ";" )
                cout << endl << " " ;
            } // if
          } // for
        } // if
      } // for

      cout << "Statement executed ..." << endl ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      throw Other() ;
    } // else if
    else if ( token.token_name == "ListAllVariables" )
    {
      cout << "> " ;
      for ( int i = 0 ; i < mreg_list.size() ; i++ )
      {
        if ( !mreg_list[i].is_function )
        {
          cout << mreg_list[i].token_name << endl ;
        } // if
      } // for
   
      cout << "Statement executed ..." << endl ;
      char temp ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      throw Other() ;
    } // else if 
    else if ( token.token_name == "ListVariable" )
    {
      char temp ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      token = Get_Token() ;
       
      cout << "> " ;
      for ( int i = 0 ; i < mreg_list.size() ; i++ )
      {
        if ( mreg_list[i].token_name == token.token_name )
        {
          for ( int j = 0 ; j < mreg_list[i].token_data.size() ; j++ )
          {
            cout << mreg_list[i].token_data[j].token_name ;
            if ( j < mreg_list[i].token_data.size() - 1 )
              if ( mreg_list[i].token_data[j+1].token_name != "[" )
                cout << " " ;
          } // for
        
          cout << endl ;
        } // if
      } // for

      cout << "Statement executed ..." << endl ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      temp = cin.get();
      Skip_White_Space( temp ) ;
      throw Other() ;
    } // else if    


    if ( Definition( token ) )
    {
      mis_def = true ; 
      while ( mrun )
      {
        if ( Definition( token ) )
        {
        } // if
        else if ( Statement( token ) )
        {
          ;  
        } // else if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while
    } // if
    else if ( Statement( token ) )
    {
      mis_def = false ;
      while ( mrun )
      {
        if ( Definition( token ) )
        {
          ;
        } // if
        else if ( Statement( token ) )
        {
          ;  
        } // else if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
        
      } // while
    } // else if
    else if ( token.token_name == "NMSL" )
    {
      ;
    } // else if
    else
    {
      throw Syntactic_Error( token.token_name ) ;
    } // else

    mreg_list_temp.clear() ;
    Reg reg ;
    if ( mis_def ) // �O�w�q 
    {
      cout << "> " ;
      bool keep_def = true ; // �i��|���h�թw�q ex: int a, b, c 
      for ( int a = 1 ; keep_def ; a = a + 2 )
      {
        keep_def = false ;
        if ( Same_in_Reg( mtoken_line[a].token_name ) )
        {
          for ( int i = 0 ; i < mreg_list.size() ; i++ )
          {
            if ( mreg_list[i].token_name == mtoken_line[a].token_name )
            {
              mreg_list[i].token_data = mtoken_line ;
            } // if
          } // for 

          cout << "New definition of " << mtoken_line[a].token_name 
               << " entered ..." << endl ;
        } // if
        else
        {
          reg.def_type = mtoken_line[0].token_name ;
          if ( mis_function )
            reg.token_name = mtoken_line[a].token_name ;
          else
            reg.token_name = mtoken_line[a].token_name ;
          reg.is_function = mis_function ;
          reg.token_data = mtoken_line ;
          mreg_list.push_back( reg ) ;

          if ( mis_function )
            cout << "Definition of " << reg.token_name << "() entered ..." << endl ;
          else 
            cout << "Definition of " << reg.token_name << " entered ..." << endl ;
     
        } // else

        if ( mtoken_line[a+1].token_name == "," ) keep_def = true ;
      } // for 
    } // if
    else // ���O�w�q 
    {
      cout << "> Statement executed ..." << endl ;
    } // else

    Reg_sort() ;
    glast_is_error = false ;
  } // User_input()

  bool Definition( Information &token )
  {
    Reg reg ;
    if ( token.token_name == "void" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( token.token_type == "id" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Function_definition_without_ID( token ) )
        {
          return true ;
        } // if
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ; 
      } // else
    } // if
    else if ( Type_specifier( token ) )
    { 
      if ( token.token_type == "id" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Function_definition_or_declarators( token ) )
        {
          return true ;
        } // if
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else 
    {
      return false ;
    } // else

    return false ;
  } // Definition()

  bool Type_specifier( Information &token )
  {
    string type = token.token_name ;
    if ( type == "int" || type == "char" || type == "float" || 
         type == "string" || type == "bool" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      return true ;
    } // if
    else
    {
      return false ;
    } // else
  } // Type_specifier()

  bool Function_definition_or_declarators( Information &token )
  {
    if ( Function_definition_without_ID( token ) )
      return true ;
    else if ( Rest_of_declarators( token ) )
      return true ;
    else
      return false ;
  } // Function_definition_or_declarators()

  bool Rest_of_declarators( Information &token )
  {
    if ( token.token_name == "[" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( token.token_type == "con" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ; 
        if ( token.token_name == "]" )
        {
          mtoken_line.push_back( token ) ;
          token = Get_Token() ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // if
    
    while ( token.token_name == "," )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( token.token_type == "id" )
      {
        if ( Same_in_Reg( token.token_name ) )
          throw Semantic_error( token.token_name ) ;
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( token.token_name == "[" )
        {
          mtoken_line.push_back( token ) ;
          token = Get_Token() ;
          if ( token.token_type == "con" )
          {
            mtoken_line.push_back( token ) ;
            token = Get_Token() ; 
            if ( token.token_name == "]" )
            {
              mtoken_line.push_back( token ) ;
              token = Get_Token() ;
            } // if
            else if ( token.token_name == "NMSL" )
            {
              ;
            } // else if
            else
            {
              throw Syntactic_Error( token.token_name ) ;
            } // else
          } // if
          else if ( token.token_name == "NMSL" )
          {
            ;
          } // else if
          else
          {
            // throw Syntactic_Error( token.token_name ) ;
          } // else
        } // if
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // while

    if ( token.token_name == ";" )
    {
      mtoken_line.push_back( token ) ;
      if ( mend_with_brackets )
      {
        token = Get_Token() ;
      } // if
      else mrun = false ;
      return true ;
    } // if
    else if ( token.token_name == "NMSL" )
    {
      ;
    } // else if
    else
    {
      throw Syntactic_Error( token.token_name ) ;
    } // else

    return false ;
  } // Rest_of_declarators()

  bool Function_definition_without_ID( Information &token )
  {
    if ( token.token_name == "(" )
    {
      mend_with_brackets = true ; // �֩w�Ofunction
      mis_function = true ; 
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( token.token_name == "void" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
      } // if
      else if ( Formal_parameter_list( token ) )
      {
      } // else if

      if ( token.token_name == ")" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        Compound_statement( token ) ;
        return true ;
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else    
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Function_definition_without_ID()

  bool Formal_parameter_list( Information &token )
  {
    Reg reg ;
    if ( Type_specifier( token ) )
    {
      if ( token.token_name == "&" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
      } // if
      
      if ( token.token_type == "id" )
      {
        reg.token_name = token.token_name ;
        reg.def_type = mtoken_line[mtoken_line.size()-1].token_name ;
        mreg_list_temp.push_back( reg ) ;
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( token.token_name == "[" )
        {
          mtoken_line.push_back( token ) ;
          token = Get_Token() ;
          if ( token.token_type == "con" )
          {
            mtoken_line.push_back( token ) ;
            token = Get_Token() ; 
            if ( token.token_name == "]" )
            {
              mtoken_line.push_back( token ) ;
              token = Get_Token() ;
            } // if
            else if ( token.token_name == "NMSL" )
            {
              ;
            } // else if
            else
            {
              throw Syntactic_Error( token.token_name ) ;
            } // else
          } // if
          else if ( token.token_name == "NMSL" )
          {
            ;
          } // else if
          else
          {
            throw Syntactic_Error( token.token_name ) ;
          } // else
        } // if

        while ( token.token_name == "," )
        {
          mtoken_line.push_back( token ) ;
          token = Get_Token() ;
          if ( Type_specifier( token ) )
          {
            if ( token.token_name == "&" )
            {
              mtoken_line.push_back( token ) ;
              token = Get_Token() ;
            } // if

            if ( token.token_type == "id" )
            {
              reg.token_name = token.token_name ;
              reg.def_type = mtoken_line[mtoken_line.size()-1].token_name ;
              mreg_list_temp.push_back( reg ) ;
              mtoken_line.push_back( token ) ;
              token = Get_Token() ; 
              if ( token.token_name == "[" )
              {
                mtoken_line.push_back( token ) ;
                token = Get_Token() ;
                if ( token.token_type == "con" )
                {
                  mtoken_line.push_back( token ) ;
                  token = Get_Token() ; 
                  if ( token.token_name == "]" )
                  {
                    mtoken_line.push_back( token ) ;
                    token = Get_Token() ;
                  } // if
                  else if ( token.token_name == "NMSL" )
                  {
                    ;
                  } // else if
                  else
                  {
                    throw Syntactic_Error( token.token_name ) ;
                  } // else
                } // if
                else if ( token.token_name == "NMSL" )
                {
                  ;
                } // else if
                else
                {
                  throw Syntactic_Error( token.token_name ) ;
                } // else
              } // if
            } // if
          } // if
          else if ( token.token_name == "NMSL" )
          {
            ;
          } // else if
          else
          {
            throw Syntactic_Error( token.token_name ) ;
          } // else
        } // while 

        return true ;
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Formal_parameter_list()

  bool Compound_statement( Information &token )
  {
    if ( token.token_name == "{" )
    {
      mend_with_brackets = true ;
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      while ( Declaration( token ) || Statement( token ) )
      {
        
      } // while
      
      if ( token.token_name == "}" )
      {
        mrun = false ;
        mtoken_line.push_back( token ) ;
        return true ;
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else

    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Compound_statement()
 
  bool Declaration( Information &token )
  {
    Reg reg ;
    if ( Type_specifier( token ) )
    {
      if ( token.token_type == "id" )
      {
        reg.token_name = token.token_name ;
        reg.def_type = mtoken_line[mtoken_line.size()-1].token_name ;
        mreg_list_temp.push_back( reg ) ;
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Rest_of_declarators( token ) )
        {
          return true ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Declaration()

  bool Statement( Information &token )
  {
    if ( token.token_name == ";" )
    {
      mtoken_line.push_back( token ) ;
      if ( mend_with_brackets )
      {
        token = Get_Token() ;
      } // if
      else mrun = false ;
      return true ;
    } // if
    else if ( Expression( token ) )
    {
      if ( token.token_name == ";" )
      {
        mtoken_line.push_back( token ) ;
        if ( mend_with_brackets )
        {
          token = Get_Token() ;
        } // if
        else mrun = false ;
        return true ;
      } // if
    } // else if
    else if ( token.token_name == "return" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( Expression( token ) )
      {
  
      } // if

      if ( token.token_name == ";" )
      {
        mtoken_line.push_back( token ) ;
        if ( mend_with_brackets )
        {
          token = Get_Token() ;
        } // if
        else mrun = false ;
        return true ;
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else if ( Compound_statement( token ) )
    {
      return true ;
    } // else if
    else if ( token.token_name == "if" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( token.token_name == "(" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Expression( token ) )
        {
          if ( token.token_name == ")" )
          {
            mtoken_line.push_back( token ) ;
            token = Get_Token() ;
            if ( Statement( token ) )
            {
              if ( token.token_name == ";" )
              {
                char temp ;
                temp = cin.peek() ;
                while ( temp == ' ' || temp == '\t' || temp == '\n' )
                {
                  if ( temp == '\n' ) gLine++ ;
                  temp = cin.get() ;
                  temp = cin.peek() ;        
                } // while

                if ( temp == 'e' )
                {
                  mtoken_line.push_back( token ) ;
                  token = Get_Token() ;
                } // if              
              } // if 
              
              if ( token.token_name == "else" )
              {
                gLine++ ;
                mtoken_line.push_back( token ) ;
                token = Get_Token() ;
                if ( Statement( token ) )
                {
                  return true ;
                } // if
                else if ( token.token_name == "NMSL" )
                {
                  ;
                } // else if
                else
                {
                  throw Syntactic_Error( token.token_name ) ;
                } // else
              } // if
            
              return true ;           
            } // if
            else if ( token.token_name == "NMSL" )
            {
              ;
            } // else if
            else
            {
              throw Syntactic_Error( token.token_name ) ;
            } // else
          } // if
          else if ( token.token_name == "NMSL" )
          {
            ;
          } // else if
          else
          {
            throw Syntactic_Error( token.token_name ) ;
          } // else
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else if ( token.token_name == "while" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( token.token_name == "(" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Expression( token ) )
        {
          if ( token.token_name == ")" )
          {
            mtoken_line.push_back( token ) ;
            token = Get_Token() ;
            if ( Statement( token ) )
            {
              return true ;           
            } // if
            else if ( token.token_name == "NMSL" )
            {
              ;
            } // else if
            else
            {
              throw Syntactic_Error( token.token_name ) ;
            } // else
          } // if
          else if ( token.token_name == "NMSL" )
          {
            ;
          } // else if
          else
          {
            throw Syntactic_Error( token.token_name ) ;
          } // else
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else if ( token.token_name == "do" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( Statement( token ) )
      {
        if ( token.token_name == "while" )
        {
          mtoken_line.push_back( token ) ;
          token = Get_Token() ;
          if ( token.token_name == "(" )
          {
            mtoken_line.push_back( token ) ;
            token = Get_Token() ;
            if ( Expression( token ) )
            {
              if ( token.token_name == ")" )
              {
                mtoken_line.push_back( token ) ;
                token = Get_Token() ;
                if ( token.token_name == ";" )
                {
                  mtoken_line.push_back( token ) ;
                  if ( mend_with_brackets )
                  {
                    token = Get_Token() ;
                  } // if
                  else mrun = false ;
                  return true ;           
                } // if
                else if ( token.token_name == "NMSL" )
                {
                  ;
                } // else if
                else
                {
                  throw Syntactic_Error( token.token_name ) ;
                } // else
              } // if
              else if ( token.token_name == "NMSL" )
              {
                ;
              } // else if
              else
              {
                throw Syntactic_Error( token.token_name ) ;
              } // else
            } // if
            else if ( token.token_name == "NMSL" )
            {
              ;
            } // else if
            else
            {
              throw Syntactic_Error( token.token_name ) ;
            } // else
          } // if
          else if ( token.token_name == "NMSL" )
          {
            ;
          } // else if
          else
          {
            throw Syntactic_Error( token.token_name ) ;
          } // else
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else
    {
      return false ;
    } // else

    return false ;
  } // Statement()

  bool Expression( Information &token )
  {
    if ( Basic_expression( token ) )
    {
      while ( token.token_name == "," )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Basic_expression( token ) )
        {
          ;  
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Expression()

  bool Basic_expression( Information &token )
  {
    if ( token.token_type == "id" )
    {
      if ( !Same_in_Reg( token.token_name ) )
        throw Semantic_error( token.token_name ) ;
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( Rest_of_Identifier_started_basic_exp( token ) )
      {
        return true ;
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // if
    else if ( token.token_name == "++" || token.token_name == "--" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( token.token_type == "id" )
      {
        if ( !Same_in_Reg( token.token_name ) )
          throw Semantic_error( token.token_name ) ;
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Rest_of_PPMM_Identifier_started_basic_exp( token ) )
        {
          return true ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else 
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else if ( Sign( token ) )
    {
      while ( Sign( token ) )
      {
      } // while

      if ( Signed_unary_exp( token ) )
      {
        if ( Romce_and_romloe( token ) )
        {
          return true ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else 
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else if ( token.token_type == "con" || token.token_name == "(" )
    {
      if ( token.token_type == "con" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
      } // if
      else if ( token.token_name == "(" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Expression( token ) )
        {
          if ( token.token_name == ")" )
          {
            mtoken_line.push_back( token ) ;
            token = Get_Token() ;
          } // if
        } // if
      } // else if

      if ( Romce_and_romloe( token ) )
      {
        return true ;
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else 
    {
      return false ;
    } // else if

    return false ;
  } // Basic_expression()

  bool Rest_of_Identifier_started_basic_exp( Information &token )
  {
    if ( token.token_name == "[" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( Expression( token ) )
      {
        if ( token.token_name == "]" )
        {
          if ( Assignment_operator( token ) )
          {
            if ( Basic_expression( token ) )
            {
              return true ;
            } // if
            else if ( token.token_name == "NMSL" )
            {
              ;
            } // else if
            else
            {
              throw Syntactic_Error( token.token_name ) ;
            } // else
          } // if
          else if ( token.token_name == "++" || token.token_name == "--" )
          {
            mtoken_line.push_back( token ) ;
            token = Get_Token() ;
            if ( Romce_and_romloe( token ) )
            {
              return true ;
            } // if
            else if ( token.token_name == "NMSL" )
            {
              ;
            } // else if
            else
            {
              throw Syntactic_Error( token.token_name ) ;
            } // else   
          } // else if
          else if ( Romce_and_romloe( token ) )
          {
            return true ;
          } // else if
          else if ( token.token_name == "NMSL" )
          {
            ;
          } // else if
          else
          {
            throw Syntactic_Error( token.token_name ) ;
          } // else
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // if
    else if ( token.token_name == "(" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( Actual_parameter_list( token ) )
      {
      } // if
      
      if ( token.token_name == ")" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Romce_and_romloe( token ) )
        {
          return true ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else if ( Assignment_operator( token ) )
    {
      if ( Basic_expression( token ) )
      {
        return true ;
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else if ( token.token_name == "++" || token.token_name == "--" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( Romce_and_romloe( token ) )
      {
        return true ;
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else   
    } // else if
    else if ( Romce_and_romloe( token ) )
    {
      return true ;
    } // else if
    else 
    {
      return false ;
    } // else

    return false ;
  } // Rest_of_Identifier_started_basic_exp()

  bool Rest_of_PPMM_Identifier_started_basic_exp( Information &token )
  {
    if ( token.token_name == "[" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( Expression( token ) )
      {
        if ( token.token_name == "]" )
        {
          mtoken_line.push_back( token ) ;
          token = Get_Token() ;
          if ( Romce_and_romloe( token ) )
          {
            return true ;
          } // if
          else if ( token.token_name == "NMSL" )
          {
            ;
          } // else if
          else
          {
            throw Syntactic_Error( token.token_name ) ;
          } // else
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // if
    else if ( Romce_and_romloe( token ) )
    {
      return true ;
    } // if
    else
    {
      return false ;
    } // else
    
    return false ;
  } // Rest_of_PPMM_Identifier_started_basic_exp()

  bool Sign( Information &token )
  {
    if ( token.token_name == "+" || token.token_name == "-" || token.token_name == "!" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Sign()

  bool Actual_parameter_list( Information &token )
  {
    if ( Basic_expression( token ) ) 
    {
      while ( token.token_name == "," )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Basic_expression( token ) )
        {
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Actual_parameter_list()

  bool Assignment_operator( Information &token )
  {
    if ( token.token_name == "=" || token.token_name == "*=" || token.token_name == "/=" ||
         token.token_name == "%=" || token.token_name == "+=" || token.token_name == "-=" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Assignment_operator()

  bool Romce_and_romloe( Information &token )
  {
    if ( Rest_of_maybe_logical_OR_exp( token ) )
    {
      if ( token.token_name == "?" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Basic_expression( token ) )
        {
          if ( token.token_name == ":" )
          {
            mtoken_line.push_back( token ) ;
            token = Get_Token() ;
            if ( Basic_expression( token ) )
            {
            } // if
            else if ( token.token_name == "NMSL" )
            {
              ;
            } // else if
            else
            {
              throw Syntactic_Error( token.token_name ) ;
            } // else
          } // if
          else if ( token.token_name == "NMSL" )
          {
            ;
          } // else if
          else
          {
            throw Syntactic_Error( token.token_name ) ;
          } // else
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Romce_and_romloe()

  bool Rest_of_maybe_logical_OR_exp( Information &token )
  {
    if ( Rest_of_maybe_logical_AND_exp( token ) )
    {
      while ( token.token_name == "||" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_logical_AND_exp( token ) )
        {
          ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Rest_of_maybe_logical_OR_exp()

  bool Maybe_logical_AND_exp( Information &token )
  {
    if ( Maybe_bit_OR_exp( token ) )
    {
      while ( token.token_name == "&&" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_bit_OR_exp( token ) )
        {
          ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Maybe_logical_AND_exp()

  bool Rest_of_maybe_logical_AND_exp( Information &token )
  {
    if ( Rest_of_maybe_bit_OR_exp( token ) )
    {
      while ( token.token_name == "&&" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_bit_OR_exp( token ) )
        {
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Rest_of_maybe_logical_AND_exp()

  bool Maybe_bit_OR_exp( Information &token )
  {
    if ( Maybe_bit_ex_OR_exp( token ) )
    {
      while ( token.token_name == "|" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_bit_ex_OR_exp( token ) )
        {
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Maybe_bit_OR_exp()

  bool Rest_of_maybe_bit_OR_exp( Information &token )
  {
    if ( Rest_of_maybe_bit_ex_OR_exp( token ) )
    {
      while ( token.token_name == "|" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_bit_ex_OR_exp( token ) )
        {
          ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Rest_of_maybe_bit_OR_exp()

  bool Maybe_bit_ex_OR_exp( Information &token )
  {
    if ( Maybe_bit_AND_exp( token ) )
    {
      while ( token.token_name == "^" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_bit_AND_exp( token ) )
        {
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Maybe_bit_ex_OR_exp()

  bool Rest_of_maybe_bit_ex_OR_exp( Information &token )
  {
    if ( Rest_of_maybe_bit_AND_exp( token ) )
    {
      while ( token.token_name == "^" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_bit_AND_exp( token ) )
        {
          ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Rest_of_maybe_bit_ex_OR_exp()

  bool Maybe_bit_AND_exp( Information &token )
  {
    if ( Maybe_equality_exp( token ) )
    {
      while ( token.token_name == "&" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_equality_exp( token ) )
        {
          ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Maybe_bit_AND_exp()

  bool Rest_of_maybe_bit_AND_exp( Information &token )
  {
    if ( Rest_of_maybe_equality_exp( token ) )
    {
      while ( token.token_name == "&" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_equality_exp( token ) )
        {
          ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Rest_of_maybe_bit_AND_exp()

  bool Maybe_equality_exp( Information &token )
  {
    if ( Maybe_relational_exp( token ) )
    {
      while ( token.token_name == "==" || token.token_name == "!=" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_relational_exp( token ) )
        {
          ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Maybe_equality_exp()

  bool Rest_of_maybe_equality_exp( Information &token )
  {
    if ( Rest_of_maybe_relational_exp( token ) )
    {
      while ( token.token_name == "==" || token.token_name == "!=" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_relational_exp( token ) )
        {
          ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Rest_of_maybe_equality_exp()

  bool Maybe_relational_exp( Information &token )
  {
    if ( Maybe_shift_exp( token ) )
    {
      while ( token.token_name == "<" || token.token_name == ">" || 
              token.token_name == "<=" || token.token_name == ">=" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_shift_exp( token ) )
        {
          ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Maybe_relational_exp()

  bool Rest_of_maybe_relational_exp( Information &token )
  {
    if ( Rest_of_maybe_shift_exp( token ) )
    {
      while ( token.token_name == "<" || token.token_name == ">" || 
              token.token_name == "<=" || token.token_name == ">=" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_shift_exp( token ) )
        {
          ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Rest_of_maybe_relational_exp()

  bool Maybe_shift_exp( Information &token )
  {
    if ( Maybe_additive_exp( token ) )
    {
      while ( token.token_name == ">>" || token.token_name == "<<" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_additive_exp( token ) )
        {
          ;  
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Maybe_shift_exp()

  bool Rest_of_maybe_shift_exp( Information &token )
  {
    if ( Rest_of_maybe_additive_exp( token ) )
    {
      while ( token.token_name == ">>" || token.token_name == "<<" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_additive_exp( token ) )
        {
          ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Rest_of_maybe_shift_exp()

  bool Maybe_additive_exp( Information &token )
  {
    if ( Maybe_mult_exp( token ) )
    {
      while ( token.token_name == "+" || token.token_name == "-" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_mult_exp( token ) )
        {
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Maybe_additive_exp()
 
  bool Rest_of_maybe_additive_exp( Information &token )
  {
    if ( Rest_of_maybe_mult_exp( token ) )
    {
      while ( token.token_name == "+" || token.token_name == "-" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Maybe_mult_exp( token ) )
        {
          ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // while

      return true ;
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Rest_of_maybe_additive_exp()

  bool Maybe_mult_exp( Information &token )
  {
    if ( Unary_exp( token ) )
    {
      if ( Rest_of_maybe_mult_exp( token ) )
      {
        return true ;
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // if
    else
    {
      return false ;
    } // else

    return false ;
  } // Maybe_mult_exp()

  bool Rest_of_maybe_mult_exp( Information &token )
  {
    while ( token.token_name == "*" || token.token_name == "/" || token.token_name == "%" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( Unary_exp( token ) )
      {
        ;
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // while

    return true ;
  } // Rest_of_maybe_mult_exp()

  bool Unary_exp( Information &token )
  {
    if ( Sign( token ) )
    {
      while ( Sign( token ) )
      {
      
      } // while
    
      if ( Signed_unary_exp( token ) )
      {
        return true ;
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // if
    else if ( Unsigned_unary_exp( token ) )
    {
      return true ;
    } // else if
    else if ( token.token_name == "++" || token.token_name == "--" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( token.token_type == "id" )
      {
        if ( !Same_in_Reg( token.token_name ) )
          throw Semantic_error( token.token_name ) ;
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( token.token_name == "[" )
        {
          mtoken_line.push_back( token ) ;
          token = Get_Token() ;
          if ( Expression( token ) )
          {
            if ( token.token_name == "]" )
            {
              ;
            } // if
            else if ( token.token_name == "NMSL" )
            {
              ;
            } // else if
            else
            {
              throw Syntactic_Error( token.token_name ) ;
            } // else
          } // if
          else if ( token.token_name == "NMSL" )
          { 
            ;
          } // else if
          else
          {
            throw Syntactic_Error( token.token_name ) ;
          } // else
        } // if       

        return true ;
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else
    {
      return false ;
    } // else

    return false ;
  } // Unary_exp()

  bool Signed_unary_exp( Information &token )
  {
    if ( token.token_type == "id" )
    {
      if ( !Same_in_Reg( token.token_name ) )
        throw Semantic_error( token.token_name ) ;
      mtoken_line.push_back( token ) ;
      token = Get_Token() ; 
      if ( token.token_name == "(" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Actual_parameter_list( token ) )
        {
        } // if

        if ( token.token_name == ")" )
        {
          mtoken_line.push_back( token ) ;
          token = Get_Token() ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "[" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Expression( token ) )
        {
          if ( token.token_name == "]" )
          {
            mtoken_line.push_back( token ) ;
            token = Get_Token() ;
          } // if
          else if ( token.token_name == "NMSL" )
          {
            ;
          } // else if
          else
          {
            throw Syntactic_Error( token.token_name ) ;
          } // else
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // else if

      return true ;
    } // if
    else if ( token.token_type == "con" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      return true ;
    } // else if
    else if ( token.token_name == "(" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( Expression( token ) )
      {
        if ( token.token_name == ")" )
        {
          mtoken_line.push_back( token ) ;
          token = Get_Token() ;
          return true ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else 
    {
      return false ;
    } // else

    return false ;
  } // Signed_unary_exp()

  bool Unsigned_unary_exp( Information &token )
  {
    if ( token.token_type == "id" )
    {
      if ( !Same_in_Reg( token.token_name ) )
        throw Semantic_error( token.token_name ) ;
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( token.token_name == "(" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Actual_parameter_list( token ) )
        {
        } // if

        if ( token.token_name == ")" )
        {
          mtoken_line.push_back( token ) ;
          token = Get_Token() ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "[" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
        if ( Expression( token ) )
        {
          if ( token.token_name == "]" )
          {
            mtoken_line.push_back( token ) ;
            token = Get_Token() ;
            if ( token.token_name == "++" || token.token_name == "--" )
            {
              mtoken_line.push_back( token ) ;
              token = Get_Token() ;
            } // if
          } // if
          else if ( token.token_name == "NMSL" )
          {
            ;
          } // else if
          else
          {
            throw Syntactic_Error( token.token_name ) ;
          } // else
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // else if
      else if ( token.token_name == "++" || token.token_name == "--" )
      {
        mtoken_line.push_back( token ) ;
        token = Get_Token() ;
      } // else if

      return true ;
    } // if
    else if ( token.token_type == "con" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      return true ;
    } // else if
    else if ( token.token_name == "(" )
    {
      mtoken_line.push_back( token ) ;
      token = Get_Token() ;
      if ( Expression( token ) )
      {
        if ( token.token_name == ")" )
        {
          mtoken_line.push_back( token ) ;
          token = Get_Token() ;
          return true ;
        } // if
        else if ( token.token_name == "NMSL" )
        {
          ;
        } // else if
        else
        {
          throw Syntactic_Error( token.token_name ) ;
        } // else
      } // if
      else if ( token.token_name == "NMSL" )
      {
        ;
      } // else if
      else
      {
        throw Syntactic_Error( token.token_name ) ;
      } // else
    } // else if
    else 
    {
      return false ;
    } // else

    return false ;
  } // Unsigned_unary_exp()
  
};

int main() 
{
  int testnum ;
  cin >> testnum ;
  
  cout << "Our-C running ..." << endl;
  Data data ;
  Information information ;
  bool done = false ;

  while ( !done )
  {
    try {
      data.User_input() ;
    } // try
    catch ( Lexical_Error e )
    {
      cout << e.mError_Message ;
      char temp[128] ;
      cin.getline( temp, 128 );
      glast_is_error = true ; 
    } // catch
    catch ( Syntactic_Error e )
    {
      cout << e.mError_Message ;
      char temp[128] ;
      cin.getline( temp, 128 );
      glast_is_error = true ;
    } // catch
    catch ( Semantic_error e )
    {
      cout << e.mError_Message ;
      char temp[128] ;
      cin.getline( temp, 128 );
      glast_is_error = true ;
    } // catch
    catch ( Divid_Zero e )
    {
      cout << e.mError_Message ;
      char temp[128] ;
      cin.getline( temp, 128 );
      glast_is_error = true ;
    } // catch
    catch ( Done e )
    {
      cout << e.mError_Message ;
      done = true ;
    } // catch
    catch( Other e )
    {
      char temp[128] ;
      cin.getline( temp, 128 );
      glast_is_error = true ;
    } // catch
  } // while

} // main() 
