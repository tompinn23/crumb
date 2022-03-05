#version 330 core

in vec2 texCoord;
in vec2 coord;

uniform vec2 tiles_shape;
uniform vec2 tiles_size;

uniform sampler2D texture_tileset;
uniform sampler2D texture_fg;
uniform sampler2D texture_bg;

void main() {
    vec2 console_pos = floor(coord);
    console_pos += vec2(0.5, 0.5);
    console_pos /= console_shape;

    vec2 tex_coord = floor(texCoord);
    vec2 tex_frac = fract(texCoord);

    tex_frac = clamp(tex_frac, 0.5 / tiles_size, 1.0 - 0.5 / tiles_size);
    tex_coord = (tex_coord + tex_frac) / tiles_shape;
    
    vec4 tile_colour = texture2D(texture_tileset, tex_coord);
    vec4 bg = texture2D(texture_bg, console_pos);
    vec4 fg = texture2D(texture_fg, console_pos);
    fg.rgb *= tile_colour.rgb;
    gl_FragColor = mix(bg, fg, tile_colour.a);
}
