#ifndef _StringBuilder_H_
#define _StringBuilder_H_

#include <list>
#include <cstring>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <sstream>

// Subset of
// http://msdn.microsoft.com/en-us/library/system.text.stringbuilder.aspx
template <typename chr>
class MyStringBuilderImpl {
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
    void Append(const string_t &src) {
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
public: //constructor
    // No copy constructor, no assignment.
    //StringBuilder(const StringBuilder &)=delete;
    MyStringBuilderImpl & operator = (MyStringBuilderImpl & builder)=delete;

    MyStringBuilderImpl(const string_t &src):m_nprecision(15) {
        if (!src.empty()) {
            m_Data.push_back(src);
        }
        m_totalSize = src.size();
    }

    MyStringBuilderImpl():m_totalSize(0),m_nprecision(15) {
    }

public:

    // TODO: Constructor that takes an array of strings.
    size_type size() const {
        return m_totalSize;
    }

    size_type length() const {
        return m_totalSize;
    }

    chr charAt(int index) const {
        if(static_cast<size_type>(index) > m_totalSize)
            return chr();
        string_t dest = toString();
        return dest[index];
    }
    
public: // insert delete remove clear append
    void clear(){ //clear all
        m_Data.clear();
        m_totalSize = 0;
    }

    MyStringBuilderImpl& setSize(size_type size) {
        return setLength(size);
    }

    MyStringBuilderImpl &setLength(size_type len) {
        if(len == 0){// remove all
            clear();
            return *this;
        }
        if (len < m_totalSize) {
            remove(len, m_totalSize);
        }
        return *this;
    }

    string_t &subprocess(string_t &str, int start, int end) {
        string_t temp;
        int length = str.size();
        if (start > end) {
            return str;
        } else if (start == end){  // do nothing
            return str;
        }
        end = end > length ? length : end;
        temp = str.substr(end);
        str = str.substr(0, start) + temp;

        return str;
    }

    MyStringBuilderImpl &remove(int start, int end) {
        int p_start = 0, p_end = -1;
        end = static_cast<size_type>(end) > m_totalSize ? m_totalSize : end;
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

    MyStringBuilderImpl &removeCharAt(int index) {
        if (static_cast<size_type>(index) > m_totalSize) {
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

    template<class ArgType>
    MyStringBuilderImpl &append(const ArgType & src){
        Append(toString(src));
        return *this;
    }

    MyStringBuilderImpl &insert(int ops, chr c) {
        if (ops > (int)m_totalSize) {
            return *this;
        } else {
            int str_start = 0, str_end = -1;
            iter_t begin(m_Data.begin()), end(m_Data.end()), iter;
            bool bInserted = false;
            for (iter = begin; iter != end; ++iter) {
                string_t temp = *iter;
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
    MyStringBuilderImpl & replace(int start, int end, string_t str)
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

    MyStringBuilderImpl &replace(int start, int end,const string_t &str) {
        remove(start, end);
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

    string_t substr(int start) { return substr(start, m_totalSize); }

    // This one lets you add any STL container to the string builder.
    string_t substr(int start, int end) const {
        end = end > (int)m_totalSize ? m_totalSize : end;
        return toString().substr(start, end - start);
    }

    // TODO: AppendFormat implementation. Not relevant for the article.
    // Note the use of reserve() to avoid reallocations.
    string_t toString(void) const {
        string_t result;
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

typedef MyStringBuilderImpl<char> MyStringBuilder;

#endif  // !_StringBuilder_H_
