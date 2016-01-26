#include <asmjit/asmjit.h>

using namespace asmjit;

int main(int argc, char* argv[]) {
	// Create JitRuntime and X86 Assembler/Compiler.
	JitRuntime runtime;
	X86Assembler a(&runtime);
	X86Compiler c(&a);

	// Build function having two arguments and a return value of type 'int'.
	// First type in function builder describes the return value. kCallConvHost
	// tells the compiler to use the host calling convention.
	c.addFunc(FuncBuilder2<int, int, int>(kCallConvHost));

	// Create 32-bit variables (virtual registers) and assign some names to
	// them. Using variable names is not necessary, however, it can make
	// debugging easier.
	X86GpVar x = c.newInt32("x");
	X86GpVar y = c.newInt32("y");

	// Tell asmjit to use these variables as function arguments.
	c.setArg(0, x);
	c.setArg(1, y);

	// x = x + y;
	c.add(x, y);

	// Tell asmjit to return `x`.
	c.ret(x);

	// Finalize the current function.
	c.endFunc();

	// Now the Compiler contains the whole function, but the code is not yet
	// generated. To tell the compiler to serialize the code to `Assembler`
	// `c.finalize()` has to be called. After finalization the `Compiler`
	// won't contain the code anymore and will be detached from the `Assembler`.
	c.finalize();

	// After finalization the code has been send to `Assembler`. It contains
	// a handy method `make()`, which returns a pointer that points to the
	// first byte of the generated code, which is the function entry in our
	// case.
	void* funcPtr = a.make();

	// In order to run 'funcPtr' it has to be casted to the desired type.
	// Typedef is a recommended and safe way to create a function-type.
	typedef int(*FuncType)(int, int);

	// Using asmjit_cast is purely optional, it's basically a C-style cast
	// that tries to make it visible that a function-type is returned.
	FuncType func = asmjit_cast<FuncType>(funcPtr);

	// Finally, run it and do something with the result...
	int z = func(1, 2);
	printf("z=%d\n", z); // Outputs "z=3".

						 // The function will remain in memory after Compiler and Assembler are
						 // destroyed. This is why the `JitRuntime` is used - it keeps track of
						 // the code generated. When `Runtime` is destroyed it also invalidates
						 // all code relocated by it (which is in our case also our `func`). So
						 // it's safe to just do nothing in our case, because destroying `Runtime`
						 // will free `func` as well, however, it's always better to release the
						 // generated code that is not needed anymore manually.
	runtime.release((void*)func);

	return 0;
}