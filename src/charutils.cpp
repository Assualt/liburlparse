#include "charutils.h"
using namespace std;
CharUtils::CharUtils(){}

//Check if Character is a valid hex character.
  bool CharUtils::isHex(char a)
{
	return (a >= '0' && a <= '9') || (a >= 'a' && a <= 'f') || (a >= 'A' && a <= 'F');
}

//Check if character is a valid alphabetic character.
  bool CharUtils::isAlpha(char a)
{
	return ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'));
}

//Checks if character is a valid numeric character. 
 bool CharUtils::isNumberic(char a)
{
	return a >= '0' && a <= '9';
}

//Checks if character is a valid alphanumeric character.
 bool CharUtils::isAlphaNumberic(char a)
{
	return isAlpha(a) || isNumberic(a);
}

//Checks if character is a valid unreserved character. This is defined by the RFC 3986 ABNF
 bool CharUtils::isUnreserved(char a)
{
	return isAlphaNumberic(a) || a == '-' || a == '.' || a == '_' || a == '~';
}

//Checks if character is a dot
 bool CharUtils::isDot(char a)
{	//判断是否为 . 。．｡  (半角和全角的点号和句号) 此处编译会出现警告 

	return (a == '.' || (int)a == 0x3002 || (int) a == 0xFF0E || (int)a == 0xFF61);
}
 bool CharUtils::isWhiteSpace(char a)
{
	return (a == '\n' || a == '\t' || a == '\r' || a == ' ');
}
/*
   	Splite a Strubf without the use of a regex,which could split by isDot() or %2e
   	@param input the input string which can be split by dot
   	@return an arry of Strings that is a patition of the original string by dot
*/
void CharUtils::splitByDot(list<string> &splitList,string &input)
{
	StringBuilder<char> section;
	if (input.empty())
	{
		//打印日志
		//std::cout<<"input string is empty!"<<__FUNCTION__<<" "<<__LINE__<<std::endl;
		//Log().log().setLevel(LOG_DEBUG_LEVEL).format("Input string is empty.[at FILE:%s FUNC:%s LINE:%d]", __FILE__, __FUNCTION__, __LINE__).toFile();
		return ;
	}
	InputTextReader reader(input);
	while (!reader.eof())
	{
		char current = reader.read();
		if (isDot(current))
		{
			splitList.push_back(section.ToString());
			section.Delete(0,section.size());
		}
		else if (current == '%' && reader.canReadChars(2) && StringUtils::stricmp(reader.peek(2).c_str(),(const char *)"2e"))
		{
			reader.read();
			reader.read();//advance past the 2e
			section.Delete(0,section.size());
		}
		else 
		{
			section.Append(current);//加到尾部
		}
	}
	splitList.push_back(section.ToString());//加到尾部
}