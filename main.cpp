// lexer headers
#include "lexer/lexer.h"
#include "lexer/token.h"
// AST headers
#include "ast/ExprAST.h"
#include "ast/FunctionAST.h"
#include "ast/PrototypeAST.h"
// parser headers
#include "parser/parser.h"
// kaleidoscope headers
#include "kaleidoscope/kaleidoscope.h"
// LLVM headers
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Module.h"
// stdlib headers
#include <cstdio>
#include <memory>

using namespace llvm;

static void HandleDefinition() {
  if (auto FnAST = ParseDefinition()) {
    if (const auto *FnIR = FnAST->codegen()) {
      fprintf(stderr, "Read function definition:");
      FnIR->print(errs());
      fprintf(stderr, "\n");
    }
  } else {
    getNextToken();
  }
}

static void HandleExtern() {
  if (auto ProtoAST = ParseExtern()) {
    if (const auto *FnIR = ProtoAST->codegen()) {
      fprintf(stderr, "Read extern:");
      FnIR->print(errs());
      fprintf(stderr, "\n");
    }
  } else {
    getNextToken();
  }
}

static void HandleTopLevelExpression() {
  if (auto FnAST = ParseTopLevelExpr()) {
    if (const auto *FnIR = FnAST->codegen()) {
      fprintf(stderr, "Read top-level expression:");
      FnIR->print(errs());
      fprintf(stderr, "\n");
    }
  } else {
    getNextToken();
  }
}

static void MainLoop() {
  while (true) {
    fprintf(stderr, "ready> ");

    switch (CurTok) {
      case tok_eof:
        return;
      case ';':
        getNextToken();
        break;
      case tok_def:
        HandleDefinition();
        break;
      case tok_extern:
        HandleExtern();
        break;
      default:
        HandleTopLevelExpression();
        break;
    }
  }
}

int main(int argc, char* argv[]) {
  // Install standard binary operators.
  // 1 is lowest precedence.
  BinopPrecedence['<'] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40; // highest.

  fprintf(stderr, "ready> ");
  getNextToken();

  TheModule = std::make_unique<Module>("My awesome JIT", TheContext);

  MainLoop();

  TheModule->print(errs(), nullptr);

  return 0;
}
