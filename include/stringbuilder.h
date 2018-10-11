#ifndef _StringBuilder_H_
#define _StringBuilder_H_

#include <list>
#include <cstring>
#include <numeric>
#include <iostream>
//日志文件
#include "./glog.h"
using namespace std;
// Subset of http://msdn.microsoft.com/en-us/library/system.text.stringbuilder.aspx
template <typename chr>
class StringBuilder
{
	typedef std::basic_string<chr> string_t;
	typedef std::list<string_t> container_t;						 // Reasons not to use vector below.
	typedef typename string_t::size_type size_type;					 // Reuse the size type in the string.
	typedef typename std::list<string_t>::const_iterator iter_t_con; //
	typedef typename std::list<string_t>::iterator iter_t;			 //
  private:
	container_t m_Data;
	size_type m_totalSize;
	void append(const string_t &src)
	{
		m_Data.push_back(src);
		m_totalSize += src.size();
	}

  public:
	// No copy constructor, no assignment.
	//	StringBuilder(const StringBuilder &);
	//	StringBuilder & operator = (StringBuilder & builder);
	explicit StringBuilder(const string_t &src)
	{
		if (!src.empty())
		{
			m_Data.push_back(src);
		}
		m_totalSize = src.size();
	}
	StringBuilder()
	{
		m_totalSize = 0;
	}
	void setSize(size_type size)
	{
		m_totalSize = size;
	}
	StringBuilder &setlength(int len)
	{
		if (len < (int)m_totalSize)
		{
			Delete(len, m_totalSize);
		}
		return *this;
	}

	// TODO: Constructor that takes an array of strings.
	size_type size()
	{
		return m_totalSize;
	}
	string &ssub(string &str, int start, int end)
	{
		string temp;
		int length = str.size();
		if (start > end)
		{
			//std::cout<<"err at ssub(string &str,int start,int end):args. start cannot be over end"<<std::endl;
			Log().log().setLevel(LOG_ERR_LEVEL).format("Err arg: start can't be over end [at FILE:%s FUNC:%s LINE:%d]", __FILE__, __FUNCTION__, __LINE__).toFile();
			return str;
		}
		else if (start == end) //do nothing
		{
			return str;
		}
		end = end > length ? length : end;
		temp = str.substr(end);
		str = str.substr(0, start) + temp;

		//		std::cout<<"sub str:"<<str<<std::endl;
		return str;
	}
	StringBuilder &Delete(int start, int end)
	{
		int p_start = 0, p_end = -1;
		/*
		if ((start > end) || (end > (int)m_totalSize))
		{
			//std::cout << "err at delete(int start,int end) const args:(start can'over end or start can't over the str len)" << std::endl;
			//日志输出
			Log().log().setLevel(LOG_ERR_LEVEL).format("Err arg: start can't be over end or start can't be over the str length [at FILE:%s FUNC:%s LINE:%d]", __FILE__, __FUNCTION__, __LINE__).toFile();
			return *this;
		}
		*/
		end = end > (int)m_totalSize ? m_totalSize : end;
		m_totalSize -= (end - start);
		iter_t m_begin = m_Data.begin();
		iter_t m_end = m_Data.end();
		for (iter_t iter = m_begin; iter != m_end; ++iter)
		{
			p_end += iter->size();
			if (start >= p_start && start <= p_end)
			{
				// string::iterator begin = iter->begin():
				if (end <= p_end)
				{
					ssub(*iter, start - p_start, end - p_start);
					// iter->erase(begin+start-p_start,begin+end-p_start);
					break;
				}
				else
				{
					ssub(*iter, start - p_start, iter->size());
					//iter->erase(begin+start-p_start,begin+iter->size());
					start = p_end + 1;
				}
			}
			p_start = p_end + 1;
		}

		return *this;
	}
	StringBuilder &DeleteCharAt(int index)
	{
		if (index > (int)m_totalSize)
		{
			return *this;
		}
		else
		{
			int str_start = 0, str_end = -1;
			iter_t begin(m_Data.begin()), end(m_Data.end());
			for (iter_t iter = begin; iter != end; ++iter)
			{
				str_end += iter->size();
				if (index >= str_start && index <= str_end)
				{
					iter->erase(index - str_start, 1);
					m_totalSize -= 1;
					break;
				}
				str_start = str_end + 1;
			}
		}
		return *this;
	}
	char charAt(int index) const
	{
		string dest;
		/*
		if (index < 0 || (index > (int)m_totalSize))
		{
			//	std::cout << "err at charAt(int index) const args:(index can't over len)" << std::endl;
			Log().log().setLevel(LOG_ERR_LEVEL).format("Err arg: index can't be over the str length. [at FILE:%s FUNC:%s LINE:%d]", __FILE__, __FUNCTION__, __LINE__).toFile();
			return '\0';
		}
		*/
		dest = ToString().c_str();
		return dest[index];
	}
	StringBuilder &Append(const string_t &src)
	{
		append(src);
		return *this; // allow chaining.
	}
	StringBuilder &Append(const char &ch)
	{
		string temp;
		temp = temp + ch;
		append(temp);
		return *this;
	}
	StringBuilder &insert(int ops, char c)
	{

		if (ops > (int)m_totalSize)
		{
			return *this;
		}
		else
		{
			int str_start = 0, str_end = -1;
			iter_t begin(m_Data.begin()), end(m_Data.end()), iter;
			for (iter = begin; iter != end; ++iter)
			{
				string temp = *iter;
				str_end += temp.size();
				if (ops >= str_start && ops <= str_end)
				{
					iter->insert(ops, 1, c);
					m_totalSize += 1;
					break;
				}
				str_start = str_end + 1;
			}
		}
		return *this;
	}
	/*
	StringBuilder & replace1(int start, int end, string str)
	{
		Delete1(start, end);
		int i = 0;
		for (;i < (int) str.size();)
		{
			insert(start + i, str[i]);
			++i;
		}
		return *this;
	}
	*/
	StringBuilder &replace(int start, int end, string str)
	{
		Delete(start, end);
		if (start > (int)m_totalSize)
		{
			return *this;
		}
		else
		{
			int str_start = 0, str_end = -1;
			iter_t m_begin(m_Data.begin()), m_end(m_Data.end()), iter;
			for (iter = m_begin; iter != m_end; ++iter)
			{
				str_end += iter->size();
				if (start >= str_start && start <= str_end)
				{
					iter->insert(start - str_start + 1, str);
					m_totalSize += str.size();
					break;
				}
				str_start = str_end + 1;
			}
		}
		return *this;
	}
	string substr(int start)
	{
		return substr(start, m_totalSize);
	}
	// This one lets you add any STL container to the string builder.
	string substr(int start, int end) const
	{
		/*
		if (start<0 || start > (int)m_totalSize || start > end) {
			std::cout << "err at string_t substr(int start) const args:(start can't below zero)" << std::endl;
			return "";
		}*/
		/*
		if (start > (int)m_totalSize) {
			std::cout << "err at string_t substr(int start) const args:(start can't over the string len)" << std::endl;
			return "";
		}
		if (start > end) {
			std::cout << "err at string_t substr(int start) const args:(start can't over the end)" << std::endl;
			return "";
		}
		if (end > (int)m_totalSize) {
			end = m_totalSize;
		}
		*/
		end = end > (int)m_totalSize ? m_totalSize : end;
		return ToString().substr(start, end - start);
	}

	// TODO: AppendFormat implementation. Not relevant for the article.

	// Like C# StringBuilder.ToString()
	// Note the use of reserve() to avoid reallocations.
	string ToString() const
	{
		string result;
		// The whole point of the exercise!
		// container has a lot of strings, reallocation (each time the result grows) will take a serious toll,
		// both in performance and chances of failure.
		// I measured (in code I cannot publish) fractions of a second using 'reserve', and almost two minutes using +=.
		result.reserve(m_totalSize + 1);
		// result = std::accumulate(m_Data.begin(), m_Data.end(), result); // This would lose the advantage of 'reserve'
		iter_t_con begin = m_Data.begin();
		iter_t_con end = m_Data.end();
		for (iter_t_con iter = begin; iter != end; ++iter)
		{
			result += *iter;
		}
		return result;
	}
}; // class StringBuilder

#endif // !_StringBuilder_H_
