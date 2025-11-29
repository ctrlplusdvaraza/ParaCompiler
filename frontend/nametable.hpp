#pragma once

#include <cassert>
#include <unordered_map>

#include "ast.hpp"

namespace compiler::frontend
{

struct Variable
{
    const std::string name;

    Variable(const std::string& n) : name(std::move(n)) {}
};

class NameTable
{
  public:
    using VariableName = std::string;
    using VariablePtr = std::unique_ptr<Variable>;

  public:
    void add_record(const std::string& varible_name)
    {
        if (data_.find(varible_name) != data_.end())
        {
            return;
        }

        data_[varible_name] = std::make_unique<Variable>(varible_name);
    }

    Variable* find_record(const std::string& varible_name) const
    {
        if (data_.find(varible_name) == data_.end())
        {
            return nullptr;
        }
        return data_.find(varible_name)->second.get();
    }

    void dump() const
    {
        for (const auto& pair : data_)
        {
            std::cout << "  " << pair.first << "\n";
        }
    }

  private:
    std::unordered_map<VariableName, VariablePtr> data_;
};

class NamesEnviroment
{
  public:
    NamesEnviroment()
    {
        scopes_chain_.push_back(NameTable()); // global scope
    }

    void put_variable(const std::string& name)
    {
        assert(!scopes_chain_.empty());
        scopes_chain_.back().add_record(name);
    }

    Variable* get_variable(const std::string& name) const
    {
        Variable* result = nullptr;

        for (auto it = scopes_chain_.rbegin(); it != scopes_chain_.rend(); ++it)
        {
            result = it->find_record(name);
            if (result != nullptr)
            {
                break;
            }
        }

        return result;
    }

    bool find_variable(const std::string& name) { return (get_variable(name) != nullptr); }

    void push_scope() { scopes_chain_.push_back(NameTable()); }

    void pop_scope()
    {
        assert(!scopes_chain_.empty());
        scopes_chain_.pop_back();
    }

    void dump() const
    {
        std::cout << "=== NamesEnviroment Dump ===\n";
        for (size_t i = 0; i < scopes_chain_.size(); ++i)
        {
            std::cout << "Scope " << i << ":\n";
            const auto& table = scopes_chain_[i];
            table.dump();
        }
        std::cout << "============================\n";
    }

  private:
    std::vector<NameTable> scopes_chain_;
};

}; // namespace compiler::frontend
