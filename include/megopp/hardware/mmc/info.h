
#ifndef MEGOPP_HW_MMC_INFO_H_INCLUDED
#define MEGOPP_HW_MMC_INFO_H_INCLUDED

#include <memepp/string.hpp>

namespace mgpp {
namespace hw {
namespace mmc {

    enum class removable_t
    {
        unknown,
        fixed,
        removable
    };

    struct info
    {
        info()
            : removable(removable_t::unknown)
        {
        }
        
        inline void reset()
        {
            dev_name.reset();
            cid.reset();
            csd.reset();
            oemid.reset();
            name.reset();
            serial.reset();
            manfid.reset();
            date.reset();
            type.reset();
            removable = removable_t::unknown;
        }

        memepp::string dev_name;
        memepp::string cid;
        memepp::string csd;
        memepp::string oemid;
        memepp::string name;
        memepp::string serial;
        memepp::string manfid;
        memepp::string date;
        memepp::string type;
        removable_t    removable;
    };

}
}
}

#endif // !MEGOPP_HW_MMC_INFO_H_INCLUDED
