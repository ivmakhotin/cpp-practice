#include <iostream>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <stack>

// trim from start
static inline std::string &ltrim(std::string &str) {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(),
                  std::not1(std::ptr_fun<int, int>(std::isspace))));
        return str;
}

// trim from end
static inline std::string &rtrim(std::string &str) {
        str.erase(std::find_if(str.rbegin(), str.rend(),
                  std::not1(std::ptr_fun<int, int>(std::isspace))).base(), str.end());
        return str;
}

// trim from both ends
static inline std::string &trim(std::string &str) {
        return ltrim(rtrim(str));
}

bool is_open_bracket(char ch) {
    return ch == '(' || ch == '[' || ch == '{';
}

bool is_close_bracket(char ch) {
    return ch == ')' || ch == ']' || ch == '}';
}


int main() {
    std::string data;

    std::getline(std::cin, data);
    data = trim(data);
    std::stack<char> brackets_stack;
    int count = 0;
    for (size_t idx = 0; idx < data.size(); ++idx) {
        char c = data[idx];
        if (is_close_bracket(c) || is_open_bracket(c)) {
            if (is_open_bracket(c)) brackets_stack.push(c);
            if (is_close_bracket(c)) {
                if (brackets_stack.empty() ||
                    static_cast<int>(c) - static_cast<int>(brackets_stack.top()) >= 3 ||
                    static_cast<int>(c) - static_cast<int>(brackets_stack.top()) <= 0) {
                    std::cout << count;
                    return 0;
                }
                if (!brackets_stack.empty() &&
                    static_cast<int>(c) - static_cast<int>(brackets_stack.top()) < 3 &&
                    static_cast<int>(c) - static_cast<int>(brackets_stack.top()) > 0) {
                    brackets_stack.pop();
                }
                
            }
            count++;
        }
    }
    
    if (brackets_stack.empty())
        std::cout << "CORRECT";
    else
        std::cout << count;

    return 0;

}
