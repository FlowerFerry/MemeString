#include <minunit.h>

#include "test_meme_version.h"
#include "test_meme_string_basics.h"
#include "test_meme_string_manip.h"
#include "test_meme_string_index.h"
#include "test_meme_string_convert.h"
#include "test_meme_string_other.h"
#include "test_meme_buffer.h"
#include "test_meme_variable_buffer.h"
#include "test_meme_rune.h"
#include "test_meme_rune_index.h"
#include "test_meme_memory.h"
#include "test_meme_string_builder.h"
#include "test_meme_variant.h"

#include "test_mmconc_atomic_fwd.h"

int main(int _argc, char *_argv[])
{
    MU_RUN_SUITE(test_meme_version);
    MU_RUN_SUITE(test_meme_string_basics);
    MU_RUN_SUITE(test_meme_string_manip);
    MU_RUN_SUITE(test_meme_string_index);
    MU_RUN_SUITE(test_meme_string_convert);
    MU_RUN_SUITE(test_meme_string_other);
    MU_RUN_SUITE(test_meme_buffer);
    MU_RUN_SUITE(test_meme_variable_buffer);
    MU_RUN_SUITE(test_meme_rune);
    MU_RUN_SUITE(test_meme_rune_index);
    MU_RUN_SUITE(test_meme_memory);
    MU_RUN_SUITE(test_meme_string_builder);
    MU_RUN_SUITE(test_meme_variant);
    MU_RUN_SUITE(test_mmconc_atomic_fwd);
    MU_REPORT();
    return MU_EXIT_CODE;
}
