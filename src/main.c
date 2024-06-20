#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

char input[100];
int pos = 0;
int indent = 0;

void print_indent() {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

void delay() {
    usleep(100000);  // 0.1 seconds ko lagi
}

char peek() {
    return input[pos];
}

char consume(){
    return input[pos++];
}

void error(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    exit(1);
}

int expr();
int term();
int factor();

int expr() {
    print_indent();
    printf("Evaluating expression: %s\n", input + pos);
    delay();
    indent++;
    
    int result = term();
    while (peek() == '+' || peek() == '-') {
        char op = consume();
        int right = term();
        print_indent();
        printf("%d %c %d\n", result, op, right);
        delay();
        if (op == '+')
            result += right;
        else
            result -= right;
        print_indent();
        printf("= %d\n", result);
        delay();
    }
    
    indent--;
    return result;
}

int term() {
    print_indent();
    printf("Evaluating term: %s\n", input + pos);
    delay();
    indent++;
    
    int result = factor();
    while (peek() == '*' || peek() == '/') {
        char op = consume();
        int right = factor();
        print_indent();
        printf("%d %c %d\n", result, op, right);
        delay();
        if (op == '*')
            result *= right;
        else {
            if (right == 0) error("Division by zero");
            result /= right;
        }
        print_indent();
        printf("= %d\n", result);
        delay();
    }
    indent--;
    return result;
}

int factor() {
    print_indent();
    printf("Evaluating factor: %s\n", input + pos);
    delay();
    indent++;
    int result;
    if (isdigit(peek())) {
        result = consume() - '0';
        print_indent();
        printf("Number: %d\n", result);
        delay();
    } else if (peek() == '(') {
        consume();  // bracket khane
        result = expr();
        if (peek() != ')') error("Expected ')'");
        consume();  // braket khane
    } else {
        error("Unexpected character");
        result = 0;
    }
    
    indent--;
    return result;
}


int main() {
    printf("Enter an arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        error("Failed to read input");
    }
    input[strcspn(input, "\n")] = 0;
    
    int result = expr();
    printf("\nFinal result: %d\n", result);
    return 0;
}