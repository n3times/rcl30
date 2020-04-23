#include "common.h"

static void test_number_edit() {
    test("", "PENTATRONICS");
    test("1", "1");
    test("12", "12");
    test("1234567890", "1234567890");
    test("12345678901", "1234567890");
    test("12345678901~", "-1234567890");
    test("12345678901~~", "1234567890");
    test("12345678901~~c", "READY");
    test("~2.3456789011", "-2.345678901");
    test("~2.3456789011%", "-2.345678901%");
    test("~2.3456789011%%", "-2.345678901");
    test(".", "0.");
    test("01", "1");
}

static void test_operation_edit() {
    test("1+1", "1+1");
    test("1+1+1+1+1+1+", "1+1+1+1+1+1+");
    test("1+1+1+1+1+1+1", "+1+1+1+1+1+1");
    test("1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1",
         "+1+1+1+1+1+1");
    test("1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+",
         "OUT OF MEM");
    test("1+*", "1X");
    test("*", "READY");
    test("1+~", "1+-");
    test("1+~1", "1+-1");
    test("1+~1~", "1+1");
    test("1234567890+1234567890+", "+1234567890+");
    test("1234567890+1234567890+c", "0+1234567890");
    test("1234567890+1234567890+cc", "1234567890+");
    test("1234567890+1234567890+ccc", "1234567890");
    test("1234567890+1234567890+cccc", "READY");
    test(".1+.1+.1+.1+.1", "+0.1+0.1+0.1+0.1");
}

static void test_arithmetic() {
    test("1+1=", "2");
    test("1+2*3=", "7");
    test("1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1=",
         "100");
    test("3-5=", "-2");
    test("9999999999*9999999999*9999999999*9999999999*9999999999*"
         "9999999999*9999999999*9999999999*9999999999*9999999000=",
         "9.999999e+99");
    test("9999999999*9999999999*9999999999*9999999999*9999999999*"
         "9999999999*9999999999*9999999999*9999999999*9999999999=",
         "OVERFLOW");
    test("10/2=", "5");
    test("30/2/5=", "3");
    test("1/0=", "DIV BY ZERO");
    test("0/0=", "DIV BY ZERO");
}

static void test_percent() {
    test("10%=", "10%");
    test("10%+10%=", "20%");
    test("10+10%=", "11");
    test("10+10%+10%=", "12.1");
}

void test_calc() {
    test_number_edit();
    test_operation_edit();
    test_arithmetic();
    test_percent();
}
