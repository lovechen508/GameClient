#ifndef _INCLUDE_FGCDBCACHE_H_
#define _INCLUDE_FGCDBCACHE_H_

#include <list>

//
// FGCDBCache : DB ���󻺳��
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
    // ����ʱ������
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
    // ����HashͰ������
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
    // ��ʼ��
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
    // �Ƿ����Key����
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
    // ��Ӷ��󣬴�ʱ��ΪDirty
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
    // ���Ҷ���, ��̬����������
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
    // ���Һ�������Ա����������
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
    // ͨ��Package���¶���
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
    // д�����ݿ�
    // 
    // ˵�����ڹ涨��ʱ���shreholds�ڣ�д���ݵ����ݿ�
    //       timeout == 0, ��ȫ����������ˢ�µ����ݿ�
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
    // ���������ʱ��С��shreholds���ͷŵ�
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
    // �ͷ�������Դ
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
    //  ���ٷ�����̭ʱ������
    //
    int myLRUPhraseOutThreshold;

    //
    // ������д���ݿ�ʱ������
    //
    int myDirtyPersistThreshold;

    //
    // Hash Buckets
    //
    int myNoOfBuckets;

    //
    // ��ά�����
    //
    LOCKED_BUCKET** myBuckets;
};

#endif