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

void SimulatorState::set_var(const std::string& name, ValueType val) { state_vals_[name] = val; }

SimulatorState::ValueType SimulatorState::get_var(const std::string& name) const
{
    auto it = state_vals_.find(name);
    if (it == state_vals_.end())
    {
        throw std::runtime_error("Undefined variable: " + name);
    }
    return it->second;
}

bool SimulatorState::has_var(const std::string& name) const
{
    return state_vals_.find(name) != state_vals_.end();
}

SimulatorState::ValueType& SimulatorState::get_var_ref(const std::string& name)
{
    if (!has_var(name))
    {
        state_vals_[name] = 0;
    }
    return state_vals_[name];
}

SimulatorState::ValueType evaluate_expr(SimulatorState& state, const AstNodePtr& node);

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

SimulatorState::ValueType evaluate_expr(SimulatorState& state, const AstNodePtr& node)
{
    if (!node)
    {
        throw std::runtime_error("Null node in expression evaluation");
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
            throw std::runtime_error("Division by zero");
        return evaluate_expr(state, node->children[0]) / divisor;
    }

    if (node->is_node_type<ModNode>())
    {
        auto divisor = evaluate_expr(state, node->children[1]);
        if (divisor == 0)
            throw std::runtime_error("Modulo by zero");
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
        SimulatorState::ValueType value;
        std::cin >> value;

        if (std::cin.fail())
        {
            throw std::runtime_error("Invalid input");
        }

        return value;
    }

    // assignments

    if (node->is_node_type<AssignmentNode>())
    {
        const auto& children = node->children;
        if (children.size() != 2)
            throw std::runtime_error("Assignment node requires 2 children");

        const auto& lhs = children[0];
        if (!lhs->is_node_type<IdentifierNode>())
            throw std::runtime_error("Left side of assignment must be identifier");

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
            throw std::runtime_error("Division by zero");
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
            throw std::runtime_error("Modulo by zero");
        var %= divisor;
        return evaluate_expr(state, lhs);
    }

    throw std::runtime_error("Unknown node type in expression evaluation");
}

void simulate_ast(SimulatorState& state, const AstRootPtr& root)
{
    for (const auto& child : root->children)
    {
        execute_stmt(state, child);
    }
}

} // namespace compiler
