#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "serialization.hpp"

namespace compiler
{

struct SimulatorNameTableVar {
    using ValueType = int64_t;
    const std::string name;
    ValueType value;

    SimulatorNameTableVar(const std::string &name, const ValueType value) :
        name(std::move(name)), value(value) {}
};

class SimulatorNameTable {
  std::unordered_map<std::string, std::unique_ptr<SimulatorNameTableVar>> data_;
  public:
    void add_record(const std::string &varible_name, const SimulatorNameTableVar::ValueType value) {
        if (data_.find(varible_name) != data_.end()) return;
    
        SimulatorNameTableVar *raw_varible = new SimulatorNameTableVar(varible_name, value);
        if (raw_varible == nullptr) throw std::bad_alloc();
    
        std::unique_ptr<SimulatorNameTableVar> variable = std::unique_ptr<SimulatorNameTableVar>(raw_varible);
        data_[variable->name] = std::move(variable);    
    }

    SimulatorNameTableVar *find_record(const std::string &varible_name) const {
        if (data_.find(varible_name) == data_.end()) return nullptr;
        return data_.find(varible_name)->second.get();
    }

    void dump() const {
        for (const auto &pair : data_) {
            std::cout << "  " << pair.first << "\n";
        }
    }
};

class SimulatorState
{
  public:
    SimulatorState() {
        scopes_chain_.push_back(SimulatorNameTable()); // global scope
    }

    void set_var(const std::string& name, const SimulatorNameTableVar::ValueType val);
    bool has_var(const std::string& name) const;

    SimulatorNameTableVar::ValueType get_var(const std::string &name) const;
    SimulatorNameTableVar::ValueType& get_var_ref(const std::string& name);

  private:
    std::vector<SimulatorNameTable> scopes_chain_;
};

class SimulatorException : public std::runtime_error
{
  public:
    using std::runtime_error::runtime_error;
};

void simulate_ast(SimulatorState& state, const AstRootPtr& root);

} // namespace compiler
