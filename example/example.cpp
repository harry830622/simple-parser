#include "../simple_parser.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace simple_parser;
using namespace std;

int main() {
  string example_text(
      string("") +
      "PINSPEC Sep. 8, 2016                                                \n" +
      "PIN.VDD_A                                                           \n" +
      "  @DIRECTION  INOUT    ;                                            \n" +
      "                                                                    \n" +
      "  PORT                                                              \n" +
      "  LAYER +METAL5 ;                                                   \n" +
      "     RECT 0 200 5 250;                                              \n" +
      "                                                                    \n" +
      "  LAYER +METAL6 ;                                                   \n" +
      "     RECT 50 400 100 350                                            \n" +
      "     ;                                                              \n" +
      "  END                                                               \n" +
      "ENDPIN.VDD_A                                                        \n" +
      "blah blah blah...                                                   \n");

  // Example 1: Parse all lines in the input stream.
  istringstream example_1(example_text);
  Parser parser_1(example_1, ".+;");
  cout << "Example 1:" << endl;
  parser_1.Parse([](const Tokens& tokens) -> bool {
    if (!tokens.empty()) {
      for (const Token& token : tokens) {
        cout << token << " ";
      }
      cout << endl;
    }

    // Return true to continue parsing, otherwise, return false.
    return true;
  });
  cout << endl;

  // Example 2: Parse all lines in the input stream again, but let the
  // function port_handler parse the PORT block this time.
  cout << "Example 2:" << endl;
  istringstream example_2(example_text);
  Parser parser_2(example_2, ".+;");

  Handler port_handler = [](const Tokens& tokens) -> bool {
    if (!tokens.empty()) {
      Token first_token = tokens.at(0);
      if (first_token == "END") {
        return false;
      }
      for (const Token& token : tokens) {
        cout << token << " ";
      }
      cout << endl;
    }
    return true;
  };

  parser_2.Parse([&](const Tokens& tokens) -> bool {
    if (!tokens.empty()) {
      Token first_token = tokens.at(0);
      if (first_token == "PORT") {
        parser_2.Parse(port_handler);
        return false;
      }
    }

    // Always return a boolean to tell the parser whether to continue parsing.
    return true;
  });
  cout << endl;

  return 0;
}
