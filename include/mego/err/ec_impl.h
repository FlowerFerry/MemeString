
#ifndef MEGO_ERROR_EC_IMPL_H_INCLUDED
#define MEGO_ERROR_EC_IMPL_H_INCLUDED


#include <mego/err/ec.h>
#include <mego/predef/os/windows.h>
#include <mego/predef/symbol/inline.h>
#include <mego/util/os/windows/windows_simplify.h>

#include <errno.h>

MG_CAPI_INLINE mgec_t mgec__from_sys_err(int _sys_err)
{	
    if (_sys_err <= 0)
		return _sys_err;
    
#if MG_OS__WIN_AVAIL

    switch (_sys_err) {

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

	case ERROR_NOT_SUPPORTED:               return MGEC__OPNOTSUPP;
	// case ERROR_BROKEN_PIPE:                 return MGEC__EOF;

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

    return MGEC__ERR;
#endif
}

MG_CAPI_INLINE mgec_t mgec__from_posix_err(int _posix_err)
{
	if (_posix_err == 0)
		return 0;
	
	switch (_posix_err) {
#ifdef EACCES
	case EACCES:            return MGEC__ACCES;
#endif
#ifdef EADDRINUSE
	case EADDRINUSE:        return MGEC__ADDRINUSE;
#endif
#ifdef EADDRNOTAVAIL
	case EADDRNOTAVAIL:     return MGEC__ADDRNOTAVAIL;
#endif
#ifdef EAFNOSUPPORT
	case EAFNOSUPPORT:      return MGEC__AFNOSUPPORT;
#endif
#ifdef EAGAIN
	case EAGAIN:            return MGEC__AGAIN;
#endif
#ifdef EALREADY
	case EALREADY:          return MGEC__ALREADY;
#endif
#ifdef EBADF
	case EBADF:             return MGEC__BADF;
#endif
#ifdef EBUSY
	case EBUSY:             return MGEC__BUSY;
#endif
#ifdef ECANCELED
	case ECANCELED:         return MGEC__CANCELED;
#endif
#ifdef ECHRNG
	case ECHRNG:            return MGEC__CHRNG;
#endif
#ifdef ECONNABORTED
	case ECONNABORTED:      return MGEC__CONNABORTED;
#endif
#ifdef ECONNREFUSED
	case ECONNREFUSED:      return MGEC__CONNREFUSED;
#endif
#ifdef ECONNRESET
	case ECONNRESET:        return MGEC__CONNRESET;
#endif
#ifdef EEXIST
	case EEXIST:            return MGEC__EXIST;
#endif
#ifdef EFAULT	
	case EFAULT:            return MGEC__FAULT;
#endif
#ifdef EHOSTUNREACH	
	case EHOSTUNREACH:      return MGEC__HOSTUNREACH;
#endif
#ifdef EINTR
	case EINTR:             return MGEC__INTR;
#endif
#ifdef EINVAL
	case EINVAL:            return MGEC__INVAL;
#endif
#ifdef EIO
	case EIO:               return MGEC__IO;
#endif
#ifdef EISCONN
	case EISCONN:           return MGEC__ISCONN;
#endif
#ifdef ELOOP
	case ELOOP:             return MGEC__LOOP;
#endif
#ifdef EMFILE
	case EMFILE:            return MGEC__MFILE;
#endif
#ifdef EMSGSIZE
	case EMSGSIZE:          return MGEC__MSGSIZE;
#endif
#ifdef ENAMETOOLONG
	case ENAMETOOLONG:      return MGEC__NAMETOOLONG;
#endif
#ifdef ENETUNREACH
	case ENETUNREACH:       return MGEC__NETUNREACH;
#endif
#ifdef ENOBUFS
	case ENOBUFS:           return MGEC__NOBUFS;
#endif
#ifdef ENOENT
	case ENOENT:            return MGEC__NOENT;
#endif
#ifdef ENOMEM
	case ENOMEM:            return MGEC__NOMEM;
#endif
#ifdef ENOSPC
	case ENOSPC:            return MGEC__NOSPC;
#endif
#ifdef ENOTCONN
	case ENOTCONN:          return MGEC__NOTCONN;
#endif
#ifdef ENOTDIR
	case ENOTDIR:           return MGEC__NOTDIR;
#endif
#ifdef ENOTEMPTY
	case ENOTEMPTY:         return MGEC__NOTEMPTY;
#endif
#ifdef ENOTSOCK
	case ENOTSOCK:          return MGEC__NOTSOCK;
#endif
#if defined(ENOTSUP) && ENOTSUP != EOPNOTSUPP
	case ENOTSUP:           return MGEC__OPNOTSUPP;
#endif
#ifdef EPIPE
	case EPIPE:             return MGEC__PIPE;
#endif
#ifdef EPROTONOSUPPORT
	case EPROTONOSUPPORT:   return MGEC__PROTONOSUPPORT;
#endif
#ifdef EROFS
	case EROFS:             return MGEC__ROFS;
#endif
#ifdef ETIMEDOUT
	case ETIMEDOUT:         return MGEC__TIMEDOUT;
#endif
#ifdef EXDEV
	case EXDEV:             return MGEC__XDEV;
#endif
#ifdef E2BIG
	case E2BIG:             return MGEC__2BIG;
#endif
#ifdef EDEADLK
	case EDEADLK:           return MGEC__DEADLK;
#endif
#ifdef EFBIG
	case EFBIG:             return MGEC__FBIG;
#endif
#ifdef EHOSTDOWN
	case EHOSTDOWN:         return MGEC__HOSTDOWN;
#endif
#ifdef EIDRM
	case EIDRM:             return MGEC__IDRM;
#endif
#ifdef EILSEQ
	case EILSEQ:            return MGEC__ILSEQ;
#endif
#ifdef EINPROGRESS
	case EINPROGRESS:       return MGEC__INPROGRESS;
#endif
#ifdef EISDIR
	case EISDIR:            return MGEC__ISDIR;
#endif
#ifdef EMLINK
	case EMLINK:            return MGEC__MLINK;
#endif
#ifdef ENETDOWN
	case ENETDOWN:          return MGEC__NETDOWN;
#endif
#ifdef ENOLCK
	case ENOLCK:            return MGEC__NOLCK;
#endif
#ifdef ENOSYS
	case ENOSYS:            return MGEC__NOSYS;
#endif
#ifdef ENOTTY
	case ENOTTY:            return MGEC__NOTTY;
#endif
#ifdef ENXIO
	case ENXIO:             return MGEC__NXIO;
#endif
#ifdef EOPNOTSUPP
	case EOPNOTSUPP:        return MGEC__OPNOTSUPP;
#endif
#ifdef EOVERFLOW
	case EOVERFLOW:         return MGEC__OVERFLOW;
#endif
#ifdef EPERM
	case EPERM:             return MGEC__PERM;
#endif
#ifdef ESRCH
	case ESRCH:             return MGEC__SRCH;
#endif
#ifdef ETXTBSY
	case ETXTBSY:           return MGEC__TXTBSY;
#endif
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
	case EWOULDBLOCK:       return MGEC__WOULDBLOCK;
#endif
#ifdef EDOM
	case EDOM:              return MGEC__DOM;
#endif
#ifdef ERANGE
	case ERANGE:            return MGEC__RANGE;
#endif
#ifdef EUCLEAN
	case EUCLEAN:           return MGEC__UCLEAN;
#endif
#ifdef ENOTRECOVERABLE
	case ENOTRECOVERABLE:   return MGEC__NOTRECOVERABLE;
#endif
#ifdef EOWNERDEAD
	case EOWNERDEAD:        return MGEC__OWNERDEAD;
#endif
#ifdef EPROTO
	case EPROTO:            return MGEC__PROTO;
#endif
#ifdef EREMOTE
	case EREMOTE:           return MGEC__REMOTE;
#endif
#ifdef EREMOTEIO
	case EREMOTEIO:         return MGEC__REMOTEIO;
#endif
#ifdef ERESTART
	case ERESTART:          return MGEC__RESTART;
#endif
#ifdef ESHUTDOWN
	case ESHUTDOWN:         return MGEC__SHUTDOWN;
#endif
#ifdef ESTRPIPE
	case ESTRPIPE:          return MGEC__STRPIPE;
#endif
#ifdef ETIME
	case ETIME:             return MGEC__TIME;
#endif
#ifdef EUNATCH
	case EUNATCH:           return MGEC__UNATCH;
#endif
#ifdef EUSERS
	case EUSERS:            return MGEC__USERS;
#endif
#ifdef EXFULL
	case EXFULL:            return MGEC__XFULL;
#endif
#ifdef EBADE
	case EBADE:             return MGEC__BADE;
#endif
#ifdef EBADFD
	case EBADFD:            return MGEC__BADFD;
#endif
#ifdef EBADR
	case EBADR:             return MGEC__BADR;
#endif
#ifdef EBADRQC
	case EBADRQC:           return MGEC__BADRQC;
#endif
#ifdef EBADSLT
	case EBADSLT:           return MGEC__BADSLT;
#endif
#ifdef EBFONT
	case EBFONT:            return MGEC__BFONT;
#endif
#ifdef ECHILD
	case ECHILD:            return MGEC__CHILD;
#endif
#ifdef ENETRESET
	case ENETRESET:         return MGEC__NETRESET;
#endif
#ifdef ENOCSI
	case ENOCSI:            return MGEC__NOCSI;
#endif
#ifdef ETOOMANYREFS
	case ETOOMANYREFS:      return MGEC__TOOMANYREFS;
#endif
#ifdef ESTALE
	case ESTALE:            return MGEC__STALE;
#endif
#ifdef ENOTNAM
	case ENOTNAM:           return MGEC__NOTNAM;
#endif
#ifdef ENAVAIL
	case ENAVAIL:           return MGEC__NAVAIL;
#endif
#ifdef EISNAM
	case EISNAM:            return MGEC__ISNAM;
#endif
#ifdef EDQUOT
	case EDQUOT:            return MGEC__DQUOT;
#endif
#ifdef ENOMEDIUM
	case ENOMEDIUM:         return MGEC__NOMEDIUM;
#endif
#ifdef EMEDIUMTYPE
	case EMEDIUMTYPE:       return MGEC__MEDIUMTYPE;
#endif
#ifdef ENOKEY
	case ENOKEY:            return MGEC__NOKEY;
#endif
#ifdef EKEYEXPIRED
	case EKEYEXPIRED:       return MGEC__KEYEXPIRED;
#endif
#ifdef EKEYREVOKED
	case EKEYREVOKED:       return MGEC__KEYREVOKED;
#endif
#ifdef EKEYREJECTED
	case EKEYREJECTED:      return MGEC__KEYREJECTED;
#endif
#ifdef ERFKILL
	case ERFKILL:           return MGEC__RFKILL;
#endif
#ifdef EHWPOISON
	case EHWPOISON:         return MGEC__HWPOISON;
#endif
#ifdef EPFNOSUPPORT
	case EPFNOSUPPORT:      return MGEC__PFNOSUPPORT;
#endif
#ifdef ESOCKTNOSUPPORT
	case ESOCKTNOSUPPORT:   return MGEC__SOCKTNOSUPPORT;
#endif
#ifdef ENOPROTOOPT
	case ENOPROTOOPT:       return MGEC__OPNOTSUPP;
#endif
#ifdef EPROTOTYPE
	case EPROTOTYPE:        return MGEC__PROTOTYPE;
#endif
#ifdef EDESTADDRREQ
	case EDESTADDRREQ:      return MGEC__DESTADDRREQ;
#endif
#ifdef EREMCHG
	case EREMCHG:           return MGEC__REMCHG;
#endif
#ifdef ELIBACC
	case ELIBACC:           return MGEC__LIBACC;
#endif
#ifdef ELIBBAD
	case ELIBBAD:           return MGEC__LIBBAD;
#endif
#ifdef ELIBSCN
	case ELIBSCN:           return MGEC__LIBSCN;
#endif
#ifdef ELIBMAX
	case ELIBMAX:           return MGEC__LIBMAX;
#endif
#ifdef ELIBEXEC
	case ELIBEXEC:          return MGEC__LIBEXEC;
#endif
#ifdef EMULTIHOP
	case EMULTIHOP:         return MGEC__MULTIHOP;
#endif
#ifdef ENOLINK
	case ENOLINK:           return MGEC__NOLINK;
#endif
#ifdef EADV
	case EADV:              return MGEC__ADV;
#endif
#ifdef ESRMNT
	case ESRMNT:            return MGEC__SRMNT;
#endif
#ifdef ECOMM
	case ECOMM:             return MGEC__COMM;
#endif
#ifdef EDOTDOT
	case EDOTDOT:           return MGEC__DOTDOT;
#endif
#ifdef EBADMSG
	case EBADMSG:           return MGEC__BADMSG;
#endif
#ifdef ENOTUNIQ
	case ENOTUNIQ:          return MGEC__NOTUNIQ;
#endif
#ifdef ENOANO
	case ENOANO:            return MGEC__NOANO;
#endif
#ifdef ENOSTR
	case ENOSTR:            return MGEC__NOSTR;
#endif
#ifdef ENODATA
	case ENODATA:           return MGEC__NODATA;
#endif
#ifdef ENOSR
	case ENOSR:             return MGEC__NOSR;
#endif
#ifdef ENONET
	case ENONET:            return MGEC__NONET;
#endif
#ifdef ENOPKG
	case ENOPKG:            return MGEC__NOPKG;
#endif
#ifdef EL2HLT
	case EL2HLT:            return MGEC__L2HLT;
#endif
#ifdef ELNRNG
	case ELNRNG:            return MGEC__LNRNG;
#endif
#ifdef EL3RST
	case EL3RST:            return MGEC__L3RST;
#endif
#ifdef EL3HLT
	case EL3HLT:            return MGEC__L3HLT;
#endif
#ifdef EL2NSYNC
	case EL2NSYNC:          return MGEC__L2NSYNC;
#endif
#ifdef ENOMSG
	case ENOMSG:            return MGEC__NOMSG;
#endif
#ifdef ESPIPE
	case ESPIPE:            return MGEC__SPIPE;
#endif
#ifdef ENFILE
	case ENFILE:            return MGEC__NFILE;
#endif
#ifdef ENODEV
	case ENODEV:            return MGEC__NODEV;
#endif
#ifdef ENOTBLK
	case ENOTBLK:           return MGEC__NOTBLK;
#endif
#ifdef ENOEXEC
	case ENOEXEC:           return MGEC__NOEXEC;
#endif
	default:                return MGEC__ERR;
	}
}


#endif // !MEGO_ERROR_EC_IMPL_H_INCLUDED
