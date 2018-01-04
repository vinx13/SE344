/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2018 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_TEXT_RENDER_H
#define SE344_TEXT_RENDER_H

#include <GL/gl3w.h>
#include <string>
#include <glm/glm.hpp>
#include <map>
#include <ft2build.h>
#include <memory>
#include FT_FREETYPE_H

#include "program.h"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint texture;   // ID handle of the glyph texture
    glm::ivec2 size;    // Size of glyph
    glm::ivec2 bearing;  // Offset from baseline to left/top of glyph
    GLuint advance;    // Horizontal offset to advance to next glyph
};

class FontManager {
public:
    static std::shared_ptr<FontManager> getDefault();

    static std::shared_ptr<FontManager> defaultInstance__;

    FontManager(const std::string &font, int width, int height);

    const Character &getCharacter(char c) {
        return cache_[c];
    }

    std::map<GLchar, Character> cache_;
};

class TextRenderer {
public:
    TextRenderer(const std::shared_ptr<FontManager> &fontManager);

    void render(const std::string &text, float x, float y, float scale);

    GLuint vao_, vbo_;
    std::unique_ptr<Program> program_;
    std::shared_ptr<FontManager> fontManager_;
};


#endif //SE344_TEXT_RENDER_H
