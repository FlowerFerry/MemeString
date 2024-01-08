
#ifndef MEGO_ERR_SQLITE3_CONVERT_H_INCLUDED
#define MEGO_ERR_SQLITE3_CONVERT_H_INCLUDED

#include <mego/err/ec.h>
#include <mego/predef/symbol/inline.h>
#include <sqlite3.h>

MG_CAPI_INLINE mgec_t mgec__from_sqlite3_err(int _rc)
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
    case SQLITE_TOOBIG:     return MGEC__SQLITE_TOOBIG;
    case SQLITE_CONSTRAINT: return MGEC__SQLITE_CONSTRAINT;
    case SQLITE_MISMATCH:   return MGEC__SQLITE_MISMATCH;
    case SQLITE_MISUSE:     return MGEC__SQLITE_MISUSE;
    case SQLITE_NOLFS:      return MGEC__SQLITE_NOLFS;
    case SQLITE_AUTH:       return MGEC__DATABASE_AUTH;
    case SQLITE_FORMAT:     return MGEC__SQLITE_FORMAT;
    case SQLITE_RANGE:      return MGEC__RANGE;
    case SQLITE_NOTADB:     return MGEC__SQLITE_NOTADB;
    case SQLITE_NOTICE:     return MGEC__SQLITE_NOTICE;
    case SQLITE_WARNING:    return MGEC__SQLITE_WARNING;
    case SQLITE_ROW:        return MGEC__SQLITE_ROW;
    case SQLITE_DONE:       return MGEC__SQLITE_DONE;
    default:                return MGEC__ERR;
    }
}

#endif // !MEGO_ERR_SQLITE3_CONVERT_H_INCLUDED
