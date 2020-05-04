#pragma once
#include "RDCase.h"
#include "MainClass.h"
#include "ClassDeclaration.h"


/*
 Goal -> MainClass { ClassDeclaration } EOF
*/
class RDCase_Goal : public RDCase {
public:
    RDCase_Goal(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
protected:
    TreeNodePtr _RunException(TreeNodePtr firstNode = nullptr)override {
        _Init(TreeMainType::Goal);
        /**********************************
         MainClass
        ************************************/
        auto mainClass = RDCase_MainClass(_reader);
        mainClass.Run();
        _CheckSubTree(&mainClass);
        _root->Child.Set(mainClass.Root.Get());
        /**********************************
         { ClassDeclaration }
        ************************************/
        TreeNodePtr preNode = _root->Child.Get();
        while (true) {
            auto classDeclaration = RDCase_ClassDeclaration(_reader);
            classDeclaration.Run();
            if (classDeclaration.Root.Get() == nullptr)
                break;
            preNode->Subling.Set(classDeclaration.Root.Get());
            preNode = classDeclaration.Root.Get();
        }
        /**********************************
         EOF
        ************************************/
        if (!_reader->IsEnd()) {
            string msg = "应该到达文件结尾";
            _reader->Index.Set(_curIndex);
            throw SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
        }

        return _root;
    }
};