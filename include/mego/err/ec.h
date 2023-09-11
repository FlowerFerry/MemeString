
#ifndef MEGO_ERROR_CODE_H_INCLUDED
#define MEGO_ERROR_CODE_H_INCLUDED

#include <stdint.h>

typedef int mgec_t;

const char* mgec__name(mgec_t _ec);
const char* mgec__desc(mgec_t _ec);

inline mgec_t mgec__from_sys_err(int _sys_err);
inline mgec_t mgec__from_posix_err(int _posix_err);

#define MGEC__POSIX_OFFSET_BEGIN  -100000
#define MGEC__POSIX_OFFSET_END    -199999

#define MGEC__OFFSET1_BEGIN        -200000
#define MGEC__OFFSET1_END          -299999

#define MGEC__OFFSET2_BEGIN        -300000
#define MGEC__OFFSET2_END          -399999

#define MGEC__OK          0      //< 成功 Success
#define MGEC__ERR        -1      //< 未知错误 Unknown error

#define MGEC__PERM                      (MGEC__POSIX_OFFSET_BEGIN - 1)      //< 不允许操作 Operation not permitted
#define MGEC__NOENT                     (MGEC__POSIX_OFFSET_BEGIN - 2)      //< 没有这样的文件或目录 No such file or directory
#define MGEC__SRCH                      (MGEC__POSIX_OFFSET_BEGIN - 3)      //< 没有这样的进程 No such process
#define MGEC__INTR                      (MGEC__POSIX_OFFSET_BEGIN - 4)      //< 中断 Interrupted system call
#define MGEC__IO                        (MGEC__POSIX_OFFSET_BEGIN - 5)      //< IO错误 Input/output error
#define MGEC__NXIO                      (MGEC__POSIX_OFFSET_BEGIN - 6)      //< 设备不存在 No such device or address
#define MGEC__2BIG                      (MGEC__POSIX_OFFSET_BEGIN - 7)      //< 参数列表太长 Arg list too long
#define MGEC__NOEXEC                    (MGEC__POSIX_OFFSET_BEGIN - 8)      //< 执行格式错误 Exec format error
#define MGEC__BADF                      (MGEC__POSIX_OFFSET_BEGIN - 9)      //< 文件描述符错误 Bad file descriptor
#define MGEC__CHILD                     (MGEC__POSIX_OFFSET_BEGIN - 10)     //< 没有子进程 No child processes
#define MGEC__AGAIN                     (MGEC__POSIX_OFFSET_BEGIN - 11)     //< 资源暂时不可用 Try again
#define MGEC__NOMEM                     (MGEC__POSIX_OFFSET_BEGIN - 12)     //< 内存不足 Out of memory
#define MGEC__ACCES                     (MGEC__POSIX_OFFSET_BEGIN - 13)     //< 权限不足 Permission denied
#define MGEC__FAULT                     (MGEC__POSIX_OFFSET_BEGIN - 14)     //< 地址错误 Bad address
#define MGEC__NOTBLK                    (MGEC__POSIX_OFFSET_BEGIN - 15)     //< 块设备错误 Block device required
#define MGEC__BUSY                      (MGEC__POSIX_OFFSET_BEGIN - 16)     //< 设备或资源忙 Device or resource busy
#define MGEC__EXIST                     (MGEC__POSIX_OFFSET_BEGIN - 17)     //< 文件已存在 File exists
#define MGEC__XDEV                      (MGEC__POSIX_OFFSET_BEGIN - 18)     //< 跨设备链接 Cross-device link
#define MGEC__NODEV                     (MGEC__POSIX_OFFSET_BEGIN - 19)     //< 设备不存在 No such device
#define MGEC__NOTDIR                    (MGEC__POSIX_OFFSET_BEGIN - 20)     //< 不是目录 Not a directory
#define MGEC__ISDIR                     (MGEC__POSIX_OFFSET_BEGIN - 21)     //< 是目录 Is a directory
#define MGEC__INVAL                     (MGEC__POSIX_OFFSET_BEGIN - 22)     //< 参数无效 Invalid argument
#define MGEC__NFILE                     (MGEC__POSIX_OFFSET_BEGIN - 23)     //< 文件描述符太多 Too many open files in system
#define MGEC__MFILE                     (MGEC__POSIX_OFFSET_BEGIN - 24)     //< 打开的文件太多 Too many open files
#define MGEC__NOTTY                     (MGEC__POSIX_OFFSET_BEGIN - 25)     //< 不是终端 Not a typewriter
#define MGEC__TXTBSY                    (MGEC__POSIX_OFFSET_BEGIN - 26)     //< 文件忙 Text file busy
#define MGEC__FBIG                      (MGEC__POSIX_OFFSET_BEGIN - 27)     //< 文件太大 File too large
#define MGEC__NOSPC                     (MGEC__POSIX_OFFSET_BEGIN - 28)     //< 没有空间 No space left on device
#define MGEC__SPIPE                     (MGEC__POSIX_OFFSET_BEGIN - 29)     //< 管道错误 Illegal seek
#define MGEC__ROFS                      (MGEC__POSIX_OFFSET_BEGIN - 30)     //< 只读文件系统 Read-only file system
#define MGEC__MLINK                     (MGEC__POSIX_OFFSET_BEGIN - 31)     //< 多重链接 Too many links
#define MGEC__PIPE                      (MGEC__POSIX_OFFSET_BEGIN - 32)     //< 管道错误 Broken pipe
#define MGEC__DOM                       (MGEC__POSIX_OFFSET_BEGIN - 33)     //< 数学域出错 Math argument out of domain of func
#define MGEC__RANGE                     (MGEC__POSIX_OFFSET_BEGIN - 34)     //< 结果太大 Result too large
#define MGEC__DEADLK                    (MGEC__POSIX_OFFSET_BEGIN - 35)     //< 资源死锁 Deadlock would occur
#define MGEC__NAMETOOLONG               (MGEC__POSIX_OFFSET_BEGIN - 36)     //< 文件名太长 File name too long
#define MGEC__NOLCK                     (MGEC__POSIX_OFFSET_BEGIN - 37)     //< 没有锁 No record locks available
#define MGEC__NOSYS                     (MGEC__POSIX_OFFSET_BEGIN - 38)     //< 函数未实现 Function not implemented
#define MGEC__NOTEMPTY                  (MGEC__POSIX_OFFSET_BEGIN - 39)     //< 目录不为空 Directory not empty
#define MGEC__LOOP                      (MGEC__POSIX_OFFSET_BEGIN - 40)     //< 符号链接太多 Too many symbolic links encountered
#define MGEC__NOMSG                     (MGEC__POSIX_OFFSET_BEGIN - 42)     //< 没有消息 No message of desired type
#define MGEC__IDRM                      (MGEC__POSIX_OFFSET_BEGIN - 43)     //< 标识符被删除 Identifier removed
#define MGEC__CHRNG                     (MGEC__POSIX_OFFSET_BEGIN - 44)     //< 设备或资源范围超出 Range error
#define MGEC__L2NSYNC                   (MGEC__POSIX_OFFSET_BEGIN - 45)     //< 级别 2 未同步 Level 2 not synchronized
#define MGEC__L3HLT                     (MGEC__POSIX_OFFSET_BEGIN - 46)     //< 级别 3 停止 Level 3 halted
#define MGEC__L3RST                     (MGEC__POSIX_OFFSET_BEGIN - 47)     //< 级别 3 复位 Level 3 reset
#define MGEC__LNRNG                     (MGEC__POSIX_OFFSET_BEGIN - 48)     //< 链接数超出 Link number out of range
#define MGEC__UNATCH                    (MGEC__POSIX_OFFSET_BEGIN - 49)     //< 协议驱动程序没有连接 Protocol driver not attached
#define MGEC__NOCSI                     (MGEC__POSIX_OFFSET_BEGIN - 50)     //< 没有CSI结构 No CSI structure available
#define MGEC__L2HLT                     (MGEC__POSIX_OFFSET_BEGIN - 51)     //< 级别 2 停止 Level 2 halted
#define MGEC__BADE                      (MGEC__POSIX_OFFSET_BEGIN - 52)     //< 无效的交换 Invalid exchange
#define MGEC__BADR                      (MGEC__POSIX_OFFSET_BEGIN - 53)     //< 无效的请求块 Invalid request descriptor
#define MGEC__XFULL                     (MGEC__POSIX_OFFSET_BEGIN - 54)     //< 交换区满 Exchange full
#define MGEC__NOANO                     (MGEC__POSIX_OFFSET_BEGIN - 55)     //< 无效的关联 Invalid request code
#define MGEC__BADRQC                    (MGEC__POSIX_OFFSET_BEGIN - 56)     //< 无效的请求码 Invalid request code
#define MGEC__BADSLT                    (MGEC__POSIX_OFFSET_BEGIN - 57)     //< 无效的槽 Invalid slot
#define MGEC__BFONT                     (MGEC__POSIX_OFFSET_BEGIN - 59)     //< 错误的字体 Font has no slots
#define MGEC__NOSTR                     (MGEC__POSIX_OFFSET_BEGIN - 60)     //< 设备或资源不可用 Device not a stream
#define MGEC__NODATA                    (MGEC__POSIX_OFFSET_BEGIN - 61)     //< 没有数据 No data available
#define MGEC__TIME                      (MGEC__POSIX_OFFSET_BEGIN - 62)     //< 计时器到期 Timer expired
#define MGEC__NOSR                      (MGEC__POSIX_OFFSET_BEGIN - 63)     //< 没有发送消息 No message of desired type
#define MGEC__NONET                     (MGEC__POSIX_OFFSET_BEGIN - 64)     //< 机器不在网络上 Machine is not on the network
#define MGEC__NOPKG                     (MGEC__POSIX_OFFSET_BEGIN - 65)     //< 包没有安装 Package not installed
#define MGEC__REMOTE                    (MGEC__POSIX_OFFSET_BEGIN - 66)     //< 对象是远程的 Object is remote
#define MGEC__NOLINK                    (MGEC__POSIX_OFFSET_BEGIN - 67)     //< 链接已断开 Link has been severed
#define MGEC__ADV                       (MGEC__POSIX_OFFSET_BEGIN - 68)     //< 广告错误 Advertise error
#define MGEC__SRMNT                     (MGEC__POSIX_OFFSET_BEGIN - 69)     //< 装载错误 Srmount error
#define MGEC__COMM                      (MGEC__POSIX_OFFSET_BEGIN - 70)     //< 通信错误 Communication error on send
#define MGEC__PROTO                     (MGEC__POSIX_OFFSET_BEGIN - 71)     //< 协议错误 Protocol error
#define MGEC__MULTIHOP                  (MGEC__POSIX_OFFSET_BEGIN - 72)     //< 多跳错误 Multihop attempted
#define MGEC__DOTDOT                    (MGEC__POSIX_OFFSET_BEGIN - 73)     //< RFS 操作不允许 RFS specific error
#define MGEC__BADMSG                    (MGEC__POSIX_OFFSET_BEGIN - 74)     //< 错误的消息 Bad message
#define MGEC__OVERFLOW                  (MGEC__POSIX_OFFSET_BEGIN - 75)     //< 值太大 Value too large for defined data type
#define MGEC__NOTUNIQ                   (MGEC__POSIX_OFFSET_BEGIN - 76)     //< 名称不唯一 Name not unique on network
#define MGEC__BADFD                     (MGEC__POSIX_OFFSET_BEGIN - 77)     //< 文件描述符错误 File descriptor in bad state
#define MGEC__REMCHG                    (MGEC__POSIX_OFFSET_BEGIN - 78)     //< 远程地址改变 Remote address changed
#define MGEC__LIBACC                    (MGEC__POSIX_OFFSET_BEGIN - 79)     //< 无法访问共享库 Cannot access a needed shared library
#define MGEC__LIBBAD                    (MGEC__POSIX_OFFSET_BEGIN - 80)     //< 访问损坏的共享库 Accessing a corrupted shared library
#define MGEC__LIBSCN                    (MGEC__POSIX_OFFSET_BEGIN - 81)     //< a.out 中的 .lib 部分已损坏 .lib section in a.out corrupted
#define MGEC__LIBMAX                    (MGEC__POSIX_OFFSET_BEGIN - 82)     //< 尝试链接过多的共享库 Attempting to link in too many shared libraries
#define MGEC__LIBEXEC                   (MGEC__POSIX_OFFSET_BEGIN - 83)     //< 不能执行共享库中的可执行文件 Cannot exec a shared library directly
#define MGEC__ILSEQ                     (MGEC__POSIX_OFFSET_BEGIN - 84)     //< 非法的字节序 Illegal byte sequence
#define MGEC__RESTART                   (MGEC__POSIX_OFFSET_BEGIN - 85)     //< 中断系统调用 Restart syscall
#define MGEC__STRPIPE                   (MGEC__POSIX_OFFSET_BEGIN - 86)     //< 流管道错误 Streams pipe error
#define MGEC__USERS                     (MGEC__POSIX_OFFSET_BEGIN - 87)     //< 用户数太多 Too many users
#define MGEC__NOTSOCK                   (MGEC__POSIX_OFFSET_BEGIN - 88)     //< 描述符不是一个套接字 Socket operation on non-socket
#define MGEC__DESTADDRREQ               (MGEC__POSIX_OFFSET_BEGIN - 89)     //< 需要目标地址 Destination address required
#define MGEC__MSGSIZE                   (MGEC__POSIX_OFFSET_BEGIN - 90)     //< 消息太长 Message too long
#define MGEC__PROTOTYPE                 (MGEC__POSIX_OFFSET_BEGIN - 91)     //< 协议错误 Protocol wrong type for socket
#define MGEC__NOPROTOOPT                (MGEC__POSIX_OFFSET_BEGIN - 92)     //< 协议不可用 Protocol not available
#define MGEC__PROTONOSUPPORT            (MGEC__POSIX_OFFSET_BEGIN - 93)     //< 协议不支持 Protocol not supported
#define MGEC__SOCKTNOSUPPORT            (MGEC__POSIX_OFFSET_BEGIN - 94)     //< 套接字类型不支持 Socket type not supported
#define MGEC__OPNOTSUPP                 (MGEC__POSIX_OFFSET_BEGIN - 95)     //< 不支持操作 Operation not supported on transport endpoint
#define MGEC__PFNOSUPPORT               (MGEC__POSIX_OFFSET_BEGIN - 96)     //< 协议族不支持 Protocol family not supported
#define MGEC__AFNOSUPPORT               (MGEC__POSIX_OFFSET_BEGIN - 97)     //< 地址族不支持 Address family not supported by protocol
#define MGEC__ADDRINUSE                 (MGEC__POSIX_OFFSET_BEGIN - 98)     //< 地址已经被使用 Address already in use
#define MGEC__ADDRNOTAVAIL              (MGEC__POSIX_OFFSET_BEGIN - 99)     //< 不能分配请求的地址 Cannot assign requested address
#define MGEC__NETDOWN                   (MGEC__POSIX_OFFSET_BEGIN - 100)    //< 网络已关闭 Network is down
#define MGEC__NETUNREACH                (MGEC__POSIX_OFFSET_BEGIN - 101)    //< 网络不可达 Network is unreachable
#define MGEC__NETRESET                  (MGEC__POSIX_OFFSET_BEGIN - 102)    //< 网络已经被重置 Network dropped connection because of reset
#define MGEC__CONNABORTED               (MGEC__POSIX_OFFSET_BEGIN - 103)    //< 连接被终止 Software caused connection abort
#define MGEC__CONNRESET                 (MGEC__POSIX_OFFSET_BEGIN - 104)    //< 连接被重置 Connection reset by peer
#define MGEC__NOBUFS                    (MGEC__POSIX_OFFSET_BEGIN - 105)    //< 没有缓冲区空间 No buffer space available
#define MGEC__ISCONN                    (MGEC__POSIX_OFFSET_BEGIN - 106)    //< 套接字已经连接 Transport endpoint is already connected
#define MGEC__NOTCONN                   (MGEC__POSIX_OFFSET_BEGIN - 107)    //< 套接字没有连接 Transport endpoint is not connected
#define MGEC__SHUTDOWN                  (MGEC__POSIX_OFFSET_BEGIN - 108)    //< 不能发送后不能接收 Cannot send after transport endpoint shutdown
#define MGEC__TOOMANYREFS               (MGEC__POSIX_OFFSET_BEGIN - 109)    //< 引用太多 Too many references: cannot splice
#define MGEC__TIMEDOUT                  (MGEC__POSIX_OFFSET_BEGIN - 110)    //< 连接超时 Connection timed out
#define MGEC__CONNREFUSED               (MGEC__POSIX_OFFSET_BEGIN - 111)    //< 连接被拒绝 Connection refused
#define MGEC__HOSTDOWN                  (MGEC__POSIX_OFFSET_BEGIN - 112)    //< 主机已关闭 Host is down
#define MGEC__HOSTUNREACH               (MGEC__POSIX_OFFSET_BEGIN - 113)    //< 没有路由到主机 No route to host
#define MGEC__ALREADY                   (MGEC__POSIX_OFFSET_BEGIN - 114)    //< 操作已经在进行中 Operation already in progress
#define MGEC__INPROGRESS                (MGEC__POSIX_OFFSET_BEGIN - 115)    //< 操作正在进行中 Operation now in progress
#define MGEC__STALE                     (MGEC__POSIX_OFFSET_BEGIN - 116)    //< 文件句柄过期 Stale NFS file handle
#define MGEC__UCLEAN                    (MGEC__POSIX_OFFSET_BEGIN - 117)    //< 结构体已经清理 Structure needs cleaning
#define MGEC__NOTNAM                    (MGEC__POSIX_OFFSET_BEGIN - 118)    //< 不是 XENIX 命名类型 Not a XENIX named type file
#define MGEC__NAVAIL                    (MGEC__POSIX_OFFSET_BEGIN - 119)    //< 没有 XENIX semaphores available
#define MGEC__ISNAM                     (MGEC__POSIX_OFFSET_BEGIN - 120)    //< 是 XENIX 命名类型 Is a named type file
#define MGEC__REMOTEIO                  (MGEC__POSIX_OFFSET_BEGIN - 121)    //< 远程IO错误 Remote I/O error
#define MGEC__DQUOT                     (MGEC__POSIX_OFFSET_BEGIN - 122)    //< 磁盘已满 Quota exceeded
#define MGEC__NOMEDIUM                  (MGEC__POSIX_OFFSET_BEGIN - 123)    //< 没有中间介质 No medium found
#define MGEC__MEDIUMTYPE                (MGEC__POSIX_OFFSET_BEGIN - 124)    //< 中间介质类型错误 Wrong medium type
#define MGEC__CANCELED                  (MGEC__POSIX_OFFSET_BEGIN - 125)    //< 操作被取消 Operation Canceled
#define MGEC__NOKEY                     (MGEC__POSIX_OFFSET_BEGIN - 126)    //< 找不到密钥 Required key not available
#define MGEC__KEYEXPIRED                (MGEC__POSIX_OFFSET_BEGIN - 127)    //< 密钥已过期 Key has expired
#define MGEC__KEYREVOKED                (MGEC__POSIX_OFFSET_BEGIN - 128)    //< 密钥已被吊销 Key has been revoked
#define MGEC__KEYREJECTED               (MGEC__POSIX_OFFSET_BEGIN - 129)    //< 密钥被拒绝 Key was rejected by service
#define MGEC__OWNERDEAD                 (MGEC__POSIX_OFFSET_BEGIN - 130)    //< 拥有者死亡 Owner died
#define MGEC__NOTRECOVERABLE            (MGEC__POSIX_OFFSET_BEGIN - 131)    //< 状态不可恢复 State not recoverable
#define MGEC__RFKILL                    (MGEC__POSIX_OFFSET_BEGIN - 132)    //< 被 RF 锁杀死 Operation not possible due to RF-kill
#define MGEC__HWPOISON                  (MGEC__POSIX_OFFSET_BEGIN - 133)    //< 硬件错误 Memory page has hardware error
#define MGEC__WOULDBLOCK                (MGEC__POSIX_OFFSET_BEGIN - 134)    //< 操作将阻塞 Operation would block

#define MGEC__INVALID_FUNC              (MGEC__OFFSET1_BEGIN - 1)           //< 无效的函数 Invalid function
#define MGEC__PATH_NOT_FOUND            (MGEC__OFFSET1_BEGIN - 3)           //< 路径不存在 The system cannot find the path specified
#define MGEC__INVALID_HANDLE            (MGEC__OFFSET1_BEGIN - 6)           //< 无效的句柄 The handle is invalid
#define MGEC__BAD_ENVIRONMENT           (MGEC__OFFSET1_BEGIN - 10)          //< 环境错误 The environment is incorrect
#define MGEC__INVALID_DATA              (MGEC__OFFSET1_BEGIN - 13)          //< 数据无效 The data is invalid
#define MGEC__DIR_NON_DELETABLE         (MGEC__OFFSET1_BEGIN - 16)          //< 该目录无法删除 The directory cannot be removed
#define MGEC__FILE_MOVE_DISK_FULL       (MGEC__OFFSET1_BEGIN - 17)          //< 无法将文件移动到不同的磁盘 The system cannot move the file to a different disk drive
#define MGEC__NO_MORE_FILES             (MGEC__OFFSET1_BEGIN - 18)          //< 没有更多的文件 No more files have been found
#define MGEC__WRITE_PROTECT             (MGEC__OFFSET1_BEGIN - 19)          //< 磁盘写保护 The media is write protected
#define MGEC__CRC                       (MGEC__OFFSET1_BEGIN - 20)          //< CRC 错误 
#define MGEC__SEEK                      (MGEC__OFFSET1_BEGIN - 25)          //< 寻找错误 The drive cannot locate a specific area or track on the disk
#define MGEC__DISK_ACCESS_DENIED        (MGEC__OFFSET1_BEGIN - 26)          //< 磁盘访问被拒绝 Access to the specified drive is denied
#define MGEC__SECTOR_NOT_FOUND          (MGEC__OFFSET1_BEGIN - 27)          //< 找不到扇区 The drive cannot find the sector requested
#define MGEC__OUT_OF_PAPER              (MGEC__OFFSET1_BEGIN - 28)          //< 打印机缺纸 The printer is out of paper
#define MGEC__WRITE_FAULT               (MGEC__OFFSET1_BEGIN - 29)          //< 写入错误 The system cannot write to the specified device
#define MGEC__READ_FAULT                (MGEC__OFFSET1_BEGIN - 30)          //< 读取错误 The system cannot read from the specified device
#define MGEC__GEN_FAILURE               (MGEC__OFFSET1_BEGIN - 31)          //< 设备未准备好 A device attached to the system is not functioning
#define MGEC__SHARING_VIOLATION         (MGEC__OFFSET1_BEGIN - 32)          //< 进程无法访问文件，因为另一个进程正在使用该文件，或者某些共享访问权限已被拒绝 A process cannot access the file because another process has locked a portion of the file
#define MGEC__LOCK_VIOLATION            (MGEC__OFFSET1_BEGIN - 33)          //< 进程无法访问文件，因为另一个进程已锁定文件的一部分 A process cannot access the file because another process has locked a portion of the file
#define MGEC__WRONG_DISK                (MGEC__OFFSET1_BEGIN - 34)          //< 无法在驱动器上运行该命令 The wrong diskette is in the drive
#define MGEC__SHARING_BUFFER_EXCEEDED   (MGEC__OFFSET1_BEGIN - 36)          //< 无法打开文件，因为共享缓冲区已满 Too many files opened for sharing
#define MGEC__EOF                       (MGEC__OFFSET1_BEGIN - 38)          //< 文件已到达结尾 The end of the file was reached
#define MGEC__DUP_NET_NAME              (MGEC__OFFSET1_BEGIN - 52)          //< 无法在网络上找到或使用指定的网络名称 The network name cannot be found
#define MGEC__BAD_NETPATH               (MGEC__OFFSET1_BEGIN - 53)          //< 网络路径无效 The network path was not found
#define MGEC__NET_BUSY                  (MGEC__OFFSET1_BEGIN - 54)          //< 网络忙 The network is busy
#define MGEC__NETDEV_NOT_EXIST          (MGEC__OFFSET1_BEGIN - 55)          //< 指定的网络资源或设备不再可用 The specified network resource or device is no longer available
#define MGEC__TOO_MANY_NETCMDS          (MGEC__OFFSET1_BEGIN - 56)          //< 网络 BIOS 命令限制已达到 The network BIOS command limit has been reached
#define MGEC__NETADAP_HDW_ERR           (MGEC__OFFSET1_BEGIN - 57)          //< 网络适配器硬件发生错误 A network adapter hardware error occurred.
#define MGEC__BAD_NET_RESP              (MGEC__OFFSET1_BEGIN - 58)          //< 指定的服务器无法执行请求的操作 The specified server cannot perform the requested operation
#define MGEC__UNEXP_NET_ERR             (MGEC__OFFSET1_BEGIN - 59)          //< 意外的网络错误 An unexpected network error occurred
#define MGEC__BAD_REM_ADAP              (MGEC__OFFSET1_BEGIN - 60)          //< 指定的网络适配器名称无效 The remote adapter is not compatible
#define MGEC__PRINTQ_FULL               (MGEC__OFFSET1_BEGIN - 61)          //< 打印队列已满 The printer queue is full
#define MGEC__NO_SPOOL_SPACE            (MGEC__OFFSET1_BEGIN - 62)          //< 服务器上没有可用空间来存储等待打印的文件 Space to store the file waiting to be printed is not available on the server.
#define MGEC__PRINT_CANCELLED           (MGEC__OFFSET1_BEGIN - 63)          //< 您等待打印的文件已被删除 The file waiting to be printed was deleted
#define MGEC__NETNAME_DELETED           (MGEC__OFFSET1_BEGIN - 64)          //< 指定的网络名称不再可用 The specified network name is no longer available
#define MGEC__BAD_NETDEV_TYPE           (MGEC__OFFSET1_BEGIN - 66)          //< 网络资源类型不正确 The network resource type is not correct
#define MGEC__BAD_NET_NAME              (MGEC__OFFSET1_BEGIN - 67)          //< 指定的网络名称无效 The specified network name is invalid
#define MGEC__TOO_MANY_NET_NAMES        (MGEC__OFFSET1_BEGIN - 68)          //< 超出本地计算机网络适配器卡的名称限制 The name limit for the local computer network adapter card was exceeded
#define MGEC__TOO_MANY_SESS             (MGEC__OFFSET1_BEGIN - 69)          //< 会话表已满 The network BIOS session limit was exceeded
#define MGEC__SHARING_PAUSED            (MGEC__OFFSET1_BEGIN - 70)          //< 文件共享暂停 The remote server has been paused or is in the process of being started
#define MGEC__REDIR_PAUSED              (MGEC__OFFSET1_BEGIN - 72)          //< 文件重定向暂停 The specified printer or disk device has been paused
#define MGEC__CANNOT_MAKE               (MGEC__OFFSET1_BEGIN - 82)          //< 目录或文件无法创建 The directory or file cannot be created
#define MGEC__FAIL_I24                  (MGEC__OFFSET1_BEGIN - 83)          //< INT 24 失败 An error occurred when opening or reading the configuration file
#define MGEC__OUT_OF_STRUCTURES         (MGEC__OFFSET1_BEGIN - 84)          //< 处理此请求的存储不可用 Storage to process this request is not available
#define MGEC__ALREADY_ASSIGNED          (MGEC__OFFSET1_BEGIN - 85)          //< 本地设备名称已在使用中 The local device name is already in use
#define MGEC__INVALID_NET_PASSWORD      (MGEC__OFFSET1_BEGIN - 86)          //< 指定的网络密码无效 The specified network password is not correct
#define MGEC__NET_WRITE_FAULT           (MGEC__OFFSET1_BEGIN - 88)          //< 网络写错误 A write fault occurred on the network
#define MGEC__NO_PROC_SLOTS             (MGEC__OFFSET1_BEGIN - 89)          //< 系统进程表已满 The system cannot start another process at this time
#define MGEC__TOO_MANY_SEMAPHORES       (MGEC__OFFSET1_BEGIN - 100)         //< 信号灯表已满 Cannot create another system semaphore
#define MGEC__EXCL_SEM_ALREADY_OWNED    (MGEC__OFFSET1_BEGIN - 101)         //< 信号灯已经拥有 The exclusive semaphore is owned by another process
#define MGEC__SEM_IS_SET                (MGEC__OFFSET1_BEGIN - 102)         //< 信号灯已设置 The semaphore is set and cannot be closed
#define MGEC__TOO_MANY_SEM_REQUESTS     (MGEC__OFFSET1_BEGIN - 103)         //< 信号灯表已满 Cannot request exclusive semaphores at interrupt time
#define MGEC__INVALID_AT_INTERRUPT_TIME (MGEC__OFFSET1_BEGIN - 104)         //< 信号灯不能在中断时间设置 Cannot request exclusive semaphores at interrupt time.
#define MGEC__SEM_OWNER_DIED            (MGEC__OFFSET1_BEGIN - 105)         //< 信号灯拥有者已死 The previous ownership of this semaphore has ended.
#define MGEC__SEM_USER_LIMIT            (MGEC__OFFSET1_BEGIN - 106)         //< 插入驱动器 %1 的软盘 Insert the diskette for drive %1
#define MGEC__DISK_CHANGE               (MGEC__OFFSET1_BEGIN - 107)         //< 插入了磁盘 The program stopped because an alternate diskette was not inserted
#define MGEC__DRIVE_LOCKED              (MGEC__OFFSET1_BEGIN - 108)         //< 磁盘被锁定 The disk is in use or locked by another process
#define MGEC__BROKEN_PIPE               (MGEC__OFFSET1_BEGIN - 109)         //< 管道已结束 The pipe has been ended
#define MGEC__OPEN_FAILED               (MGEC__OFFSET1_BEGIN - 110)         //< 打开设备或文件失败 Cannot open the device or file specified
#define MGEC__DISK_FULL                 (MGEC__OFFSET1_BEGIN - 112)         //< 磁盘空间不足 There is not enough space on the disk
#define MGEC__NO_MORE_SEARCH_HANDLES    (MGEC__OFFSET1_BEGIN - 113)         //< 无法创建更多的搜索句柄 No more internal file identifiers available
#define MGEC__INVALID_TARGET_HANDLE     (MGEC__OFFSET1_BEGIN - 114)         //< 目标句柄不正确 The target internal file identifier is incorrect
#define MGEC__INVALID_CATEGORY          (MGEC__OFFSET1_BEGIN - 117)         //< 无效的I/O类别 The IOCTL call made by the application program is not correct
#define MGEC__INVALID_VERIFY_SWITCH     (MGEC__OFFSET1_BEGIN - 118)         //< 写入验证开关参数值不正确 The verify-on-write switch parameter value is not correct
#define MGEC__BAD_DRIVER_LEVEL          (MGEC__OFFSET1_BEGIN - 119)         //< 系统调用级别错误 The system does not support the command requested
#define MGEC__DATABASE_LOCKED           (MGEC__OFFSET1_BEGIN - 1055)        //< 数据库文件被锁定 Database file is locked
#define MGEC__DATABASE_DOES_NOT_EXIST   (MGEC__OFFSET1_BEGIN - 1065)        //< 数据库文件不存在 Database file does not exist
#define MGEC__DATABASE_FAILURE          (MGEC__OFFSET1_BEGIN - 4313)        //< 数据库操作失败 Database operation failed
#define MGEC__DATABASE_FULL             (MGEC__OFFSET1_BEGIN - 4314)        //< 数据库已满 Database or disk is full
#define MGEC__DATABASE_ERROR            (MGEC__OFFSET1_BEGIN - 8409)        //< 数据库错误 Internal database error

#define MGEC__SQLITE_INTERNAL           (MGEC__OFFSET2_BEGIN - 2)           //< SQLite内部逻辑错误 Internal logic error in SQLite
#define MGEC__CALLBACK_ABORTED          (MGEC__OFFSET2_BEGIN - 4)           //< 回调函数请求中止 Callback routine requested an abort
#define MGEC__DATABASE_BUSY             (MGEC__OFFSET2_BEGIN - 5)           //< 数据库忙 Database is locked
#define MGEC__DATABASE_READONLY         (MGEC__OFFSET2_BEGIN - 8)           //< 尝试写入只读数据库 Attempt to write a readonly database
#define MGEC__SQLITE_INTERRUPT          (MGEC__OFFSET2_BEGIN - 9)           //< 操作被sqlite3_interrupt()中断 Operation terminated by sqlite3_interrupt()
#define MGEC__DATABASE_CORRUPT          (MGEC__OFFSET2_BEGIN - 11)          //< 数据库文件已损坏 Database disk image is malformed
#define MGEC__SQLITE_NOTFOUND           (MGEC__OFFSET2_BEGIN - 12)          //< sqlite3_file_control() 中的未知操作码 Unknown opcode in sqlite3_file_control()
#define MGEC__SQLITE_CANTOPEN           (MGEC__OFFSET2_BEGIN - 14)          //< 无法打开数据库 Unable to open the database file 
#define MGEC__SQLITE_EMPTY              (MGEC__OFFSET2_BEGIN - 16)          //< Internal use only
#define MGEC__DATABASE_SCHEMA           (MGEC__OFFSET2_BEGIN - 17)          //< 数据库模式已更改 Database schema has changed


#endif // !MEGO_ERROR_CODE_H_INCLUDED
