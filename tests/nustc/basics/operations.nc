namespace tests::lexer

def int Main() {
    
    int x = 20;

    int y = 20;

    int mult = x * y; //expected: 400

    int add = x + y; //expected: 40

    int sub = x - y; //expected: 0

    int div = x / y; //expected: 1

    if (mult != 400) {
        $__PRINT_A_STRING__("Error on Mult!")
        return 1;
    }
    if (add != 40) {
        $__PRINT_A_STRING__("Error on Add!")
        return 1;
    }
    if (sub != 0) {
        $__PRINT_A_STRING__("Error on Sub!")
        return 1;
    }
    if (div != 1) {
        $__PRINT_A_STRING__("Error on Div!")
        return 1;
    }

    return 0;
}
