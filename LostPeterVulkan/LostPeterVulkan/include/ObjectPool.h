/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-25
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OBJECT_POOL_H_
#define _OBJECT_POOL_H_

#include <list>
#include <vector>

namespace LostPeterVulkan
{
    template<typename T>
    class ObjectPointerPool 
    {
    public:
        ObjectPointerPool() 
            : listFree() 
            , stepCount(5)
        { 

        }
        virtual ~ObjectPointerPool()
        {
            Destroy();
        }

    public:
        std::list<T*> listFree;
        int stepCount;

    public:
        T* Get() 
        {
            if (listFree.empty())  
            {
                if (stepCount > 0)
                {
                    for (int i = 0; i < stepCount; i++)
                        listFree.push_back(New());
                }
                else
                {
                    return New();
                }
            }
            auto obj = listFree.back();
            listFree.pop_back();
            return obj;
        }

        inline void Back(T* obj) 
        { 
            listFree.emplace_back(obj); 
        }
        inline void Back(const std::vector<T*>& objs)
        {
            size_t count = objs.size();
            for (size_t i = 0; i < count; i++)
                Back(objs[i]);
        } 
        inline void Back(const std::list<T*>& objs)
        {
            for (auto it = objs.begin();
                 it != objs.end(); ++it)
            {
                Back(*it);
            }
        } 

        T* New()
        {
            return new T();
        }

        void Reserve(int count)
        {
            int count_free = (int)listFree.size();
            if (count_free >= count)
                return;
            int count_new = count_free - count;
            for (int i = 0; i < count_new; i++)
            {
                listFree.push_back(New());
            }
        }
        void Destroy()
        {
            for (auto it = listFree.begin();
                 it != listFree.end(); ++it)
            {
                delete *it;
            }
            listFree.clear();
        }
    };


    template<typename T>
    class ObjectManagedPool 
    {
    private:
        std::list<T> listObjs;
        std::list<T*> listFree;
        std::list<T*> listUsed;

    public:
        ObjectManagedPool() 
            : listFree()
            , listUsed() 
        { 

        }

        T* Get() 
        {
            if (listFree.empty()) 
            {  
                listObjs.push_back(T());
                listUsed.push_back(&listObjs.back());
            } 
            else 
            {  
                listUsed.push_back(listFree.back());
                listFree.pop_back();
            }
            return listUsed.back();
        }

        inline void Back(T* obj) 
        {
            listUsed.remove(obj);
            listFree.push_back(obj);
        }
    };

    template<typename T>
    using ObjectPool = ObjectPointerPool<T>;

}; //LostPeterVulkan

#endif