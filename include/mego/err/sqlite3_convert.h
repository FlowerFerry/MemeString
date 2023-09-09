
#ifndef MEGO_ERR_SQLITE3_CONVERT_H_INCLUDED
#define MEGO_ERR_SQLITE3_CONVERT_H_INCLUDED

#include <mego/err/ec.h>
#include <sqlite3/sqlite3.h>

inline mgec_t mgec__from_sqlite3_err(int _rc)
{
    switch (_rc)
    {
    case SQLITE_OK:         return MGEC__OK;
    // case SQLITE_INTERNAL:   return mgec_sqlite3_internal;
    case SQLITE_PERM:       return MGEC__PERM;
    // case SQLITE_ABORT:      return mgec_sqlite3_abort;
    case SQLITE_BUSY:       return MGEC__BUSY;
    // case SQLITE_LOCKED:     return mgec_sqlite3_locked;
    case SQLITE_NOMEM:      return MGEC__NOMEM;
    // case SQLITE_READONLY:   return mgec_sqlite3_readonly;
    // case SQLITE_INTERRUPT:  return MGEC__INTR;
    // case SQLITE_IOERR:      return mgec_sqlite3_ioerr;
    // case SQLITE_CORRUPT:    return mgec_sqlite3_corrupt;
    // case SQLITE_NOTFOUND:   return mgec_sqlite3_notfound;
    // case SQLITE_FULL:       return mgec_sqlite3_full;
    // case SQLITE_CANTOPEN:   return mgec_sqlite3_cantopen;
    case SQLITE_PROTOCOL:   return MGEC__PROTO;
    // case SQLITE_EMPTY:      return mgec_sqlite3_empty;
    // case SQLITE_SCHEMA:     return mgec_sqlite3_schema;
    // case SQLITE_TOOBIG:     return mgec_sqlite3_toobig;
    // case SQLITE_CONSTRAINT: return mgec_sqlite3_constraint;
    // case SQLITE_MISMATCH:   return mgec_sqlite3_mismatch;
    // case SQLITE_MISUSE:     return mgec_sqlite3_misuse;
    // case SQLITE_NOLFS:      return mgec_sqlite3_nolfs;
    // case SQLITE_AUTH:       return mgec_sqlite3_auth;
    // case SQLITE_FORMAT:     return mgec_sqlite3_format;
    case SQLITE_RANGE:      return MGEC__RANGE;
    // case SQLITE_NOTADB:     return mgec_sqlite3_notadb;
    // case SQLITE_NOTICE:     return mgec_sqlite3_notice;
    // case SQLITE_WARNING:    return mgec_sqlite3_warning;
    // case SQLITE_ROW:        return mgec_sqlite3_row;
    // case SQLITE_DONE:       return mgec_sqlite3_done;
    default:                return MGEC__ERR;
    }
}

#endif // !MEGO_ERR_SQLITE3_CONVERT_H_INCLUDED
