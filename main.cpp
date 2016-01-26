#include <asmjit/asmjit.h>

using namespace asmjit;
using namespace asmjit::x86;

int main(int argc, char* argv[]) {
	// Create JitRuntime and X86 Assembler/Compiler.
	JitRuntime runtime;
	X86Assembler a(&runtime);

	const int arg_offset = 8;
	const int arg_size = 12;

	Label L_Loop(a);

	// Prolog
	a.push(ebp);
	a.mov(ebp, esp);
	a.push(esi);
	a.push(edi);

	// Fetch arguments
	a.mov(esi, dword_ptr(ebp, arg_offset + 0));
	a.mov(esi, dword_ptr(ebp, arg_offset + 4));
	a.mov(ecx, dword_ptr(ebp, arg_offset + 8));

	// Bind L_Loop label to here
	a.bind(L_Loop);

	// Copy 4 bytes
	a.mov(eax, dword_ptr(esi));
	a.mov(dword_ptr(edi), eax);

	// Increment pointers
	a.add(esi, 4);
	a.add(edi, 4);

	// Repeat Loop until (--ecx != 0)
	a.dec(ecx);
	a.jz(L_Loop);

	// Epilog
	a.pop(edi);
	a.pop(esi);
	a.mov(esp, ebp);
	a.pop(ebp);

	// Return: Pop the stack by 'arg_size' as defined by 'STDCALL' convention
	a.ret(arg_size);

	return 0;
}