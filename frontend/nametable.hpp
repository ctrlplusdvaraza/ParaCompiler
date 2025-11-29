#pragma once

#include <stack>
#include <cassert>
#include <unordered_map>
#include "ast.hpp"
namespace compiler
{

struct NameTableVariable {
    const std::string name;

    NameTableVariable(const std::string &n) : name(std::move(n)) {}
};

class NameTable {
    std::unordered_map<std::string, std::unique_ptr<NameTableVariable>> data_;
  public:
    void add_record(const std::string &varible_name) {
        if (data_.find(varible_name) != data_.end()) return;
    
        NameTableVariable *raw_varible = new NameTableVariable(varible_name);
        if (raw_varible == nullptr) throw std::bad_alloc();
    
        std::unique_ptr<NameTableVariable> variable = std::unique_ptr<NameTableVariable>(raw_varible);
        data_[variable->name] = std::move(variable);    
    }

    NameTableVariable *find_record(const std::string &varible_name) const {
        if (data_.find(varible_name) == data_.end()) return nullptr;
        return data_.find(varible_name)->second.get();
    }

    void dump() const {
        for (const auto &pair : data_) {
            std::cout << "  " << pair.first << "\n";
        }
    }
};

class NamesEnviroment 
{
    std::vector<NameTable> scopes_chain_;
  public:
    NamesEnviroment() {
        scopes_chain_.push_back(NameTable()); // global scope
    }

    void put_variable(const std::string &name) {
        assert(!scopes_chain_.empty());
        scopes_chain_.back().add_record(name); 
    }

    bool find_varible_in_top_scope(const std::string &name) const {
        return (scopes_chain_.back().find_record(name) != nullptr);
    }

    NameTableVariable *get_variable(const std::string &name) const {
        NameTableVariable *result = nullptr;
    
        for (auto it = scopes_chain_.rbegin(); it != scopes_chain_.rend(); it++) {
            result = it->find_record(name);
            if (result != nullptr) break;
        }

        return result;
    }
    
    void push_scope() { scopes_chain_.push_back(NameTable()); }

    void pop_scope() {
        assert(!scopes_chain_.empty());
        scopes_chain_.pop_back();
    }

    void dump() const {
        std::cout << "=== NamesEnviroment Dump ===\n";
        for (size_t i = 0; i < scopes_chain_.size(); ++i) {
            std::cout << "Scope " << i << ":\n";
            const auto &table = scopes_chain_[i];
            table.dump();
        }
        std::cout << "============================\n";
    }
};

}; // namespace compiler
