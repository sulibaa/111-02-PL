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

struct Information // �C��token����� 
{
  string token_name ;
  string token_type ;
};

struct Reg // �Q�w�q��token��� ex: salary := 3000 
{
  string token_name ;
  float value ;
  bool boolean ;
};

class Data 
{
public:
  vector<Information> mToken_line ; // �stoken���������vector
  bool mSemicolon ;                 // �P�_�O�_Ū����� 
  bool mQuit ;                      // �P�_�O�_Ū��quit 

  Data() // ��l�� 
  {
    mQuit = false ;  
  } // Data()
    
  void Remove_White_Space() // ���L�Ҧ��Ů檽��Ū��D�Ů�r������
  {
    while ( cin.peek() == ' ' || cin.peek() == '\t' || cin.peek() == '\n' )
    {
      cin.get() ;
    } // while
  } // Remove_White_Space()

  void Skip_White_Space( char &ch ) // ���L�Ҧ��Ů檽��Ū��D�Ů�r������A�æ^�ǸӦr�� 
  {
    while ( ch == ' ' || ch == '\n' || ch == '\t' ) 
    {
      ch = cin.get() ;
    } // while 
  } // Skip_White_Space()
  
  void Gettoken_Result_Print() // �N����������token��� (using for test)
  {
    cout << "the testbanch: " ;
    for ( int i = 0 ; i < mToken_line.size() ; i++ )
    {
      cout << mToken_line[i].token_name << " " ;  
    } // for

    cout << endl ; 
  } // Gettoken_Result_Print()
  
  void Get_Token() { // �}�lŪ�J�r���ä������ƭ�token 
            // �p�G�O�r���}�Y �᭱�i�H�O�r���B�Ʀr�B���u
            // �p�G�O�Ʀr�}�Y ���᭱�i�H���Ʀr�M. 
            // �]�|���p�Ƥ��ü{�Ʀr�@�߳]�w�� float 
            // ��l���h�����}�Y ������@token�B�z 
            
    char temp ;                  // �ȦsŪ�J���r��
    char peek ;                  // �Ȧs�U�ӭnŪ�J���r�� �s��P�_�ɻݭn�Ψ� ex: ���� 
    Information token ;          // �N�r����X �������itoken_line�� 
    bool frist_char = true ;     // �O�_���Ĥ@�Ӧr��  
    bool frist_token = true ;    // �O�_���Ĥ@��token �P�_quit�O���O�u���nquit 
    bool decimal_point = false ; // �Ʀr�O�_�w�g��p���I �p���I�̦h�@�� ex: 1.2.3 (error)
    bool frist_isdot = false ;   // �Ĥ@�Ӧr����"." �᭱���Ʀr���� 0.xx ex: .9 == 0.9   
    
    while ( !mSemicolon && !mQuit ) // ����� or quit �Y���� 
    {
      temp = cin.get() ;
      Skip_White_Space( temp ) ;
    
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
            Remove_White_Space() ;
            peek = cin.peek() ;
            if ( peek == '/' ) // �T�w������ 88 
            {
              char command_temp[128] ;
              cin.getline( command_temp, 128 ) ;
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              mToken_line.push_back( token ) ;
              frist_char = true ;
              frist_token = false ;  
            } // else
          } // if
          else if ( temp == ';' ) // �����O������ ���浲�� 
          {
            mSemicolon = true ;
          } // else if
          else if ( temp == '.' ) // 
          {
            
          } // else if
          else // �@��Ÿ� 
          {
            token.token_name = temp ;
            token.token_type = "sign" ;
            mToken_line.push_back( token ) ;
            frist_char = true ;
            frist_token = false ;  
          } // else
        } // else
      } // if
      else // �w�M�wtoken�Ĥ@�Ӧr�������� 
      {
        if ( token.token_type == "id" ) // token���r�� 
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
        else if ( token.token_type == "num" ) // token���Ʀr
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
    
      if ( token.token_name == "quit" && frist_token ) mQuit = true ; // �J��quit�� ���q���u 

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
  vector<Information> mToken_line ; // �stoken���������vector ( �qdata�Ө�
  int mtoken_counter ;              // �ˬd��ĴX��token 
  bool mcontinue ;                  // �O�_�~������Ķ
                                    // �J��error�N�令false 
                                    // print�X���~��T �������� 
  string implement_type ;           // �n����������� ex: 1+2 -> math 
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

  bool Unrecognized( string s ) // ���L���k���~ 
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
 
  void Unrecognized_token( string str ) // ���k���~ ex : 2 + $ - 5
  {                                     // Unrecognized token with first char : '$'
    cout << "Unrecognized token with first char : '" << str << "'" << endl ;
    mcontinue = false ;  
  } // Unrecognized_token()

  void Unexpected_token( string str ) // �y�k���~ ex : 2 + * - 5 
  {                                   // Unexpected token : '*'
    cout << "Unexpected token : '" << str << "'" << endl ;  
  } // Unexpected_token()

  void Undefined_identifier( string str ) // �y�q���~ ex : bcd * 2 (�|���ŧibcd) 
  {                                       // Undefined identifier : 'bcd'
                                          // ��k�L���~
    cout << "Undefined identifier : '" << str << "'" << endl ;  
  } // Undefined_identifier()

  void Command( vector<Information> &mToken_line )
  {
    int mtoekn_counter = 0 ;

    for ( int i = 0 ; i < mToken_line.size() ; i++ ) // ���ˬd����token�O�_�t�����k���~ 
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
    else // <command> -> �Dident 
    {
      cout << "it not a ident" << endl;

      if ( mcontinue ) NOT_ID_StartArithExpOrBexp() ;
    } // else 

     
  } // Command() 

  
  //do_recursive -> �P�_�u�n����᭱������
  //�ݭn���滼�j�������ư����function�D���n������
  //ex: NOT_ID_StartArithExp ::= <...> {...|...}
  //�ݭn���j�������u��{...|...}������
  //�����p�Ndo_recursive = true 

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

  void Implement() // �T�{�L���~��}�l���� 
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
