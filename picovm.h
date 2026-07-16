#ifndef PICOVM_H_
#define PICOVM_H_

#ifndef PICOVM_REGISTERS
#define PICOVM_REGISTERS 8
#endif

#ifndef PICOVM_WORD_SIZE
#define PICOVM_WORD_SIZE 32
#endif

#if PICOVM_WORD_SIZE == 8
#define PICOVM_WORD uint8_t
#define PICOVM_SIGNED_WORD int8_t
#define PICOVM_SIGNED_WORD_MIN INT8_MIN
#elif PICOVM_WORD_SIZE == 16
#define PICOVM_WORD uint16_t
#define PICOVM_SIGNED_WORD int16_t
#define PICOVM_SIGNED_WORD_MIN INT16_MIN
#elif PICOVM_WORD_SIZE == 32
#define PICOVM_WORD uint32_t
#define PICOVM_SIGNED_WORD int32_t
#define PICOVM_SIGNED_WORD_MIN INT32_MIN
#else
#error "UNSUPPORTED PICOVM_WORD_SIZE"
#endif
#define PICOVM_WORD_BYTES ((PICOVM_WORD)sizeof(PICOVM_WORD))
#define PICOVM_SHIFT_MASK ((PICOVM_WORD)(PICOVM_WORD_SIZE - 1))

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    PICOVM_OP_EXIT = 0,
    
    PICOVM_OP_EXTERN, PICOVM_OP_FUNCTION, PICOVM_OP_RETURN, PICOVM_OP_CALL, 

    PICOVM_OP_PUSH, PICOVM_OP_POP,
    
    PICOVM_OP_MOVR,
    PICOVM_OP_MOVBSSPP, PICOVM_OP_MOVBSSPI, PICOVM_OP_MOVBSSIP, PICOVM_OP_MOVBSSII,
    PICOVM_OP_MOVWSSPP, PICOVM_OP_MOVWSSPI, PICOVM_OP_MOVWSSIP, PICOVM_OP_MOVWSSII, 
    PICOVM_OP_MOVBSDPP, PICOVM_OP_MOVBSDPI, PICOVM_OP_MOVBSDIP, PICOVM_OP_MOVBSDII,
    PICOVM_OP_MOVWSDPP, PICOVM_OP_MOVWSDPI, PICOVM_OP_MOVWSDIP, PICOVM_OP_MOVWSDII,  
    PICOVM_OP_MOVBDSPP, PICOVM_OP_MOVBDSPI, PICOVM_OP_MOVBDSIP, PICOVM_OP_MOVBDSII,
    PICOVM_OP_MOVWDSPP, PICOVM_OP_MOVWDSPI, PICOVM_OP_MOVWDSIP, PICOVM_OP_MOVWDSII, 
    PICOVM_OP_MOVBDDPP, PICOVM_OP_MOVBDDPI, PICOVM_OP_MOVBDDIP, PICOVM_OP_MOVBDDII,
    PICOVM_OP_MOVWDDPP, PICOVM_OP_MOVWDDPI, PICOVM_OP_MOVWDDIP, PICOVM_OP_MOVWDDII, 

    PICOVM_OP_SETR, 
    PICOVM_OP_SETBSP, PICOVM_OP_SETBSI, PICOVM_OP_SETBDP, PICOVM_OP_SETBDI,
    PICOVM_OP_SETWSP, PICOVM_OP_SETWSI, PICOVM_OP_SETWDP, PICOVM_OP_SETWDI,

    PICOVM_OP_STOREBSP, PICOVM_OP_STOREBSI, PICOVM_OP_STOREBDP, PICOVM_OP_STOREBDI,
    PICOVM_OP_STOREWSP, PICOVM_OP_STOREWSI, PICOVM_OP_STOREWDP, PICOVM_OP_STOREWDI,

    PICOVM_OP_FETCHBSP, PICOVM_OP_FETCHBSI, PICOVM_OP_FETCHBDP, PICOVM_OP_FETCHBDI,
    PICOVM_OP_FETCHWSP, PICOVM_OP_FETCHWSI, PICOVM_OP_FETCHWDP, PICOVM_OP_FETCHWDI,
    
    PICOVM_OP_JMP, PICOVM_OP_JMPG, PICOVM_OP_JMPL, PICOVM_OP_JMPNE, PICOVM_OP_JMPE,

    PICOVM_OP_ADDR, PICOVM_OP_ADDI, PICOVM_OP_SUBR, PICOVM_OP_SUBI, PICOVM_OP_MULR, PICOVM_OP_MULI, 
    PICOVM_OP_DIVR, PICOVM_OP_DIVI, PICOVM_OP_MODR, PICOVM_OP_MODI,

    PICOVM_OP_ANDR, PICOVM_OP_ANDI, PICOVM_OP_LORR, PICOVM_OP_LORI, PICOVM_OP_XORR, PICOVM_OP_XORI, 
    PICOVM_OP_SHLR, PICOVM_OP_SHLI, PICOVM_OP_SHRR, PICOVM_OP_SHRI, 

    PICOVM_OP_NOTR, PICOVM_OP_NEGR,
} PICOVM_Opcode;

typedef struct {
    uint8_t opcode;
    PICOVM_WORD a, b;
} PICOVM_Instruction;

typedef struct {PICOVM_WORD return_address, base_pointer;} PICOVM_Call;
typedef struct PICOVM_Context PICOVM_Context;
typedef void (*PICOVM_ForeignFunction)(PICOVM_Context*);
typedef struct {bool registered; PICOVM_WORD location;} PICOVM_NativeFunctionTable;
typedef struct {bool registered; PICOVM_ForeignFunction foreign_function;} PICOVM_ForeignFunctionTable;

struct PICOVM_Context {
    bool finished;

    PICOVM_WORD pc;
    PICOVM_Instruction* instructions;
    PICOVM_WORD instructions_count;

    PICOVM_WORD registers[PICOVM_REGISTERS];

    uint8_t* stack_memory;
    PICOVM_WORD stack_memory_size;
    PICOVM_WORD base_pointer;
    PICOVM_WORD stack_pointer;

    uint8_t* data_memory;
    PICOVM_WORD data_memory_size;

    PICOVM_Call* call_stack;
    PICOVM_WORD call_stack_capacity;
    PICOVM_WORD call_stack_count;

    PICOVM_NativeFunctionTable* native_functions;
    PICOVM_WORD native_functions_capacity;

    PICOVM_ForeignFunctionTable* foreign_functions;
    PICOVM_WORD foreign_functions_capacity;
};

#ifndef PICOVM_DEF
#define PICOVM_DEF static inline
#endif

PICOVM_DEF bool PICOVM_prior(PICOVM_Context* context);
PICOVM_DEF bool PICOVM_step(PICOVM_Context* context);
PICOVM_DEF bool PICOVM_register_foreign_function(PICOVM_Context* context, PICOVM_ForeignFunction foreign_function, PICOVM_WORD foreign_function_id);
PICOVM_DEF bool PICOVM_call_native_function(PICOVM_Context* context, PICOVM_WORD native_function_id, uint8_t* args, PICOVM_WORD args_size, uint8_t* return_value, PICOVM_WORD return_value_size);
PICOVM_DEF bool PICOVM_get_args(PICOVM_Context* context, PICOVM_WORD return_value_size, PICOVM_WORD offset, PICOVM_WORD args_size, uint8_t* args);
PICOVM_DEF bool PICOVM_return_foreign_function(PICOVM_Context* context, uint8_t* return_value, PICOVM_WORD return_value_size, PICOVM_WORD args_size);
PICOVM_DEF bool PICOVM_get_data(PICOVM_Context* context, PICOVM_WORD offset, PICOVM_WORD data_size, uint8_t* data);
PICOVM_DEF bool PICOVM_set_data(PICOVM_Context* context, uint8_t* data, PICOVM_WORD data_size, PICOVM_WORD offset);

#ifdef PICOVM_IMPLEMENTATION

static inline bool PICOVM__prior_verify_operand(PICOVM_Context* context, bool is_data, bool is_ptr, bool is_word, PICOVM_WORD operand) {
    if (is_ptr) {
        if (operand >= PICOVM_REGISTERS) return false;
        return true;
    }
    if (!is_data) return true;
    else if (operand >= context->data_memory_size || (is_word ? PICOVM_WORD_BYTES : 1) > context->data_memory_size - operand) return false;
    return true;
}

static inline bool PICOVM__step_resolve_operand(PICOVM_Context* context, bool is_data, bool is_ptr, bool is_word, PICOVM_WORD operand, uint8_t** segment_ptr) {
    PICOVM_WORD offset;
    uint8_t* segment = is_data ? context->data_memory : context->stack_memory;
    PICOVM_WORD size = is_word ? PICOVM_WORD_BYTES : 1;
    PICOVM_WORD frame_size = context->stack_pointer - context->base_pointer;
    if (is_ptr) {
        offset = context->registers[operand];
        if (is_data) {
            if (offset > context->data_memory_size || size > context->data_memory_size - offset) return false;
        } 
        else {
            if (offset > frame_size || size > frame_size - offset) return false;
            offset += context->base_pointer;
        }
    }
    else if (!is_data) {
        if (operand > frame_size || size > frame_size - operand) return false;
        offset = context->base_pointer + operand;
    } 
    else offset = operand;
    *segment_ptr = segment + offset;
    return true;
}

PICOVM_DEF bool PICOVM_prior(PICOVM_Context* context) {
    PICOVM_Instruction instruction = {0};
    bool in_function = false;
    PICOVM_WORD function_length = 0;
    PICOVM_WORD function_declared_length = 0;
    context->pc = 0;
    while (context->pc < context->instructions_count) {
        instruction = context->instructions[context->pc];
        switch (instruction.opcode) {
        case PICOVM_OP_FUNCTION:
            if (in_function || instruction.a >= context->native_functions_capacity || context->native_functions[instruction.a].registered) return false;
            context->native_functions[instruction.a].registered = true;
            context->native_functions[instruction.a].location = context->pc + 1;
            in_function = true;
            function_length = 0;
            function_declared_length = instruction.b;
        break;
        case PICOVM_OP_RETURN:
            if (!in_function || function_length + 1 != function_declared_length) return false;
            ++function_length;
            in_function = false;
        break;
        default: break;
        }
        if (in_function) ++function_length;
        ++context->pc;
    }
    if (in_function) return false;

    context->pc = 0;
    while (context->pc < context->instructions_count) {
        instruction = context->instructions[context->pc];
        switch (instruction.opcode) {
        case PICOVM_OP_EXTERN:
            if (instruction.a >= context->foreign_functions_capacity || !(context->foreign_functions[instruction.a].registered)) return false;
        break;
        case PICOVM_OP_CALL:
            if (instruction.a >= context->native_functions_capacity || !(context->native_functions[instruction.a].registered)) return false;
        break;

        case PICOVM_OP_MOVR:
        case PICOVM_OP_ADDR: case PICOVM_OP_SUBR: case PICOVM_OP_MULR: case PICOVM_OP_DIVR: case PICOVM_OP_MODR:
        case PICOVM_OP_ANDR: case PICOVM_OP_LORR: case PICOVM_OP_XORR: case PICOVM_OP_SHLR: case PICOVM_OP_SHRR:
        case PICOVM_OP_NOTR: case PICOVM_OP_NEGR:
            if (instruction.a >= PICOVM_REGISTERS || instruction.b >= PICOVM_REGISTERS) return false;
        break;
        case PICOVM_OP_SETR:
        case PICOVM_OP_ADDI: case PICOVM_OP_SUBI: case PICOVM_OP_MULI:
        case PICOVM_OP_ANDI: case PICOVM_OP_LORI: case PICOVM_OP_XORI: case PICOVM_OP_SHLI: case PICOVM_OP_SHRI:
            if (instruction.b >= PICOVM_REGISTERS) return false;
        break;
        case PICOVM_OP_DIVI: case PICOVM_OP_MODI:
            if (instruction.b >= PICOVM_REGISTERS || instruction.a == 0) return false;
        break;

        case PICOVM_OP_JMP: 
            if (instruction.b >= context->instructions_count) return false;
        break;
        case PICOVM_OP_JMPG: case PICOVM_OP_JMPL: case PICOVM_OP_JMPNE: case PICOVM_OP_JMPE:
            if (instruction.a >= PICOVM_REGISTERS || instruction.b >= context->instructions_count) return false;
        break;

        case PICOVM_OP_MOVBSSPP: case PICOVM_OP_MOVBSSPI: case PICOVM_OP_MOVBSSIP: case PICOVM_OP_MOVBSSII:
        case PICOVM_OP_MOVWSSPP: case PICOVM_OP_MOVWSSPI: case PICOVM_OP_MOVWSSIP: case PICOVM_OP_MOVWSSII:
        case PICOVM_OP_MOVBSDPP: case PICOVM_OP_MOVBSDPI: case PICOVM_OP_MOVBSDIP: case PICOVM_OP_MOVBSDII:
        case PICOVM_OP_MOVWSDPP: case PICOVM_OP_MOVWSDPI: case PICOVM_OP_MOVWSDIP: case PICOVM_OP_MOVWSDII:
        case PICOVM_OP_MOVBDSPP: case PICOVM_OP_MOVBDSPI: case PICOVM_OP_MOVBDSIP: case PICOVM_OP_MOVBDSII:
        case PICOVM_OP_MOVWDSPP: case PICOVM_OP_MOVWDSPI: case PICOVM_OP_MOVWDSIP: case PICOVM_OP_MOVWDSII:
        case PICOVM_OP_MOVBDDPP: case PICOVM_OP_MOVBDDPI: case PICOVM_OP_MOVBDDIP: case PICOVM_OP_MOVBDDII:
        case PICOVM_OP_MOVWDDPP: case PICOVM_OP_MOVWDDPI: case PICOVM_OP_MOVWDDIP: case PICOVM_OP_MOVWDDII: {
            PICOVM_WORD flags = instruction.opcode - PICOVM_OP_MOVBSSPP;
            bool src_is_data = flags & 0x10, dst_is_data = flags & 0x08, is_word = flags & 0x04;
            bool src_is_ptr  = !(flags & 0x02), dst_is_ptr  = !(flags & 0x01);
            if (!PICOVM__prior_verify_operand(context, src_is_data, src_is_ptr, is_word, instruction.a) || !PICOVM__prior_verify_operand(context, dst_is_data, dst_is_ptr, is_word, instruction.b)) return false;
        } break;

        case PICOVM_OP_SETBSP: case PICOVM_OP_SETBSI: case PICOVM_OP_SETBDP: case PICOVM_OP_SETBDI:
        case PICOVM_OP_SETWSP: case PICOVM_OP_SETWSI: case PICOVM_OP_SETWDP: case PICOVM_OP_SETWDI: {
            PICOVM_WORD flags = instruction.opcode - PICOVM_OP_SETBSP;
            bool is_word = flags & 0x04, is_data = flags & 0x02, is_ptr = !(flags & 0x01);
            if (!PICOVM__prior_verify_operand(context, is_data, is_ptr, is_word, instruction.b)) return false;
        } break;
        case PICOVM_OP_STOREBSP: case PICOVM_OP_STOREBSI: case PICOVM_OP_STOREBDP: case PICOVM_OP_STOREBDI:
        case PICOVM_OP_STOREWSP: case PICOVM_OP_STOREWSI: case PICOVM_OP_STOREWDP: case PICOVM_OP_STOREWDI: {
            PICOVM_WORD flags = instruction.opcode - PICOVM_OP_STOREBSP;
            bool is_word = flags & 0x04, is_data = flags & 0x02, is_ptr = !(flags & 0x01);
            if (instruction.a >= PICOVM_REGISTERS || !PICOVM__prior_verify_operand(context, is_data, is_ptr, is_word, instruction.b)) return false;
        } break;
        case PICOVM_OP_FETCHBSP: case PICOVM_OP_FETCHBSI: case PICOVM_OP_FETCHBDP: case PICOVM_OP_FETCHBDI:
        case PICOVM_OP_FETCHWSP: case PICOVM_OP_FETCHWSI: case PICOVM_OP_FETCHWDP: case PICOVM_OP_FETCHWDI: {
            PICOVM_WORD flags = instruction.opcode - PICOVM_OP_FETCHBSP;
            bool is_word = flags & 0x04, is_data = flags & 0x02, is_ptr = !(flags & 0x01);
            if (!PICOVM__prior_verify_operand(context, is_data, is_ptr, is_word, instruction.a) || instruction.b >= PICOVM_REGISTERS) return false;
        } break;

        default: break;
        }
        ++context->pc;
    }
    context->pc = 0;
    return true;
}

PICOVM_DEF bool PICOVM_step(PICOVM_Context* context) {
    PICOVM_Instruction instruction = {0};
    if (context->pc >= context->instructions_count) return false;
    instruction = context->instructions[context->pc];
    switch (instruction.opcode) {
    case PICOVM_OP_EXIT:
        context->finished = true;
    break;

    case PICOVM_OP_EXTERN: {
        if (context->call_stack_count == context->call_stack_capacity || instruction.b > context->stack_pointer - context->base_pointer) return false;
        PICOVM_WORD call_stack_count_when_called = context->call_stack_count;
        context->call_stack[context->call_stack_count++] = (PICOVM_Call){context->pc + 1, context->base_pointer};
        context->base_pointer = context->stack_pointer - instruction.b;
        context->foreign_functions[instruction.a].foreign_function(context);
        if (context->call_stack_count != call_stack_count_when_called) return false;
    } break;
    case PICOVM_OP_FUNCTION:
        context->pc += instruction.b;
    break;
    case PICOVM_OP_RETURN: {
        if (context->call_stack_count == 0) return false;
        PICOVM_Call last_call = context->call_stack[--context->call_stack_count];
        context->pc = last_call.return_address;
        context->base_pointer = last_call.base_pointer;
    } break;
    case PICOVM_OP_CALL:
        if (context->call_stack_count == context->call_stack_capacity || instruction.b > context->stack_pointer - context->base_pointer) return false;
        context->call_stack[context->call_stack_count++] = (PICOVM_Call){context->pc + 1, context->base_pointer};
        context->base_pointer = context->stack_pointer - instruction.b;
        context->pc = context->native_functions[instruction.a].location;
    break;

    case PICOVM_OP_PUSH:
        if (instruction.a > context->stack_memory_size - context->stack_pointer) return false;
        context->stack_pointer += instruction.a;
        ++context->pc;
    break;
    case PICOVM_OP_POP:
        if (instruction.a > context->stack_pointer - context->base_pointer) return false;
        context->stack_pointer -= instruction.a;
        ++context->pc;
    break;

    case PICOVM_OP_MOVR:
        context->registers[instruction.b] = context->registers[instruction.a];
        ++context->pc;
    break;
    case PICOVM_OP_MOVBSSPP: case PICOVM_OP_MOVBSSPI: case PICOVM_OP_MOVBSSIP: case PICOVM_OP_MOVBSSII:
    case PICOVM_OP_MOVWSSPP: case PICOVM_OP_MOVWSSPI: case PICOVM_OP_MOVWSSIP: case PICOVM_OP_MOVWSSII:
    case PICOVM_OP_MOVBSDPP: case PICOVM_OP_MOVBSDPI: case PICOVM_OP_MOVBSDIP: case PICOVM_OP_MOVBSDII:
    case PICOVM_OP_MOVWSDPP: case PICOVM_OP_MOVWSDPI: case PICOVM_OP_MOVWSDIP: case PICOVM_OP_MOVWSDII:
    case PICOVM_OP_MOVBDSPP: case PICOVM_OP_MOVBDSPI: case PICOVM_OP_MOVBDSIP: case PICOVM_OP_MOVBDSII:
    case PICOVM_OP_MOVWDSPP: case PICOVM_OP_MOVWDSPI: case PICOVM_OP_MOVWDSIP: case PICOVM_OP_MOVWDSII:
    case PICOVM_OP_MOVBDDPP: case PICOVM_OP_MOVBDDPI: case PICOVM_OP_MOVBDDIP: case PICOVM_OP_MOVBDDII:
    case PICOVM_OP_MOVWDDPP: case PICOVM_OP_MOVWDDPI: case PICOVM_OP_MOVWDDIP: case PICOVM_OP_MOVWDDII: {
        PICOVM_WORD flags = instruction.opcode - PICOVM_OP_MOVBSSPP;
        bool src_is_data = flags & 0x10, dst_is_data = flags & 0x08, is_word = flags & 0x04;
        bool src_is_ptr  = !(flags & 0x02), dst_is_ptr  = !(flags & 0x01);
        uint8_t *src, *dst;
        if (!PICOVM__step_resolve_operand(context, src_is_data, src_is_ptr, is_word, instruction.a, &src) || !PICOVM__step_resolve_operand(context, dst_is_data, dst_is_ptr, is_word, instruction.b, &dst)) return false;
        memcpy(dst, src, is_word ? PICOVM_WORD_BYTES : 1);
        ++context->pc;
    } break;

    case PICOVM_OP_SETR:
        context->registers[instruction.b] = instruction.a;
        ++context->pc;
    break;
    case PICOVM_OP_SETBSP: case PICOVM_OP_SETBSI: case PICOVM_OP_SETBDP: case PICOVM_OP_SETBDI:
    case PICOVM_OP_SETWSP: case PICOVM_OP_SETWSI: case PICOVM_OP_SETWDP: case PICOVM_OP_SETWDI: {
        PICOVM_WORD flags = instruction.opcode - PICOVM_OP_SETBSP;
        bool is_word = flags & 0x04, is_data = flags & 0x02, is_ptr = !(flags & 0x01);
        uint8_t* dst;
        if (!PICOVM__step_resolve_operand(context, is_data, is_ptr, is_word, instruction.b, &dst)) return false;
        memcpy(dst, &instruction.a, is_word ? PICOVM_WORD_BYTES : 1);
        ++context->pc;
    } break;

    case PICOVM_OP_STOREBSP: case PICOVM_OP_STOREBSI: case PICOVM_OP_STOREBDP: case PICOVM_OP_STOREBDI:
    case PICOVM_OP_STOREWSP: case PICOVM_OP_STOREWSI: case PICOVM_OP_STOREWDP: case PICOVM_OP_STOREWDI: {
        PICOVM_WORD flags = instruction.opcode - PICOVM_OP_STOREBSP;
        bool is_word = flags & 0x04, is_data = flags & 0x02, is_ptr = !(flags & 0x01);
        uint8_t* dst;
        if (!PICOVM__step_resolve_operand(context, is_data, is_ptr, is_word, instruction.b, &dst)) return false;
        memcpy(dst, &context->registers[instruction.a], is_word ? PICOVM_WORD_BYTES : 1);
        ++context->pc;
    } break;
    case PICOVM_OP_FETCHBSP: case PICOVM_OP_FETCHBSI: case PICOVM_OP_FETCHBDP: case PICOVM_OP_FETCHBDI:
    case PICOVM_OP_FETCHWSP: case PICOVM_OP_FETCHWSI: case PICOVM_OP_FETCHWDP: case PICOVM_OP_FETCHWDI: {
        PICOVM_WORD flags = instruction.opcode - PICOVM_OP_FETCHBSP;
        bool is_word = flags & 0x04, is_data = flags & 0x02, is_ptr = !(flags & 0x01);
        uint8_t* src;
        if (!PICOVM__step_resolve_operand(context, is_data, is_ptr, is_word, instruction.a, &src)) return false;
        context->registers[instruction.b] = 0;
        memcpy(&context->registers[instruction.b], src, is_word ? PICOVM_WORD_BYTES : 1);
        ++context->pc;
    } break;

    case PICOVM_OP_JMP:
        context->pc = instruction.b;
    break;
    case PICOVM_OP_JMPG: case PICOVM_OP_JMPL: case PICOVM_OP_JMPNE: case PICOVM_OP_JMPE: {
        PICOVM_WORD reg = context->registers[instruction.a];
        bool take;
        switch (instruction.opcode) {
        case PICOVM_OP_JMPG:  take = (PICOVM_SIGNED_WORD)reg > 0; break;
        case PICOVM_OP_JMPL:  take = (PICOVM_SIGNED_WORD)reg < 0; break;
        case PICOVM_OP_JMPNE: take = reg != 0; break;
        case PICOVM_OP_JMPE:  take = reg == 0; break;
        default: return false;
        }
        context->pc = take ? instruction.b : context->pc + 1;
    } break;

    case PICOVM_OP_ADDR: context->registers[instruction.b] += context->registers[instruction.a]; ++context->pc; break;
    case PICOVM_OP_ADDI: context->registers[instruction.b] += instruction.a; ++context->pc; break;
    case PICOVM_OP_SUBR: context->registers[instruction.b] -= context->registers[instruction.a]; ++context->pc; break;
    case PICOVM_OP_SUBI: context->registers[instruction.b] -= instruction.a; ++context->pc; break;
    case PICOVM_OP_MULR: context->registers[instruction.b] *= context->registers[instruction.a]; ++context->pc; break;
    case PICOVM_OP_MULI: context->registers[instruction.b] *= instruction.a; ++context->pc; break;
    case PICOVM_OP_DIVR: case PICOVM_OP_MODR: {
        PICOVM_SIGNED_WORD divisor = (PICOVM_SIGNED_WORD)context->registers[instruction.a];
        if (divisor == 0 || (divisor == -1 && (PICOVM_SIGNED_WORD)context->registers[instruction.b] == PICOVM_SIGNED_WORD_MIN)) return false;
        context->registers[instruction.b] = (PICOVM_WORD)(instruction.opcode == PICOVM_OP_DIVR ? (PICOVM_SIGNED_WORD)context->registers[instruction.b] / divisor : (PICOVM_SIGNED_WORD)context->registers[instruction.b] % divisor);
        ++context->pc;
    } break;
    case PICOVM_OP_DIVI: case PICOVM_OP_MODI: {
        PICOVM_SIGNED_WORD divisor = (PICOVM_SIGNED_WORD)instruction.a;
        if (divisor == 0 || (divisor == -1 && (PICOVM_SIGNED_WORD)context->registers[instruction.b] == PICOVM_SIGNED_WORD_MIN)) return false;
        context->registers[instruction.b] = (PICOVM_WORD)(instruction.opcode == PICOVM_OP_DIVI ? (PICOVM_SIGNED_WORD)context->registers[instruction.b] / divisor : (PICOVM_SIGNED_WORD)context->registers[instruction.b] % divisor);
        ++context->pc;
    } break;

    case PICOVM_OP_ANDR: context->registers[instruction.b] &= context->registers[instruction.a]; ++context->pc; break;
    case PICOVM_OP_ANDI: context->registers[instruction.b] &= instruction.a; ++context->pc; break;
    case PICOVM_OP_LORR: context->registers[instruction.b] |= context->registers[instruction.a]; ++context->pc; break;
    case PICOVM_OP_LORI: context->registers[instruction.b] |= instruction.a; ++context->pc; break;
    case PICOVM_OP_XORR: context->registers[instruction.b] ^= context->registers[instruction.a]; ++context->pc; break;
    case PICOVM_OP_XORI: context->registers[instruction.b] ^= instruction.a; ++context->pc; break;
    case PICOVM_OP_SHLR: context->registers[instruction.b] <<= context->registers[instruction.a] & PICOVM_SHIFT_MASK; ++context->pc; break;
    case PICOVM_OP_SHLI: context->registers[instruction.b] <<= instruction.a & PICOVM_SHIFT_MASK; ++context->pc; break;
    case PICOVM_OP_SHRR: context->registers[instruction.b] >>= context->registers[instruction.a] & PICOVM_SHIFT_MASK; ++context->pc; break;
    case PICOVM_OP_SHRI: context->registers[instruction.b] >>= instruction.a & PICOVM_SHIFT_MASK; ++context->pc; break;

    case PICOVM_OP_NOTR: context->registers[instruction.b] = ~context->registers[instruction.a]; ++context->pc; break;
    case PICOVM_OP_NEGR: context->registers[instruction.b] = -context->registers[instruction.a]; ++context->pc; break;

    default:
        return false;
    break;
    }
    return true;
}

PICOVM_DEF bool PICOVM_register_foreign_function(PICOVM_Context* context, PICOVM_ForeignFunction foreign_function, PICOVM_WORD foreign_function_id) {
    if (foreign_function_id >= context->foreign_functions_capacity || context->foreign_functions[foreign_function_id].registered) return false;
    context->foreign_functions[foreign_function_id].registered = true;
    context->foreign_functions[foreign_function_id].foreign_function = foreign_function;
    return true;
}

PICOVM_DEF bool PICOVM_call_native_function(PICOVM_Context* context, PICOVM_WORD native_function_id, uint8_t* args, PICOVM_WORD args_size, uint8_t* return_value, PICOVM_WORD return_value_size) {
    if (context->call_stack_count == context->call_stack_capacity || return_value_size > context->stack_memory_size - context->stack_pointer || args_size > context->stack_memory_size - context->stack_pointer - return_value_size || native_function_id >= context->native_functions_capacity || !(context->native_functions[native_function_id].registered)) return false;
    PICOVM_WORD stack_pointer_when_called = context->stack_pointer;
    PICOVM_WORD call_stack_when_called = context->call_stack_count;    
    context->stack_pointer += return_value_size;
    if (args_size > 0 && args != NULL) memcpy(context->stack_memory + context->stack_pointer, args, args_size);
    context->stack_pointer += args_size;
    context->call_stack[context->call_stack_count++] = (PICOVM_Call){context->pc + 1, context->base_pointer};
    context->base_pointer = context->stack_pointer - (return_value_size + args_size);
    context->pc = context->native_functions[native_function_id].location;
    while((context->call_stack_count) != call_stack_when_called && !(context->finished)) if (!PICOVM_step(context)) return false;
    if (return_value_size > 0 && return_value != NULL) memcpy(return_value, context->stack_memory + stack_pointer_when_called, return_value_size);
    context->stack_pointer = stack_pointer_when_called;
    return true;
}

PICOVM_DEF bool PICOVM_get_args(PICOVM_Context* context, PICOVM_WORD return_value_size, PICOVM_WORD offset, PICOVM_WORD args_size, uint8_t* args) {
    if (context->call_stack_count == 0 || return_value_size > context->stack_pointer - context->base_pointer || offset > context->stack_pointer - context->base_pointer - return_value_size || args_size > context->stack_pointer - context->base_pointer - return_value_size - offset) return false;
    memcpy(args, context->stack_memory + context->base_pointer + return_value_size + offset, args_size);
    return true;
}
PICOVM_DEF bool PICOVM_return_foreign_function(PICOVM_Context* context, uint8_t* return_value, PICOVM_WORD return_value_size, PICOVM_WORD args_size) {
    if (context->call_stack_count == 0 || return_value_size > context->stack_pointer - context->base_pointer || args_size > context->stack_pointer - context->base_pointer - return_value_size) return false;
    context->stack_pointer -= args_size;
    if (return_value_size > 0 && return_value != NULL) memcpy(context->stack_memory + context->base_pointer, return_value, return_value_size);
    PICOVM_Call last_call = context->call_stack[--context->call_stack_count];
    context->pc = last_call.return_address;
    context->base_pointer = last_call.base_pointer;
    return true;
}

PICOVM_DEF bool PICOVM_get_data(PICOVM_Context* context, PICOVM_WORD offset, PICOVM_WORD data_size, uint8_t* data) {
    if (offset > context->data_memory_size || data_size > context->data_memory_size - offset) return false;
    memcpy(data, context->data_memory + offset, data_size);
    return true;
}
PICOVM_DEF bool PICOVM_set_data(PICOVM_Context* context, uint8_t* data, PICOVM_WORD data_size, PICOVM_WORD offset) {
    if (offset > context->data_memory_size || data_size > context->data_memory_size - offset) return false;
    memcpy(context->data_memory + offset, data, data_size);
    return true;
}

#endif // PICOVM_IMPLEMENTATION

#endif // PICOVM_H_
