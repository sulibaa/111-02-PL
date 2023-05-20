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

struct Information // 每個token的資料 
{
  string token_name ;
  string token_type ; // 
  
  bool is_function ;
};

int gLine ; // 未到分號前的第幾行 

class Exception // 設定錯誤訊息 給各種錯誤繼承 
{
  
public:
  
  string mError_Message;
  Exception() {  }

  Exception( string str ) {
    mError_Message = str;
  } // Exception()

}; //  class Expception

class Lexical_Error : public Exception // 詞法錯誤 (根本沒這字 
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

class Syntactic_Error : public Exception // 語法錯誤
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

class Semantic_error : public Exception // 語義錯誤 (未定義 
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

class Divid_Zero : public Exception // 除0 
{  

public:

  Divid_Zero() {
    mError_Message = "Error\n";
  } // Divid_Zero()

};

class Done : public Exception // 徹底結束 
{ 

public:

  Done() {
    mError_Message = "Our-C exited ...";

  } // Done()

};

class Data 
{
public:

  void Remove_White_Space() // 跳過所有空格直到讀到非空格字元為止
  {
    while ( cin.peek() == ' ' || cin.peek() == '\t' )
    {
      cin.get() ;
    } // while
  } // Remove_White_Space()

  void Skip_White_Space( char &ch ) // 跳過所有空格直到讀到非空格字元為止，並回傳該字元 
  {
    while ( ch == ' ' || ch == '\t' ) 
    {
      ch = cin.get() ;
    } // while 
  } // Skip_White_Space()
  
  Information Get_Token() { // 開始讀入字元分類token並回傳 
            // 如果是字母開頭 後面可以是字母、數字、底線
            // 如果是數字開頭 那後面可以接數字和. .開頭+數字也視為數字 
            // 特殊符號 >= <= 等等 
            // 其餘的則不能當開頭 視為單一token處理 
            // 每切出一個token 就進interpret階段 
            
    char temp ;                  // 暫存讀入的字元
    char peek ;                  // 暫存下個要讀入的字元 連續判斷時需要用到 ex: 註解     
    bool decimal_point = false ; // 數字是否已經函小數點 小數點最多一個 ex: 1.2.3 (error)
    bool frist_isdot = false ;   // 第一個字元為"." 後面接數字視為 0.xx ex: .9 == 0.9  
    bool frist_char = true ;     // 是否為第一個字元
    bool finish_cut = false ;    // 是否切完此次token
    Information token ;     
    
    while ( !finish_cut ) // 切出一個token就停止 
    {
      temp = cin.get() ;
      if ( frist_char ) Skip_White_Space( temp ) ;
          
      if ( frist_char ) // 尚未決定第一個字元之種類 
      {
        if ( isalpha( temp ) ) // 首字為字母 
        {
          token.token_name = temp ;
          token.token_type = "id" ;
          frist_char = false ; 
        } // if
        else if ( isdigit( temp ) ) // 首字為數字 
        {
          decimal_point = false ;
          token.token_name = temp ;
          token.token_type = "num" ;
          frist_char = false ;
        } // else if
        else // 首字為符號 
        {
          if ( temp == '/' ) // 可能為註解 peek下一個如果也是'/'就整行88
          {
            peek = cin.peek() ;
            if ( peek == '/' ) // 確定為註解 88 
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
            else // 一般符號 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else
          } // if
          else if ( temp == '.' ) // 可能為小數 
          {
            peek = cin.peek() ;
            if ( isdigit( peek ) ) // 確定為小數 
            {
              token.token_name = ".";
              token.token_type = "num" ;
              frist_isdot = true ;
              frist_char = false ; 
            } // if
            else // 一般符號 
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
            else // 一般符號 
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
            else // 一般符號 
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
            else // 一般符號 
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
            else // 一般符號 
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
            else // 一般符號 
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
            else // 一般符號 
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
            else // 一般符號 
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
            else // 一般符號 
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
            else // 一般符號 
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
          else // 詞法錯誤 根本沒這字 
          {
            throw Lexical_Error( temp );
          } // else
        } // else
      } // if
      else // 已決定token第一個字元的種類 
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
        else if ( token.token_type == "id" ) // token為字母 
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
        else if ( token.token_type == "num" ) // token為數字
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
