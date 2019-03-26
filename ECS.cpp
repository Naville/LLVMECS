/*
 *  LLVM Obfuscation Pass
    Copyright (C) 2017 Zhang(https://github.com/Naville/)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.
    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "llvm/ADT/SmallVector.h"
#include "llvm/Transforms/Obfuscation/Obfuscation.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
using namespace llvm;
using namespace std;
namespace llvm {
struct ECS : public FunctionPass {
  static char ID;
  bool flag;
  ECS() : FunctionPass(ID) { this->flag = true; }
  ECS(bool flag) : FunctionPass(ID) { this->flag = flag; }
  StringRef getPassName() const override { return StringRef("ExposeClassStruct"); }
  virtual bool runOnFunction(Function &F) override {
    for (inst_iterator I = inst_begin(F); I != inst_end(F); I++) {
    Instruction *Inst = &*I;
    if (CallInst *CI = dyn_cast<CallInst>(Inst)) {
      if (CI->getCalledFunction() != nullptr &&
          CI->getCalledFunction()->getName().contains("hikari_ecs")) {
        Value* arg=CI->getArgOperand(0);
        arg=arg->stripPointerCasts();
        //可能需要解GEP
        GlobalVariable* GV=cast<GlobalVariable>(arg);
        ConstantDataSequential* str=cast<ConstantDataSequential>(GV->getInitializer());
        StringRef clsName=str->getAsCString();
        Constant* CS=F.getParent()->getGlobalVariable(StringRef("OBJC_CLASS_$_"+clsName.str()),true);
        if(CS!=nullptr){
          errs()<<"Found ECS"<<*CS<<"\n";
          CI->replaceAllUsesWith(ConstantExpr::getBitCast(CS,CI->getType()));
        }
        else{
          errs()<<"ObjC Class Struct for ClassName:"<<clsName<<" not Found! Replacing with NULL Pointer"<<"\n";
          CI->replaceAllUsesWith(ConstantPointerNull::get(cast<PointerType>(CI->getType())));
        }

      }
    }
  }
    return true;
  }
}; // namespace llvm
FunctionPass *createECSPass() { return new ECS(); }
FunctionPass *createECSPass(bool flag) {
  return new ECS(flag);
}
} // namespace llvm
char ECS::ID = 0;
INITIALIZE_PASS(ECS, "ecs", "Enable ExposeClassStruct Intrinsics",
                true, true)
