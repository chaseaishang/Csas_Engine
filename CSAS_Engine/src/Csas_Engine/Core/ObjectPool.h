//
// Created by chaseaishang on 23-8-26.
//

#pragma once

#include <queue>
#include <list>
#include "glm/glm.hpp"
namespace CsasEngine
{

    static const size_t kObjectPoolDefaultSize = 100;
    static const size_t kObjectPoolDefaultExtendSize = 10;


    
    template <typename ObjectType,
            size_t N = kObjectPoolDefaultSize>
    class SharedObjectPool
    {
    public:
        typedef void (*Releaser)(ObjectType*);
        typedef void (*Initializer)(ObjectType*);

        static SharedObjectPool& GetInstance(Initializer initializer,Releaser releaser)
        {
            kInitializer=initializer;
            kReleaser=releaser;
            static SharedObjectPool pool(N);
            return pool;
        }
        
        std::shared_ptr<ObjectType> Get()
        {
            ObjectType *ptr = nullptr;
            if (queue_.empty())
            {
                Extend(kObjectPoolDefaultExtendSize);
            }
            ptr = queue_.front();
            queue_.pop();
            kInitializer(ptr);
            return std::shared_ptr<ObjectType>(ptr,
            [&](ObjectType *p)
                {
                    kReleaser(p);
                    queue_.push(p);
                });
        }
        int capacity() const {return capacity_;}
        
        void set_capacity(const size_t capacity)
        {
            if (capacity_ < capacity)
            {
                Extend(capacity - capacity_);
            }
        }
        
    private:
        explicit SharedObjectPool(const size_t pool_size) :
                kDefaultCacheSize(pool_size) {
            cache_ = new ObjectType[kDefaultCacheSize];
            for (size_t i = 0; i < kDefaultCacheSize; ++i)
            {
                queue_.push(&cache_[i]);
                kInitializer(&cache_[i]);
            }
            capacity_ = kDefaultCacheSize;
        }
        
        ~SharedObjectPool()
        {
            if (cache_)
            {
                delete[] cache_;
                cache_ = nullptr;
            }
            for (auto &ptr : extended_cache_) {
                delete ptr;
            }
            extended_cache_.clear();
        }
        void Extend(const size_t num) {
            for (size_t i = 0; i < num; ++i)
            {
                ObjectType *ptr = new ObjectType;
                extended_cache_.push_back(ptr);
                queue_.push(ptr);
                kInitializer(ptr);
            }
            capacity_ = kDefaultCacheSize + extended_cache_.size();
        }
        
    private:
        size_t capacity_ = 0;
        std::queue<ObjectType*> queue_;
        ObjectType *cache_ = nullptr;
        std::list<ObjectType*> extended_cache_;
        const size_t kDefaultCacheSize;
        static Initializer        kInitializer;
        static Releaser           kReleaser;
        //static const Releaser    KReleaser;
    };

    template<typename Object_type,size_t N>
    typename SharedObjectPool<Object_type,N>::Initializer SharedObjectPool<Object_type,N>::kInitializer;
    template<typename Object_type,size_t N>
    typename SharedObjectPool<Object_type,N>::Releaser    SharedObjectPool<Object_type,N>::kReleaser;


}