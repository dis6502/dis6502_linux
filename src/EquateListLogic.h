#pragma once
#include "Syntax.h"
class EquateList;

class EquateListLogic {


public:
    EquateListLogic();
    ~EquateListLogic() = default;


    bool Load(EquateList& equateList, wstring_view filePath);

    /*
    ** Save profile to disk.
    */
    void Save(const EquateList& equateList, wstring_view filePath, bool xasm);

};