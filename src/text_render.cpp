/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2018 Wuwei Lin. All rights reserved.
 *
 */
#include "text_render.h"
#include "program.h"
#include "uiapplication.h"


FontManager::FontManager(const std::string &font, int width, int height) {
    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    assert(0 == FT_Init_FreeType(&ft));

    // Load font as face
    FT_Face face;
    assert(0 == FT_New_Face(ft, font.c_str(), 0, &face));
    assert(face);
    // Set size to load glyphs as
    assert(0 == FT_Set_Pixel_Sizes(face, width, height));
    glActiveTexture(GL_TEXTURE1);
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++) {
        // Load character glyph
        assert(0 == FT_Load_Char(face, c, FT_LOAD_RENDER));

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use

        cache_[c] = Character {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };

    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

TextRenderer::TextRenderer(const std::shared_ptr<FontManager> &fontManager) : fontManager_(fontManager) {
    program_ = std::make_unique<Program>(
        std::make_shared<Shader>(GL_VERTEX_SHADER, "shader/text.vert"),
        std::make_shared<Shader>(GL_FRAGMENT_SHADER, "shader/text.frag")
    );
    program_->link();
    program_->use();
    auto projection = glm::ortho(0.0f, static_cast<float>(UIApplication::getInstance().getScreenWidth()), 0.0f,
                                 static_cast<float>(UIApplication::getInstance().getScreenHeight()));
    program_->setMat4("projection", projection);
    glUniform1i(program_->getUniform("text"), 1);

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TextRenderer::render(const std::string &text, float x, float y, float scale) {
    // Activate corresponding render state
    program_->use();
    //shader.Use();
    //glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE1);
    glBindVertexArray(vao_);

    for (const auto c: text) {
        assert(glGetError() == 0);
        auto &ch = fontManager_->getCharacter(c);

        GLfloat xpos = x + ch.bearing.x * scale;
        GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            {xpos,     ypos + h, 0.0, 0.0},
            {xpos,     ypos,     0.0, 1.0},
            {xpos + w, ypos,     1.0, 1.0},

            {xpos,     ypos + h, 0.0, 0.0},
            {xpos + w, ypos,     1.0, 1.0},
            {xpos + w, ypos + h, 1.0, 0.0}
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texture);
        // Update content of VBO memory
        assert(glGetError() == 0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices),
                        vertices); // Be sure to use glBufferSubData and not glBufferData
        assert(glGetError() == 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) *
             scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        assert(glGetError() == 0);
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    assert(glGetError() == 0);
}
