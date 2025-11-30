#pragma once

#include <string>
#include <memory>
namespace compiler::frontend
{

class AbstractNameTableRecord;

using RecordNameType = std::string;
using RecordPtr = std::unique_ptr<AbstractNameTableRecord>;

class AbstractNameTableRecord
{
  public:
    explicit AbstractNameTableRecord(RecordNameType name) : name_(name) {}

    AbstractNameTableRecord(const AbstractNameTableRecord& other) = delete;
    AbstractNameTableRecord(AbstractNameTableRecord&& other) = delete;

    AbstractNameTableRecord& operator=(const AbstractNameTableRecord& other) = delete;
    AbstractNameTableRecord& operator=(AbstractNameTableRecord&& other) = delete;

    virtual ~AbstractNameTableRecord() = default;

  public:
    const std::string& get_name() const { return name_; }

  public:
    class Visitor;
    virtual void accept(Visitor& visitor) const = 0;

    template <typename T>
    bool is_node_type() const;

  protected:
    RecordNameType name_;
};


class AbstractNameTableRecord::Visitor
{
  public:
    virtual void visit(const class NameTableVariable& token) = 0;
};

template <typename Derived>
class BaseNameTableRecord : public AbstractNameTableRecord
{
  public:
    using AbstractNameTableRecord::AbstractNameTableRecord;

  public:
    virtual void accept(Visitor& visitor) const override
    {
        visitor.visit(static_cast<const Derived&>(*this));
    }
};

template <typename T>
class NameTableTypeCheckVisitor : public AbstractNameTableRecord::Visitor
{
  public:
    void visit(const NameTableVariable&)  override { check_type<NameTableVariable>(); }

    bool is_type() const { return result_; }

  private:
    template <typename U>
    void check_type()
    {
        result_ = std::is_same_v<T, U>;
    }

  private:
    bool result_ = false;
};

template <typename T>
bool AbstractNameTableRecord::is_node_type() const
{
    NameTableTypeCheckVisitor<T> visitor;
    accept(visitor);
    return visitor.is_type();
}

class NameTableVariable : public BaseNameTableRecord<NameTableVariable>
{
  public:
    using BaseNameTableRecord<NameTableVariable>::BaseNameTableRecord;
};

}

