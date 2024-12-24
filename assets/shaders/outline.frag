// SPDX-License-Identifier: CC-BY-SA-4.0
#version 330 core

out vec4 frag_Target;

uniform vec4 u_Modulate;

void main(void)
{
    frag_Target = u_Modulate;
}
