// SPDX-License-Identifier: BSD-2-Clause
#include "common/precompiled.hh"
#include "common/fstools.hh"

const char *fstools::error(void)
{
    return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
}

bool fstools::read_bytes(const std::string &path, std::vector<std::uint8_t> &buffer)
{
    PHYSFS_File *file = PHYSFS_openRead(path.c_str());

    if(file) {
        buffer.resize(PHYSFS_fileLength(file));
        PHYSFS_readBytes(file, buffer.data(), buffer.size());
        PHYSFS_close(file);
        return true;
    }

    buffer.clear();
    return false;
}

bool fstools::read_string(const std::string &path, std::string &buffer)
{
    PHYSFS_File *file = PHYSFS_openRead(path.c_str());

    if(file) {
        buffer.resize(PHYSFS_fileLength(file));
        PHYSFS_readBytes(file, buffer.data(), buffer.size());
        PHYSFS_close(file);
        return true;
    }

    buffer.clear();
    return false;
}

bool fstools::write_bytes(const std::string &path, const std::vector<std::uint8_t> &buffer)
{
    PHYSFS_File *file = PHYSFS_openWrite(path.c_str());

    if(file) {
        PHYSFS_writeBytes(file, buffer.data(), buffer.size());
        PHYSFS_close(file);
        return true;
    }

    return false;
}

bool fstools::write_string(const std::string &path, const std::string &buffer)
{
    PHYSFS_File *file = PHYSFS_openWrite(path.c_str());

    if(file) {
        PHYSFS_writeBytes(file, buffer.data(), buffer.size());
        PHYSFS_close(file);
        return true;
    }

    return false;
}

bool fstools::read_line(PHYSFS_File *file, std::string &line)
{
    if((file == nullptr) || PHYSFS_eof(file)) {
        // Nothing else to read
        return false;
    }

    line.clear();

    char temp[2] = {};
    while(PHYSFS_readBytes(file, temp, 1) == 1) {
        temp[1] = 0x00; // Make sure it's terminated

        if(temp[0] == '\n')
            break;
        if(temp[0] == '\r')
            continue;
        line.append(temp);
    }

    return true;
}
