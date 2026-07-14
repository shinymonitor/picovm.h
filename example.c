#define PICOVM_IMPLEMENTATION
#include "picovm.h"

#include <stdio.h>
#include <stdlib.h>

#define INIT_CONTEXT_STACK_MEM_SIZE 32
#define INIT_CONTEXT_DATA_MEM_SIZE 32
#define INIT_CONTEXT_CALL_STACK_SIZE 16
#define INIT_CONTEXT_NATIVE_FUNCTIONS_CAPACITY 8
#define INIT_CONTEXT_FOREIGN_FUNCTIONS_CAPACITY 8
static inline PICOVM_Context init_context(
        PICOVM_Instruction* instructions, PICOVM_WORD instructions_count,
        uint8_t* data_memory, PICOVM_WORD data_memory_size
    ) {
    PICOVM_Context context = {0};
    context.instructions = instructions;
    context.instructions_count = instructions_count;
    context.stack_memory = (uint8_t*)calloc(INIT_CONTEXT_STACK_MEM_SIZE, 1);
    context.stack_memory_size = INIT_CONTEXT_STACK_MEM_SIZE;
    context.data_memory = data_memory;
    context.data_memory_size = data_memory_size;
    context.call_stack = (PICOVM_Call*)calloc(INIT_CONTEXT_CALL_STACK_SIZE, sizeof(PICOVM_Call));
    context.call_stack_capacity = INIT_CONTEXT_CALL_STACK_SIZE;
    context.native_functions = (PICOVM_NativeFunctionTable*)calloc(INIT_CONTEXT_NATIVE_FUNCTIONS_CAPACITY, sizeof(PICOVM_NativeFunctionTable));
    context.native_functions_capacity = INIT_CONTEXT_NATIVE_FUNCTIONS_CAPACITY;
    context.foreign_functions = (PICOVM_ForeignFunctionTable*)calloc(INIT_CONTEXT_FOREIGN_FUNCTIONS_CAPACITY, sizeof(PICOVM_ForeignFunctionTable)); 
    context.foreign_functions_capacity = INIT_CONTEXT_FOREIGN_FUNCTIONS_CAPACITY;
    return context;
}
static inline void free_context(PICOVM_Context* context){
    if (!context) return;
    if (context->stack_memory) free(context->stack_memory);
    if (context->call_stack) free(context->call_stack);
    if (context->native_functions) free(context->native_functions);
    if (context->foreign_functions) free(context->foreign_functions);
}


#define INSTRUCTIONS_SIZE 32
PICOVM_Instruction instructions[INSTRUCTIONS_SIZE] = {0};
size_t instructions_count = 0;
uint8_t data[] = {72, 69, 76, 76, 79, 32, 87, 79, 82, 76, 68, 10, 0};

static inline void print_context_memory(PICOVM_Context* context){
    printf("STACK["); for (size_t i = 0; i < context->stack_memory_size; ++i) printf("%02x ", context->stack_memory[i]); printf("]\n");
    printf("DATA["); for (size_t i = 0; i < context->data_memory_size; ++i) printf("%02x ", context->data_memory[i]); printf("]\n");
    if (!PICOVM_return_foreign_function(context, NULL, 0, 0)) printf("%s", "RETURN FOREIGN FUNCTION FAILED\n");
}
static inline void print_ascii_and_call_native(PICOVM_Context* context){
    uint8_t data_ptr = 0;
    if (!PICOVM_get_args(context, 0, 0, 1, (uint8_t*)&data_ptr)) printf("%s", "FOREIGN FUNCTION GET ARGS FAILED\n");
    printf("\x1b[0;31m");
    while (context->data_memory[data_ptr] != 0) {
        printf("%c", context->data_memory[data_ptr]);
        ++data_ptr;
    }
    printf("\x1b[0m");
    if (!PICOVM_call_native_function(context, 2, NULL, 0, NULL, 0)) printf("%s", "CALL NATIVE FUNCTION FAILED\n");
    if (!PICOVM_return_foreign_function(context, NULL, 0, 1)) printf("%s", "RETURN FOREIGN FUNCTION FAILED\n");
}

int main(){
    instructions[instructions_count++] = (PICOVM_Instruction){PICOVM_OP_FUNCTION, 2, 3};
    instructions[instructions_count++] = (PICOVM_Instruction){PICOVM_OP_EXTERN, 0, 0};
    instructions[instructions_count++] = (PICOVM_Instruction){PICOVM_OP_RETURN, 0, 0};
    
    instructions[instructions_count++] = (PICOVM_Instruction){PICOVM_OP_PUSH, 1, 0};
    instructions[instructions_count++] = (PICOVM_Instruction){PICOVM_OP_SETBSI, 0, 0};
    instructions[instructions_count++] = (PICOVM_Instruction){PICOVM_OP_EXTERN, 1, 1};
    
    instructions[instructions_count++] = (PICOVM_Instruction){PICOVM_OP_EXIT, 0, 0};

    PICOVM_Context context = init_context(instructions, instructions_count, data, sizeof(data)/sizeof(data[0]));

    PICOVM_register_foreign_function(&context, print_context_memory, 0);
    PICOVM_register_foreign_function(&context, print_ascii_and_call_native, 1);
    if (!PICOVM_prior(&context)) return 1;
    while (!context.finished) {
        if (!PICOVM_step(&context)) return 1;
    }
    free_context(&context);

    return 0;
}
