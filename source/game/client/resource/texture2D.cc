// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/resource/texture2D.hh"

#include "common/resource/image.hh"


static emhash8::HashMap<std::string, std::shared_ptr<const Texture2D>> texture_map = {};

template<>
std::shared_ptr<const Texture2D> resource::load<Texture2D>(const std::string &path, unsigned int load_flags)
{
    const auto it = texture_map.find(path);

    if(it != texture_map.cend()) {
        spdlog::warn("Texture2D: {} is already loaded", path);
        return it->second;
    }

    unsigned int image_load_flags;
    if(load_flags & TEXTURE2D_LOAD_VFLIP)
        image_load_flags = IMAGE_LOAD_VFLIP;
    else image_load_flags = 0U;

    if(auto image = resource::load<Image>(path, image_load_flags)) {
        auto new_texture = std::make_shared<Texture2D>();

        glGenTextures(1, &new_texture->handle);
        glBindTexture(GL_TEXTURE_2D, new_texture->handle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

        if(load_flags & TEXTURE2D_LOAD_CLAMP_S)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

        if(load_flags & TEXTURE2D_LOAD_CLAMP_T)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if(load_flags & TEXTURE2D_LOAD_LINEAR_MAG)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        if(load_flags & TEXTURE2D_LOAD_LINEAR_MIN)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        new_texture->imgui = reinterpret_cast<ImTextureID>(static_cast<std::uintptr_t>(new_texture->handle));
        new_texture->height = image->height;
        new_texture->width = image->width;

        return texture_map.insert_or_assign(path, new_texture).first->second;
    }

    return nullptr;
}

template<>
std::shared_ptr<const Texture2D> resource::find<Texture2D>(const std::string &path)
{
    const auto it = texture_map.find(path);
    if(it != texture_map.cend())
        return it->second;
    return nullptr;
}

template<>
void resource::hard_cleanup<Texture2D>(void)
{
    for(const auto &texture : texture_map) {
        if(texture.second.use_count() > 1L)
            spdlog::warn("texture2D: hard_cleanup: zombie resource {} [use_count={}]", texture.first, texture.second.use_count());
        spdlog::debug("texture2D: hard_cleanup: purging {}", texture.first);
        glDeleteTextures(1, &texture.second->handle);
    }

    texture_map.clear();
}

template<>
void resource::soft_cleanup<Texture2D>(void)
{
    auto texture = texture_map.cbegin();

    while(texture != texture_map.cend()) {
        if(texture->second.use_count() == 1L) {
            spdlog::debug("texture2D: soft_cleanup: unloading {}", texture->first);
            glDeleteTextures(1, &texture->second->handle);
            texture = texture_map.erase(texture);
            continue;
        }

        texture++;
    }
}
