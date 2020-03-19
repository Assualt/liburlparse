#ifndef _StringBuilder_H_
#define _StringBuilder_H_

#include <list>
#include <cstring>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "glog.h"
using namespace std;
// Subset of
// http://msdn.microsoft.com/en-us/library/system.text.stringbuilder.aspx
template <typename chr>
class StringBuilder {
    typedef std::basic_string<chr> string_t;
    typedef std::list<string_t> container_t;  // Reasons not to use vector below.
    typedef typename string_t::size_type size_type;  // Reuse the size type in the string.
    typedef typename std::basic_stringstream<chr> my_stringstream;    // convert the any type of data to string_t 
    typedef typename std::list<string_t>::const_iterator iter_t_con;  //
    typedef typename std::list<string_t>::iterator iter_t;            //
private:
    container_t m_Data;
    size_type m_totalSize;
    size_t m_nprecision;
private:    
    void append(const string_t &src) {
        m_Data.push_back(src);
        m_totalSize += src.size();
    }
    template <class argType>
    string_t toString(const argType &ar) {
        my_stringstream stream;
        stream << ar;
        return stream.str();
    }
    string_t toString(const float &arg){
        my_stringstream stream;
        stream << std::setprecision(m_nprecision) << arg;
        return stream.str();
    }
    string_t toString(const double &arg){
        my_stringstream stream;
        stream << std::setprecision(m_nprecision) << arg;
        return stream.str();
    }
public:
    // No copy constructor, no assignment.
    //StringBuilder(const StringBuilder &)=delete;
    // StringBuilder & operator = (StringBuilder & builder)=delete;
    StringBuilder(const string_t &src):m_nprecision(15) {
        if (!src.empty()) {
            m_Data.push_back(src);
        }
        m_totalSize = src.size();
    }
    StringBuilder():m_totalSize(0),m_nprecision(15) {
    }
    StringBuilder& setSize(size_type size) {
        return setlength(size);
    }
    StringBuilder &setlength(size_type len) {
        if(len == 0){// remove all
            m_Data.clear();
            m_totalSize = 0;
            return *this;
        }
        if (len < m_totalSize) {
            Delete(len, m_totalSize);
        }
        return *this;
    }

    // TODO: Constructor that takes an array of strings.
    size_type size() {
        return m_totalSize;
    }
    string &subprocess(string &str, int start, int end) {
        string temp;
        int length = str.size();
        if (start > end) {
            Log().log().setLevel(LOG_ERR_LEVEL).format("Err arg: start can't be over end [at FILE:%s "
                "FUNC:%s LINE:%d]", __FILE__,__FUNCTION__,__LINE__).toFile();
            return str;
        } else if (start == end){  // do nothing
            return str;
        }
        end = end > length ? length : end;
        temp = str.substr(end);
        str = str.substr(0, start) + temp;

        return str;
    }
    StringBuilder &Delete(int start, int end) {
        int p_start = 0, p_end = -1;
        end = end > (int)m_totalSize ? m_totalSize : end;
        m_totalSize -= (end - start);
        iter_t m_begin = m_Data.begin();
        iter_t m_end = m_Data.end();
        for (iter_t iter = m_begin; iter != m_end; ++iter) {
            p_end += iter->size();
            if (start >= p_start && start <= p_end) {
                // string::iterator begin = iter->begin():
                if (end <= p_end) {
                    subprocess(*iter, start - p_start, end - p_start);
                    // iter->erase(begin+start-p_start,begin+end-p_start);
                    break;
                } else {
                    subprocess(*iter, start - p_start, iter->size());
                    // iter->erase(begin+start-p_start,begin+iter->size());
                    start = p_end + 1;
                }
            }
            p_start = p_end + 1;
        }

        return *this;
    }
    StringBuilder &DeleteCharAt(int index) {
        if (index > (int)m_totalSize) {
            return *this;
        } else {
            int str_start = 0, str_end = -1;
            iter_t begin(m_Data.begin()), end(m_Data.end());
            for (iter_t iter = begin; iter != end; ++iter) {
                str_end += iter->size();
                if (index >= str_start && index <= str_end) {
                    iter->erase(index - str_start, 1);
                    m_totalSize -= 1;
                    break;
                }
                str_start = str_end + 1;
            }
        }
        return *this;
    }
    chr charAt(int index) const {
        string dest;
        dest = ToString().c_str();
        return dest[index];
    }
    template<class ArgType>
    StringBuilder &Append(const ArgType & src){
        append(toString(src));
        return *this;
    }
    StringBuilder &Append(const chr c){
        string_t temp(1,c);
        append(temp);
        return *this;
    }
    StringBuilder &insert(int ops, chr c) {
        if (ops > (int)m_totalSize) {
            return *this;
        } else {
            int str_start = 0, str_end = -1;
            iter_t begin(m_Data.begin()), end(m_Data.end()), iter;
            bool bInserted = false;
            for (iter = begin; iter != end; ++iter) {
                string temp = *iter;
                str_end += temp.size();
                if (ops >= str_start && ops <= str_end) {
                    bInserted = true;
                    iter->insert(ops, 1, c);
                    m_totalSize += 1;
                    break;
                }
                str_start = str_end + 1;
            }
            if(!bInserted){
                m_totalSize += 1;
                begin = m_Data.begin();
                begin->push_back(c);
            }
        }
        return *this;
    }
    /*
     *  it apply the repeat to replace and cost a lot of time use
    StringBuilder & replace(int start, int end, string str)
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
    StringBuilder &replace(int start, int end, string str) {
        Delete(start, end);
        if (start > (int)m_totalSize) {
            return *this;
        } else {
            int str_start = 0, str_end = -1;
            iter_t m_begin(m_Data.begin()), m_end(m_Data.end()), iter;
            for (iter = m_begin; iter != m_end; ++iter) {
                str_end += iter->size();
                if (start >= str_start && start <= str_end) {
                    iter->insert(start - str_start + 1, str);
                    m_totalSize += str.size();
                    break;
                }
                str_start = str_end + 1;
            }
        }
        return *this;
    }
    string substr(int start) {
        return substr(start, m_totalSize);
    }
    // This one lets you add any STL container to the string builder.
    string substr(int start, int end) const {
        end = end > (int)m_totalSize ? m_totalSize : end;
        return ToString().substr(start, end - start);
    }

    // TODO: AppendFormat implementation. Not relevant for the article.

    // Like C# StringBuilder.ToString()
    // Note the use of reserve() to avoid reallocations.
    string ToString() const {
        string result;
        // The whole point of the exercise!
        // container has a lot of strings, reallocation (each time the result
        // grows) will take a serious toll, both in performance and chances of
        // failure. I measured (in code I cannot publish) fractions of a second
        // using 'reserve', and almost two minutes using +=.
        result.reserve(m_totalSize + 1);
        // result = std::accumulate(m_Data.begin(), m_Data.end(), result); //
        // This would lose the advantage of 'reserve'
        iter_t_con begin = m_Data.begin();
        iter_t_con end = m_Data.end();
        for (iter_t_con iter = begin; iter != end; ++iter) {
            result += *iter;
        }
        return result;
    }
};  // class StringBuilder

#endif  // !_StringBuilder_H_
