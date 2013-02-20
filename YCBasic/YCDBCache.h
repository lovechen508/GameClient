#ifndef _INCLUDE_FGCDBCACHE_H_
#define _INCLUDE_FGCDBCACHE_H_

#include <list>

//
// FGCDBCache : DB 对象缓冲池
//
template <typename K, 
    typename V,
    typename RWLock>
class FGCDBCache
{    
    typedef std::list<V*> BUCKET_LIST;

    enum {
        MIN_LRU_THRESHOLD = 60,
        MAX_LRU_THRESHOLD = 300,

        MIN_DIRTY_THRESHOLD = 3,
        MAX_DIRTY_THRESHOLD = 60,

        MIN_BUCKETS = 500,
        DEFAULT_BUCKETS = 2000,
        MAX_BUCKETS = 5000
    };

    typedef struct LOCKED_BUCKET {
        RWLock lock;
        BUCKET_LIST list;
    } LOCKED_BUCKET;

public:

    FGCDBCache()
        : myLRUPhraseOutThreshold(MAX_LRU_THRESHOLD)
        , myDirtyPersistThreshold(MAX_DIRTY_THRESHOLD)
        , myNoOfBuckets(DEFAULT_BUCKETS)
    {
    }

    ~FGCDBCache()
    {
    }

    //
    // 设置时间限制
    //
    void setThreshold(int lruThreshold, int dirtyThreshold)
    {
        if (lruThreshold > MIN_LRU_THRESHOLD && lruThreshold < MAX_LRU_THRESHOLD)
        {
            myLRUPhraseOutThreshold = lruThreshold;
        }

        if (dirtyThreshold > MIN_DIRTY_THRESHOLD && dirtyThreshold < MAX_DIRTY_THRESHOLD)
        {
            myDirtyPersistThreshold = dirtyThreshold;
        }
    }

    //
    // 设置Hash桶的数量
    //
    void setBuckets(int buckets)
    {
        SMART_ASSERT(myBuckets == NULL);

        if (buckets > MIN_BUCKETS && buckets < MAX_BUCKETS)
        {
            myNoOfBuckets = buckets;
        }
    }

    //
    // 初始化
    //
    bool initialize()
    {
        myBuckets = new LOCKED_BUCKET*[myNoOfBuckets];

        for (int i = 0; i < myNoOfBuckets; ++i)
        {
            myBuckets[i] = new LOCKED_BUCKET();
        }

        return true;
    }

    //
    // 是否存在Key对象
    //
    bool exist(K k)
    {
        LOCKED_BUCKET* bucket = myBuckets[k % myNoOfBuckets];
        FGPRLockGuard<RWLock> guard(bucket->lock);
        for (BUCKET_LIST::iterator it = bucket->list.begin(); 
            it != bucket->list.end(); 
            ++it)
        {
            V* item = *it;
            if (item->getKey() == k)
            {
                return true;
            }
        }

        return false;
    }

    //
    // 添加对象，此时不为Dirty
    //
    void insert(K k, V* v)
    {
        LOCKED_BUCKET* bucket = myBuckets[k % myNoOfBuckets];
        //FGPWLockGuard<RWLock> guard(bucket->lock);
        for (BUCKET_LIST::iterator it = bucket->list.begin();
            it != bucket->list.end();
            ++it)
        {
            V* item = *it;
            if (item->getKey() == k)
            {
                //throw FGPException(2002, "Duplicated key!");
                return;
            }
        }

        bucket->list.push_back(v);
        v->setLastVisited();
    }

    //
    // 查找对象, 静态函数访问器
    //
    template<typename F>
    bool visit(K k, F& f)
    {
        bool found = false;

        LOCKED_BUCKET* bucket = myBuckets[k % myNoOfBuckets];
        //FGPRLockGuard<RWLock> guard(bucket->lock);
        for (BUCKET_LIST::iterator it = bucket->list.begin();
            it != bucket->list.end() && !found;
            ++it)
        {
            V* item = *it;
            if (item->getKey() == k)
            {
                found = true;
                (*f)(item);
                item->setLastVisited();
            }
        }

        return found;
    }

    //
    // 查找函数，成员函数访问器
    //
    template<typename T, typename MF>
    bool visit(K k, T* t, MF& f)
    {
        bool found = false;

        LOCKED_BUCKET* bucket = myBuckets[k % myNoOfBuckets];
       // FGPRLockGuard<RWLock> guard(bucket->lock);
        for (BUCKET_LIST::iterator it = bucket->list.begin();
            it != bucket->list.end() && !found;
            ++it)
        {
            V* item = *it;
            if (item->getKey() == k)
            {
                found = true;
                (t->*f)(item);
                item->setLastVisited();
            }
        }

        return found;
    }


    //
    // 通过Package更新对象
    //
    template <typename P>
    bool update(K k, const P* p)
    {
        bool found = false;

        LOCKED_BUCKET* bucket = myBuckets[k % myNoOfBuckets];
       // FGPWLockGuard<RWLock> guard(bucket->lock);
        for (BUCKET_LIST::iterator it = bucket->list.begin();
            it != bucket->list.end() && !found;
            ++it)
        {
            V* item = *it;
            if (item->getKey() == k)
            {
                found = true;
                item->update(p);
                item->setLastUpdate();            
            }
        }

        return found;
    }

    //
    // 写到数据库
    // 
    // 说明：在规定的时间段shreholds内，写数据到数据库
    //       timeout == 0, 则全部把脏数据刷新到数据库
    //
    template <typename DAO>
    void flush(DAO* dao, bool flushAll)
    {        
        time_t shreholds = time(NULL) - myDirtyPersistThreshold;

        for (int i = 0; i < myNoOfBuckets; ++i)
        {
            LOCKED_BUCKET* bucket = myBuckets[i];
            //FGPWLockGuard<RWLock> guard(bucket->lock);
            for (BUCKET_LIST::iterator it = bucket->list.begin();
                it != bucket->list.end();
                ++it)
            {
                V* item = *it;
                if (item->isDirty())
                {
                    if (flushAll || item->getLastUpdate() < shreholds)
                    {
                        dao->persist(item);
                        item->setDirty(false);
                    }
                }
            }           
        }
    }

    //
    // 把最近访问时间小于shreholds的释放掉
    //
    void shrink()
    {
        time_t shreholds = time(NULL) - myLRUPhraseOutThreshold;

        for (int i = 0; i < myNoOfBuckets; ++i)
        {
            LOCKED_BUCKET* bucket = myBuckets[i];
            //FGPWLockGuard<RWLock> guard(bucket->lock);
            BUCKET_LIST::iterator it = bucket->list.begin();
            while (it != bucket->list.end())
            {
                V* v = *it;
                if (!v->isDirty() && v->getLastVisited() < shreholds )
                {
                    it = bucket->list.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }        
    }

    //
    // 释放所有资源
    //
    void finalize()
    {
        for (int i = 0; i < myNoOfBuckets; ++i)
        {
            LOCKED_BUCKET* bucket = myBuckets[i];
            for (BUCKET_LIST::iterator it = bucket->list.begin();
                it != bucket->list.end();
                ++it)
            {
                SAFE_DELETE(*it);
            }
            SAFE_DELETE(bucket);
        }

        SAFE_DELETE_ARRAY(myBuckets);
    }

private:

    //
    //  最少访问淘汰时间限制
    //
    int myLRUPhraseOutThreshold;

    //
    // 脏数据写数据库时间限制
    //
    int myDirtyPersistThreshold;

    //
    // Hash Buckets
    //
    int myNoOfBuckets;

    //
    // 二维缓冲池
    //
    LOCKED_BUCKET** myBuckets;
};

#endif