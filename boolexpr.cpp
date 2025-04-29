#include "boolexpr.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>
BooleanExpression::BooleanExpression(const char* input) {
    queue_ = MakePrefix(input);
    std::sort(vars_.begin(), vars_.end());
    for (size_t i = 0; i < vars_.size() / 2; ++i) {
        std::swap(vars_[i], vars_[vars_.size() - 1 - i]);
    }
}

std::string BooleanExpression::cnf() {
    std::string result;
    bool first = true;
    if (vars_.size() == 1 && vars_[0] == "0" || vars_[0] == "1") {return vars_[0];}
    for ( long long n = 0; n < static_cast<int>(std::pow(2, vars_.size())); ++n) {
        if (!Result(n)) { 
            std::string block = "(";
            for (int i = 0; i < vars_.size(); ++i) {

                if (((n & (1 << i)) >> i) == 0) {
                    block += vars_[i];
                } else {
                    block += "~";
                    block += vars_[i];
                }
                if (i != vars_.size() - 1) {
                    block += " v ";
                }
            }

            block += ")";
            if (!first) { result += " & "; }

            first = false;
            result += block;

        }
    }
    return result;
}

std::string BooleanExpression::dnf() {

    std::string result;
    bool first = true;
    if (vars_.size() == 1 && vars_[0] == "0" || vars_[0] == "1") {return vars_[0];}
    for (long long n = 0; n < static_cast<int>(std::pow(2, vars_.size())); ++n) {
        if (Result(n)) {
            std::string block = "(";
            for( int i = 0; i < vars_.size(); ++i) {
                if (((n & (1 << i)) >> i) == 0) {

                    block += "~";
                    block += vars_[i];

                } else {

                    block += vars_[i];

                }
                if (i != vars_.size() - 1) {

                    block += " & ";

                }
            }

            block += ")";
            if (!first) { result += " v "; }

            first = false;
            result += block;

        }
    }

    return result;
}

std::string BooleanExpression::zhegalkin() {

    std::string result;
    std::vector<bool> table_row(static_cast<int>(std::pow(2, vars_.size())));
    bool first = true;
    if (vars_.size() == 1 && vars_[0] == "0" || vars_[0] == "1") {
        return vars_[0];
    }
    for (long long n = 0; n < static_cast<int>(std::pow(2, vars_.size())); ++n) {
        table_row[n] = Result(n);
    }

    for (long long n = 0; n < static_cast<int>(std::pow(2, vars_.size())); ++n) {

        if (table_row[0]) {

            std::string block;
            bool first_num = true;

            if (n == 0) {
                first_num = false;
                block += "1";
            }

            for (int i = 0; i < vars_.size(); ++i) {

                if (((n & (1 << i)) >> i) == 1) {

                    if (!first_num) { block += " & "; }

                    first_num = false;
                    block += vars_[i];
                }
            }

            if (!first) { result += " + "; }

            first = false;
            result += block;
        }

        std::vector<bool> tmp_row(table_row.size() - 1);

        for (int i = 0; i < table_row.size() - 1; ++i) { tmp_row[i] = table_row[i] ^ table_row[i + 1];}
        std::swap(table_row, tmp_row);

    }

    return result;
}


inline bool BooleanExpression::Linear() const {

    bool result = true;
    std::vector<bool> table_row(static_cast<int>(std::pow(2, vars_.size())));

    for (long long n = 0; n < static_cast<int>(std::pow(2, vars_.size())); ++n) { table_row[n] = Result(n); }

    for (long long n = 0; n < static_cast<int>(std::pow(2, vars_.size())); ++n) {

        if (table_row[0]) {

            int count = 0;

            for (int i = 0; i < vars_.size(); ++i) {

                if (((n & (1 << i)) >> i) == 1) {

                    ++count;

                    if (count > 1) {
                        result = false;
                        break;
                    }

                }
            }
            if (!result) {break;}
        }

        std::vector<bool> tmp_row(table_row.size() - 1);

        for (int i = 0; i < table_row.size() - 1; ++i) { tmp_row[i] = table_row[i] ^ table_row[i + 1]; }
        std::swap(table_row, tmp_row);
    }
    return result;
}

inline int BooleanExpression::GetPriority(char ch) {
    
    int ans;
    switch (ch) {
        case '=':ans = 0;
            break;
        case '<':
        case '>':ans = 1;
            break;
        case '|':ans = 2;
            break;
        case '^':ans = 3;
            break;
        case '+':ans = 4;
            break;
        case 'v':ans = 5;
            break;
        case '&':ans = 6;
            break;
        default:ans = -1;
    }
    return ans;
}

BooleanExpression::operator std::string() const {
    std::string result;
    std::string RESULT;
    result += '(';
    std::vector<std::string> stack;
    int lit = 0;
    unsigned int size = queue_.size();
    if (size == 1) { return result = queue_[0]; }
    std::string top;
    bool end = true;
    for (int i = 0; i < size; ++i) {
        if (GetPriority(queue_[i][0]) == -1) {
            ++lit;
            stack.emplace_back(queue_[i]);
        }

        if (lit == 2 && GetPriority(queue_[i][0]) > -1) {
            top = stack.front();
            stack.erase(stack.begin());
            result += top + " " + queue_[i][0];
            top = stack.front();
            stack.erase(stack.begin());
            result += " " + top;
            lit = 0;
            if (i + 2 < size) {
                if (GetPriority(queue_[i + 2][0]) > -1) {
                    continue;
                }

                result += ')';
                RESULT += result + ' ';
                result = "";
                result += '(';
                RESULT += queue_[size - 1][0];
                RESULT += " ";
                --size;

            }
        }
        else if (lit == 1 && GetPriority(queue_[i][0]) > -1) {
            result += ' ';
            result += queue_[i][0];
            top = stack.front();
            stack.erase(stack.begin());
            result += " " + top;
            lit = 0;
            if (i + 1 != queue_.size()) {
                if (GetPriority(queue_[i + 1][0]) == -1) {
                    result += ')';

                    RESULT += result + ' ';
                    result = "";
                    result += '(';
                    RESULT += queue_[size - 1][0];
                    RESULT += " ";
                    --size;
                }
            }
        }
    }

    if (!stack.empty()) {
        while (!stack.empty()) {
            top = stack.front();
            stack.erase(stack.begin());
            result += " " + top;
        }
        result += ')';
        RESULT += result;
    }
    else {
        result += ')';
        RESULT += result;
    }
    return RESULT;
}

inline void BooleanExpression::AddVar(std::string var) {
    if (var[0] == '~') {
        var = {var.begin() + 1, var.end()};
    }
    bool place = true;
    for (auto& i : vars_) {
        if (var == i) {
            place = false;
            break;
        }
    }
    if (place) {
        vars_.emplace_back(std::move(var));
    }
}

std::vector<std::string> BooleanExpression::MakePrefix(const char* input) {
    //(x1&x2) data[i]=&
    std::vector<std::string> stack;
    std::vector<std::string> stack_2;
    std::vector<std::string> queue;
    char* data = new char[strlen(input)+1];
   // std::cout << '\n';
    strncpy(data, input,strlen(input)+1);
    
    char pch[128] = "";
    int i = 0;
    int j = 0;
    bool UNION = false;
    bool UNION_CLOSE = true;
    
    while ((GetPriority(data[i])==-1 )&& data[i] != ' '&& i+1 != strlen(data)) {
        if (data[i] == '(') { ++i; UNION = true; UNION_CLOSE = false; continue; }
        pch[j] = data[i];
        ++i;
        ++j;
    }
    if (data[i]>='0'&&data[i]<='9') {
        pch[j] = data[i];
    }
    while (data[i] == ' ' && i + 1 != strlen(data)) { ++i; }
    //---------------------------------------------
    while (pch[0]!='\0') {
        if (strlen(pch) != 1||(strlen(pch)==1 && pch[0]>='0'&&pch[0]<='9')) {
            queue.emplace_back(pch);
            AddVar(pch);
        } else {

            if (stack.empty()) {
                if (UNION && UNION_CLOSE) {
                    stack_2.emplace_back(pch);

                }
                else {
                    stack.emplace_back(pch);
                }
            }
            else {
                if (GetPriority(pch[0]) == -1) {
                    if (pch[0] == '~') { throw std::runtime_error("Bad name"); }
                    queue.emplace_back(pch);
                    AddVar(pch);

                }
                else if (GetPriority(pch[0]) > GetPriority(stack.front()[0])) {
                    stack.emplace(stack.begin(), pch);
                }
                else {

                    while (!stack.empty() && GetPriority(pch[0]) <= GetPriority(stack.front()[0])) {
                        auto top = stack.front();
                        stack.erase(stack.begin());
                        queue.emplace_back(top);
                    }

                    stack.emplace(stack.begin(), pch);

                }
            }
        }
        memset(pch, '\0', strlen(pch));
        j = 0;

        if (i + 1 != strlen(data)) {
            while (data[i] == ' ' && i + 1 != strlen(data)) { ++i; if (i + 1 == strlen(data)) { break; } }
            while ((GetPriority(data[i]) ==-1 )&& data[i] != ' ') {
                if (data[i] == ')' && !UNION_CLOSE) {
                    UNION_CLOSE = true;
                    
                    if (i + 1 == strlen(data)) {
                        break;
                    }
                    
                    continue;
                }
                if (data[i] == '(') {
                    if (!stack.empty()) {
                        auto top = stack.front();
                        stack_2.emplace_back(top);
                        stack.erase(stack.begin());
                    }
                    UNION_CLOSE = false;
                    UNION = true;
                    if (i + 1 == strlen(data)) {
                        break;
                    }
                    ++i;
                    continue;
                }
                if (UNION_CLOSE && UNION) {
                    UNION = false;
                    queue.emplace_back(pch);
                    AddVar(pch);
                    auto top = stack.front();
                    stack.erase(stack.begin());
                    queue.emplace_back(top);
                    ++i;
                    memset(pch, '\0', strlen(pch));
                    j = 0;
                }
                while (data[i] == ' ') { ++i; }
                pch[j] = data[i];
               
                if (i + 1 != strlen(data)) {
                    ++i;
                    ++j;
                }
                else {
                    break;
                }
            }
            if (strlen(pch) > 0) { continue; }
            if (GetPriority(data[i]) > -1) {
                pch[j] = data[i];
                if (i + 1 != strlen(data)) {
                    ++i;
                }
            }
            
        }
    }

    while (!stack.empty()) {
        auto top = stack.front();
        stack.erase(stack.begin());
        queue.emplace_back(top);
    }
    while (!stack_2.empty()) {
        auto top = stack_2.front();
        stack_2.erase(stack_2.begin());
        queue.emplace_back(top);
       
    }

 
    delete[] data;
    return queue;
}


inline bool BooleanExpression::GetSmallResult(const std::string& now, bool value, bool value_1) {

    bool ans;

    switch (now[0]) {
        case '=':ans = value == value_1;
            break;
        case '<':ans = static_cast<int>(value) <= static_cast<int>(value_1);//
            break;
        case '>':ans = static_cast<int>(value) >= static_cast<int>(value_1);//
            break;
        case '|':ans = !(value && value_1);
            break;
        case '^':ans = !(value || value_1);
            break;
        case '+':ans = value ^ value_1;
            break;
        case 'v':ans = value || value_1;
            break;
        case '&':ans = value && value_1;
            break;
        default:ans = false;
    }
    return ans;
}

bool BooleanExpression::Result(long long value) const {

    std::vector<std::string> stack;
    std::vector<std::string> queue_cp = queue_;
    int operator_count = 0;
    while (!queue_cp.empty()) {

        auto now = queue_cp.front();//берем 1 элемент
        queue_cp.erase(queue_cp.begin());//удаляем из очереди
        if (now == "1" && value ==0 && vars_.size() == 1) { return true; }
        if (now == "0" && value != 0 && vars_.size() == 1) { return false; }
        if (now.length() >= 1 && GetPriority(now[0])==-1) {

            auto a = stack.begin();
            
            stack.emplace(a, now);
         
            
        } else {
            
            if (stack.size()<2){
                throw std::runtime_error("Bad expression");
            }

            ++operator_count;
            auto right = stack.front();
            stack.erase(stack.begin());
            auto left = stack.front();
            stack.erase(stack.begin());
            bool right_value;
            bool left_value;

            if (right[0] == '1'|| right == "true") {
                right_value = true;
            }
            else if (right[0] == '0'|| right == "false") {
                right_value = false;
            }
            else{
                right_value = GetValue(value, right);
            }


            
            if (left[0] == '1'|| left == "true") {
                left_value = true;
            }
            else if (left[0] == '0'|| left == "false") {
                left_value = false;
            }
            else {
                left_value = GetValue(value, left);
            }
            
            bool small_result = GetSmallResult(now, right_value, left_value);

            const char* c_result = small_result ? "true" : "false";
            stack.emplace(stack.begin(), c_result);
        }
        
    }
    if (operator_count == 0 && vars_.size() == 1 && stack.size() == 1) {

        auto single = stack.front();
        stack.erase(stack.begin());

        bool small_result = GetValue(value, single);
        std::string c_result = small_result ? "true" : "false";
        stack.emplace(stack.begin(), c_result);
    }

    if (stack.size() != 1 || stack.front() != "true" && stack.front() != "false") { throw std::runtime_error("Bad expression");}

    return stack.front() == "true";
}

inline bool BooleanExpression::GetValue(long long value, std::string var_name) const {

    bool result = false;
    bool invert = false;

    if (var_name[0] == '~' ) {
        invert = true;
       
        var_name = { var_name.begin() + 1, var_name.end() };
        
    }
        for (int i = 0; i < vars_.size(); ++i) {
            if (vars_[i] == var_name) {
                result = ((value & (1 << i)) >> i) != 0;
                break;
            }
        }
    
    if (invert) {
        result = !result;
    }

    return result;
}

bool IsFullSystem(const std::vector<BooleanExpression>& funcs) {

    bool Monotonic = true;
    bool Linear = true;
    bool SelfDual = true;
    bool SaveOne = true;
    bool SaveZero = true;
   

    for (const auto& i : funcs) {
        SaveOne &= i.SaveOne();
        SaveZero &= i.SaveZero();
        Linear &= i.Linear();
        SelfDual &= i.SelfDual();
        Monotonic &= i.Monotonic();
    }

    return !Monotonic && !Linear && !SelfDual && !SaveOne && !SaveZero;
}

inline bool BooleanExpression::SaveZero() const { return !Result(0); }

inline bool BooleanExpression::SaveOne() const { return Result(static_cast<int>(std::pow(2, vars_.size()) - 1)); }

inline bool BooleanExpression::SelfDual() const {

    bool result = true;

    for ( long long n = 0; n < static_cast<int>(std::pow(2, vars_.size()) / 2); ++n) {

        if (Result(n) == Result(static_cast<int>(std::pow(2, vars_.size())) - 1 - n)) {
            result = false;
            break;
        }
    }

    return result;
}
inline bool BooleanExpression::Monotonic() const {

    bool result = true;
    bool prev = Result(0);

    for (long long n = 0; n < static_cast<int>(std::pow(2, vars_.size())); ++n) {

        bool now = Result(n);

        if (prev && !now) {
            result = false;
            break;
        }

        prev = now;
    }
    return result;
}

std::string BooleanExpression::make_table() {

    std::string out;

    for (int i = 0; i < vars_.size(); ++i) {
        out += vars_[i] + ' ';
    }
    out += "result";
    out += '\n';

    for (long long n = 0; n < static_cast<int>(std::pow(2, vars_.size())); ++n) {
        for (int i = 0; i < vars_.size(); ++i) {
            if (vars_[i] == "1" || vars_[i] == "0") {
                out += vars_[i];
            }
            else {
                out += (GetValue(n, vars_[i])) ? " 1" : " 0";
            }
            out += " ";
        }
        if (Result(n)) {
            out += " 1 ";
        }
        else {
            out += " 0 ";
        }
        out += '\n';
    }

    return out;
}
