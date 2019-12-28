#include "stringutils.h"

void StringUtils::split(std::list<std::string> &v, const std::string &s, const std::string &c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}
/*
void StringUtils::split(std::list<string> &splitList,const string &s, const string &seperator)
{
	typedef string::size_type string_size;

	string_size i = 0;

	while (i != s.size())
	{
		//�ҵ��ַ������׸������ڷָ�������ĸ��
		int flag = 0;
		string_size size = seperator.size();
		while (i != s.size() && flag == 0)
		{
			flag = 1;
			
			for (string_size x = 0; x < size; ++x) {
				if (s[i] == seperator[x])
				{
					++i;
					flag = 0;
					break;
				}
			}
		}
		//�ҵ���һ���ָ������������ָ���֮����ַ���ȡ����
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0)
		{
			for (string_size x = 0; x < size; ++x)
			{
				if (s[j] == seperator[x])
				{
					flag = 1;
					break;
				}
			}
			if (flag == 0)
			{
				++j;
			}
		}
		if (i != j)
		{
			splitList.push_back(s.substr(i, j - i));
			i = j;
		}
	}
}
*/
/*
bool StringUtils::trans(const char *pFromCode, const char *pToCode, const char *pInBuf, size_t iInLen, char *pOutBuf, size_t iOutLen)
{
	//���ַ���ת��
	iconv_t hIconv = iconv_open(pToCode, pFromCode);
	if (!hIconv) return false;
	//��ʼת��
	size_t iRet =  iconv(hIconv, (char **)(&pInBuf), &iInLen, &pOutBuf, &iOutLen);
	//�ر��ַ���ת��
	iconv_close(hIconv);
	return (bool)iRet;
}
*/
std::string &StringUtils::toLowerCase(string &src)
{
	transform(src.begin(), src.end(), src.begin(), ::tolower);
	return src;
}
std::string &StringUtils::toUpperCase(string &src)
{
	transform(src.begin(), src.end(), src.begin(), ::toupper);
	return src;
}
/*
std::string& StringUtils::toLowerCase(string &src)
{
	if (src.empty())
	{
		std::cout << "The string is empty().Can't do Such this function "<<__FUNCTION__<<" line:"<<__LINE__ << std::endl;
		src = "";
		return src;
	}
	int len = (int)src.size();
	for (int i = 0; i < len; i++)
	{
		char ch = src[i];
		if (ch >= 'A' && ch <= 'Z')
		{
			ch += 32;
		}
		src[i] = ch;
	}
	return src;
}

std::string& StringUtils::toUpperCase(string &src)
{
	if (src.empty())
	{
		std::cout << "The string is empty().Can't do Such this function "<<__FUNCTION__<<" line:"<<__LINE__ << std::endl;
		src = "";
		return src;
	}
	int len = (int)src.size();
	for (int i = 0; i < len; i++)
	{
		char ch = src[i];
		if (ch >= 'a' && ch <= 'z')
		{
			ch -= 32;
		}
		src[i] = ch;
	}
	return src;
}
*/
bool StringUtils::startsWith(const string &s, const string &sub)
{
	return s.find(sub) == 0 ? true : false;
}

bool StringUtils::endsWith(const string &s, const string &sub)
{
	return s.rfind(sub) == (s.size() - sub.size()) ? true : false;
}

std::string &StringUtils::replaceAll(string &old, char oldChar, char newChar)
{
	int oldSize = (int)old.size();
	for (int i = 0; i < oldSize; i++)
	{
		if (old[i] == oldChar)
		{
			old[i] = newChar;
		}
	}
	return old;
}
std::string &StringUtils::replaceAlls(string &old, const string &oldStr, const string &newStr)
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = oldStr.size();
	std::string::size_type dstlen = newStr.size();

	while ((pos = old.find(oldStr, pos)) != std::string::npos)
	{
		old.replace(pos, srclen, newStr);
		pos += dstlen;
	}
	return old;
}

std::string &StringUtils::trim(std::string &s)
{
	if (s.empty())
	{
		return s;
	}
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}
std::string &StringUtils::defaultstring(std::string &str)
{
	if (str.empty())
	{
		str = "";
	}
	return str;
}
bool StringUtils::stricmp(const char *dst, const char *src)
{
	int ch1, ch2;
	do
	{
		if (((ch1 = (unsigned char)(*(dst++))) >= 'A') && (ch1 <= 'Z'))
			ch1 += 0x20;
		if (((ch2 = (unsigned char)(*(src++))) >= 'A') && (ch2 <= 'Z'))
			ch2 += 0x20;
	} while (ch1 && (ch1 == ch2));
	return (ch1 - ch2) == 0 ? true : false;
}
int StringUtils::min(int num1, int num2)
{
	return num1 > num2 ? num2 : num1;
}
int StringUtils::max(int num1, int num2)
{
	return num1 > num2 ? num1 : num2;
}

void StringUtils::list2vector(vector<string> &result, list<string> &li)
{
	list<string>::iterator begin = li.begin();
	list<string>::iterator end = li.end();
	for (list<string>::iterator iter = begin; iter != end; ++iter)
	{
		result.push_back(*iter);
	}
}
