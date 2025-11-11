#include "driver.hh"
#include <cstdio>
#include <fstream>
#include <iostream>

extern FILE *yyin; /* created by flex */

driver::driver()
  : result(0), trace_parsing(false), trace_scanning(false)
{
}

void driver::scan_begin()
{
  // open file (driver.file must be set by parse())
  if (file.empty()) return;
  yyin = std::fopen(file.c_str(), "r");
  if (!yyin) {
    std::cerr << "Cannot open input file: " << file << '\n';
  }
}

void driver::scan_end()
{
  if (yyin) {
    std::fclose(yyin);
    yyin = nullptr;
  }
}

int driver::parse(const std::string& f)
{
  file = f;
  scan_begin();
  yy::parser parser(*this);    // pass driver to parser ctor
  // parser.set_debug(static_cast<int>(trace_parsing));   // REMOVE this line
  int res = parser.parse();
  scan_end();
  return res;
}
