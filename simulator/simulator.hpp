#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "serialization.hpp"

namespace compiler
{

class SimulatorState
{
  public:
    using ValueType = int64_t;

    SimulatorState() = default;

    void set_var(const std::string& name, ValueType val);
    ValueType get_var(const std::string& name) const;
    bool has_var(const std::string& name) const;
    ValueType& get_var_ref(const std::string& name);

  private:
    std::unordered_map<std::string, ValueType> state_vals_;
};

void simulate_ast(SimulatorState& state, const AstRootPtr& root);

} // namespace compiler
