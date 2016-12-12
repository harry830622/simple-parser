#include "../simple_parser.hpp"

#include <gtest/gtest.h>
#include <string>

using namespace simple_parser;
using namespace std;

class SimpleParserTest : public testing::Test {
 protected:
  SimpleParserTest()
      : input_(new istringstream(
            string("") +
            "PINSPEC Sep. 8, 2016                                         \n" +
            "PIN.VDD_A                                                    \n" +
            "  @DIRECTION  INOUT    ;                                     \n" +
            "                                                             \n" +
            "  PORT >//<                                                  \n" +
            "  LAYER + METAL5 ;                                           \n" +
            "     RECT (0 200 5 250);                                     \n" +
            "                                                             \n" +
            "  LAYER METAL6 ;                                             \n" +
            "     RECT 50 400 100 350                                     \n" +
            "     ;                                                       \n" +
            "  END                                                        \n" +
            "  USELESSLINE                                                \n" +
            "ENDPIN.VDD_A                                                 \n" +
            "blah blah blah...                                            \n")),
        parser_(*input_, ".+;()><@/") {}

  unique_ptr<istream> input_;
  Parser parser_;
};

TEST_F(SimpleParserTest, Parse) {
  parser_.Parse([](const Tokens& tokens) -> bool {
    if (!tokens.empty()) {
      if (tokens.at(0) == "blah") {
        EXPECT_EQ(tokens.at(1), "blah");
      }
    }
    return true;
  });
}
