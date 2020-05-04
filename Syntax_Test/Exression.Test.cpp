#include "pch.h"

/* 测试太让人头秃了 */
/* 通过一系列字符串，生成一个 RDCase_Expression */
shared_ptr<RDCase_Expression> ConstructRDCase(string str) {
    vector<string> strs;
    strs.push_back(str);
    LexialAnalyzer analyzer;
    analyzer.LexicalAnlyze(strs);
    auto tokens = analyzer.GetResultTokens();
    auto reader = shared_ptr<TokenReader>(new TokenReader(tokens));
    auto expression = shared_ptr<RDCase_Expression>(new RDCase_Expression(reader));
    expression->Run();
    return expression;
}
/* 获得一个节点的expression子类型 */
TreeSubType_Expression GetSubTypeExp(shared_ptr<TreeNode> root) {
    return root->Type.Get().GetSubType_Expression();
}
/* 获得一个节点的主类型 */
TreeMainType GetMainType(TreeNodePtr root) {
    return root->Type.Get().GetMainType();
}


#pragma region 以终结符开头的产生式测试
/*
 "this"
 -------------------
 this-null
  |
 null
*/
TEST(ExpressionTermin, This) {
    auto expression = ConstructRDCase("this");

    auto root = expression->Root.Get();
    EXPECT_NE(root, nullptr);
    EXPECT_EQ(GetMainType(root), TreeMainType::Expression);
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_This);
    EXPECT_EQ(root->Child.Get(), nullptr);
    EXPECT_EQ(root->Subling.Get(), nullptr);
}
/*
 "true"
-------------------
 true-null
  |
 null
*/
TEST(ExpressionTermin, True) {
    auto exp = ConstructRDCase("true");

    auto root = exp->Root.Get();
    EXPECT_NE(root, nullptr);
    EXPECT_EQ(GetMainType(root), TreeMainType::Expression);
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_True);
    EXPECT_EQ(root->Subling.Get(), nullptr);
    EXPECT_EQ(root->Child.Get(), nullptr);
}
/*
 "false"
 -------------------
 false - null
  |
 null
*/
TEST(ExpressionTermin, False) {
    auto exp = ConstructRDCase("false");

    auto root = exp->Root.Get();
    EXPECT_NE(root, nullptr);
    EXPECT_EQ(GetMainType(root), TreeMainType::Expression);
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_False);
    EXPECT_EQ(root->Subling.Get(), nullptr);
    EXPECT_EQ(root->Child.Get(), nullptr);
}
/*
 233
 -------------------
 IntegerExp - null
  |
 Integer(23) - null
  |
 null
*/
TEST(ExpressionTermin, Integer) {
    auto exp = ConstructRDCase("233");

    auto root = exp->Root.Get();
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_IntegerLiteral);
    
    auto child = root->Child.Get();
    EXPECT_EQ(GetMainType(child), TreeMainType::IntegerLeterial);
    EXPECT_EQ(child->NumValue.Get(), 233);
}
/*
 "acc"
 -------------------
 IdentifierExp - null
  |
 Identifier("acc") - null
  |
 null
*/
TEST(ExpressionTermin, Identifier) {
    auto exp = ConstructRDCase("acc");

    auto root = exp->Root.Get();
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_Identifier);

    auto child = root->Child.Get();
    EXPECT_EQ(GetMainType(child), TreeMainType::Identifier);
    EXPECT_EQ(child->StrValue.Get(), "acc");
}
/*
 "new MyClass()"
 -------------------
 newIdentifier - null
  |
 Idntifier("MyClass") - null
  |
 null
*/
TEST(ExpressionTermin, NewIdentifier) {
    auto exp = ConstructRDCase("new MyClass()");

    auto root = exp->Root.Get();
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_NewIdentifier);

    auto child = root->Child.Get();
    EXPECT_EQ(GetMainType(child), TreeMainType::Identifier);
    EXPECT_EQ(child->StrValue.Get(), "MyClass");
}
/*
 "!this"
 -------------------
 notExp - null
  |
 thisExp - null
  |
 null
*/
TEST(ExpressionTermin, Not) {
    auto exp = ConstructRDCase("!this");

    auto root = exp->Root.Get();
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_Not);

    auto child = root->Child.Get();
    EXPECT_EQ(GetSubTypeExp(child), TreeSubType_Expression::Exp_This);
}
/*
 "(tihs)"
 -------------------
 bracketExp - null
  |
 thisEpx - null
  |
 null
*/
TEST(ExpressionTermin, Bracket) {
    auto exp = ConstructRDCase("(this)");

    auto root = exp->Root.Get();
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_Bracket);

    auto child = root->Child.Get();
    EXPECT_EQ(GetSubTypeExp(child), TreeSubType_Expression::Exp_This);
}
/*
 "new int[233]"
 -------------------
 newIntArrayExp - null
  | 
 IntegerExp - null
  | 
 Integer(233) - null
  | 
 null
*/
TEST(ExpressionTermin, NewIntArray) {
    auto exp = ConstructRDCase("new int[233]");

    auto root = exp->Root.Get();
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_NewIntArray);

    auto child = root->Child.Get();
    EXPECT_EQ(GetSubTypeExp(child), TreeSubType_Expression::Exp_IntegerLiteral);

    auto gchild = child->Child.Get();
    EXPECT_EQ(GetMainType(gchild), TreeMainType::IntegerLeterial);
    EXPECT_EQ(gchild->NumValue.Get(), 233);
}
#pragma endregion

#pragma region 以非终结符开头的产生式测试

/*
 "this.length"
 ----------------
 lengthExp - null
  |
 thisExp - null
  |
 null
*/
TEST(ExpressionNonTermin, Length) {
    auto exp = ConstructRDCase("this.length");

    auto root = exp->Root.Get();
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_Length);

    auto child = root->Child.Get();
    EXPECT_EQ(GetSubTypeExp(child), TreeSubType_Expression::Exp_This);
}
/*
 "acc[233]"
 -----------
 arrayExp
  |
 IdenExp  ---   IntegerExp
  |              |
 iden("acc")    integer(233)  
*/
TEST(ExpressionNonTermin, ArrayExp) {
    auto exp = ConstructRDCase("acc[233]");

    auto root = exp->Root.Get();
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_ArrayExpression);

    auto child = root->Child.Get();
    EXPECT_EQ(GetSubTypeExp(child), TreeSubType_Expression::Exp_Identifier);

    auto cc = child->Child.Get();
    EXPECT_EQ(GetMainType(cc), TreeMainType::Identifier);
    EXPECT_EQ(cc->StrValue.Get(), "acc");

    auto cs = child->Subling.Get();
    EXPECT_EQ(GetSubTypeExp(cs), TreeSubType_Expression::Exp_IntegerLiteral);
    
    auto csc = cs->Child.Get();
    EXPECT_EQ(GetMainType(csc), TreeMainType::IntegerLeterial);
    EXPECT_EQ(csc->NumValue.Get(), 233);
}
/*
 "1+2"
 ---------
 operateExp
  |
 integerExp   ---   integerExp
  |                  |
 integer(1)         interger(2)
*/
TEST(ExpressionNonTermin, Operator) {
    auto exp = ConstructRDCase("1+2");

    auto root = exp->Root.Get();
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_Operate);

    auto child = root->Child.Get();
    EXPECT_EQ(GetSubTypeExp(child), TreeSubType_Expression::Exp_IntegerLiteral);
    EXPECT_EQ(GetMainType(child->Child.Get()), TreeMainType::IntegerLeterial);
    EXPECT_EQ(child->Child.Get()->NumValue.Get(), 1);

    auto cs = child->Subling.Get();
    EXPECT_EQ(GetSubTypeExp(cs), TreeSubType_Expression::Exp_IntegerLiteral);
    EXPECT_EQ(GetMainType(cs->Child.Get()), TreeMainType::IntegerLeterial);
    EXPECT_EQ(cs->Child.Get()->NumValue.Get(), 2);
}

/*
 "system.out(a, b)"
 ------------------
 functionExp
  |
 identiExp   ---   Iden("out")   ---   defautExp
  |                                     |
 iden("system")                        identiExp   ---   identiExp
                                        |                 |
                                       iden("a")         iden("b")  
*/
TEST(ExpressionNonTermin, FuncitonCall) {
    auto exp = ConstructRDCase("system.out(a, b)");

    auto root = exp->Root.Get();
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_FunctionCall);

    auto child1 = root->Child.Get();
    EXPECT_EQ(GetSubTypeExp(child1), TreeSubType_Expression::Exp_Identifier);
    EXPECT_EQ(GetMainType(child1->Child.Get()), TreeMainType::Identifier);
    EXPECT_EQ(child1->Child.Get()->StrValue.Get(), "system");

    auto child2 = child1->Subling.Get();
    EXPECT_EQ(GetMainType(child2), TreeMainType::Identifier);
    EXPECT_EQ(child2->StrValue.Get(), "out");

    auto child3 = child2->Subling.Get();
    EXPECT_EQ(GetMainType(child3), TreeMainType::Default);

    auto child3_1 = child3->Child.Get();
    EXPECT_EQ(GetSubTypeExp(child3_1), TreeSubType_Expression::Exp_Identifier);
    EXPECT_EQ(GetMainType(child3_1->Child.Get()), TreeMainType::Identifier);
    EXPECT_EQ(child3_1->Child.Get()->StrValue.Get(), "a");

    auto child3_2 = child3_1->Subling.Get();
    EXPECT_EQ(GetSubTypeExp(child3_2), TreeSubType_Expression::Exp_Identifier);
    EXPECT_EQ(GetMainType(child3_2->Child.Get()), TreeMainType::Identifier);
    EXPECT_EQ(child3_2->Child.Get()->StrValue.Get(), "b");
}

#pragma endregion

#pragma region 复合情况测试
/*
 "this.length.length"
 -------------------
 length-null
   |
 length-null
   |
  this-null
   |
  null
*/
TEST(ExpressionComplex, ThisLengthLength) {
    auto expression = ConstructRDCase("this.length.length");

    auto root = expression->Root.Get();
    EXPECT_NE(root, nullptr);
    EXPECT_EQ(GetMainType(root), TreeMainType::Expression);
    EXPECT_EQ(GetSubTypeExp(root), TreeSubType_Expression::Exp_Length);
    EXPECT_EQ(root->Subling.Get(), nullptr);

    auto child = root->Child.Get();
    EXPECT_EQ(GetMainType(child), TreeMainType::Expression);
    EXPECT_EQ(GetSubTypeExp(child), TreeSubType_Expression::Exp_Length);
    EXPECT_EQ(child->Subling.Get(), nullptr);

    auto grandChild = child->Child.Get();
    EXPECT_EQ(GetMainType(grandChild), TreeMainType::Expression);
    EXPECT_EQ(GetSubTypeExp(grandChild), TreeSubType_Expression::Exp_This);
    EXPECT_EQ(grandChild->Subling.Get(), nullptr);
    EXPECT_EQ(grandChild->Child.Get(), nullptr);
}
#pragma endregion

#pragma region 错误测试


TEST(ExpressionErrorTest, acc) {
    auto exp = ConstructRDCase("(this");
    
    auto root = exp->Root.Get();
    EXPECT_EQ(root, nullptr);

    auto errors = exp->Errors.Get();
    EXPECT_EQ(errors.size(), 1);
}

#pragma endregion

