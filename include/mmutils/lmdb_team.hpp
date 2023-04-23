
#ifndef MMUTILS_LMDB_TEAM_HPP_INCLUDED
#define MMUTILS_LMDB_TEAM_HPP_INCLUDED

#include <memepp/buffer_view.hpp>
#include <megopp/util/scope_cleanup.h>
#include <mmutils/program_path.hpp>

#include <liblmdb/lmdb.h>
#include <ghc/filesystem.hpp>

#include <mutex>
#include <tuple>
#include <unordered_map>
#include <memory>

namespace mmutils {
    
    struct lmdb_env
    {
        lmdb_env() = delete;
        
        inline lmdb_env(::MDB_env* _obj)
            : _obj(_obj)
        {}

        lmdb_env(const lmdb_env&) = delete;
        lmdb_env(lmdb_env&&) = default;
        
        inline ~lmdb_env()
        {
            if (MEGO_SYMBOL__LIKELY(_obj != NULL))
            {
                ::mdb_env_close(_obj);
            }
        }

        lmdb_env& operator=(const lmdb_env&) = delete;
        lmdb_env& operator=(lmdb_env&&) = default;
        
        inline constexpr ::MDB_env* native_handle() const noexcept { return _obj; }

    private:
        ::MDB_env* _obj;
    };
    using lmdb_env_ptr = std::shared_ptr<lmdb_env>;

    struct lmdb_team
    {
        lmdb_team();
        ~lmdb_team();

        void set_dir_path(const memepp::string_view& _path);

        std::tuple<cmnerrno_t, memepp::string> get_value(
            const memepp::buffer_view& _key,
            memepp::buffer& _value) const;

        std::tuple<cmnerrno_t, memepp::string> set_values(
            std::unordered_map<memepp::buffer_view, memepp::buffer_view>& _kvs) const;
        
        std::tuple<cmnerrno_t, memepp::string> get_values(
            const std::vector<memepp::buffer_view>& _keys,
            std::vector<memepp::buffer>& _values) const;

        std::tuple<int, lmdb_env_ptr> __get_env(int _index) const;

        inline static uint32_t __fnv1a_hash(const memepp::buffer_view& input)
        {
            const uint32_t prime = 0x01000193;
            uint32_t hash = 0x811C9DC5;

            for (auto c : input) {
                hash ^= c;
                hash *= prime;
            }

            return hash;
        }

        mutable std::mutex mtx_;
        memepp::string dir_path_;
        memepp::string file_prefix_;
        memepp::string file_suffix_;
        int db_count_;
        mutable std::map<int, lmdb_env_ptr> envs_;
    };

    lmdb_team::lmdb_team(): 
        dir_path_(relative_with_program_path("/dbs")),
        file_prefix_("lt"),
        file_suffix_("db"),
        db_count_(10)
    {
    }

    lmdb_team::~lmdb_team()
    {}

    inline void lmdb_team::set_dir_path(const memepp::string_view& _path)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        dir_path_ = _path.to_string();
    }

    inline std::tuple<cmnerrno_t, memepp::string> lmdb_team::get_value(
        const memepp::buffer_view& _key,
        memepp::buffer& _value) const
    {
        auto hash = __fnv1a_hash(_key);
        auto db_index = hash % db_count_;

        int ec = 0;
        lmdb_env_ptr env = nullptr;
        std::tie(ec, env) = __get_env(db_index);
        if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
            return std::make_tuple(ec, "get env failed");
        }

        ::MDB_txn* txn = nullptr;
        ec = ::mdb_txn_begin(env->native_handle(), nullptr, MDB_RDONLY, &txn);
        if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
            return std::make_tuple(ec, "mdb_txn_begin failed");
        }
        auto txn_cleanup = megopp::util::scope_cleanup__create([&] {
            ::mdb_txn_abort(txn);
        });

        ::MDB_dbi dbi = 0;
        ec = ::mdb_dbi_open(txn, nullptr, 0, &dbi);
        if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
            return std::make_tuple(ec, "mdb_dbi_open failed");
        }
        auto dbi_cleanup = megopp::util::scope_cleanup__create([&] {
            ::mdb_dbi_close(env->native_handle(), dbi);
        });

        ::MDB_val key = { static_cast<size_t>(_key.size()), const_cast<uint8_t*>(_key.data()) };
        ::MDB_val value = { 0, nullptr };
        ec = ::mdb_get(txn, dbi, &key, &value);
        if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
            return std::make_tuple(ec, "mdb_get failed");
        }

        _value = memepp::buffer{ 
            static_cast<uint8_t*>(value.mv_data), 
            static_cast<MemeInteger_t>(value.mv_size) };
        return std::make_tuple(0, "");
    }

    inline std::tuple<cmnerrno_t, memepp::string> lmdb_team::set_values(
        std::unordered_map<memepp::buffer_view, memepp::buffer_view>& _kvs) const
    {
        std::vector< std::unordered_map<memepp::buffer_view, memepp::buffer_view> > 
            diversion_kvs{ size_t(db_count_) };
        
        for (auto& it : _kvs) {
            auto hash = __fnv1a_hash(it.first);
            auto db_index = hash % db_count_;
            diversion_kvs[db_index].insert(it);
        }

        for (auto index = 0; index < db_count_; ++index) {
            auto& kvs = diversion_kvs[index];
            if (kvs.empty()) {
                continue;
            }

            int ec = 0;
            lmdb_env_ptr env = nullptr;
            std::tie(ec, env) = __get_env(index);
            if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
                return std::make_tuple(ec, "get env failed");
            }

            ::MDB_txn* txn = nullptr;
            ec = ::mdb_txn_begin(env->native_handle(), nullptr, 0, &txn);
            if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
                return std::make_tuple(ec, "mdb_txn_begin failed");
            }
            auto txn_cleanup = megopp::util::scope_cleanup__create([&] {
                ::mdb_txn_abort(txn);
            });

            ::MDB_dbi dbi = 0;
            ec = ::mdb_dbi_open(txn, nullptr, 0, &dbi);
            if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
                return std::make_tuple(ec, "mdb_dbi_open failed");
            }
            MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] {
                ::mdb_dbi_close(env->native_handle(), dbi);
            });

            for (auto& it : kvs) {
                ::MDB_val key = { static_cast<size_t>(it.first.size()) , const_cast<uint8_t*>(it.first.data()) };
                ::MDB_val val = { static_cast<size_t>(it.second.size()), const_cast<uint8_t*>(it.second.data()) };
                ec = ::mdb_put(txn, dbi, &key, &val, 0);
                if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
                    return std::make_tuple(ec, "mdb_put failed");
                }
            }

            ec = ::mdb_txn_commit(txn);
            if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
                return std::make_tuple(ec, "mdb_txn_commit failed");
            }
            
            txn_cleanup.cancel();
        }
    }

    inline std::tuple<cmnerrno_t, memepp::string> lmdb_team::get_values(
        const std::vector<memepp::buffer_view>& _keys,
        std::vector<memepp::buffer>& _values) const
    {
        std::vector< std::unordered_map<memepp::buffer_view, int> >
            diversion_keys{ size_t(db_count_) };

        _values.resize(_keys.size());

        for (auto index = 0; index < _keys.size(); ++index) {
            auto& key = _keys[index];
            auto hash = __fnv1a_hash(key);
            auto db_index = hash % db_count_;
            diversion_keys[db_index].insert(std::make_pair(key, index));
        }

        for (auto index = 0; index < db_count_; ++index) {
            auto& keys = diversion_keys[index];
            if (keys.empty()) {
                continue;
            }

            int ec = 0;
            lmdb_env_ptr env = nullptr;
            std::tie(ec, env) = __get_env(index);
            if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
                return std::make_tuple(ec, "get env failed");
            }

            ::MDB_txn* txn = nullptr;
            ec = ::mdb_txn_begin(env->native_handle(), nullptr, MDB_RDONLY, &txn);
            if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
                return std::make_tuple(ec, "mdb_txn_begin failed");
            }
            auto txn_cleanup = megopp::util::scope_cleanup__create([&] {
                ::mdb_txn_abort(txn);
            });

            ::MDB_dbi dbi = 0;
            ec = ::mdb_dbi_open(txn, nullptr, 0, &dbi);
            if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
                return std::make_tuple(ec, "mdb_dbi_open failed");
            }
            MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] {
                ::mdb_dbi_close(env->native_handle(), dbi);
            });

            for (auto& it : keys) {
                ::MDB_val key = { static_cast<size_t>(it.first.size()) , const_cast<uint8_t*>(it.first.data()) };
                ::MDB_val val = { 0, nullptr };
                ec = ::mdb_get(txn, dbi, &key, &val);
                if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
                    return std::make_tuple(ec, "mdb_get failed");
                }

                _values[it.second] = 
                    memepp::buffer{ 
                        static_cast<uint8_t*>(val.mv_data), 
                        static_cast<MemeInteger_t>(val.mv_size) };
            }

        }
    }

    inline std::tuple<int, lmdb_env_ptr> lmdb_team::__get_env(int _index) const
    {
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = envs_.find(_index);
        if (it != envs_.end()) {
            return std::make_tuple(0, it->second);
        }
        
        std::error_code errc;
        ghc::filesystem::create_directories(mm_to<std::string>(dir_path_), errc);
        if (errc) {
            return std::make_tuple(-(errc.value()), nullptr);
        }
        
        auto db_path = memepp::c_format(1024, "%s/%s.%d.%s",
            dir_path_.c_str(), file_prefix_.c_str(), _index, file_suffix_.c_str());
        
        ::MDB_env* env = nullptr;
        auto ec = ::mdb_env_create(&env);
        if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
            return std::make_tuple(ec, nullptr);
        }

        ec = ::mdb_env_set_mapsize(env, 10485760);
        if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
            return std::make_tuple(ec, nullptr);
        }

        ec = ::mdb_env_open(env, db_path.c_str(), 0, 0664);
        if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
            return std::make_tuple(ec, nullptr);
        }

        auto env_ptr = std::make_shared<lmdb_env>(env);
        envs_.insert(std::make_pair(_index, env_ptr));
        return std::make_tuple(0, env_ptr);
    }
    
};

#endif // ! MMUTILS_LMDB_TEAM_HPP_INCLUDED
