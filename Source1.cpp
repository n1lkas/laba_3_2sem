#include <iostream>
#include <stack>
#include <string>


using namespace std;

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}
int precedence(char c) {
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    if (c == '^') return 3;
    return 0;
}
int applyOperator(int a, int b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') return a / b;
    if (op == '^') {
        int result = 1;
        for (int i = 0; i < b; ++i) {
            result *= a;
        }
        return result;
    }
    return 0;
}


//1
//Postfix-обратная
//revers- обртаная
string infixToPrefix(const string& expression) {
    stack<char> operators;
    string prefix;

    cout << "Intermediate steps for converting to prefix notation:" << endl;

    for (int i = expression.size() - 1; i >= 0; --i) {
        char token = expression[i];
        cout << "Token: " << token << endl;
        if (isDigit(token)) {
            prefix = token + prefix;
            cout << "Current prefix: " << prefix << endl;
        }
        else if (token == ')') {
            operators.push(token); 
            cout << "Right parenthesis pushed to stack." << endl;
        }
        else if (token == '(') {
            while (!operators.empty() && operators.top() != ')') {
                prefix = operators.top() + prefix;
                cout << "Operator: " << operators.top() << " popped from stack and added to prefix notation." << endl;
                operators.pop();
            }
            if (!operators.empty()) { // Check if stack is empty
                operators.pop(); // Discard ')'
                cout << "Left parenthesis encountered. Popped all operators until right parenthesis." << endl;
            }
            else {
                // Handle error: unmatched left parenthesis
                cout << "Error: Unmatched left parenthesis." << endl;
                return "";
            }
        }
        else if (isOperator(token)) {
            while (!operators.empty() && operators.top() != ')' && precedence(operators.top()) >= precedence(token)) {
                prefix = operators.top() + prefix;
                cout << "Operator: " << operators.top() << " popped from stack and added to prefix notation." << endl;
                operators.pop();
            }
            operators.push(token);
            cout << "Operator: " << token << " pushed to stack." << endl;
        }
    }

    while (!operators.empty()) {
        prefix = operators.top() + prefix;
        cout << "Operator: " << operators.top() << " popped from stack and added to prefix notation." << endl;
        operators.pop();
    }

    cout << "Final prefix notation: " << prefix << endl;
    return prefix;
}
string infixToPostfix(const string& expression) {
    stack<char> operators;
    string postfix;

    cout << "Intermediate steps for converting to postfix notation:" << endl;

    for (char token : expression) {
        cout << "Token: " << token << endl;
        if (isDigit(token)) {
            postfix += token;
            cout << "Current postfix: " << postfix << endl;
        }
        else if (token == '(') {
            operators.push(token);
            cout << "Pushed to stack: " << token << endl;
        }
        else if (token == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                cout << "Popped from stack: " << operators.top() << " (added to postfix)" << endl;
                operators.pop();
            }
            operators.pop(); // Discard '('
            cout << "Discarded '(' from stack" << endl;
        }
        else if (isOperator(token)) {
            while (!operators.empty() && precedence(operators.top()) >= precedence(token)) {
                postfix += operators.top();
                cout << "Popped from stack: " << operators.top() << " (added to postfix)" << endl;
                operators.pop();
            }
            operators.push(token);
            cout << "Pushed to stack: " << token << endl;
        }
    }

    while (!operators.empty()) {
        postfix += operators.top();
        cout << "Popped from stack: " << operators.top() << " (added to postfix)" << endl;
        operators.pop();
    }

    cout << "Final postfix notation: " << postfix << endl;
    return postfix;
}

void evaluatePrefix(const string& expression) {
    string prefix = infixToPrefix(expression);
    cout << "Prefix notation: " << prefix << endl;
}
void evaluatePostfix(const string& expression) {
    string postfix = infixToPostfix(expression);
    cout << "Postfix notation: " << postfix << endl;
}

//2
bool isValidExpression(const string& expression) {
    int parenthesesCount = 0;
    for (char token : expression) {
        if ((token >= '0' && token <= '9') || isOperator(token) || token == '(' || token == ')') {
            if (token == '(') {
                parenthesesCount++;
            }
            else if (token == ')') {
                parenthesesCount--;
                if (parenthesesCount < 0) return false; // More closing parentheses than opening ones
            }
        }
        else {
            return false; // Invalid character
        }
    }
    return parenthesesCount == 0;
}
bool isValidPolishNotation(const string& expression) {
    stack<int> operandsCount;

    for (char token : expression) {
        if (token >= '0' && token <= '9') {
            operandsCount.push(1);
        }
        else if (isOperator(token)) {
            if (operandsCount.size() < 2) return false; // Not enough operands
            operandsCount.pop();
            operandsCount.pop(); // Pop two operands
            operandsCount.push(1); // Push result
        }
        else {
            return false; // Invalid character
        }
    }

    return operandsCount.size() == 1;
}

//3
int evaluateSimpleExpression(const string& expression) {
    stack<int> numbers;
    stack<char> operators;

    for (char token : expression) {
        if (isDigit(token)) {
            numbers.push(token - '0');
        }
        else if (isOperator(token)) {
            while (!operators.empty() && operators.top() != '(' && precedence(operators.top()) >= precedence(token)) {
                int b = numbers.top(); numbers.pop();
                int a = numbers.top(); numbers.pop();
                numbers.push(applyOperator(a, b, operators.top()));
                operators.pop();
            }
            operators.push(token);
        }
    }

    while (!operators.empty()) {
        int b = numbers.top(); numbers.pop();
        int a = numbers.top(); numbers.pop();
        numbers.push(applyOperator(a, b, operators.top()));
        operators.pop();
    }

    return numbers.top();
}
int evaluatePolishNotation(const string& expression) {
    stack<int> operands;

    cout << "Evaluating expression in prefix notation:" << endl;

    // Обходим выражение справа налево
    for (int i = expression.size() - 1; i >= 0; --i) {
        char token = expression[i];

        if (isdigit(token)) {
            operands.push(token - '0'); // Преобразуем символ в цифру и добавляем в стек
            cout << "Operand: " << token << " pushed to stack." << endl;
        }
        else if (isOperator(token)) {
            // Извлекаем два операнда из стека
            int operand1 = operands.top(); operands.pop();
            int operand2 = operands.top(); operands.pop();

            // Выполняем операцию и помещаем результат обратно в стек
            int result;
            if (token == '+') {
                result = operand1 + operand2;
            }
            else if (token == '-') {
                result = operand1 - operand2;
            }
            else if (token == '*') {
                result = operand1 * operand2;
            }
            else if (token == '/') {
                result = operand1 / operand2;
            }
            else if (token == '^') {
                result = 1;
                for (int j = 0; j < operand2; ++j) {
                    result *= operand1;
                }
            }
            operands.push(result);
            cout << "Operator: " << token << " applied to operands " << operand1 << " and " << operand2 << ", result pushed to stack." << endl;
        }
    }

    // Вернуть результат из стека
    return operands.top();
}
int evaluateReversePolishNotation(const string& expression) {
    stack<int> numbers;

    for (char token : expression) {
        if (isDigit(token)) {
            numbers.push(token - '0');
        }
        else if (isOperator(token)) {
            int b = numbers.top(); numbers.pop();
            int a = numbers.top(); numbers.pop();
            if (token == '+') {
                numbers.push(a + b);
            }
            else if (token == '-') {
                numbers.push(a - b);
            }
            else if (token == '*') {
                numbers.push(a * b);
            }
            else if (token == '/') {
                numbers.push(a / b);
            }
            else if (token == '^') {
                int result = 1;
                for (int i = 0; i < b; ++i) {
                    result *= a;
                }
                numbers.push(result);
            }
        }
    }

    return numbers.top();
}


int main() {
    string expression;
    cout << "Enter a mathematical expression: ";
    getline(cin, expression);
    while (true) {
        cout << "Menu:" << endl;
        cout << "1. Converted expression in direct Polish notation" << endl;
        cout << "2. Converted expression in reverse Polish notation" << endl;
        cout << "3. Correctness with direct Polish notation" << endl;
        cout << "4. Correctness with reverse Polish notation" << endl;
        cout << "5. Calculating a simple expression" << endl;
        cout << "6. Calculating the direct Polish notation" << endl;
        cout << "7. Calculating the reverse Polish notation" << endl;
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        string postfixExpression;
        string prefixExpression;
        
        bool expressionValid = false;
        switch (choice) {
        case 1: {
            evaluatePrefix(expression);
            break;
        }
        case 2: {
            evaluatePostfix(expression);
           
            break;
        }
        case 3: {
            if (!expression.empty()) {
                postfixExpression = infixToPostfix(expression);
                if (isValidPolishNotation(postfixExpression)) {
                    cout << "The expression is valid." << endl;
                }
                else {
                    cout << "The expression is invalid." << endl;
                }
            }
            else {
                cout << "Please enter a mathematical expression first (option 1)." << endl;
            }
            break;
        }
        case 4: {
            if (!expression.empty()) {
                prefixExpression = infixToPrefix(expression);
                if (isValidPolishNotation(prefixExpression)) {
                    cout << "The expression is valid." << endl;
                }
                else {
                    cout << "The expression is invalid." << endl;
                }
            }
            else {
                cout << "Please enter a mathematical expression first (option 1)." << endl;
            }
            break;
        }
        case 5: {
            int simpleResult = evaluateSimpleExpression(expression);
            cout << "Result of simple expression: " << simpleResult << endl;
            break;
        }
        case 6: {
            string prefixExpression = infixToPrefix(expression);
            int prefixResult = evaluatePolishNotation(prefixExpression);
            cout << "Result of expression in prefix notation: " << prefixResult << endl;
            break;
        }
        case 7: {
            string postfixExpression = infixToPostfix(expression);
            int postfixResult = evaluateReversePolishNotation(postfixExpression);
            cout << "Result of expression in postfix notation: " << postfixResult << endl;
            break;
        }
        default:
            cout << "Invalid choice." << endl;
        }
    }
    return 0;
}
