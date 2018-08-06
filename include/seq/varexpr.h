#ifndef SEQ_VAREXPR_H
#define SEQ_VAREXPR_H

#include "var.h"
#include "func.h"
#include "expr.h"

namespace seq {

	class VarExpr : public Expr {
	private:
		Var *var;
	public:
		explicit VarExpr(Var *var);
		llvm::Value *codegen(BaseFunc *base, llvm::BasicBlock*& block) override;
		types::Type *getType() const override;
		VarExpr *clone(types::RefType *ref) override;
	};

	class FuncExpr : public Expr {
	private:
		BaseFunc *func;
	public:
		explicit FuncExpr(BaseFunc *func);
		llvm::Value *codegen(BaseFunc *base, llvm::BasicBlock*& block) override;
		types::Type *getType() const override;
		FuncExpr *clone(types::RefType *ref) override;
	};

}

#endif /* SEQ_VAREXPR_H */
