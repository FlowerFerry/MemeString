
#ifndef MEGO_ERROR_EC_IMPL_H_INCLUDED
#define MEGO_ERROR_EC_IMPL_H_INCLUDED

#include <mego/error/ec.h>
#include <mego/predef/os/windows.h>

inline mgec_t mgec__from_sys_err(int _sys_err)
{	
    if (_sys_err <= 0)
		return _sys_err;
    
#if MG_OS__WIN_AVAIL
    switch (_sys_ecode) {
	case ERROR_NOACCESS:                    return MGEC__ACCES;
	case WSAEACCES:                         return MGEC__ACCES;
	case ERROR_ELEVATION_REQUIRED:          return MGEC__ACCES;
	case ERROR_ADDRESS_ALREADY_ASSOCIATED:  return MGEC__ADDRINUSE;
	case WSAEADDRINUSE:                     return MGEC__ADDRINUSE;
	case WSAEADDRNOTAVAIL:                  return MGEC__ADDRNOTAVAIL;
	case WSAEAFNOSUPPORT:                   return MGEC__AFNOSUPPORT;
	case WSAEWOULDBLOCK:                    return MGEC__AGAIN;
	case WSAEALREADY:                       return MGEC__ALREADY;
	case ERROR_INVALID_FLAGS:               return MGEC__BADF;
	case ERROR_INVALID_HANDLE:              return MGEC__BADF;
	case ERROR_LOCK_VIOLATION:              return MGEC__BUSY;
	case ERROR_PIPE_BUSY:                   return MGEC__BUSY;
	case ERROR_SHARING_VIOLATION:           return MGEC__BUSY;
	case ERROR_OPERATION_ABORTED:           return MGEC__CANCELED;
	case WSAEINTR:                          return MGEC__CANCELED;
	case ERROR_NO_UNICODE_TRANSLATION:      return MGEC__CHRNG;
	case ERROR_CONNECTION_ABORTED:          return MGEC__CONNABORTED;
	case WSAECONNABORTED:                   return MGEC__CONNABORTED;
	case ERROR_CONNECTION_REFUSED:          return MGEC__CONNREFUSED;
	case WSAECONNREFUSED:                   return MGEC__CONNREFUSED;
	case ERROR_NETNAME_DELETED:             return MGEC__CONNRESET;
	case WSAECONNRESET:                     return MGEC__CONNRESET;
	case ERROR_ALREADY_EXISTS:              return MGEC__EXIST;
	case ERROR_FILE_EXISTS:                 return MGEC__EXIST;
	case ERROR_BUFFER_OVERFLOW:             return MGEC__FAULT;
	case WSAEFAULT:                         return MGEC__FAULT;
	case ERROR_HOST_UNREACHABLE:            return MGEC__HOSTUNREACH;
	case WSAEHOSTUNREACH:                   return MGEC__HOSTUNREACH;
	case ERROR_INSUFFICIENT_BUFFER:         return MGEC__INVAL;
	case ERROR_INVALID_DATA:                return MGEC__INVAL;
	case ERROR_INVALID_PARAMETER:           return MGEC__INVAL;
	case ERROR_SYMLINK_NOT_SUPPORTED:       return MGEC__INVAL;
	case WSAEINVAL:                         return MGEC__INVAL;
	case WSAEPFNOSUPPORT:                   return MGEC__INVAL;
	case WSAESOCKTNOSUPPORT:                return MGEC__INVAL;
	case ERROR_BEGINNING_OF_MEDIA:          return MGEC__IO;
	case ERROR_BUS_RESET:                   return MGEC__IO;
	case ERROR_CRC:                         return MGEC__IO;
	case ERROR_DEVICE_DOOR_OPEN:            return MGEC__IO;
	case ERROR_DEVICE_REQUIRES_CLEANING:    return MGEC__IO;
	case ERROR_DISK_CORRUPT:                return MGEC__IO;
	case ERROR_EOM_OVERFLOW:                return MGEC__IO;
	case ERROR_FILEMARK_DETECTED:           return MGEC__IO;
	case ERROR_GEN_FAILURE:                 return MGEC__IO;
	case ERROR_INVALID_BLOCK_LENGTH:        return MGEC__IO;
	case ERROR_IO_DEVICE:                   return MGEC__IO;
	case ERROR_NO_DATA_DETECTED:            return MGEC__IO;
	case ERROR_NO_SIGNAL_SENT:              return MGEC__IO;
	case ERROR_OPEN_FAILED:                 return MGEC__IO;
	case ERROR_SETMARK_DETECTED:            return MGEC__IO;
	case ERROR_SIGNAL_REFUSED:              return MGEC__IO;
	case WSAEISCONN:                        return MGEC__ISCONN;
	case ERROR_CANT_RESOLVE_FILENAME:       return MGEC__LOOP;
	case ERROR_TOO_MANY_OPEN_FILES:         return MGEC__MFILE;
	case WSAEMFILE:                         return MGEC__MFILE;
	case WSAEMSGSIZE:                       return MGEC__MSGSIZE;
	case ERROR_FILENAME_EXCED_RANGE:        return MGEC__NAMETOOLONG;
	case ERROR_NETWORK_UNREACHABLE:         return MGEC__NETUNREACH;
	case WSAENETUNREACH:                    return MGEC__NETUNREACH;
	case WSAENOBUFS:                        return MGEC__NOBUFS;
	case ERROR_BAD_PATHNAME:                return MGEC__NOENT;
	case ERROR_DIRECTORY:                   return MGEC__NOENT;
	case ERROR_FILE_NOT_FOUND:              return MGEC__NOENT;
	case ERROR_INVALID_NAME:                return MGEC__NOENT;
	case ERROR_INVALID_DRIVE:               return MGEC__NOENT;
	case ERROR_INVALID_REPARSE_DATA:        return MGEC__NOENT;
	case ERROR_MOD_NOT_FOUND:               return MGEC__NOENT;
	case ERROR_PATH_NOT_FOUND:              return MGEC__NOENT;
	case WSAHOST_NOT_FOUND:                 return MGEC__NOENT;
	case WSANO_DATA:                        return MGEC__NOENT;
	case ERROR_NOT_ENOUGH_MEMORY:           return MGEC__NOMEM;
	case ERROR_OUTOFMEMORY:                 return MGEC__NOMEM;
	case ERROR_CANNOT_MAKE:                 return MGEC__NOSPC;
	case ERROR_DISK_FULL:                   return MGEC__NOSPC;
	case ERROR_EA_TABLE_FULL:               return MGEC__NOSPC;
	case ERROR_END_OF_MEDIA:                return MGEC__NOSPC;
	case ERROR_HANDLE_DISK_FULL:            return MGEC__NOSPC;
	case ERROR_NOT_CONNECTED:               return MGEC__NOTCONN;
	case WSAENOTCONN:                       return MGEC__NOTCONN;
	case ERROR_DIR_NOT_EMPTY:               return MGEC__NOTEMPTY;
	case WSAENOTSOCK:                       return MGEC__NOTSOCK;
	case ERROR_NOT_SUPPORTED:               return MGEC__NOTSUP;
	case ERROR_BROKEN_PIPE:                 return MGEC__EOF;
	case ERROR_ACCESS_DENIED:               return MGEC__PERM;
	case ERROR_PRIVILEGE_NOT_HELD:          return MGEC__PERM;
	case ERROR_BAD_PIPE:                    return MGEC__PIPE;
	case ERROR_NO_DATA:                     return MGEC__PIPE;
	case ERROR_PIPE_NOT_CONNECTED:          return MGEC__PIPE;
	case WSAESHUTDOWN:                      return MGEC__PIPE;
	case WSAEPROTONOSUPPORT:                return MGEC__PROTONOSUPPORT;
	case ERROR_WRITE_PROTECT:               return MGEC__ROFS;
	case ERROR_SEM_TIMEOUT:                 return MGEC__TIMEDOUT;
	case WSAETIMEDOUT:                      return MGEC__TIMEDOUT;
	case ERROR_NOT_SAME_DEVICE:             return MGEC__XDEV;
	case ERROR_INVALID_FUNCTION:            return MGEC__ISDIR;
	case ERROR_META_EXPANSION_TOO_LONG:     return MGEC__2BIG;
	default:                                return MGEC__ERR;
	}
#else
    return MEEC__ERR;
#endif
}


#endif // !MEGO_ERROR_EC_IMPL_H_INCLUDED
