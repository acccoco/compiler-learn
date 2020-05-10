#include "pch.h"

/*
  TokenReader 的测试
 */

class TokenReaderTest : public ::testing::Test {
protected:
    TokenReader* reader = nullptr;
    /*
     构造一个Reader出来
    */
    void SetUp() override {
        auto tokens = ConstructTokens();
        reader = new TokenReader(tokens);
    }
    void TearDown() override {
        delete reader;
    }

    /* 
     tokens 的数据集：
     10行10列，都是关键字 "acc row, column"
    */
    vector<vector<shared_ptr<Token>>> ConstructTokens() {
        vector<vector<shared_ptr<Token>>> tokens;
        for (int i = 0; i < 10; ++i) {
            vector<shared_ptr<Token>> tokensRow;
            for (int j = 0; j < 10; ++j) {
                char temp[256];
                string strValue = "acc ";
                _itoa_s(i + 1, temp, 10);
                strValue += temp;
                strValue += ", ";
                _itoa_s(j + 1, temp, 10);
                strValue += temp;
                tokensRow.emplace_back(new Token(TokenTypeEnum::IDENTIFIER, strValue));
            }
            tokens.push_back(tokensRow);
        }
        return tokens;
    }
};

/*
 测试Index属性的读写是否正常
*/
TEST_F(TokenReaderTest, Index) {
    EXPECT_EQ(reader->Index.Get(), 0);     // 初始index为0
    reader->RecoveryIndex(1);
    EXPECT_EQ(reader->Index.Get(), 1);
}

/*
 测试TokenCount属性
*/
TEST_F(TokenReaderTest, TokenCount) {
    EXPECT_EQ(reader->TokenCount.Get(), 100);
}

/*
 测试SeekToken
 SeekToken不会改变 index，count等属性
 SeekToken可以正常读取值
*/
TEST_F(TokenReaderTest, SeekToken) {
    EXPECT_EQ(reader->SeekToken()->StrValue.Get(), "acc 1, 1");
    EXPECT_EQ(reader->Index.Get(), 0);
    EXPECT_EQ(reader->TokenCount.Get(), 100);
}
/*
 测试GetToken
 GetToken应该会改变index
*/
TEST_F(TokenReaderTest, GetToken) {
    // 进行一次读：
    reader->RecoveryIndex(26);
    auto token = reader->GetToken();
    EXPECT_EQ(token->StrValue.Get(), "acc 3, 7");
    EXPECT_EQ(token->LineNum.Get(), 3);
    EXPECT_EQ(reader->Index.Get(), 27);
    EXPECT_EQ(reader->GetLastLineNum(), 3);
    EXPECT_EQ(reader->GetCurLineNum(), 3);
}
/*
 测试边界
 走到边界后，就无法再走了
*/
TEST_F(TokenReaderTest, Boder) {
    for (int i = 0; i < 99; ++i) {
        reader->GetToken();
        EXPECT_EQ(reader->Index.Get(), 1 + i);
        EXPECT_EQ(reader->IsEnd(), false);
    }
    auto token = reader->GetToken();
    EXPECT_EQ(token->StrValue.Get(), "acc 10, 10");
    EXPECT_EQ(reader->IsEnd(), true);
    EXPECT_EQ(reader->SeekToken(), nullptr);
    EXPECT_EQ(reader->GetCurLineNum(), 0);
    EXPECT_EQ(reader->GetLastLineNum(), 10);
    EXPECT_EQ(reader->Index.Get(), 100);
}