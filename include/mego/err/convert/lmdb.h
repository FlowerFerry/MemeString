
#ifndef MGEC_ERR_CONVERT_LMDB_H_INCLUDED
#define MGEC_ERR_CONVERT_LMDB_H_INCLUDED

#include <mego/err/ec.h>
#include <mego/predef/symbol/inline.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <liblmdb/lmdb.h>

MG_CAPI_INLINE mgec_t mgec__from_lmdb_err(int _err)
{
    switch(_err) {
        case MDB_SUCCESS: return MGEC__OK;
        case MDB_NOTFOUND: return MGEC__NOKEY;
        // case MDB_KEYEXIST: return MGEC_LMDB_KEYEXIST;
        // case MDB_PAGE_NOTFOUND: return MGEC_LMDB_PAGE_NOTFOUND;
        // case MDB_CORRUPTED: return MGEC_LMDB_CORRUPTED;
        // case MDB_PANIC: return MGEC_LMDB_PANIC;
        // case MDB_VERSION_MISMATCH: return MGEC_LMDB_VERSION_MISMATCH;
        // case MDB_INVALID: return MGEC_LMDB_INVALID;
        // case MDB_MAP_FULL: return MGEC_LMDB_MAP_FULL;
        // case MDB_DBS_FULL: return MGEC_LMDB_DBS_FULL;
        // case MDB_READERS_FULL: return MGEC_LMDB_READERS_FULL;
        // case MDB_TLS_FULL: return MGEC_LMDB_TLS_FULL;
        // case MDB_TXN_FULL: return MGEC_LMDB_TXN_FULL;
        // case MDB_CURSOR_FULL: return MGEC_LMDB_CURSOR_FULL;
        // case MDB_PAGE_FULL: return MGEC_LMDB_PAGE_FULL;
        // case MDB_MAP_RESIZED: return MGEC_LMDB_MAP_RESIZED;
        // case MDB_INCOMPATIBLE: return MGEC_LMDB_INCOMPATIBLE;
        // case MDB_BAD_RSLOT: return MGEC_LMDB_BAD_RSLOT;
        // case MDB_BAD_TXN: return MGEC_LMDB_BAD_TXN;
        // case MDB_BAD_VALSIZE: return MGEC_LMDB_BAD_VALSIZE;
        // case MDB_BAD_DBI: return MGEC_LMDB_BAD_DBI;
        // case MDB_LAST_ERRCODE: return MGEC_LMDB_LAST_ERRCODE;
        default: return MGEC__ERR;
    }
}

#ifdef __cplusplus
}
#endif

#endif // !MGEC_ERR_CONVERT_LMDB_H_INCLUDED
