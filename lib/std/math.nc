namespace std::math

use std::math_defines

public global const double PI = 3.141592653589793;

public global const double EULER = 2.718281828459045;

public global const double EULER_GAMMA = 0.577215664901532;

private global const double EPSILON = 0.000000000000001; //precision

def double Abs(double value) {
    return value < 0 ? -value : value;
}

def double Sqrt(double radicand) {
    mut double result = radicand;
    mut double prev = 0;

    while (Abs(prev - result) > EPSILON) {
        prev = result;
        result = (result + radicand / result) * 0.5;
    }

    return result;
}

def double Exp(double expoent) {
    //TODO: use Taylor Series
}
