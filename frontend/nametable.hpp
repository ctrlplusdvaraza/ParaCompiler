#pragma once

#include <cassert>
#include <unordered_map>

#include "ast.hpp"
#include "nametable_record.hpp"

namespace compiler::frontend
{

class NameTable
{
  public:
    void add_record(const RecordNameType& record_name)
    {
        if (data_.find(record_name) != data_.end())
        {
            return;
        }

        data_[record_name] = std::make_unique<NameTableVariable>(record_name);
    }

    const AbstractNameTableRecord *get_record(const RecordNameType& record_name) const
    {
        if (data_.find(record_name) == data_.end()) return nullptr;
        return data_.find(record_name)->second.get();
    }

    void dump() const
    {
        for (const auto& pair : data_)
        {
            std::cout << "  " << pair.first << "\n";
        }
    }

  private:
    std::unordered_map<RecordNameType, RecordPtr> data_;
};

class NamesEnviroment
{
  public:
    NamesEnviroment()
    {
        scopes_chain_.push_back(NameTable()); // global scope
    }

    void push_scope() { scopes_chain_.push_back(NameTable()); }
    void pop_scope()
    {
        assert(!scopes_chain_.empty());
        scopes_chain_.pop_back();
    }

    void put_variable(const RecordNameType& name)
    {
        assert(!scopes_chain_.empty());
        scopes_chain_.back().add_record(name);
    }
    // put_function, put_class etc...
    
    bool find_variable(const RecordNameType& name) {
        const AbstractNameTableRecord *result = get_record_from_chain(name);
        if (result == nullptr) return false;
        if (!result->is_node_type<NameTableVariable>()) return false;

        return true;
    }
    // find_function, find_class etc...
    
    bool check_variable_usage_semantic(const RecordNameType& name) {
        return find_variable(name);
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

    const AbstractNameTableRecord* get_record_from_chain(const RecordNameType& name) const
    {
        const AbstractNameTableRecord* result = nullptr;

        for (auto it = scopes_chain_.rbegin(); it != scopes_chain_.rend(); ++it)
        {
            result = it->get_record(name);
            if (result != nullptr)
            {
                break;
            }
        }

        return result;
    }
};

}; // namespace compiler::frontend
