#pragma once

#include "Syntax.h"

class Workspace;

class Pass1 {

public:
    /*
    ** Pass 1: attach SDX label fix-ups on each segment.
    */
    static void Execute(Workspace& workspace);
};