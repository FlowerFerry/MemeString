
#include <catch2/catch.hpp>

#include <mego/mem/pool.h>

//#define LBDL_LIST_FOREACH_CHECK_REF_COUNT(LIST, COUNT) \
    //do { \
    //    const mgc_lbdl_list_node* curr_index = mgc_lbdl_list_chead(LIST); \
    //    while(curr_index != NULL) { \
    //        curr_index = mgc_lbdl_list_node_cnext(curr_index); \
    //    } \
    //} while(0);

TEST_CASE("mgmem_pool - 01", "init")
{
    mgec_t ec;
    mgmem_pool* pool = NULL;
    ec = mgmem_pool_create(&pool, 1);
    REQUIRE(ec == 0);   
    REQUIRE(mgmem_pool_data_size(pool) == 1);
    REQUIRE(mgmem_pool_used_size(pool) == 0);
    ec = mgmem_pool_destroy(&pool);
    REQUIRE(ec == 0);
    REQUIRE(pool == NULL);
    
}

TEST_CASE("mgmem_pool - 02", "alloc")
{
    mgec_t ec;
    mgmem_pool* pool = NULL;
    ec = mgmem_pool_create(&pool, 1);
    REQUIRE(ec == 0);
    REQUIRE(mgmem_pool_data_size(pool) == 1);
    REQUIRE(mgmem_pool_used_size(pool) == 0);
    void* obj = mgmem_pool_alloc(pool);
    REQUIRE(obj != NULL);
    REQUIRE(mgmem_pool_used_size(pool) == 1);
    REQUIRE(mgmem_pool_obj_from_data_ptr(obj) != NULL);

    //LBDL_LIST_FOREACH_CHECK_REF_COUNT(&pool->used_list_, 2);
    
    ec = mgmem_pool_free(pool, obj);
    REQUIRE(ec == 0);
    REQUIRE(mgmem_pool_used_size(pool) == 0);

    //LBDL_LIST_FOREACH_CHECK_REF_COUNT(&pool->free_list_, 2);

    ec = mgmem_pool_destroy(&pool);
    REQUIRE(ec == 0);
    REQUIRE(pool == NULL);
}

TEST_CASE("mgmem_pool - 03", "alloc")
{
    mgec_t ec;
    mgmem_pool* pool = NULL;
    ec = mgmem_pool_create(&pool, 1);
    REQUIRE(ec == 0);
    REQUIRE(mgmem_pool_data_size(pool) == 1);
    REQUIRE(mgmem_pool_used_size(pool) == 0);
    void* obj1 = mgmem_pool_alloc(pool);
    REQUIRE(obj1 != NULL);
    REQUIRE(mgmem_pool_used_size(pool) == 1);
    REQUIRE(mgmem_pool_obj_from_data_ptr(obj1) != NULL);
    void* obj2 = mgmem_pool_alloc(pool);
    REQUIRE(obj2 != NULL);
    REQUIRE(mgmem_pool_used_size(pool) == 2);
    REQUIRE(mgmem_pool_obj_from_data_ptr(obj2) != NULL);

    //LBDL_LIST_FOREACH_CHECK_REF_COUNT(&pool->used_list_, 2);

    ec = mgmem_pool_free(pool, obj1);
    REQUIRE(ec == 0);
    REQUIRE(mgmem_pool_used_size(pool) == 1);
    ec = mgmem_pool_free(pool, obj2);
    REQUIRE(ec == 0);
    REQUIRE(mgmem_pool_used_size(pool) == 0);

    //LBDL_LIST_FOREACH_CHECK_REF_COUNT(&pool->free_list_, 2);

    ec = mgmem_pool_destroy(&pool);
    REQUIRE(ec == 0);
    REQUIRE(pool == NULL);
}
