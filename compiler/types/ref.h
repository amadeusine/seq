#pragma once

#include "types/func.h"
#include "types/generic.h"
#include "types/record.h"
#include <map>
#include <utility>
#include <vector>

namespace seq {
class Func;

namespace types {
class GenericType;

class RefType : public Type, public Generic {
private:
  /*
   * Whether the class is finished being constructed
   */
  bool done;

  /*
   * The base reference if generic
   */
  RefType *root;

  /*
   * Our own realization cache
   */
  RCache<types::Type> realizationCache;

  RecordType *contents;
  std::vector<std::string> membNamesDeduced;
  std::vector<Expr *> membExprsDeduced;

  explicit RefType(std::string name);
  llvm::Type *getStructPointerType(llvm::LLVMContext &context) const;
  void setDeducedContents();

public:
  RefType(RefType const &) = delete;
  void operator=(RefType const &) = delete;
  void setDone();
  void setContents(RecordType *contents);
  void addMember(std::string name, Expr *expr);
  std::string getName() const override;
  std::string genericName() override;
  types::Type *realize(std::vector<types::Type *> types);
  std::vector<types::Type *>
  deduceTypesFromArgTypes(std::vector<types::Type *> argTypes,
                          std::vector<std::string> names = {});

  llvm::Value *memb(llvm::Value *self, const std::string &name,
                    llvm::BasicBlock *block) override;

  Type *membType(const std::string &name) override;

  llvm::Value *setMemb(llvm::Value *self, const std::string &name,
                       llvm::Value *val, llvm::BasicBlock *block) override;

  llvm::Value *defaultValue(llvm::BasicBlock *block) override;

  void initOps() override;
  void initFields() override;
  bool isAtomic() const override;
  bool is(types::Type *type) const override;
  unsigned numBaseTypes() const override;
  Type *getBaseType(unsigned idx) const override;
  llvm::Type *getLLVMType(llvm::LLVMContext &context) const override;
  size_t size(llvm::Module *module) const override;

  RefType *asRef() override;

  llvm::Value *make(llvm::BasicBlock *block,
                    std::vector<llvm::Value *> vals = {});
  static RefType *get(std::string name);

  types::RefType *clone(Generic *ref) override;
  void addCachedRealized(std::vector<types::Type *> types, Generic *x) override;

  static RefType *none();
};

class MethodType : public RecordType {
private:
  Type *self;
  FuncType *func;
  MethodType(Type *self, FuncType *func);

public:
  MethodType(MethodType const &) = delete;
  void operator=(MethodType const &) = delete;

  llvm::Value *call(BaseFunc *base, llvm::Value *self,
                    const std::vector<llvm::Value *> &args,
                    llvm::BasicBlock *block, llvm::BasicBlock *normal,
                    llvm::BasicBlock *unwind) override;

  bool is(types::Type *type) const override;
  unsigned numBaseTypes() const override;
  Type *getBaseType(unsigned idx) const override;
  Type *getCallType(const std::vector<Type *> &inTypes) override;
  llvm::Value *make(llvm::Value *self, llvm::Value *func,
                    llvm::BasicBlock *block);
  static MethodType *get(Type *self, FuncType *func);

  MethodType *clone(Generic *ref) override;
};
} // namespace types
} // namespace seq
