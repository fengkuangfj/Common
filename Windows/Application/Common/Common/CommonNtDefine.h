
#pragma once

#include "CommonPublic.h"
#include <windows.h>
#include <winternl.h>

#ifndef _Out_writes_bytes_
#define _Out_writes_bytes_(size) _Out_
#endif

#ifndef _In_reads_bytes_
#define _In_reads_bytes_(size) _In_
#endif

#ifndef _In_reads_bytes_opt_
#define _In_reads_bytes_opt_(size) _In_
#endif

#ifndef _Outptr_result_bytebuffer_
#define _Outptr_result_bytebuffer_(size) _Out_
#endif

#ifndef _Out_writes_bytes_opt_
#define _Out_writes_bytes_opt_(size) _Out_
#endif

#ifndef _Inout_updates_opt_
#define _Inout_updates_opt_(size) _Inout_
#endif

#ifndef _Out_writes_bytes_to_opt_
#define _Out_writes_bytes_to_opt_(s,c) _Out_
#endif

#ifndef _Reserved_
#define _Reserved_
#endif

#define ROUND_TO_SIZE(_length, _alignment) \
    ((((ULONG_PTR)(_length)) + ((_alignment)-1)) & ~(ULONG_PTR) ((_alignment) - 1))

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L) // ntsubauth
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)
#define STATUS_END_OF_FILE ((NTSTATUS)0xC0000011L)
#define STATUS_INVALID_FILE_FOR_SECTION ((NTSTATUS)0xC0000020L)
#define STATUS_ACCESS_DENIED ((NTSTATUS)0xC0000022L)
#define STATUS_BUFFER_TOO_SMALL ((NTSTATUS)0xC0000023L)
#define STATUS_OBJECT_NAME_NOT_FOUND ((NTSTATUS)0xC0000034L)
#define STATUS_BUFFER_OVERFLOW ((NTSTATUS)0x80000005L)

#define IOCTL_LMR_DISABLE_LOCAL_BUFFERING 0x140390
#define FSCTL_LMR_GET_HINT_SIZE 0x1401C4

#define FILE_WRITE_TO_END_OF_FILE 0xffffffff
#define FILE_USE_FILE_POINTER_POSITION 0xfffffffe

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN10_RS1)
#define FILE_RENAME_REPLACE_IF_EXISTS 0x00000001
#define FILE_RENAME_POSIX_SEMANTICS 0x00000002
#endif

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN10_RS3)
#define FILE_RENAME_SUPPRESS_PIN_STATE_INHERITANCE 0x00000004
#endif

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN10_RS5)
#define FILE_RENAME_SUPPRESS_STORAGE_RESERVE_INHERITANCE 0x00000008
#define FILE_RENAME_NO_INCREASE_AVAILABLE_SPACE 0x00000010
#define FILE_RENAME_NO_DECREASE_AVAILABLE_SPACE 0x00000020
#define FILE_RENAME_PRESERVE_AVAILABLE_SPACE 0x00000030 // combination
#define FILE_RENAME_IGNORE_READONLY_ATTRIBUTE 0x00000040
#endif

typedef enum _SS_DEDS_FILE_INFORMATION_CLASS
{
    SsDedsFileDirectoryInformation = FileDirectoryInformation,
    FileFullDirectoryInformation, // 2
    FileBothDirectoryInformation, // 3
    FileBasicInformation, // 4
    FileStandardInformation, // 5
    FileInternalInformation, // 6
    FileEaInformation, // 7
    FileAccessInformation, // 8
    FileNameInformation, // 9
    FileRenameInformation, // 10
    FileLinkInformation, // 11
    FileNamesInformation, // 12
    FileDispositionInformation, // 13
    FilePositionInformation, // 14
    FileFullEaInformation, // 15
    FileModeInformation, // 16
    FileAlignmentInformation, // 17
    FileAllInformation, // 18
    FileAllocationInformation, // 19
    FileEndOfFileInformation, // 20
    FileAlternateNameInformation, // 21
    FileStreamInformation, // 22
    FilePipeInformation, // 23
    FilePipeLocalInformation, // 24
    FilePipeRemoteInformation, // 25
    FileMailslotQueryInformation, // 26
    FileMailslotSetInformation, // 27
    FileCompressionInformation, // 28
    FileObjectIdInformation, // 29
    FileCompletionInformation, // 30
    FileMoveClusterInformation, // 31
    FileQuotaInformation, // 32
    FileReparsePointInformation, // 33
    FileNetworkOpenInformation, // 34
    FileAttributeTagInformation, // 35
    FileTrackingInformation, // 36
    FileIdBothDirectoryInformation, // 37
    FileIdFullDirectoryInformation, // 38
    FileValidDataLengthInformation, // 39
    FileShortNameInformation, // 40
    FileIoCompletionNotificationInformation, // 41
    FileIoStatusBlockRangeInformation, // 42
    FileIoPriorityHintInformation, // 43
    FileSfioReserveInformation, // 44
    FileSfioVolumeInformation, // 45
    FileHardLinkInformation, // 46
    FileProcessIdsUsingFileInformation, // 47
    FileNormalizedNameInformation, // 48
    FileNetworkPhysicalNameInformation, // 49
    FileIdGlobalTxDirectoryInformation, // 50
    FileIsRemoteDeviceInformation, // 51
    FileUnusedInformation, // 52
    FileNumaNodeInformation, // 53
    FileStandardLinkInformation, // 54
    FileRemoteProtocolInformation, // 55

    //
    // These are special versions of these operations (defined earlier)
    // which can be used by kernel mode drivers only to bypass security
    // access checks for Rename and HardLink operations. These operations
    // are only recognized by the IOManager, a file system should never
    // receive these.
    //

    FileRenameInformationBypassAccessCheck, // 56
    FileLinkInformationBypassAccessCheck, // 57

    //
    // End of special information classes reserved for IOManager.
    //

    FileVolumeNameInformation, // 58
    FileIdInformation, // 59
    FileIdExtdDirectoryInformation, // 60
    FileReplaceCompletionInformation, // 61
    FileHardLinkFullIdInformation, // 62
    FileIdExtdBothDirectoryInformation, // 63
    FileDispositionInformationEx, // 64
    FileRenameInformationEx, // 65
    FileRenameInformationExBypassAccessCheck, // 66
    FileDesiredStorageClassInformation, // 67
    FileStatInformation, // 68
    FileMemoryPartitionInformation, // 69
    FileStatLxInformation, // 70
    FileCaseSensitiveInformation, // 71
    FileLinkInformationEx, // 72
    FileLinkInformationExBypassAccessCheck, // 73
    FileStorageReserveIdInformation, // 74
    FileCaseSensitiveInformationForceAccessCheck, // 75
    FileMaximumInformation
} SS_DEDS_FILE_INFORMATION_CLASS, *PSS_DEDS_FILE_INFORMATION_CLASS;

typedef enum _SECTION_INHERIT
{
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT;

typedef enum _SS_DEDS_OBJECT_INFORMATION_CLASS
{
    ObjectNameInformation = 1
} SS_DEDS_OBJECT_INFORMATION_CLASS, *PSS_DEDS_OBJECT_INFORMATION_CLASS;

typedef struct _FILE_RENAME_INFORMATION
{
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN10_RS1)
    union
    {
        BOOLEAN ReplaceIfExists; // FileRenameInformation
        ULONG Flags; // FileRenameInformationEx
    } DUMMYUNIONNAME;
#else
    BOOLEAN ReplaceIfExists;
#endif
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_RENAME_INFORMATION, *PFILE_RENAME_INFORMATION;

typedef struct _FILE_END_OF_FILE_INFORMATION
{
    LARGE_INTEGER EndOfFile;
} FILE_END_OF_FILE_INFORMATION, *PFILE_END_OF_FILE_INFORMATION;

typedef struct _FILE_POSITION_INFORMATION
{
    LARGE_INTEGER CurrentByteOffset;
} FILE_POSITION_INFORMATION, *PFILE_POSITION_INFORMATION;

typedef struct _FILE_VALID_DATA_LENGTH_INFORMATION
{
    LARGE_INTEGER ValidDataLength;
} FILE_VALID_DATA_LENGTH_INFORMATION, *PFILE_VALID_DATA_LENGTH_INFORMATION;

typedef struct _FILE_INTERNAL_INFORMATION
{
    LARGE_INTEGER IndexNumber;
} FILE_INTERNAL_INFORMATION, *PFILE_INTERNAL_INFORMATION;

typedef struct _FILE_EA_INFORMATION
{
    ULONG EaSize;
} FILE_EA_INFORMATION, *PFILE_EA_INFORMATION;

typedef struct _FILE_ACCESS_INFORMATION
{
    ACCESS_MASK AccessFlags;
} FILE_ACCESS_INFORMATION, *PFILE_ACCESS_INFORMATION;

typedef struct _FILE_MODE_INFORMATION
{
    ULONG Mode;
} FILE_MODE_INFORMATION, *PFILE_MODE_INFORMATION;

typedef struct _FILE_BASIC_INFORMATION
{
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    ULONG FileAttributes;
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;

typedef struct _FILE_STANDARD_INFORMATION
{
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG NumberOfLinks;
    BOOLEAN DeletePending;
    BOOLEAN Directory;
} FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;

typedef struct _FILE_ALIGNMENT_INFORMATION
{
    ULONG AlignmentRequirement;
} FILE_ALIGNMENT_INFORMATION, *PFILE_ALIGNMENT_INFORMATION;

typedef struct _FILE_NAME_INFORMATION
{
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;

typedef struct _FILE_NETWORK_OPEN_INFORMATION
{
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
} FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION;

typedef struct _FILE_COMPRESSION_INFORMATION
{
    LARGE_INTEGER CompressedFileSize;
    USHORT CompressionFormat;
    UCHAR CompressionUnitShift;
    UCHAR ChunkShift;
    UCHAR ClusterShift;
    UCHAR Reserved[3];
} FILE_COMPRESSION_INFORMATION, *PFILE_COMPRESSION_INFORMATION;

typedef struct _FILE_ALLOCATION_INFORMATION
{
    LARGE_INTEGER AllocationSize;
} FILE_ALLOCATION_INFORMATION, *PFILE_ALLOCATION_INFORMATION;

typedef struct _FILE_ALL_INFORMATION
{
    FILE_BASIC_INFORMATION BasicInformation;
    FILE_STANDARD_INFORMATION StandardInformation;
    FILE_INTERNAL_INFORMATION InternalInformation;
    FILE_EA_INFORMATION EaInformation;
    FILE_ACCESS_INFORMATION AccessInformation;
    FILE_POSITION_INFORMATION PositionInformation;
    FILE_MODE_INFORMATION ModeInformation;
    FILE_ALIGNMENT_INFORMATION AlignmentInformation;
    FILE_NAME_INFORMATION NameInformation;
} FILE_ALL_INFORMATION, *PFILE_ALL_INFORMATION;

typedef struct _OBJECT_NAME_INFORMATION
{
    UNICODE_STRING Name;
} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;

typedef struct _FILE_ATTRIBUTE_TAG_INFORMATION
{
    ULONG FileAttributes;
    ULONG ReparseTag;
} FILE_ATTRIBUTE_TAG_INFORMATION, *PFILE_ATTRIBUTE_TAG_INFORMATION;

#define MEM_EXTENDED_PARAMETER_TYPE_BITS 8

#pragma warning(push)
#pragma warning(disable : 4201)
typedef struct MEM_EXTENDED_PARAMETER
{
    struct
    {
        DWORD64 Type : MEM_EXTENDED_PARAMETER_TYPE_BITS;
        DWORD64 Reserved : 64 - MEM_EXTENDED_PARAMETER_TYPE_BITS;
    } DUMMYSTRUCTNAME;

    union
    {
        DWORD64 ULong64;
        PVOID Pointer;
        SIZE_T Size;
        HANDLE Handle;
        DWORD ULong;
    } DUMMYUNIONNAME;
} MEM_EXTENDED_PARAMETER, *PMEM_EXTENDED_PARAMETER;
#pragma warning(pop)

typedef struct _FILE_STAT_INFORMATION
{
    LARGE_INTEGER FileId;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
    ULONG ReparseTag;
    ULONG NumberOfLinks;
    ACCESS_MASK EffectiveAccess;
} FILE_STAT_INFORMATION, *PFILE_STAT_INFORMATION;

typedef struct _FILE_STAT_LX_INFORMATION
{
    LARGE_INTEGER FileId;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
    ULONG ReparseTag;
    ULONG NumberOfLinks;
    ACCESS_MASK EffectiveAccess;
    ULONG LxFlags;
    ULONG LxUid;
    ULONG LxGid;
    ULONG LxMode;
    ULONG LxDeviceIdMajor;
    ULONG LxDeviceIdMinor;
} FILE_STAT_LX_INFORMATION, *PFILE_STAT_LX_INFORMATION;

typedef struct _FILE_DIRECTORY_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;

typedef struct _FILE_BOTH_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    WCHAR FileName[1];
} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

typedef struct _FILE_FULL_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    WCHAR FileName[1];
} FILE_FULL_DIR_INFORMATION, *PFILE_FULL_DIR_INFORMATION;

typedef struct _FILE_ID_BOTH_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    LARGE_INTEGER FileId;
    WCHAR FileName[1];
} FILE_ID_BOTH_DIR_INFORMATION, *PFILE_ID_BOTH_DIR_INFORMATION;

typedef struct _FILE_ID_128
{ // winnt
    UCHAR Identifier[16]; // winnt
} FILE_ID_128, *PFILE_ID_128; // winnt

typedef struct _FILE_ID_EXTD_BOTH_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    ULONG ReparsePointTag;
    FILE_ID_128 FileId;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    WCHAR FileName[1];
} FILE_ID_EXTD_BOTH_DIR_INFORMATION, *PFILE_ID_EXTD_BOTH_DIR_INFORMATION;

typedef struct _FILE_ID_FULL_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    LARGE_INTEGER FileId;
    WCHAR FileName[1];
} FILE_ID_FULL_DIR_INFORMATION, *PFILE_ID_FULL_DIR_INFORMATION;

typedef struct _FILE_ID_EXTD_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    ULONG ReparsePointTag;
    FILE_ID_128 FileId;
    WCHAR FileName[1];
} FILE_ID_EXTD_DIR_INFORMATION, *PFILE_ID_EXTD_DIR_INFORMATION;

typedef struct _FILE_ID_GLOBAL_TX_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    LARGE_INTEGER FileId;
    GUID LockingTransactionId;
    ULONG TxInfoFlags;
    WCHAR FileName[1];
} FILE_ID_GLOBAL_TX_DIR_INFORMATION, *PFILE_ID_GLOBAL_TX_DIR_INFORMATION;

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtClose_Proc_Type )(
    IN HANDLE Handle
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtQueryInformationFile_Proc_Type)(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtReadFile_Proc_Type)(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtReadFileScatter_Proc_Type)(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN FILE_SEGMENT_ELEMENT SegmentArray[],
    IN ULONG Length,
    IN PLARGE_INTEGER ByteOffset,
    IN PULONG Key OPTIONAL
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtSetInformationFile_Proc_Type)(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtWriteFile_Proc_Type)(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtWriteFileGather_Proc_Type)(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN FILE_SEGMENT_ELEMENT SegmentArray[],
    IN ULONG Length,
    IN PLARGE_INTEGER ByteOffset,
    IN PULONG Key OPTIONAL
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtCreateFile_Proc_Type)(
    _Out_ PHANDLE FileHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_opt_ PLARGE_INTEGER AllocationSize,
    _In_ ULONG FileAttributes,
    _In_ ULONG ShareAccess,
    _In_ ULONG CreateDisposition,
    _In_ ULONG CreateOptions,
    _In_reads_bytes_opt_(EaLength) PVOID EaBuffer,
    _In_ ULONG EaLength
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtOpenFile_Proc_Type)(
    _Out_ PHANDLE FileHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG ShareAccess,
    _In_ ULONG OpenOptions
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtCreateSection_Proc_Type)(
    _Out_ PHANDLE SectionHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_opt_ PLARGE_INTEGER MaximumSize,
    _In_ ULONG SectionPageProtection,
    _In_ ULONG AllocationAttributes,
    _In_opt_ HANDLE FileHandle
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtCreateSectionEx_Proc_Type )(
    _Out_ PHANDLE SectionHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_opt_ PLARGE_INTEGER MaximumSize,
    _In_ ULONG SectionPageProtection,
    _In_ ULONG AllocationAttributes,
    _In_opt_ HANDLE FileHandle,
    _Inout_updates_opt_(ExtendedParameterCount) PMEM_EXTENDED_PARAMETER ExtendedParameters,
    _In_ ULONG ExtendedParameterCount
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtUnmapViewOfSection_Proc_Type)(
    _In_ HANDLE ProcessHandle,
    _In_opt_ PVOID BaseAddress
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtUnmapViewOfSectionEx_Proc_Type)(
    _In_ HANDLE ProcessHandle,
    _In_opt_ PVOID BaseAddress,
    _In_ ULONG Flags
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtMapViewOfSection_Proc_Type)(
    _In_ HANDLE SectionHandle,
    _In_ HANDLE ProcessHandle,
    _Outptr_result_bytebuffer_(*ViewSize) PVOID *BaseAddress,
    _In_ ULONG_PTR ZeroBits,
    _In_ SIZE_T CommitSize,
    _Inout_opt_ PLARGE_INTEGER SectionOffset,
    _Inout_ PSIZE_T ViewSize,
    _In_ SECTION_INHERIT InheritDisposition,
    _In_ ULONG AllocationType,
    _In_ ULONG Win32Protect
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtMapViewOfSectionEx_Proc_Type )(
    _In_ HANDLE SectionHandle,
    _In_ HANDLE ProcessHandle,
    _Inout_ /*_At_ (*BaseAddress, _Readable_bytes_ (*ViewSize) _Writable_bytes_ (*ViewSize) _Post_readable_byte_size_ (*ViewSize)) */PVOID *BaseAddress,
    _Inout_opt_ PLARGE_INTEGER SectionOffset,
    _Inout_ PSIZE_T ViewSize,
    _In_ ULONG AllocationType,
    _In_ ULONG Win32Protect,
    _Inout_updates_opt_(ParameterCount) MEM_EXTENDED_PARAMETER* Parameters,
    _In_ ULONG ParameterCount
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtQueryObject_Proc_Type )(
    _In_opt_ HANDLE Handle,
    _In_ OBJECT_INFORMATION_CLASS ObjectInformationClass,
    _Out_writes_bytes_opt_(ObjectInformationLength) PVOID ObjectInformation,
    _In_ ULONG ObjectInformationLength,
    _Out_opt_ PULONG ReturnLength
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtQueryFullAttributesFile_Proc_Type)(
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PFILE_NETWORK_OPEN_INFORMATION FileInformation
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtDeviceIoControlFile_Proc_Type )(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG IoControlCode,
    IN PVOID InputBuffer OPTIONAL,
    IN ULONG InputBufferLength,
    OUT PVOID OutputBuffer OPTIONAL,
    IN ULONG OutputBufferLength
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtQueryDirectoryFile_Proc_Type) (
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass,
    _In_ BOOLEAN ReturnSingleEntry,
    _In_opt_ PUNICODE_STRING FileName,
    _In_ BOOLEAN RestartScan
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtQueryDirectoryFileEx_Proc_Type) (
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass,
    _In_ ULONG QueryFlags, // Valid flags are in SL_QUERY_DIRECTORY_MASK
    _In_opt_ PUNICODE_STRING FileName
    );

typedef
    VOID
    (NTAPI * Ntdll_dll_RtlGetNtVersionNumbers_Proc_Type)(
    DWORD * dwMajorVersion,
    DWORD * dwMinorVersion,
    DWORD * dwBuildNumber
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtQueryInformationProcess_Proc_Type)(
    __in HANDLE ProcessHandle,
    __in PROCESSINFOCLASS ProcessInformationClass,
    __out PVOID ProcessInformation,
    __in ULONG ProcessInformationLength,
    __out_opt PULONG ReturnLength
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_LdrLoadDll_Proc_Type) (
    IN PWSTR SearchPath OPTIONAL,
    IN PULONG DllCharacteristics OPTIONAL,
    IN PUNICODE_STRING DllName,
    OUT PVOID * BaseAddress
    ) ;

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtCreateProcessEx_Proc_Type )(
    OUT PHANDLE ProcessHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN HANDLE ParentProcess,
    IN ULONG Flags,
    IN HANDLE SectionHandle OPTIONAL,
    IN HANDLE DebugPort OPTIONAL,
    IN HANDLE ExceptionPort OPTIONAL,
    IN BOOLEAN InJob
    ) ;

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtCreateUserProcess_Proc_Type)(
    OUT PHANDLE ProcessHandle,
    OUT PHANDLE ThreadHandle,
    IN ACCESS_MASK ProcessDesiredAccess,
    IN ACCESS_MASK ThreadDesiredAccess,
    IN POBJECT_ATTRIBUTES ProcessObjectAttributes OPTIONAL,
    IN POBJECT_ATTRIBUTES ThreadObjectAttributes OPTIONAL,
    IN ULONG ProcessFlags,
    IN ULONG ThreadFlags,
    IN PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
    IN OUT PVOID/*PPS_CREATE_INFO*/ CreateInfo,
    IN PVOID/*PPS_ATTRIBUTE_LIST*/ AttributeList
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtTerminateProcess_Proc_Type) (
    _In_opt_ HANDLE ProcessHandle,
    _In_ NTSTATUS ExitStatus
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_RtlCreateProcessParameters_Proc_Type )(
    _Out_ PRTL_USER_PROCESS_PARAMETERS * ProcessParameters,
    _In_ PUNICODE_STRING ImagePathName,
    _In_opt_ PUNICODE_STRING DllPath,
    _In_opt_ PUNICODE_STRING CurrentDirectory,
    _In_opt_ PUNICODE_STRING CommandLine,
    _In_opt_ PWSTR Environment,
    _In_opt_ PUNICODE_STRING WindowTitle,
    _In_opt_ PUNICODE_STRING DesktopInfo,
    _In_opt_ PUNICODE_STRING ShellInfo,
    _In_ PUNICODE_STRING RuntimeInfo
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_RtlCreateProcessParametersEx_Proc_Type )(
    _Out_ PRTL_USER_PROCESS_PARAMETERS * ProcessParameters,
    _In_ PUNICODE_STRING ImagePathName,
    _In_opt_ PUNICODE_STRING DllPath,
    _In_opt_ PUNICODE_STRING CurrentDirectory,
    _In_opt_ PUNICODE_STRING CommandLine,
    _In_opt_ PWSTR Environment,
    _In_opt_ PUNICODE_STRING WindowTitle,
    _In_opt_ PUNICODE_STRING DesktopInfo,
    _In_opt_ PUNICODE_STRING ShellInfo,
    _In_opt_ PUNICODE_STRING RuntimeInfo,
    _In_ DWORD unkonw
    );

// RtlCreateProcessParametersWithTemplate

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtLockFile_Proc_Type) (
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ PLARGE_INTEGER ByteOffset,
    _In_ PLARGE_INTEGER Length,
    _In_ ULONG Key,
    _In_ BOOLEAN FailImmediately,
    _In_ BOOLEAN ExclusiveLock
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtUnlockFile_Proc_Type )(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ PLARGE_INTEGER ByteOffset,
    _In_ PLARGE_INTEGER Length,
    _In_ ULONG Key
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtOpenEvent_Proc_Type) (
    _Out_ PHANDLE EventHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtOpenSection_Proc_Type)(
    _Out_ PHANDLE SectionHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
    );

typedef enum _KEY_VALUE_INFORMATION_CLASS
{
    KeyValueBasicInformation,
    KeyValueFullInformation,
    KeyValuePartialInformation,
    KeyValueFullInformationAlign64,
    KeyValuePartialInformationAlign64,
    KeyValueLayerInformation,
    MaxKeyValueInfoClass // MaxKeyValueInfoClass should always be the last enum
} KEY_VALUE_INFORMATION_CLASS;

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtQueryValueKey_Proc_Type)(
    _In_ HANDLE KeyHandle,
    _In_ PUNICODE_STRING ValueName,
    _In_ KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    _Out_writes_bytes_to_opt_(Length, *ResultLength) PVOID KeyValueInformation,
    _In_ ULONG Length,
    _Out_ PULONG ResultLength
    );

typedef struct _KEY_VALUE_FULL_INFORMATION
{
    ULONG TitleIndex;
    ULONG Type;
    ULONG DataOffset;
    ULONG DataLength;
    ULONG NameLength;
    WCHAR Name[1]; // Variable size
    // Data[1]; // Variable size data not declared
} KEY_VALUE_FULL_INFORMATION, *PKEY_VALUE_FULL_INFORMATION;

typedef struct _KEY_VALUE_PARTIAL_INFORMATION
{
    ULONG TitleIndex;
    ULONG Type;
    ULONG DataLength;
    UCHAR Data[1]; // Variable size
} KEY_VALUE_PARTIAL_INFORMATION, *PKEY_VALUE_PARTIAL_INFORMATION;

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtEnumerateValueKey_Proc_Type)(
    _In_ HANDLE KeyHandle,
    _In_ ULONG Index,
    _In_ KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    _Out_writes_bytes_to_opt_(Length, *ResultLength) PVOID KeyValueInformation,
    _In_ ULONG Length,
    _Out_ PULONG ResultLength
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtOpenKey_Proc_Type)(
    _Out_ PHANDLE KeyHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtOpenKeyEx_Proc_Type)(
    _Out_ PHANDLE KeyHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_ ULONG OpenOptions
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtSetValueKey_Proc_Type)(
    _In_ HANDLE KeyHandle,
    _In_ PUNICODE_STRING ValueName,
    _In_opt_ ULONG TitleIndex,
    _In_ ULONG Type,
    _In_reads_bytes_opt_(DataSize) PVOID Data,
    _In_ ULONG DataSize
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtDeleteValueKey_Proc_Type)(
    _In_ HANDLE KeyHandle,
    _In_ PUNICODE_STRING ValueName
    );

typedef
    NTSTATUS
    ( NTAPI * Ntdll_dll_NtOpenMutant_Proc_Type)(
    OUT PHANDLE MutantHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes
    );

typedef
    NTSTATUS
    (NTAPI * Ntdll_dll_NtFsControlFile_Proc_Type)(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG FsControlCode,
    _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _Out_writes_bytes_opt_(OutputBufferLength) PVOID OutputBuffer,
    _In_ ULONG OutputBufferLength
    );
