/*************************************************************************
        > File Name: stringbuilder_1.h
        > Author:
        > Mail:
        > Created Time: 2020年01月06日 星期一 16时02分19秒
 ************************************************************************/

#ifndef _STRINGBUILDER_1_H
#define _STRINGBUILDER_1_H

#include <list>
#include <string.h>
#include <numeric>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include "glog.h"
using namespace std;
template <typename chr>
class StringBuilderImpl {
    typedef typename std::basic_string<chr> string_t;
    typedef typename std::basic_stringstream<chr> my_stringstream;
    typedef typename string_t::size_type size_type;

public: 
    /**
     * static method to provide to replace the copy/assign
     * function. and avoid the warning message of valgrind
     * like "Invalid free()/delete()/malloc()/new() "
     * happending
     *
     * @param : StringBuilderImpl
     * @return: StringBuilderImpl 
     */
    static StringBuilderImpl& copyOf(const StringBuilderImpl& src, StringBuilderImpl& dst){
        dst.setSize(src.capacity());
        dst.append(src.toString());
        return dst;
    }
public:
    // delete the default function due to free double
    StringBuilderImpl operator=(const StringBuilderImpl& other)=delete;
    /**
     * the default construcor and init the 
     * total size and capacity and set precision 
     * of double/float and append current val to 
     * the inner data
     *
     */
    StringBuilderImpl(const string_t &src) :
            m_Data(nullptr),
            m_nTotalSize(0),
            m_nCapacity(16),
            m_nPrecision(15) {
        ExpandCapacity(m_nCapacity);
        Append(src);
    }
    /**
     * the default construcor and init the 
     * total size and capacity and set precision 
     * of double/float
     *
     */
    StringBuilderImpl(void) :
            m_Data(nullptr),
            m_nTotalSize(0),
            m_nCapacity(16),
            m_nPrecision(15) {
        ExpandCapacity(m_nCapacity);
    }
    ~StringBuilderImpl() {
        if (nullptr != m_Data)
            delete[] m_Data;
        m_nTotalSize = 0;
        m_nCapacity = 0;
    }

public:
    template<class T>
    StringBuilderImpl& operator << (const T &val){
        return append(val);
    }
    StringBuilderImpl &clear(){
        return setSize(0);
    }
    template <class typeArg>
    StringBuilderImpl &append(const typeArg &arg) {
        Append(toString(arg));
        return *this;
    }
    string_t toString(void) const {
        string_t temp(m_Data, m_nTotalSize);
        return temp;
    }
    chr charAt(size_type nIndex) {
        if (nIndex > 0 && nIndex < m_nTotalSize) {
            return m_Data[nIndex];
        }
        throw std::range_error("charAt Index out of range");
    }
    chr operator[](size_type nIndex) {
        return charAt(nIndex);
    }

    size_type size() const {
        return m_nTotalSize;
    }
    size_type length() const {
        return m_nTotalSize;
    }
    size_type capacity() const {
        return m_nCapacity;
    }

    string_t substr(off_t nBegin, off_t nEnd = -1) {
        // safe check first
        if (nBegin < 0 || (nBegin > nEnd && nEnd != -1) || nEnd > m_nTotalSize)
            throw std::invalid_argument("invalid Begining and Ending");
        if (nEnd == -1)
            nEnd = m_nTotalSize;
        string_t temp(m_Data + nBegin, nEnd - nBegin);
        return temp;
    }

    // have to reset some thing between (nSize, m_nTotalSize]
    StringBuilderImpl &setSize(off_t nSize) {
        if (nSize > m_nTotalSize)
            ExpandCapacity(nSize);
        else {
            memset(m_Data + nSize, 0, m_nTotalSize - nSize);
            m_nTotalSize = nSize;
        }
        return *this;
    }
    StringBuilderImpl &setLength(off_t nSize){
        return setSize(nSize);
    }

    StringBuilderImpl &remove(off_t nStart, off_t nEnd = -1) {
        // safe check first
        if (nStart < 0 || (nStart > nEnd && nEnd != -1) || nEnd > m_nTotalSize)
            throw std::invalid_argument("invalid Begining and Ending");
        if (nEnd == -1)
            nEnd = m_nTotalSize;
        memset(m_Data + nStart, 0, nEnd - nStart);
        memcpy(m_Data + nStart, m_Data + nEnd, m_nTotalSize - nEnd);
        m_nTotalSize -= (nEnd - nStart);
        return *this;
    }
    StringBuilderImpl &removeCharAt(off_t nIndex) {
        if (nIndex < 0 || nIndex > m_nTotalSize)
            throw std::invalid_argument("invalid Index to remove");
        return remove(nIndex, nIndex + 1);
    }

    StringBuilderImpl &insert(off_t nIndex, chr arg, size_type nCnt = 1) {
        // safe Check
        if (nIndex < 0 || nIndex > m_nTotalSize)
            throw std::invalid_argument("invalid Index to insert");
        string_t temp(nCnt, arg);
        off_t checkSize = CheckSizeEnough(nCnt);
        if (checkSize) {
            ExpandCapacity(checkSize);
        }
        memcpy(m_Data + nIndex + nCnt, m_Data + nIndex, m_nTotalSize - nIndex);
        memset(m_Data + nIndex, 0, nCnt);
        mempcpy(m_Data + nIndex, temp.c_str(), nCnt);
        m_nTotalSize += nCnt;
        return *this;
    }
    StringBuilderImpl &replace(off_t nStart, off_t nEnd, const string_t &str) {
        // safe check first
        if (nStart < 0 || (nStart > nEnd && nEnd != -1) || nEnd > m_nTotalSize)
            throw std::invalid_argument("invalid Begining and Ending in Replace");

        int NeedAllocatedLength = (nEnd - nStart) - str.size();
        off_t checkSize = CheckSizeEnough(NeedAllocatedLength);
        if (checkSize)
            ExpandCapacity(checkSize);
        // move (nStart, m_nTotalSize] to
        // (nStart+str.size(),m_nTotalSize+str.size()]
        memcpy(m_Data + nStart + str.size(),
                m_Data + nStart,
                m_nTotalSize - nStart);
        // move str to (nStart, nStart+str.size()]
        memcpy(m_Data + nStart, str.c_str(), str.size());
        return *this;
    }

private:
    template <class argType>
    string_t toString(const argType &ar) {
        my_stringstream stream;
        stream << ar;
        return stream.str();
    }
    // float double 会出现精度丢失现象，默认保留15
    string_t toString(float arg) {
        my_stringstream stream;
        stream << std::setprecision(m_nPrecision) << arg;
        return stream.str();
    }
    string_t toString(double arg) {
        my_stringstream stream;
        stream << std::setprecision(m_nPrecision) << arg;
        return stream.str();
    }
    void Append(const string_t &src) {
        off_t nCheckSize = CheckSizeEnough(src.size());
        if (nCheckSize) {  // should expand capacity
            ExpandCapacity(nCheckSize);
        }
        memcpy(m_Data + m_nTotalSize, src.data(), src.size());
        m_nTotalSize += src.size();
    }
    /**
     * check current size is enough
     * if enough return 0;
     * else return the size of should expand
     *
     * @param: nSize the enoutg
     * @return: the size of should expand
     */
    off_t CheckSizeEnough(size_type nSize) {
        if ( static_cast<off_t>(nSize) + m_nTotalSize > m_nCapacity) {
            // should ExpandCapacity
            m_nCapacity = (m_nCapacity + nSize) << 1;  //扩大为原始的2倍
            return m_nCapacity;
        }
        return 0;
    }

protected:
    /**
     * expand the current size to nSize
     *
     * @param:nSize the total size
     */
    void ExpandCapacity(off_t nSize) {
        if (nSize < m_nCapacity) {
            return;
        }
        std::auto_ptr<chr> _au(new chr[nSize]);
        if (nullptr != m_Data) {
            memcpy(_au.get(),m_Data,m_nTotalSize);
            delete[] m_Data;
        }
        m_Data = _au.release();
        m_nCapacity = nSize;
    }
    StringBuilderImpl(const StringBuilderImpl&)=delete;
private:
    chr *m_Data;
    off_t m_nTotalSize;   //字符串实际长度
    off_t m_nCapacity;    //字符串容纳体积
    size_t m_nPrecision;  //浮点数精度
};
typedef StringBuilderImpl<char> StringBuilder;

#endif
