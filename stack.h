#ifndef STACK_H
#define STACK_H


struct BirthInfo {
    const char* file;
    const char* func;
    const char* name;
    size_t line;
};

struct Stack_t {
    int* data;
    size_t size;
    size_t capacity;
    BirthInfo* info;
    int poison = 666;
};

#define INIT_STACK(stk, capacity) do { \
    BirthInfo info_got = {__FILE__, __func__, #stk, __LINE__}; \
    StackCtor(&stk, capacity * 2, &info_got); \
}while(0)

enum Stack_Err_t {
    Ok,
    Error
};


Stack_Err_t StackCtor(Stack_t* stk, size_t capacity, BirthInfo* info);
Stack_Err_t StackPush(Stack_t* stk, int element);
Stack_Err_t StackPop(Stack_t* stk);
Stack_Err_t StkErr(Stack_t* stk);
void StkDump(Stack_t* stk);
void StkDtor(Stack_t* stk);
void FillPoison(Stack_t* stk);


#endif
