
#ifndef MGHW_MEMORY_DEF_H_INCLUDED
#define MGHW_MEMORY_DEF_H_INCLUDED

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cppplusplus

struct mghw_memory_status
{
    uint32_t st_size;             //! 结构体的大小
    uint32_t load_physical;       //! 物理内存的使用率
    uint64_t total_physical;      //! 系统总的物理内存量
    uint64_t available_physical;  //! 系统可用的物理内存量
    uint64_t total_virtual;       //! 系统总的虚拟内存量
    uint64_t available_virtual;   //! 系统可用的虚拟内存量
};

// inline int mghw_get_memory_status(struct mghw_memory_status *status);

#ifdef __cplusplus
}
#endif // __cppplusplus

#endif // !MGHW_MEMORY_DEF_H_INCLUDED
