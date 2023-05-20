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
  string token_type ; // 
  
  bool is_function ;
};

int gLine ; // ��������e���ĴX�� 

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
    mError_Message = "Line " + line + " : " + mError_Message +
      "unrecognized token with first char : '" + token + "'\n";

  } // Lexical_Error()

  Lexical_Error( char token ) { 
    stringstream ss;
    ss << gLine;
    string line = ss.str();
    mError_Message = "Line " + line + " : " + mError_Message +
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
    mError_Message = "Line " + line + " : " + mError_Message +
      "unexpected token : '" + token + "'\n";

  } // Syntactic_Error()

  Syntactic_Error( char token ) {

    stringstream ss;
    ss << gLine;
    string line = ss.str();
    mError_Message = "Line " + line + " : " + mError_Message +
      "unexpected token : '" + token + "'\n";

  } // Syntactic_Error()

};

class Semantic_error : public Exception // �y�q���~ (���w�q 
{ 

public:

  semantic_error( string token ) {

    stringstream ss;
    ss << gLine;
    string line = ss.str();
    mError_Message = "Line " + line + " : " + mError_Message +
      "undefined identifier : '" + token + "'\n";

  } // semantic_error()

  semantic_error( char token ) {

    stringstream ss;
    ss << gLine;
    string line = ss.str();
    mError_Message = "Line " + line + " : " + mError_Message +
      "undefined identifier : '" + token + "'\n";

  } // semantic_error()

};

class Divid_Zero : public Exception // ��0 
{  

public:

  Divid_Zero() {
    mError_Message = "Error\n";
  } // Divid_Zero()

};

class Done : public Exception // �������� 
{ 

public:

  Done() {
    mError_Message = "Our-C exited ...";

  } // Done()

};

class Data 
{
public:

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
  
  Information Get_Token() { // �}�lŪ�J�r������token�æ^�� 
            // �p�G�O�r���}�Y �᭱�i�H�O�r���B�Ʀr�B���u
            // �p�G�O�Ʀr�}�Y ���᭱�i�H���Ʀr�M. .�}�Y+�Ʀr�]�����Ʀr 
            // �S��Ÿ� >= <= ���� 
            // ��l���h�����}�Y ������@token�B�z 
            // �C���X�@��token �N�iinterpret���q 
            
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
          token.token_type = "num" ;
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
              token.token_type = "num" ;
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
              token.token_type = "sign" ;
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
            } // if
          } // else if
          else if ( temp == '"' ) // " "
          {
            token.token_name = temp ;
            token.token_type = "sign" ;
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
        else if ( token.token_type == "num" ) // token���Ʀr
        {
          if ( isdigit( temp ) || ( temp == '.' && !decimal_point ) )
          {
            if ( temp == '.' ) 
            {
              decimal_point = true ;
              token.token_type = "num" ;
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
   
    return token ; 
  } // Get_Token()

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
      gLine = 1 ;
      information = data.Get_Token() ;
      cout << information.token_name ;
      system("pause") ;
    } // try
    catch ( Lexical_Error e )
    {
      cout << e.mError_Message ;
      char temp[128] ;
      cin.getline( temp, 128 );
    } // catch
    catch ( Syntactic_Error e )
    {
      cout << e.mError_Message ;
      char temp[128] ;
      cin.getline( temp, 128 );

    } // catch
    catch ( Semantic_error e )
    {
      cout << e.mError_Message ;
      char temp[128] ;
      cin.getline( temp, 128 );
    } // catch
    catch ( Divid_Zero e )
    {
      cout << e.mError_Message ;
      char temp[128] ;
      cin.getline( temp, 128 );
    } // catch
    catch ( Done e )
    {
      done = true ;
    } // catch
  } // while
  
  cout << "> Program exits..." ;


} // main() 
