/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef LOADER_H
#define LOADER_H

#include <memory>
#include <glm/detail/type_mat.hpp>

class Drawable;

class Loader {
public:
    virtual std::shared_ptr<Drawable> load(const std::string &path) = 0;
};




#endif //SE344_LOADER_H
