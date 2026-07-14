# picovm.h

A single-header portable bytecode virtual machine built to be embeddable and fully sandboxed.

# Uses
- **Platform Independence**: System agnostic binaries like JVM.
- **Runtime Programming**: Game modding, live coding environment, etc.

# Why use picovm.h

- **Fast, Flexible, and Embeddable**: Simple bytecode, cache friendly interpreter, host defined context, single header.
- **Good C Interop**: Easy foreign (VM to C) and native (C to VM) function calls with full re-entry support.
- **Fully Sandboxed**: Load-time and run-time checks to prevent sandbox escape.
- **Fixed Memory Footprint**: Up front buffer allocation by host and no mallocs.
- **JIT and Compiler Codegen Targetable**: Small and uniform instruction set for easy compilation targeting

# Requirements

- Little-endian Host

