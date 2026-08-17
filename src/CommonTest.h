#pragma once

#include "Syntax.h"

class Allocatable final {
public:
    Allocatable();

    Allocatable(const Allocatable&) = delete;               // copy constructor		
    Allocatable& operator=(const Allocatable&) = delete;    // copy assignment
    Allocatable(Allocatable&&) = delete;                    // move constructor
    Allocatable& operator=(Allocatable&&) = delete;         // move assignment

    ~Allocatable();
    void Test() const;

    static void Initialize();
    static void AssertAllDestroyed();

private:
    static long instanceCount;
    long instanceNumber;
};

class CommonTest
{

public:
    static void TestCommon();

private:
    static void TestByteArray();
    static void TestDatatypeUtility();
    static void TestString();
    static void TestPointers();
};

