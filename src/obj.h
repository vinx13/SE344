/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "drawable.h"
#include "loader.h"

class ObjDrawable : public Drawable {
    friend class ObjLoader;

public:
    ObjDrawable() = default;

    void render(const glm::mat4 &model) override;

protected:
    void init();

private:
    GLuint vbo_coords_, vao_, ebo_;
    std::vector<glm::vec3> vertices_, normals_;
    std::vector<glm::vec2> textures_;
    std::vector<int> numVertPerParts_;
    std::vector<unsigned int> vertexIndices_, uvIndices_, normalIndices_;
};

class ObjLoader : public Loader {
public:
    virtual std::shared_ptr<Drawable> load(const std::string &path);

private:

};

#endif //OBJ_H
