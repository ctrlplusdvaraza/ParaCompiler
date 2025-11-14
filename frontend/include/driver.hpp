#ifndef DRIVER_HH
#define DRIVER_HH
#include <string>
#include <map>
#include "parser.hpp"
#include "ast.hpp"


// Give Flex the prototype of yylex we want ...
# define YY_DECL \
    yy::parser::symbol_type yylex (driver& drv)
// ... and declare it for the parser's sake.
YY_DECL;


// Conducting the whole scanning and parsing of Calc++.
class driver
{
public:
    Driver() : debug_parsing_(false), debug_lexing_(false) {}

    AstRootPtr ast_root;

    AstRootPtr get_ast_root() { return std::move(ast_root); }

    const std::string& get_file() const { return file_; }
    yy::location& get_location() { return location_; }
    bool is_debug_lexing() const { return debug_lexing_; }
    bool is_debug_parsing() const { return debug_lexing_; }

    void set_trace_lexing(bool value = true) { debug_lexing_ = value; }
    void set_trace_parsing(bool value = true) { debug_lexing_ = value; }

    int parse(const std::string& file_path)
    {
        file_ = file_path;
        location_.initialize(&get_file());
 
        scan_begin();
        yy::parser parser(*this);

        parser.set_debug_level(is_debug_parsing());
        int res = parser();

        scan_end();
        return res;
    }

    // Handling the scanner.
    void scan_begin();
    void scan_end();

  private:
    std::string file_;
    yy::location location_;
    bool debug_lexing_;
    bool debug_parsing_;
};




#endif // ! DRIVER_HH
