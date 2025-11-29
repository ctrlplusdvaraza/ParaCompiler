#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "ast.hpp"
#include "serialization.hpp"
#include "simulator.hpp"

namespace compiler
{

SimulatorNameTableVar *SimulatorState::get_table_var_ptr(const std::string &name) const {
    SimulatorNameTableVar *result = nullptr;
    for (auto it = scopes_chain_.rbegin(); it != scopes_chain_.rend(); it++) {
        result = it->find_record(name);
        if (result != nullptr) break;
    }

    return result;
}

void SimulatorState::set_var(const std::string& name, const SimulatorNameTableVar::ValueType val) {
    assert(!scopes_chain_.empty());

    if (!has_var(name)) { 
        scopes_chain_.back().add_record(name, val); 
        return;
    }
    else {get_var_ref(name) = val; }
}

SimulatorNameTableVar::ValueType SimulatorState::get_var(const std::string &name) const {
    assert(has_var(name));
    return get_table_var_ptr(name)->value;
}

bool SimulatorState::has_var(const std::string& name) const
{
    return (get_table_var_ptr(name) != nullptr);
}

void SimulatorState::push_scope() { scopes_chain_.push_back(SimulatorNameTable()); }

void SimulatorState::pop_scope() {
    assert(!scopes_chain_.empty());
    scopes_chain_.pop_back();
}

SimulatorNameTableVar::ValueType& SimulatorState::get_var_ref(const std::string& name)
{
    assert(has_var(name));
    return get_table_var_ptr(name)->value;
}

SimulatorNameTableVar::ValueType evaluate_expr(SimulatorState& state, const AstNodePtr& node);

bool execute_stmt(SimulatorState& state, const AstNodePtr& node)
{
    if (!node)
        return true;

    if (node->is_node_type<IfNode>())
    {
        const auto& children = node->children;
        auto condition = evaluate_expr(state, children[0]);

        if (condition)
        {
            if (children.size() > 1)
            {
                for (const auto& stmt : children[1]->children)
                {
                    if (!execute_stmt(state, stmt))
                        return false;
                }
            }
        }
        else
        {
            if (children.size() > 2)
            {
                for (const auto& stmt : children[2]->children)
                {
                    if (!execute_stmt(state, stmt))
                        return false;
                }
            }
        }
        return true;
    }

    if (node->is_node_type<WhileNode>())
    {
        const auto& children = node->children;

        while (evaluate_expr(state, children[0]))
        {
            for (const auto& stmt : children[1]->children)
            {
                if (!execute_stmt(state, stmt))
                    break;
            }
        }
        return true;
    }

    if (node->is_node_type<PrintNode>())
    {
        auto value = evaluate_expr(state, node->children[0]);
        std::cout << value << std::endl;
        return true;
    }

    evaluate_expr(state, node);
    return true;
}

SimulatorNameTableVar::ValueType evaluate_expr(SimulatorState& state, const AstNodePtr& node)
{
    if (!node)
    {
        throw SimulatorException("Null node in expression evaluation");
    }

    if (node->is_node_type<LiteralNode>())
    {
        return std::stoll(node->get_string_lexeme());
    }

    if (node->is_node_type<IdentifierNode>())
    {
        return state.get_var(node->get_string_lexeme());
    }

    if (node->is_node_type<AddNode>())
    {
        return evaluate_expr(state, node->children[0]) + evaluate_expr(state, node->children[1]);
    }

    if (node->is_node_type<SubNode>())
    {
        return evaluate_expr(state, node->children[0]) - evaluate_expr(state, node->children[1]);
    }

    if (node->is_node_type<MulNode>())
    {
        return evaluate_expr(state, node->children[0]) * evaluate_expr(state, node->children[1]);
    }

    if (node->is_node_type<DivNode>())
    {
        auto divisor = evaluate_expr(state, node->children[1]);
        if (divisor == 0)
            throw SimulatorException("Division by zero");
        return evaluate_expr(state, node->children[0]) / divisor;
    }

    if (node->is_node_type<ModNode>())
    {
        auto divisor = evaluate_expr(state, node->children[1]);
        if (divisor == 0)
            throw SimulatorException("Modulo by zero");
        return evaluate_expr(state, node->children[0]) % divisor;
    }

    if (node->is_node_type<EqualNode>())
    {
        return evaluate_expr(state, node->children[0]) == evaluate_expr(state, node->children[1])
                   ? 1
                   : 0;
    }

    if (node->is_node_type<NotEqualNode>())
    {
        return evaluate_expr(state, node->children[0]) != evaluate_expr(state, node->children[1])
                   ? 1
                   : 0;
    }

    if (node->is_node_type<LessNode>())
    {
        return evaluate_expr(state, node->children[0]) < evaluate_expr(state, node->children[1])
                   ? 1
                   : 0;
    }

    if (node->is_node_type<LessEqualNode>())
    {
        return evaluate_expr(state, node->children[0]) <= evaluate_expr(state, node->children[1])
                   ? 1
                   : 0;
    }

    if (node->is_node_type<GreaterNode>())
    {
        return evaluate_expr(state, node->children[0]) > evaluate_expr(state, node->children[1])
                   ? 1
                   : 0;
    }

    if (node->is_node_type<GreaterEqualNode>())
    {
        return evaluate_expr(state, node->children[0]) >= evaluate_expr(state, node->children[1])
                   ? 1
                   : 0;
    }

    if (node->is_node_type<NotNode>())
    {
        return !evaluate_expr(state, node->children[0]);
    }

    if (node->is_node_type<AndNode>())
    {
        return evaluate_expr(state, node->children[0]) && evaluate_expr(state, node->children[1])
                   ? 1
                   : 0;
    }

    if (node->is_node_type<OrNode>())
    {
        return evaluate_expr(state, node->children[0]) || evaluate_expr(state, node->children[1])
                   ? 1
                   : 0;
    }

    if (node->is_node_type<UnaryPlusNode>())
    {
        return evaluate_expr(state, node->children[0]);
    }

    if (node->is_node_type<UnaryMinusNode>())
    {
        return -evaluate_expr(state, node->children[0]);
    }

    if (node->is_node_type<PrefixIncrementNode>())
    {
        auto& var = state.get_var_ref(node->children[0]->get_string_lexeme());
        return ++var;
    }

    if (node->is_node_type<PostfixIncrementNode>())
    {
        auto& var = state.get_var_ref(node->children[0]->get_string_lexeme());
        return var++;
    }

    if (node->is_node_type<PrefixDecrementNode>())
    {
        auto& var = state.get_var_ref(node->children[0]->get_string_lexeme());
        return --var;
    }

    if (node->is_node_type<PostfixDecrementNode>())
    {
        auto& var = state.get_var_ref(node->children[0]->get_string_lexeme());
        return var--;
    }

    if (node->is_node_type<InputNode>())
    {
        SimulatorNameTableVar::ValueType value;
        std::cin >> value;

        if (std::cin.fail())
        {
            throw SimulatorException("Invalid input");
        }

        return value;
    }

    if (node->is_node_type<ScopeNode>()) {
        state.push_scope();
        for (const auto &stmt : node->children) {
            execute_stmt(state, stmt);
        }
        state.pop_scope();

        return true;
    }

    // assignments

    if (node->is_node_type<AssignmentNode>())
    {
        const auto& children = node->children;
        if (children.size() != 2)
            throw SimulatorException("Assignment node requires 2 children");

        const auto& lhs = children[0];
        if (!lhs->is_node_type<IdentifierNode>())
            throw SimulatorException("Left side of assignment must be identifier");

        auto value = evaluate_expr(state, children[1]);
        state.set_var(lhs->get_string_lexeme(), value);

        return evaluate_expr(state, lhs);
    }

    if (node->is_node_type<AddAssignmentNode>())
    {
        const auto& children = node->children;
        const auto& lhs = children[0];
        auto& var = state.get_var_ref(lhs->get_string_lexeme());
        var += evaluate_expr(state, children[1]);
        return evaluate_expr(state, lhs);
    }

    if (node->is_node_type<SubAssignmentNode>())
    {
        const auto& children = node->children;
        const auto& lhs = children[0];
        auto& var = state.get_var_ref(lhs->get_string_lexeme());
        var -= evaluate_expr(state, children[1]);
        return evaluate_expr(state, lhs);
    }

    if (node->is_node_type<MulAssignmentNode>())
    {
        const auto& children = node->children;
        const auto& lhs = children[0];
        auto& var = state.get_var_ref(lhs->get_string_lexeme());
        var *= evaluate_expr(state, children[1]);
        return evaluate_expr(state, lhs);
    }

    if (node->is_node_type<DivAssignmentNode>())
    {
        const auto& children = node->children;
        const auto& lhs = children[0];
        auto& var = state.get_var_ref(lhs->get_string_lexeme());
        auto divisor = evaluate_expr(state, children[1]);
        if (divisor == 0)
            throw SimulatorException("Division by zero");
        var /= divisor;
        return evaluate_expr(state, lhs);
    }

    if (node->is_node_type<ModAssignmentNode>())
    {
        const auto& children = node->children;
        const auto& lhs = children[0];
        auto& var = state.get_var_ref(lhs->get_string_lexeme());
        auto divisor = evaluate_expr(state, children[1]);
        if (divisor == 0)
            throw SimulatorException("Modulo by zero");
        var %= divisor;
        return evaluate_expr(state, lhs);
    }

    throw SimulatorException("Unknown node type in expression evaluation");
}

void simulate_ast(SimulatorState& state, const AstRootPtr& root)
{
    for (const auto& child : root->children)
    {
        execute_stmt(state, child);
    }
}

} // namespace compiler
