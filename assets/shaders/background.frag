// SPDX-License-Identifier: CC-BY-SA-4.0
#version 330 core

in vec2 vs_TileCoord;

out vec4 frag_Target;

uniform sampler2D u_Texture;

void main(void)
{
    frag_Target = texture(u_Texture, vs_TileCoord);
}
