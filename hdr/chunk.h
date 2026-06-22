#pragma once
#ifndef CHUNK_H
#define CHUNK_H
#include<vector>


class Chunk{
public:
    std::vector<Hold> holds_chunk;
    
    bool isActive = false;
};

#endif
