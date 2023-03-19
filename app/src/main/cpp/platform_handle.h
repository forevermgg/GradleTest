// PlatformHandle should be used only for types that are HANDLE(s) in Windows.
// It should NOT be used to blanket-replace "int fd" in the codebase.
// Windows has two types of "handles", which, in UNIX-land, both map to int:
// 1. File handles returned by the posix-compatibility API like _open().
//    These are just int(s) and should stay such, because all the posix-like API
//    in Windows.h take an int, not a HANDLE.
// 2. Handles returned by old-school WINAPI like CreateFile, CreateEvent etc.
//    These are proper HANDLE(s). PlatformHandle should be used here.
using PlatformHandle = int;
struct PlatformHandleChecker {
  static inline bool IsValid(PlatformHandle h) { return h >= 0; }
};

// The definition of this lives in base/file_utils.cc (to avoid creating an
// extra build edge for a one liner). This is really an alias for close() (UNIX)
// CloseHandle() (Windows). THe indirection layer is just to avoid leaking
// system headers like Windows.h through perfetto headers.
// Thre return value is always UNIX-style: 0 on success, -1 on failure.
int ClosePlatformHandle(PlatformHandle);