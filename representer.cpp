#include <clang-c/Index.h>

#include <fstream>
#include <iostream>

static int counter = 1;
static std::ofstream ostrm;

int main() {
  CXIndex idx = clang_createIndex(0, 0);
  CXTranslationUnit tu = clang_parseTranslationUnit(
      idx, "test.c", nullptr, 0, nullptr, 0, CXTranslationUnit_None);
  if (tu == nullptr) {
    return 1;
  }
  CXCursor cursor = clang_getTranslationUnitCursor(tu);
  ostrm.open("replace.txt");
  clang_visitChildren(
      cursor,
      [](CXCursor cursor, CXCursor parent, CXClientData client_data) {
        CXSourceLocation location = clang_getCursorLocation(cursor);
        if (!clang_Location_isFromMainFile(location)) {
          return CXChildVisit_Recurse;
        }
        CXCursorKind kind = clang_getCursorKind(cursor);
        if (kind == CXCursor_VarDecl) {
          CXString spelling = clang_getCursorSpelling(cursor);
          ostrm << "PLACEHOLDER_" << counter++ << ' ';
          ostrm << clang_getCString(spelling) << '\n';
          clang_disposeString(spelling);
        }
        return CXChildVisit_Recurse;
      },
      nullptr);
  clang_disposeTranslationUnit(tu);
  clang_disposeIndex(idx);
}
