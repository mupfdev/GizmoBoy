macro(get_dirent version)
  FetchContent_Populate(
    dirent
    URL https://github.com/tronkko/dirent/archive/refs/tags/${version}.zip
    URL_HASH
    SHA1=0059b1031f4cd078ab0f9bfcadc3903949e4dd3d)

    set(DIRENT_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/dirent-src/include)
    message(${DIRENT_INCLUDE_DIR})
endmacro(get_dirent)
