cmake_minimum_required(VERSION 3.2)

project(deepin-installer-unsquashfs C)

set(CMAKE_C_FLAGS "-Wall -O2 -Wl,-O1 -Wl,--gc-sections")

add_definitions("-DXATTR_SUPPORT -DXATTR_DEFAULT -D_FILE_OFFSET_BITS=64")
add_definitions("-D_LARGEFILE_SOURCE -D_GNU_SOURCE")
add_definitions("-DCOMP_DEFAULT=\"gzip\"")
add_definitions("-DGZIP_SUPPORT")
add_definitions("-DLZMA_SUPPORT")
add_definitions("-DLZO_SUPPORT")
add_definitions("-DLZ4_SUPPORT")
add_definitions("-DXZ_SUPPORT")

find_package(PkgConfig REQUIRED)

pkg_search_module(LZMA REQUIRED liblzma)
pkg_search_module(LZ4 REQUIRED liblz4)

include_directories(AFTER ${LZMA_INCLUDE_DIRS})
include_directories(AFTER ${LZ4_INCLUDE_DIRS})

set(SOURCES
    compressor.c
    compressor.h
    error.h
    gzip_wrapper.c
    gzip_wrapper.h
    lz4_wrapper.c
    lz4_wrapper.h
    lzma_xz_wrapper.c
    lzo_wrapper.c
    lzo_wrapper.h
    read_fs.h
    read_xattrs.c
    squashfs_compat.h
    squashfs_fs.h
    squashfs_swap.h
    swap.c
    unsquash-1.c
    unsquash-2.c
    unsquash-3.c
    unsquash-4.c
    unsquashfs.c
    unsquashfs.h
    unsquashfs_info.c
    unsquashfs_info.h
    unsquashfs_xattr.c
    xattr.h
    xz_wrapper.c
    xz_wrapper.h
    )
add_executable(deepin-installer-unsquashfs ${SOURCES})
target_link_libraries(deepin-installer-unsquashfs
    
    lzo2
    m
    pthread
    ${LZMA_LIBRARIES}
    ${LZ4_LIBRARIES}
    z
    )

install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/deepin-installer-unsquashfs"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
