#pragma once
#include <cstddef>

#include <map>
#include <memory_resource>
#include <unordered_map>
#include <vector>

class mem_pool
{
    using Pool = std::pmr::synchronized_pool_resource;
    // std::map<int, std::vector<char>> pool;
    Pool pool;
    std::unordered_map<uint64_t, size_t> sizes;

  public:
    void *malloc(size_t n);

    void free(void *p);

    static mem_pool &get_default();
};
