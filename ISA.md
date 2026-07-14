# ISA

### Operand types

- id = bind id of a native or foreign function
- frame = function frame size (may include return value and args setup by caller)
- body = size of a declared native function body (address_of_RETURN − address_of_FUNCTION + 1)
- reg = register id 
- val = immediate literal value, embedded in the instruction
- addr = immediate literal offset, embedded in the instruction (stack-relative or data-segment, depending on opcode)
- ptr = register id holding an address
- n = count of stack slots

### Opcode types

- B/W = operand size: Byte or Word
- S/D = location: Stack (frame-relative) or Data (data segment)
- P/I = addressing mode: Pointer (address from register) or Immediate (address in instruction)

## Meta
| Opcode | A | B | Description |
|--------|---|---|-------------|
| EXIT   | - | - | Finish      |

## Functions
|  Opcode  | A  |   B   |             Description                |
|----------|----|-------|-----------------------------------------|
|  EXTERN  | id | frame | Call a foreign function (Host/Syscall) |
| FUNCTION | id | body  |       Declare a native function        |
|  RETURN  | -  |   -   |        Return from call stack          |
|   CALL   | id | frame |        Call a native function          |

## Stack

| Opcode | A | B |         Description         |
|--------|---|---|-----------------------------|
|  PUSH  | n | - | Push n spaces to the stack  |
|  POP   | n | - | Pop n spaces from the stack |

## Move

Two location letters and two mode letters (source, then destination). 
On stack, addr and ptr are bounded by the current frame. 
Anything cross-frame must live in data memory

| Opcode    | A        | B        | Description                       |
|-----------|----------|----------|------------------------------------|
| MOVR      | src reg  | dst reg  | Copy register -> register           |
| MOVBSSPP  | src ptr  | dst ptr  | Copy byte: stack(ptr) -> stack(ptr) |
| MOVBSSPI  | src ptr  | dst addr | Copy byte: stack(ptr) -> stack(addr)|
| MOVBSSIP  | src addr | dst ptr  | Copy byte: stack(addr) -> stack(ptr)|
| MOVBSSII  | src addr | dst addr | Copy byte: stack(addr) -> stack(addr)|
| MOVWSSPP  | src ptr  | dst ptr  | Copy word: stack(ptr) -> stack(ptr) |
| MOVWSSPI  | src ptr  | dst addr | Copy word: stack(ptr) -> stack(addr)|
| MOVWSSIP  | src addr | dst ptr  | Copy word: stack(addr) -> stack(ptr)|
| MOVWSSII  | src addr | dst addr | Copy word: stack(addr) -> stack(addr)|
| MOVBSDPP  | src ptr  | dst ptr  | Copy byte: stack(ptr) -> data(ptr)  |
| MOVBSDPI  | src ptr  | dst addr | Copy byte: stack(ptr) -> data(addr) |
| MOVBSDIP  | src addr | dst ptr  | Copy byte: stack(addr) -> data(ptr) |
| MOVBSDII  | src addr | dst addr | Copy byte: stack(addr) -> data(addr)|
| MOVWSDPP  | src ptr  | dst ptr  | Copy word: stack(ptr) -> data(ptr)  |
| MOVWSDPI  | src ptr  | dst addr | Copy word: stack(ptr) -> data(addr) |
| MOVWSDIP  | src addr | dst ptr  | Copy word: stack(addr) -> data(ptr) |
| MOVWSDII  | src addr | dst addr | Copy word: stack(addr) -> data(addr)|
| MOVBDSPP  | src ptr  | dst ptr  | Copy byte: data(ptr) -> stack(ptr)  |
| MOVBDSPI  | src ptr  | dst addr | Copy byte: data(ptr) -> stack(addr) |
| MOVBDSIP  | src addr | dst ptr  | Copy byte: data(addr) -> stack(ptr) |
| MOVBDSII  | src addr | dst addr | Copy byte: data(addr) -> stack(addr)|
| MOVWDSPP  | src ptr  | dst ptr  | Copy word: data(ptr) -> stack(ptr)  |
| MOVWDSPI  | src ptr  | dst addr | Copy word: data(ptr) -> stack(addr) |
| MOVWDSIP  | src addr | dst ptr  | Copy word: data(addr) -> stack(ptr) |
| MOVWDSII  | src addr | dst addr | Copy word: data(addr) -> stack(addr)|
| MOVBDDPP  | src ptr  | dst ptr  | Copy byte: data(ptr) -> data(ptr)   |
| MOVBDDPI  | src ptr  | dst addr | Copy byte: data(ptr) -> data(addr)  |
| MOVBDDIP  | src addr | dst ptr  | Copy byte: data(addr) -> data(ptr)  |
| MOVBDDII  | src addr | dst addr | Copy byte: data(addr) -> data(addr) |
| MOVWDDPP  | src ptr  | dst ptr  | Copy word: data(ptr) -> data(ptr)   |
| MOVWDDPI  | src ptr  | dst addr | Copy word: data(ptr) -> data(addr)  |
| MOVWDDIP  | src addr | dst ptr  | Copy word: data(addr) -> data(ptr)  |
| MOVWDDII  | src addr | dst addr | Copy word: data(addr) -> data(addr) |

## Set, Store, Fetch

One location letter and one mode letter, one side is always a register.

| Opcode   | A       | B        | Description                          |
|----------|---------|----------|----------------------------------------|
| SETR     | val     | dst reg  | Set register: reg[B] = A               |
| SETBSP   | val     | ptr      | Set byte at stack(ptr) = A             |
| SETBSI   | val     | addr     | Set byte at stack(addr) = A            |
| SETBDP   | val     | ptr      | Set byte at data(ptr) = A              |
| SETBDI   | val     | addr     | Set byte at data(addr) = A             |
| SETWSP   | val     | ptr      | Set word at stack(ptr) = A             |
| SETWSI   | val     | addr     | Set word at stack(addr) = A            |
| SETWDP   | val     | ptr      | Set word at data(ptr) = A              |
| SETWDI   | val     | addr     | Set word at data(addr) = A             |
| STOREBSP | src reg | ptr      | Store byte reg[A] into stack(ptr)      |
| STOREBSI | src reg | addr     | Store byte reg[A] into stack(addr)     |
| STOREBDP | src reg | ptr      | Store byte reg[A] into data(ptr)       |
| STOREBDI | src reg | addr     | Store byte reg[A] into data(addr)      |
| STOREWSP | src reg | ptr      | Store word reg[A] into stack(ptr)      |
| STOREWSI | src reg | addr     | Store word reg[A] into stack(addr)     |
| STOREWDP | src reg | ptr      | Store word reg[A] into data(ptr)       |
| STOREWDI | src reg | addr     | Store word reg[A] into data(addr)      |
| FETCHBSP | ptr     | dst reg  | Fetch byte from stack(ptr) into reg[B] |
| FETCHBSI | addr    | dst reg  | Fetch byte from stack(addr) into reg[B]|
| FETCHBDP | ptr     | dst reg  | Fetch byte from data(ptr) into reg[B]  |
| FETCHBDI | addr    | dst reg  | Fetch byte from data(addr) into reg[B] |
| FETCHWSP | ptr     | dst reg  | Fetch word from stack(ptr) into reg[B] |
| FETCHWSI | addr    | dst reg  | Fetch word from stack(addr) into reg[B]|
| FETCHWDP | ptr     | dst reg  | Fetch word from data(ptr) into reg[B]  |
| FETCHWDI | addr    | dst reg  | Fetch word from data(addr) into reg[B] |

## Jumps

| Opcode | A          | B      | Description                                |
|--------|------------|--------|--------------------------------------------|
| JMP    | -          | target | Jump to B unconditionally                  |
| JMPG   | r          | target | Jump to B if reg[r] > 0 (signed)           |
| JMPL   | r          | target | Jump to B if reg[r] < 0 (signed)            |
| JMPNE  | r          | target | Jump to B if reg[r] != 0                   |
| JMPE   | r          | target | Jump to B if reg[r] == 0                   |

## Math

| Opcode | A       | B       | Description          |
|--------|---------|---------|----------------------|
| ADDR   | src reg | dst reg | reg[B] += reg[A]     |
| ADDI   | val     | dst reg | reg[B] += A          |
| SUBR   | src reg | dst reg | reg[B] -= reg[A]     |
| SUBI   | val     | dst reg | reg[B] -= A          |
| MULR   | src reg | dst reg | reg[B] *= reg[A]     |
| MULI   | val     | dst reg | reg[B] *= A          |
| DIVR   | src reg | dst reg | reg[B] /= reg[A] (signed)    |
| DIVI   | val     | dst reg | reg[B] /= A  (signed)        |
| MODR   | src reg | dst reg | reg[B] %= reg[A] (signed)    |
| MODI   | val     | dst reg | reg[B] %= A  (signed)        |

## Logic

| Opcode | A       | B       | Description         |
|--------|---------|---------|----------------------|
| ANDR   | src reg | dst reg | reg[B] &= reg[A]     |
| ANDI   | val     | dst reg | reg[B] &= A          |
| LORR   | src reg | dst reg | reg[B] \|= reg[A]    |
| LORI   | val     | dst reg | reg[B] \|= A         |
| XORR   | src reg | dst reg | reg[B] ^= reg[A]     |
| XORI   | val     | dst reg | reg[B] ^= A          |
| SHLR   | src reg | dst reg | reg[B] <<= reg[A]    |
| SHLI   | val     | dst reg | reg[B] <<= A         |
| SHRR   | src reg | dst reg | reg[B] >>= reg[A]    |
| SHRI   | val     | dst reg | reg[B] >>= A         |
| NOTR   | src reg | dst reg | reg[B] = ~reg[A]     |
| NEGR   | src reg | dst reg | reg[B] = -reg[A]     |
