/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_MESH_H
#define SE344_MESH_H

class Program;

#include <glm/glm.hpp>
class Mesh {
    virtual void render(Program &program) = 0;
    virtual void bind() = 0;
};


#endif //SE344_MESH_H
