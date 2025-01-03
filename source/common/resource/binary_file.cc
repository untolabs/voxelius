// SPDX-License-Identifier: BSD-2-Clause
#include "common/precompiled.hh"
#include "common/resource/binary_file.hh"

#include "common/fstools.hh"


static emhash8::HashMap<std::string, std::shared_ptr<const BinaryFile>> file_map = {};


template<>
std::shared_ptr<const BinaryFile> resource::load<BinaryFile>(const std::string &name, unsigned int load_flags)
{
    const auto it = file_map.find(name);
    if(it != file_map.cend()) {
        return it->second;
    }

    PHYSFS_File *file = PHYSFS_openRead(name.c_str());

    if(!file) {
        spdlog::warn("binary_file: {}: {}", name, fstools::error());
        return nullptr;
    }

    auto new_file = std::make_shared<BinaryFile>();
    new_file->name = std::string(name);
    new_file->length = PHYSFS_fileLength(file);
    new_file->buffer = new std::uint8_t[new_file->length];

    PHYSFS_readBytes(file, new_file->buffer, new_file->length);
    PHYSFS_close(file);

    return file_map.insert_or_assign(name, new_file).first->second;
}

template<>
void resource::hard_cleanup<BinaryFile>(void)
{
    for(const auto &file : file_map) {
        if(file.second.use_count() > 1L)
            spdlog::warn("binary_file: hard_cleanup: zombie resource {} [use_count={}]", file.first, file.second.use_count());
        spdlog::debug("binary_file: hard_cleanup: purging {}", file.first);
        delete[] file.second->buffer;
    }

    file_map.clear();
}

template<>
void resource::soft_cleanup<BinaryFile>(void)
{
    auto file = file_map.cbegin();

    while(file != file_map.cend()) {
        if(file->second.use_count() == 1L) {
            spdlog::debug("binary_file: soft_cleanup: unloading {}", file->first);
            delete[] file->second->buffer;
            file = file_map.erase(file);
            continue;
        }

        file++;
    }
}
