#include "EquateList.h"
#include "EquateListLogic.h"
#include "Syntax.h"


EquateListLogic::EquateListLogic() {

}

bool EquateListLogic::Load(EquateList& equateList, wstring_view filePath) {
    return equateList.Load(filePath);
}

void EquateListLogic::Save(const EquateList& equateList, wstring_view filePath, bool xasm) {
    equateList.Save(filePath, xasm);
}
