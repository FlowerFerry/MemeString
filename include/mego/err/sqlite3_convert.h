
#ifndef MEGO_ERR_SQLITE3_CONVERT_H_INCLUDED
#define MEGO_ERR_SQLITE3_CONVERT_H_INCLUDED

#include <mego/err/ec.h>
#include <sqlite3/sqlite3.h>

inline mgec_t mgec__from_sqlite3_err(int _rc)
{
    switch (_rc)
    {
    case SQLITE_OK:         return MGEC__OK;
    case SQLITE_INTERNAL:   return MGEC__SQLITE_INTERNAL;
    case SQLITE_PERM:       return MGEC__PERM;
    case SQLITE_ABORT:      return MGEC__CALLBACK_ABORTED;
    case SQLITE_BUSY:       return MGEC__DATABASE_BUSY;
    case SQLITE_LOCKED:     return MGEC__DATABASE_LOCKED;
    case SQLITE_NOMEM:      return MGEC__NOMEM;
    case SQLITE_READONLY:   return MGEC__DATABASE_READONLY;
    case SQLITE_INTERRUPT:  return MGEC__SQLITE_INTERRUPT;
    case SQLITE_IOERR:      return MGEC__IO;
    case SQLITE_CORRUPT:    return MGEC__DATABASE_CORRUPT;
    case SQLITE_NOTFOUND:   return MGEC__SQLITE_NOTFOUND;
    case SQLITE_FULL:       return MGEC__DATABASE_FULL;
    case SQLITE_CANTOPEN:   return MGEC__SQLITE_CANTOPEN;
    case SQLITE_PROTOCOL:   return MGEC__PROTO;
    case SQLITE_EMPTY:      return MGEC__SQLITE_EMPTY;
    case SQLITE_SCHEMA:     return MGEC__DATABASE_SCHEMA;
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
