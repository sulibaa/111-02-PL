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
# include  <iomanip>
# include  <sstream>
# include  <cctype>
# include <exception>
# include <string.h>

using namespace std;

struct Information // 每個token的資料 
{
  string token_name ;
  string token_type ;
};

struct Reg // 被定義的token資料 ex: salary := 3000 
{
  string token_name ;
  float value ;
  bool boolean ;
};

class Data 
{
public:
  vector<Information> mToken_line ; // 存token直到分號的vector
  bool mSemicolon ;                 // 判斷是否讀到分號 
  bool mQuit ;                      // 判斷是否讀到quit 

  Data() // 初始化 
  {
    mQuit = false ;  
  } // Data()
    
  void Remove_White_Space() // 跳過所有空格直到讀到非空格字元為止
  {
    while ( cin.peek() == ' ' || cin.peek() == '\t' || cin.peek() == '\n' )
    {
      cin.get() ;
    } // while
  } // Remove_White_Space()

  void Skip_White_Space( char &ch ) // 跳過所有空格直到讀到非空格字元為止，並回傳該字元 
  {
    while ( ch == ' ' || ch == '\n' || ch == '\t' ) 
    {
      ch = cin.get() ;
    } // while 
  } // Skip_White_Space()
  
  void Gettoken_Result_Print() // 將分類完成的token顯示 (using for test)
  {
    cout << "the testbanch: " ;
    for ( int i = 0 ; i < mToken_line.size() ; i++ )
    {
      cout << mToken_line[i].token_name << " " ;  
    } // for

    cout << endl ; 
  } // Gettoken_Result_Print()
  
  void Get_Token() { // 開始讀入字元並分類成數個token 
            // 如果是字母開頭 後面可以是字母、數字、底線
            // 如果是數字開頭 那後面可以接數字和. 
            // 因會有小數之疑慮數字一律設定為 float 
            // 其餘的則不能當開頭 視為單一token處理 
            
    char temp ;                  // 暫存讀入的字元
    char peek ;                  // 暫存下個要讀入的字元 連續判斷時需要用到 ex: 註解 
    Information token ;          // 將字元整合 完成後放進token_line中 
    bool frist_char = true ;     // 是否為第一個字元  
    bool frist_token = true ;    // 是否為第一個token 判斷quit是不是真的要quit 
    bool decimal_point = false ; // 數字是否已經函小數點 小數點最多一個 ex: 1.2.3 (error)
    bool frist_isdot = false ;   // 第一個字元為"." 後面接數字視為 0.xx ex: .9 == 0.9   
    
    while ( !mSemicolon && !mQuit ) // 到分號 or quit 即停止 
    {
      temp = cin.get() ;
      Skip_White_Space( temp ) ;
    
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
            Remove_White_Space() ;
            peek = cin.peek() ;
            if ( peek == '/' ) // 確定為註解 88 
            {
              char command_temp[128] ;
              cin.getline( command_temp, 128 ) ;
            } // if
            else // 一般符號 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              mToken_line.push_back( token ) ;
              frist_char = true ;
              frist_token = false ;  
            } // else
          } // if
          else if ( temp == ';' ) // 哈哈是分號拉 此行結束 
          {
            mSemicolon = true ;
          } // else if
          else if ( temp == '.' ) // 
          {
            
          } // else if
          else // 一般符號 
          {
            token.token_name = temp ;
            token.token_type = "sign" ;
            mToken_line.push_back( token ) ;
            frist_char = true ;
            frist_token = false ;  
          } // else
        } // else
      } // if
      else // 已決定token第一個字元的種類 
      {
        if ( token.token_type == "id" ) // token為字母 
        {
          if ( isalpha( temp ) || isdigit( temp ) || temp == '_' )
          {
            token.token_name += temp ;
          } // if 
          else 
          {
            mToken_line.push_back( token ) ;
            cin.putback( temp ) ;
            frist_char = true ;
            frist_token = false ;  
          } // else
        } // if
        else if ( token.token_type == "num" ) // token為數字
        {
          if ( isdigit( temp ) || ( temp == '.' && !decimal_point ) )
          {
            if ( temp == '.' ) decimal_point = true ;
            token.token_name += temp ;
          } // if
          else 
          {
            if (  )
            mToken_line.push_back( token ) ;
            cin.putback( temp ) ;
            frist_char = true ;
            frist_token = false ;  
          } // else
        } // else if 
      } // else 
    
      if ( token.token_name == "quit" && frist_token ) mQuit = true ; // 遇到quit拉 高歌離席 

    } // while
  
    if ( !mQuit ) {
      Gettoken_Result_Print() ;
    } // if
  } // Get_Token()

};
/*
class Interpret
{
public:
  vector<Information> mToken_line ; // 存token直到分號的vector ( 從data而來
  int mtoken_counter ;              // 檢查到第幾個token 
  bool mcontinue ;                  // 是否繼續執行解譯
                                    // 遇到error將改成false 
                                    // print出錯誤資訊 直接結束 
  string implement_type ;           // 要執行哪種類型 ex: 1+2 -> math 
                                    // 1>0 -> boolean, a := 1 -> ident 

  Interpret()
  { 
    mcontinue = true ;
  } // Interpret()

  bool Is_sign( string str )
  {
    if( str == "+" || str == "-" ) return true ;
    else return false ;
  } // Is_sign()

  bool Unrecognized( string s ) // 有無詞法錯誤 
  {
    if ( s == "+" || s == "-" || s == "*" || s == "/" || s == "=" || s == "(" ||
         s == ")" || s == "<" || s == ">" || s == ":" )
    {
      return false ;
    } // if
    else 
    {
      Unrecognized_token( s ) ;
      return true ;
    } // else

  } // Unrecognized()
 
  void Unrecognized_token( string str ) // 詞法錯誤 ex : 2 + $ - 5
  {                                     // Unrecognized token with first char : '$'
    cout << "Unrecognized token with first char : '" << str << "'" << endl ;
    mcontinue = false ;  
  } // Unrecognized_token()

  void Unexpected_token( string str ) // 語法錯誤 ex : 2 + * - 5 
  {                                   // Unexpected token : '*'
    cout << "Unexpected token : '" << str << "'" << endl ;  
  } // Unexpected_token()

  void Undefined_identifier( string str ) // 語義錯誤 ex : bcd * 2 (尚未宣告bcd) 
  {                                       // Undefined identifier : 'bcd'
                                          // 文法無錯誤
    cout << "Undefined identifier : '" << str << "'" << endl ;  
  } // Undefined_identifier()

  void Command( vector<Information> &mToken_line )
  {
    int mtoekn_counter = 0 ;

    for ( int i = 0 ; i < mToken_line.size() ; i++ ) // 先檢查此行token是否含有詞法錯誤 
    {
      if ( mToken_line[i].token_type == "sign" ) 
        Unrecognized( mToken_line[i].token_name ) ;
    } // for

    if ( mToken_line[0].token_type == "id" && mToken_line[1].token_name == ":" && 
         mToken_line[2].token_name == "=" ) // <command> -> ident 
    {
      cout << "it a ident" << endl ;
      if( mcontinue ) ArithExp();
      if( mcontinue ) IDlessArithExpOrBexp();
    } // if
    else // <command> -> 非ident 
    {
      cout << "it not a ident" << endl;

      if ( mcontinue ) NOT_ID_StartArithExpOrBexp() ;
    } // else 

     
  } // Command() 

  
  //do_recursive -> 判斷只要執行後面的部分
  //需要執行遞迴但不重複執行該function非必要的部分
  //ex: NOT_ID_StartArithExp ::= <...> {...|...}
  //需要遞迴的部分只有{...|...}的部分
  //此情況將do_recursive = true 

  void IDlessArithExpOrBexp()
  {
  
  } // IDlessArithExpOrBexp()
  
  void BooleanOperator() 
  {
     
  } // BooleanOperator()

  void NOT_ID_StartArithExpOrBexp()
  {
    if( mcontinue ) NOT_ID_StartArithExp() ;
    if( mcontinue ) BooleanOperator() ;
    if( mcontinue ) ArithExp() ;

  } // NOT_IDStartArithExpOrBexp()

  void NOT_ID_StartArithExp( bool do_recursive )  
  {                           
    if( mcontinue && !do_recursive ) NOT_ID_StartTerm() ;
    if( mcontinue )
    {
      if ( mtoken_line[mtoken_counter].token_name == "+" ||
           mtoken_line[mtoken_counter].token_name == "-" )
      {   
        mtoken_counter++ ;
        Term( false ) ;
        if( mcontinue ) NOT_ID_StartArithExp( true ) ;
      } // if
    } // if 
  } // NOT_ID_StartArithExp()

  void NOT_ID_StartTerm()
  {
  
  } // NOT_ID_StartTerm()

  void NOT_ID_StartFactor()
  {
  
  } // NOT_ID_StartFactor()

  void ArithExp()
  {
  
  } // ArithExp()

  void Term( bool do_recursive )
  {
    if( mcontinue && !do_recursive ) Factor() ;
    if( mcontinue )
    {
      if ( mtoken_line[mtoken_counter].token_name == "*" ||
           mtoken_line[mtoken_counter].token_name == "/" )
      {   
        mtoken_counter++ ;
        Factor() ;
        if( mcontinue ) Term( true ) ;
      } // if
    } // if 
  } // Term()

  void Factor()
  {
    if ( mcontinue )
    {
      if ( mToken_line[mtoken_counter].token_type == "id" ||
           mToken_line[mtoken_counter].token_type == "num" ||
           Is_sign( mToken_line[mtoken_counter].token_name ) ||
           mToken_line[mtoken_counter].token_name == "(" )
      {
           



      } // if
    } // if
  } // Factor()

  void Implement() // 確認無錯誤後開始執行 
  {
    cout << "> not finish yet~~" << endl ;
  } // Implement()
};
*/
int main() 
{
  int testnum ;
  cin >> testnum ;
  cout << "Program starts..." << endl ;
  Data data ;
  //Interpret interpret ;
    
  while ( !data.mQuit )
  {
    cout << "> enter the input : " ;
    data.mToken_line.clear() ;
    data.mSemicolon = false ;
    data.Get_Token() ;
    //if ( !data.mQuit ) interpret.Command( data.mToken_line ) ;
  } // while
  
  cout << ">Program exits..." ;


} // main() 
