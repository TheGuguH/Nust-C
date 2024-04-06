namespace std::exception

dictionary ExceptionType{
    CustomByProgramers,
    BufferUnderflow,
    BufferOverflow,
    InvalidCast,
    InvalidUse,
    InvalidAcess,
    UnsafeOnSafe,
    UnsafeOnSecurity,
    SafeOnSecurity,
}

struct Exception{
    string exception;
    string exceptionMessage;
    ExceptionType exceptionType;
    i4 line = -1; //only in debbuger
}

def StopExecution{
    $__EXIT_PROGRAM__(0)
}

def Exception CreateException(string exception, string exceptionMessage, ExceptionType){
    Exception ex = {
        .exception = exception;
        .exceptionMesage = exceptionMessage;
        .exceptionType = ExceptionType.CustomByProgramers;
    };
    return ex;
}

def TrownNewException(string exception, string message){
    Exception exception = CreateException(exception, message, ExceptionType.CustomByProgramers);

    $__TROWN_EXCEPTION__(exception)
}

def TrownNotANumberException{
    Exception exception = CreateException("Invalid use of a constant or variable", 
    "An invalid use of a constant or variable value is used in a number context", 
    ExceptionType.InvalidUse);

    $__TROWN_EXCEPTION__(exception)
}

def TrownUnsafeOnSecurity{
    Exception exception = CreateException("Unsafe on security context",
    "Use of a unsafe function in a security context",
    ExceptionType.UnsafeOnSecurity);

    $__TROWN_EXCEPTION__(exception)
}