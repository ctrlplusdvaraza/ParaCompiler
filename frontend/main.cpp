#include "assignment_tokens.hpp"
#include "ast.hpp"
#include "driver.hpp"
#include "tokens.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>


void traverse_ast(const compiler::AstNodePtr& node)
{
    if (node->token.get()->is_token_of_type<compiler::AssignmentToken>())
    {
        std::cout << node->children[0]->token->get_string_token() << node->token->get_string_token()
                  << node->children[1]->token->get_string_token() << std::endl;
    }

    for (const auto& cur : node->children)
    {
        traverse_ast(cur);
    }
}

void traverse_ast_root(const compiler::AstRootPtr& root)
{
    for (const auto& child : root->children)
    {
        traverse_ast(child);
    }
}

int main(int argc, char* argv[])
{
    int res = 0;
    Driver driver;
    for (int i = 1; i < argc; ++i)
    {
        if (!driver.parse(argv[i]))
        {
            auto root = driver.get_ast_root();
            traverse_ast_root(root);
        }
        else
        {
            res = 1;
        }
    }

    return res;
}
