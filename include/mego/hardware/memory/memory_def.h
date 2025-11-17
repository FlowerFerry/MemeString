
#ifndef MGHW_MEMORY_DEF_H_INCLUDED
#define MGHW_MEMORY_DEF_H_INCLUDED

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cppplusplus

typedef struct mghw_memory_status
{
    uint32_t st_size;             //!< The size of the structure
    uint32_t load_physical;       //!< The usage rate of physical memory
    uint64_t total_physical;      //!< The total amount of physical memory in the system
    uint64_t available_physical;  //!< The available amount of physical memory in the system
    uint64_t total_virtual;       //!< The total amount of virtual memory in the system
    uint64_t available_virtual;   //!< The available amount of virtual memory in the system
} mghw_memory_status_t;

// inline int mghw_get_memory_status(struct mghw_memory_status *status);

#ifdef __cplusplus
}
#endif // __cppplusplus

#endif // !MGHW_MEMORY_DEF_H_INCLUDED
