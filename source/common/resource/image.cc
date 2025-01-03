// SPDX-License-Identifier: BSD-2-Clause
#include "common/precompiled.hh"
#include "common/resource/image.hh"

#include "common/fstools.hh"


static emhash8::HashMap<std::string, std::shared_ptr<const Image>> image_map = {};

template<>
std::shared_ptr<const Image> resource::load<Image>(const std::string &name, unsigned int load_flags)
{
    const auto it = image_map.find(name);
    if(it != image_map.cend()) {
        return it->second;
    }

    std::vector<std::uint8_t> buffer = {};

    if(!fstools::read_bytes(name, buffer)) {
        spdlog::warn("image: {}: {}", name, fstools::error());
        return nullptr;
    }

    if(load_flags & IMAGE_LOAD_VFLIP)
        stbi_set_flip_vertically_on_load(true);
    else stbi_set_flip_vertically_on_load(false);

    auto new_image = std::make_shared<Image>();
    new_image->name = std::string(name);

    if(load_flags & IMAGE_LOAD_GRAYSCALE)
        new_image->pixels = stbi_load_from_memory(buffer.data(), buffer.size(), &new_image->width, &new_image->height, nullptr, STBI_grey);
    else new_image->pixels = stbi_load_from_memory(buffer.data(), buffer.size(), &new_image->width, &new_image->height, nullptr, STBI_rgb_alpha);

    if(!new_image->pixels) {
        spdlog::warn("image: {}: stbi_load failed", name);
        return nullptr;
    }

    if(!new_image->width || !new_image->height) {
        spdlog::warn("image: {}: non-valid image dimensions", name);
        stbi_image_free(new_image->pixels);
        return nullptr;
    }

    return image_map.insert_or_assign(name, new_image).first->second;
}

template<>
void resource::hard_cleanup<Image>(void)
{
    for(const auto &image : image_map) {
        if(image.second.use_count() > 1L)
            spdlog::warn("image: zombie resource {} [use_count={}]", image.first, image.second.use_count());
        spdlog::debug("image: hard_cleanup: purging {}", image.first);
        stbi_image_free(image.second->pixels);
    }

    image_map.clear();
}

template<>
void resource::soft_cleanup<Image>(void)
{
    auto image = image_map.cbegin();

    while(image != image_map.cend()) {
        if(image->second.use_count() == 1L) {
            spdlog::debug("image: soft_cleanup: unloading {}", image->first);
            stbi_image_free(image->second->pixels);
            image = image_map.erase(image);
            continue;
        }

        image++;
    }
}
