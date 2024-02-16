#include<cstdio>

#include <faabric/util/mem_pool.h>

void *mem_pool::malloc(size_t n)
{
    // std::fprintf(stderr, "%s(%d)\n", __func__, (int) n);
    // void *p = pool.upstream_resource()->allocate(n);
    // sizes.emplace((uint64_t)(p), n);
    // return p;
    return std::malloc(n);
}

void mem_pool::free(void *p)
{
    return std::free(p);
    // std::fprintf(stderr, "%s(%p)\n", __func__, p);
    // size_t n = sizes.at((uint64_t)(p));
    // auto it = sizes.find((uint64_t)(p));
    // sizes.erase(it);
    // pool.upstream_resource()->deallocate(p, n);
}

mem_pool &mem_pool::get_default()
{
    static mem_pool instance;
    return instance;
}
