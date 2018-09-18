#ifndef  mmcTemplateClass_INC
#define  mmcTemplateClass_INC

//#include "vxWorks.h"
#include <iostream>
#include <deque>
#include <algorithm>
#include <iterator>
#include <vector>
#include <utility>

namespace MMC
{
    /*
     * =====================================================================================
     *        Class:  ScanBuffer
     *  Description:  扫描缓冲区
     * =====================================================================================
     */
    using namespace std;
    template < class T >
        class TBoundedBuffer
        {
            public:
                typedef T value_type;           /* 暂定用unsigned char 和 frameStruct作为值类型*/
                typedef vector<value_type> vector_value_type;
                TBoundedBuffer ();              /* constructor */
                TBoundedBuffer (int capacity);  /* constructor */
                unsigned int Size();
                bool HasFulled();

                /* 
                 * ===  FUNCTION  ======================================================================
                 *         Name:  InBuffer
                 *  Description:  数据写入缓冲区
                 *  Return: 是否成功写入,true成功，false失败（由缓存满引起）
                 * =====================================================================================
                 */
                bool InBuffer(const vector_value_type &data);
                bool InBuffer(const value_type &data);
                bool InBuffer(const value_type *data, unsigned int length);
                /* 
                 * ===  FUNCTION  ======================================================================
                 *         Name:  OutBuffer
                 *  Description:  数据从缓冲区中读出
                 *  data:   vector<T>类型的数据，从缓存中读出的数据直接追加到vector尾端
                 *  Return: 读出的数据量，vector的长度
                 * =====================================================================================
                 */
                unsigned int OutBuffer(vector_value_type *data);

            private:
                /* ====================  METHODS       ======================================= */

                /* ====================  DATA MEMBERS  ======================================= */
                deque<value_type> buffer;
                int maxLength;
                bool hasFulled;                 /* 缓冲区曾经满过 */

        }; /* ----------  end of template class ScanBuffer  ---------- */
    template < class T >
        TBoundedBuffer<T>::TBoundedBuffer ( )
        :maxLength(1024),
        hasFulled(false) { }		
    template< class T >
        TBoundedBuffer<T>::TBoundedBuffer (int capacity)
        :maxLength(capacity),
        hasFulled(false) { }
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  InBuffer
     *  Description:  
     * =====================================================================================
     */
    template < class T >
        bool TBoundedBuffer<T>::InBuffer (const typename TBoundedBuffer<T>::vector_value_type &data )
        {
            if(data.size() + buffer.size() <= maxLength)
            {
                copy(data.begin(), data.end(), back_inserter(buffer));
                return true;
            }
            else
            {
                hasFulled = true;
                return false;
            }
        }		
    template < class T >
        bool TBoundedBuffer<T>::InBuffer(const value_type &data)
        {
            if(buffer.size() < maxLength)
            {
                buffer.push_back(data);
                return true;
            }
            else
            {
                hasFulled = true;
                return false;
            }
        }
    template < class T >
        bool TBoundedBuffer<T>::InBuffer (const typename TBoundedBuffer<T>::value_type *data,unsigned int length)
        {
            if(length + buffer.size() <= maxLength)
            {
                copy(data, data + length, back_inserter(buffer));
                return true;
            }
            else
            {
                hasFulled = true;
                return false;
            }
        }		/* -----  end of function InBuffer  ----- */

    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  OutBuffer
     *  Description:  
     * =====================================================================================
     */
    template < class T >
        unsigned int TBoundedBuffer<T>::OutBuffer (typename TBoundedBuffer<T>::vector_value_type *data )
        {
            unsigned int length = buffer.size();
            copy(buffer.begin(), buffer.end(), back_inserter(*data));
            buffer.clear();
            return length;
        }
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  Size
     *  Description:  
     * =====================================================================================
     */
    template < class T >
        unsigned int TBoundedBuffer<T>::Size ()
        {
            return buffer.size();
        }		/* -----  end of function Size  ----- */

    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  HasFull
     *  Description:  
     * =====================================================================================
     */
    template < class T >
        bool TBoundedBuffer<T>::HasFulled ()
        {
            return hasFulled;
        }		/* -----  end of function HasFull  ----- */

    template < class T1, class T2>
        class TDequePair
        {
            public:
                typedef T1 key_type;
                typedef T2 value_type;
                typedef deque<pair<key_type, value_type> > data_type; /* deque实现，vector实现会有问题，源于内存分配方式的差异 */
                typedef typename data_type::iterator iterator;
                typedef typename data_type::const_iterator const_iterator;
                iterator MyFind(key_type key)
                {
                    for(iterator it = data.begin(); it != data.end(); ++it)
                    {
                        if(it->first == key)
                        {
                            return it;
                        }
                    }
                    return data.end();
                }

                size_t size() const
                {
                    return data.size();
                }
                value_type& operator[](const key_type& _Keyval)
                {
                    iterator _Where = AddElement(_Keyval);
                    return ((*_Where).second);
                }
                bool contains(key_type key) const
                {
                    for(const_iterator it = data.begin(); it != data.end(); ++it)
                    {
                        if(it->first == key)
                        {
                            return true;
                        }
                    }
                    return false;
                }
                void clear()
                {
                    data.clear();
                }

                bool empty()
                {
                    return data.empty();
                }

                void AddElement(key_type key,const value_type &value)
                {
                    iterator it = MyFind(key);
                    if(it != data.end())
                    {
                        it->second = value;
                    }
                    else
                    {
                        data.push_back(make_pair(key, value));
                    }
                }
                
                iterator AddElement(key_type key)
                {
                    iterator it = MyFind(key);
                    if(it != data.end())
                    {
                        return it;
                    }
                    else
                    {
                        data.push_back(make_pair(key, value_type()));
                        return MyFind(key);
                    }
                }
                
                void MySort()
                {
                    sort(data.begin(), data.end());
                }

                iterator begin()
                {
                    return data.begin();
                }

                iterator end()
                {
                    return data.end();
                }
                
                const_iterator begin() const
                {
                    return data.begin();
                }

                const_iterator end() const
                {
                    return data.end();
                }

            private:
                data_type data;
        };
}
#endif   /* ----- #ifndef mmcTemplateClass_INC  ----- */
