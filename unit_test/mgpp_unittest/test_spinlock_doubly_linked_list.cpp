
#include <mego/container/slbd_list.h>

#include <catch2/catch.hpp>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("mgc_slbd_list - 01", "init")
{
    mgc_slbd_list lst;
    auto ec = mgc_slbd_list_init(&lst, 1);
    REQUIRE(ec == 0);

    REQUIRE(mgc_slbd_list_size(&lst) == 0);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == true);
    REQUIRE(mgc_slbd_list_head (&lst) == NULL);
    REQUIRE(mgc_slbd_list_tail (&lst) == NULL);
    REQUIRE(mgc_slbd_list_chead(&lst) == NULL);
    REQUIRE(mgc_slbd_list_ctail(&lst) == NULL);

    REQUIRE(mgc_slbd_list_alloc_fn(&lst) == mgc_slbd_list_default_alloc);
    REQUIRE(mgc_slbd_list_free_fn (&lst) == mgc_slbd_list_default_free);
 
}

#define LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(LIST) \
    do { \
        const mgc_slbd_list_node* next_index; \
        const mgc_slbd_list_node* prev_index; \
        const mgc_slbd_list_node* curr_index = mgc_slbd_list_chead(LIST); \
        while(curr_index != NULL) { \
            next_index = mgc_slbd_list_node_cnext(curr_index); \
            prev_index = mgc_slbd_list_node_cprev(curr_index); \
            if (prev_index != NULL) { \
                const mgc_slbd_list_node* index = mgc_slbd_list_node_cnext(prev_index); \
                REQUIRE(index == curr_index); \
            } \
            if (next_index != NULL) { \
                const mgc_slbd_list_node* index = mgc_slbd_list_node_cprev(next_index); \
                REQUIRE(index == curr_index); \
            } \
            curr_index = next_index; \
        } \
    } while(0); 

#define LBDL_LIST_FOREACH_CHECK_REF_COUNT(LIST, COUNT) \
    //do { \
    //    const mgc_slbd_list_node* curr_index = mgc_slbd_list_chead(LIST); \
    //    while(curr_index != NULL) { \
    //        curr_index = mgc_slbd_list_node_cnext(curr_index); \
    //    } \
    //} while(0);

TEST_CASE("mgc_slbd_list - 03", "insert_head")
{
    mgc_slbd_list lst;
    mgec_t ec;
    const mgc_slbd_list_node* compare;
    //const mgc_slbd_list_node* curr_index;
    //const mgc_slbd_list_node* next_index;
    
    ec = mgc_slbd_list_init(&lst, 1);
    REQUIRE(ec == 0);
    
    auto node1 = mgc_slbd_list_create_node(&lst);
    REQUIRE(node1 != NULL);
    
    ec = mgc_slbd_list_push_head(&lst, node1);
    REQUIRE(ec == 0);
    
    REQUIRE(mgc_slbd_list_size(&lst) == 1);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == true);
    compare = mgc_slbd_list_head(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_tail(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node1);
    REQUIRE(mgc_slbd_list_node_data_ptr (node1) != NULL);
    REQUIRE(mgc_slbd_list_node_data_size(node1) == 1);
    REQUIRE(mgc_slbd_list_node_parent(node1) == &lst);
    REQUIRE(mgc_slbd_list_node_prev(node1) == NULL);
    REQUIRE(mgc_slbd_list_node_next(node1) == NULL);

    LBDL_LIST_FOREACH_CHECK_REF_COUNT (&lst, 1);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);

    auto node2 = mgc_slbd_list_create_node(&lst);
    REQUIRE(node2 != NULL);
    
    ec = mgc_slbd_list_push_head(&lst, node2);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 2);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node2) == true);
    REQUIRE(mgc_slbd_list_node_is_isolated(node2) == false);
    compare = mgc_slbd_list_head(&lst);
    REQUIRE(compare == node2);
    compare = mgc_slbd_list_tail(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node2);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node1);
    REQUIRE(mgc_slbd_list_node_data_ptr (node2) != NULL);
    REQUIRE(mgc_slbd_list_node_data_size(node2) == 1);
    REQUIRE(mgc_slbd_list_node_parent(node2) == &lst);
    REQUIRE(mgc_slbd_list_node_prev(node2) == NULL);
    compare = mgc_slbd_list_node_next(node2);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_node_prev(node1);
    REQUIRE(compare == node2);
    REQUIRE(mgc_slbd_list_node_next(node1) == NULL);

    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 1);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);

    auto node3 = mgc_slbd_list_create_node(&lst);
    REQUIRE(node3 != NULL);

    ec = mgc_slbd_list_node_insert_before(node2, node3);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 3);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node2) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == true);
    REQUIRE(mgc_slbd_list_node_is_isolated(node3) == false);
    compare = mgc_slbd_list_head(&lst);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_tail(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node1);
    REQUIRE(mgc_slbd_list_node_data_ptr (node3) != NULL);
    REQUIRE(mgc_slbd_list_node_data_size(node3) == 1);
    REQUIRE(mgc_slbd_list_node_parent(node3) == &lst);
    REQUIRE(mgc_slbd_list_node_prev(node3) == NULL);
    compare = mgc_slbd_list_node_next(node3);
    REQUIRE(compare == node2);
    compare = mgc_slbd_list_node_prev(node2);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_node_next(node2);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_node_prev(node1);
    REQUIRE(compare == node2);
    REQUIRE(mgc_slbd_list_node_next(node1) == NULL);

    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 1);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);

    ec = mgc_slbd_list_destroy_node(&lst, node1);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 2);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node2) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == true);
    compare = mgc_slbd_list_head(&lst);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_tail(&lst);
    REQUIRE(compare == node2);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node2);
    REQUIRE(mgc_slbd_list_node_next(node2) == NULL);
    
    ec = mgc_slbd_list_destroy_node(&lst, node2);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 1);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == true);
    compare = mgc_slbd_list_head(&lst);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_tail(&lst);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node3);
    REQUIRE(mgc_slbd_list_node_next(node3) == NULL);
    
    ec = mgc_slbd_list_destroy_node(&lst, node3);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 0);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == true);
    REQUIRE(mgc_slbd_list_head (&lst) == NULL);
    REQUIRE(mgc_slbd_list_tail (&lst) == NULL);
    REQUIRE(mgc_slbd_list_chead(&lst) == NULL);
    REQUIRE(mgc_slbd_list_ctail(&lst) == NULL);
}

TEST_CASE("mgc_slbd_list - 04", "insert_tail")
{
    const mgc_slbd_list_node* compare = NULL;

    mgc_slbd_list lst;
    int ec = mgc_slbd_list_init(&lst, 1);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 0);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == true);
    REQUIRE(mgc_slbd_list_head (&lst) == NULL);
    REQUIRE(mgc_slbd_list_tail (&lst) == NULL);
    REQUIRE(mgc_slbd_list_chead(&lst) == NULL);
    REQUIRE(mgc_slbd_list_ctail(&lst) == NULL);

    auto node1 = mgc_slbd_list_create_node(&lst);
    REQUIRE(node1 != NULL);

    ec = mgc_slbd_list_push_tail(&lst, node1);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 1);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    compare = mgc_slbd_list_head(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_tail(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node1);
    REQUIRE(mgc_slbd_list_node_data_ptr (node1) != NULL);
    REQUIRE(mgc_slbd_list_node_data_size(node1) == 1);
    REQUIRE(mgc_slbd_list_node_parent(node1) == &lst);
    REQUIRE(mgc_slbd_list_node_prev(node1) == NULL);
    REQUIRE(mgc_slbd_list_node_next(node1) == NULL);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == true);

    auto node2 = mgc_slbd_list_create_node(&lst);
    REQUIRE(node2 != NULL);

    ec = mgc_slbd_list_push_tail(&lst, node2);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 2);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node2) == true);
    compare = mgc_slbd_list_head(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_tail(&lst);
    REQUIRE(compare == node2);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node2);
    REQUIRE(mgc_slbd_list_node_data_ptr (node2) != NULL);
    REQUIRE(mgc_slbd_list_node_data_size(node2) == 1);
    REQUIRE(mgc_slbd_list_node_parent(node2) == &lst);
    compare = mgc_slbd_list_node_prev(node2);
    REQUIRE(compare == node1);
    REQUIRE(mgc_slbd_list_node_next(node2) == NULL);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node2) == false);

    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 2);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);
    
    auto node3 = mgc_slbd_list_create_node(&lst);
    REQUIRE(node3 != NULL);
    
    ec = mgc_slbd_list_push_tail(&lst, node3);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 3);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node2) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == true);
    compare = mgc_slbd_list_head(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_tail(&lst);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node3);
    REQUIRE(mgc_slbd_list_node_data_ptr(node3) != NULL);
    REQUIRE(mgc_slbd_list_node_data_size(node3) == 1);
    REQUIRE(mgc_slbd_list_node_parent(node3) == &lst);
    compare = mgc_slbd_list_node_prev(node3);
    REQUIRE(compare == node2);
    REQUIRE(mgc_slbd_list_node_next(node3) == NULL);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node2) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node3) == false);

    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 2);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);
    
    auto node4 = mgc_slbd_list_create_node(&lst);
    REQUIRE(node4 != NULL);
    
    ec = mgc_slbd_list_push_tail(&lst, node4);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 4);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node2) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node4) == true);
    compare = mgc_slbd_list_head(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_tail(&lst);
    REQUIRE(compare == node4);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node4);
    REQUIRE(mgc_slbd_list_node_data_ptr (node4) != NULL);
    REQUIRE(mgc_slbd_list_node_data_size(node4) == 1);
    REQUIRE(mgc_slbd_list_node_parent(node4) == &lst);
    compare = mgc_slbd_list_node_prev(node4);
    REQUIRE(compare == node3);
    REQUIRE(mgc_slbd_list_node_next(node4) == NULL);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node2) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node3) == false);

    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 2);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);
    
    auto node5 = mgc_slbd_list_create_node(&lst);
    REQUIRE(node5 != NULL);
    
    ec = mgc_slbd_list_push_tail(&lst, node5);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 5);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node2) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node4) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node5) == true);
    compare = mgc_slbd_list_head(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_tail(&lst);
    REQUIRE(compare == node5);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node5);
    REQUIRE(mgc_slbd_list_node_data_ptr (node5) != NULL);
    REQUIRE(mgc_slbd_list_node_data_size(node5) == 1);
    REQUIRE(mgc_slbd_list_node_parent(node5) == &lst);
    compare = mgc_slbd_list_node_prev(node5);
    REQUIRE(compare == node4);
    REQUIRE(mgc_slbd_list_node_next(node5) == NULL);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node2) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node3) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node4) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node5) == false);

    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 2);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);
    
    auto node6 = mgc_slbd_list_create_node(&lst);
    REQUIRE(node6 != NULL);
    
    ec = mgc_slbd_list_push_tail(&lst, node6);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 6);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node2) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node4) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node5) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node6) == true);
    compare = mgc_slbd_list_head(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_tail(&lst);
    REQUIRE(compare == node6);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node6);
    REQUIRE(mgc_slbd_list_node_data_ptr (node6) != NULL);
    REQUIRE(mgc_slbd_list_node_data_size(node6) == 1);
    REQUIRE(mgc_slbd_list_node_parent(node6) == &lst);
    compare = mgc_slbd_list_node_prev(node6);
    REQUIRE(compare == node5);
    REQUIRE(mgc_slbd_list_node_next(node6) == NULL);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node2) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node3) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node4) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node5) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node6) == false);

    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 2);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);
    
    auto node7 = mgc_slbd_list_create_node(&lst);
    REQUIRE(node7 != NULL);
    
    ec = mgc_slbd_list_push_tail(&lst, node7);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 7);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node2) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node4) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node5) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node6) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node7) == true);
    compare = mgc_slbd_list_head(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_tail(&lst);
    REQUIRE(compare == node7);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node7);
    REQUIRE(mgc_slbd_list_node_data_ptr (node7) != NULL);
    REQUIRE(mgc_slbd_list_node_data_size(node7) == 1);
    REQUIRE(mgc_slbd_list_node_parent(node7) == &lst);
    compare = mgc_slbd_list_node_prev(node7);
    REQUIRE(compare == node6);
    REQUIRE(mgc_slbd_list_node_next(node7) == NULL);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node2) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node3) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node4) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node5) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node6) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node7) == false);

    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 2);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);

    ec = mgc_slbd_list_node_release_from_lst(node4);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 6);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node2) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node4) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node5) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node6) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node7) == true);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node7);
    REQUIRE(mgc_slbd_list_node_data_ptr(node4) != NULL);
    compare = mgc_slbd_list_node_next(node3);
    REQUIRE(compare == node5);
    compare = mgc_slbd_list_node_prev(node5);
    REQUIRE(compare == node3);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node2) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node3) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node4) == true);
    REQUIRE(mgc_slbd_list_node_is_isolated(node5) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node6) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node7) == false);
    

    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 2);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);
    
    ec = mgc_slbd_list_node_destroy(node4);
    REQUIRE(ec == 0);
    
    ec = mgc_slbd_list_node_destroy(node2);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 5);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node5) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node6) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node7) == true);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node7);
    compare = mgc_slbd_list_node_next(node1);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_node_prev(node3);
    REQUIRE(compare == node1);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node3) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node5) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node6) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node7) == false);

    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 2);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);
    
    ec = mgc_slbd_list_node_destroy(node6);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 4);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node5) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node7) == true);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node7);
    compare = mgc_slbd_list_node_next(node5);
    REQUIRE(compare == node7);
    compare = mgc_slbd_list_node_prev(node7);
    REQUIRE(compare == node5);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node3) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node5) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node7) == false);
    
    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 2);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);

    mgc_slbd_list_node* node = NULL;
    ec = mgc_slbd_list_pop_head(&lst, &node);
    REQUIRE(ec == 0);
    REQUIRE(node == node1);
    REQUIRE(mgc_slbd_list_size(&lst) == 3);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node1) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node5) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node7) == true);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node7);
    compare = mgc_slbd_list_node_next(node3);
    REQUIRE(compare == node5);
    compare = mgc_slbd_list_node_prev(node5);
    REQUIRE(compare == node3);
    REQUIRE(mgc_slbd_list_node_prev(node3) == NULL);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == true);
    REQUIRE(mgc_slbd_list_node_is_isolated(node3) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node5) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node7) == false);

    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 2);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);
    
    ec = mgc_slbd_list_node_destroy(node);
    REQUIRE(ec == 0);

    ec = mgc_slbd_list_pop_tail(&lst, &node);
    REQUIRE(ec == 0);
    REQUIRE(node == node7);
    REQUIRE(mgc_slbd_list_size(&lst) == 2);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node3) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node5) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&lst, node7) == false);
    compare = mgc_slbd_list_chead(&lst);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_ctail(&lst);
    REQUIRE(compare == node5);
    compare = mgc_slbd_list_node_next(node3);
    REQUIRE(compare == node5);
    compare = mgc_slbd_list_node_prev(node5);
    REQUIRE(compare == node3);
    REQUIRE(mgc_slbd_list_node_next(node5) == NULL);
    REQUIRE(mgc_slbd_list_node_is_isolated(node3) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node5) == false);
    REQUIRE(mgc_slbd_list_node_is_isolated(node7) == true);

    LBDL_LIST_FOREACH_CHECK_REF_COUNT(&lst, 2);
    LBDL_LIST_FOREACH_CHECK_PREV_AND_NEXT(&lst);
    
    ec = mgc_slbd_list_node_destroy(node);
    REQUIRE(ec == 0);

    ec = mgc_slbd_list_clear(&lst);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&lst) == 0);
    REQUIRE(mgc_slbd_list_is_empty(&lst) == true);
    REQUIRE(mgc_slbd_list_chead(&lst) == NULL);
    REQUIRE(mgc_slbd_list_ctail(&lst) == NULL);
    
}

TEST_CASE("mgc_slbd_list - 05", "move node")
{
    mgec_t ec;

    mgc_slbd_list list1;
    mgc_slbd_list list2;
    const mgc_slbd_list_node* compare;

    ec = mgc_slbd_list_init(&list1, 1);
    REQUIRE(ec == 0);
    ec = mgc_slbd_list_init(&list2, 1);
    REQUIRE(ec == 0);

    auto node1 = mgc_slbd_list_create_node(&list1);
    REQUIRE(node1 != NULL);
    ec = mgc_slbd_list_push_tail(&list1, node1);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&list1) == 1);
    REQUIRE(mgc_slbd_list_is_empty(&list1) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&list1, node1) == true);
    compare = mgc_slbd_list_chead(&list1);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&list1);
    REQUIRE(compare == node1);
    REQUIRE(mgc_slbd_list_node_parent(node1) == &list1);
    REQUIRE(mgc_slbd_list_node_is_isolated(node1) == true);
    
    auto node2 = mgc_slbd_list_create_node(&list1);
    REQUIRE(node2 != NULL);
    ec = mgc_slbd_list_push_tail(&list1, node2);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&list1) == 2);
    REQUIRE(mgc_slbd_list_is_empty(&list1) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&list1, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&list1, node2) == true);
    compare = mgc_slbd_list_chead(&list1);
    REQUIRE(compare == node1);
    compare = mgc_slbd_list_ctail(&list1);
    REQUIRE(compare == node2);
    REQUIRE(mgc_slbd_list_node_parent(node2) == &list1);
    
    auto node3 = mgc_slbd_list_create_node(&list2);
    REQUIRE(node3 != NULL);
    ec = mgc_slbd_list_push_tail(&list2, node3);
    REQUIRE(ec == 0);
    REQUIRE(mgc_slbd_list_size(&list2) == 1);
    REQUIRE(mgc_slbd_list_is_empty(&list2) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&list2, node3) == true);
    compare = mgc_slbd_list_chead(&list2);
    REQUIRE(compare == node3);
    compare = mgc_slbd_list_ctail(&list2);
    REQUIRE(compare == node3);
    REQUIRE(mgc_slbd_list_node_parent(node3) == &list2);

    ec = mgc_slbd_list_release(&list2, node1);
    //REQUIRE(ec != 0);
    REQUIRE(mgc_slbd_list_exist(&list1, node1) == true);
    REQUIRE(mgc_slbd_list_exist(&list2, node3) == true);
    REQUIRE(mgc_slbd_list_size(&list1) == 2);
    REQUIRE(mgc_slbd_list_size(&list2) == 1);
    REQUIRE(mgc_slbd_list_is_empty(&list1) == false);
    REQUIRE(mgc_slbd_list_is_empty(&list2) == false);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&list1, node1) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&list1, node2) == true);
    REQUIRE(mgc_slbd_list_is_head_or_tail(&list2, node3) == true);
}
