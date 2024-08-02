#include "front-end/token.h"

void tk_print(Token *tk) {

    printf("Line:%lu<Type:%d, Value:%s, Value len:%lu>", tk->line, tk->type, tk->value, tk->value_s);
}

void tk_println(Token *tk) {

    tk_print(tk);
    puts("");
}
