# LLVMECS
Exposing ObjC Class Structures from LLVM, per request by [Rozbo](https://github.com/Rozbo)

# Building
**Note this Pass still uses Hikari's cleanup Infrastructure so you need that copy of LLVM**

- Put ``ECS.h`` under ``include/llvm/Obfuscation/``
- Put ``ECS.cpp`` under ``lib/Transforms/Obfuscation/``
- Modify ``include/llvm/Obfuscation/Obfuscation.h`` to include the new header
- Modify ``lib/Transforms/Obfuscation/Obfuscation.cpp``, at the beginning add:  

```
FunctionPass* ECSPass=createECSPass();
    for (Module::iterator iter = M.begin(); iter != M.end(); iter++) {
      Function &F = *iter;
      ECSPass->runOnFunction(F);
    }
    delete ECSPass;
```
- Modify ``lib/Transforms/Obfuscation/CMakeLists.txt``m add ``ECS.cpp`` right before ``DEPENDS``
