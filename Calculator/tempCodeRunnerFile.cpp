#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

// Token "kind" values
char const number = '8';    // a floating-point number
char const quit = 'q';      // an exit command
char const print = ';';     // a print command

// Class token
class token
{
    char kind_;
    double value_;
    std::string name_;

public:
    token(char ch)
      : kind_(ch)
      , value_(0)
      , name_()
    {
    }
    token(double val)
      : kind_(number)
      , value_(val)
      , name_()
    {
    }
    token(char ch, std::string n)
      : kind_(ch)
      , value_(0)
      , name_(n)
    {
    }

    char kind() const
    {
        return kind_;
    }
    double value() const
    {
        return value_;
    }
    std::string name() const
    {
        return name_;
    }
};

// Class token_stream
class token_stream
{
    bool full;
    token buffer;

public:
    token_stream()
      : full(false)
      , buffer('\0')
    {
    }

    token get();
    void putback(token);
    void ignore(char);
};

token_stream ts;

// Variable map for storing variables
std::unordered_map<std::string, double> variables;

// Pre-defined symbolic values
std::unordered_map<std::string, double> symbolic_values = {
    {"pi", 3.14159265358979323846},
    {"e", 2.71828182845904523536}
};

// Function prototypes
double expression();
double term();
double primary();
double get_value(std::string);
void set_value(std::string, double);

// Token_stream methods
token token_stream::get()
{
    if (full)
    {
        full = false;
        return buffer;
    }

    char ch;
    std::cin >> ch;

    switch (ch)
    {
    case '(':
    case ')':
    case ';':
    case 'q':
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
        return token(ch);
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        std::cin.putback(ch);
        double val;
        std::cin >> val;
        return token(val);
    }
    default:
        if (isalpha(ch))
        {
            std::string s;
            s += ch;
            while (std::cin.get(ch) && (isalnum(ch) || ch == '_'))
                s += ch;
            std::cin.putback(ch);
            if (s == "let") return token('a');
            if (symbolic_values.find(s) != symbolic_values.end()) return token(symbolic_values[s], s);
            return token(number, s);
        }
        throw std::runtime_error("Bad token");
    }
}



void token_stream::putback(token t)
{
    if (full)
        throw std::runtime_error("putback() into a full buffer");
    buffer = t;
    full = true;
}

void token_stream::ignore(char c)
{
    if (full && c == buffer.kind())
    {
        full = false;
        return;
    }
    full = false;

    char ch = 0;
    while (std::cin >> ch)
        if (ch == c) return;
}

double get_value(std::string var)
{
    if (variables.find(var) != variables.end())
        return variables[var];
    else if (symbolic_values.find(var) != symbolic_values.end())
        return symbolic_values[var];
    else
        throw std::runtime_error("Undefined variable or symbolic value: " + var);
}

void set_value(std::string var, double val)
{
    variables[var] = val;
}

double primary()
{
    token t = ts.get();
    switch (t.kind())
    {
    case '(':
    {
        double d = expression();
        t = ts.get();
        if (t.kind() != ')')
            throw std::runtime_error("')' expected");
        return d;
    }
    case number:
        return t.value();
    case '-':
        return -primary();
    case '+':
        return primary();
    case 'a':
    {
        token t2 = ts.get();
        if (t2.kind() != '=')
            throw std::runtime_error("= missing in variable assignment");
        double d = expression();
        set_value(t.name(), d);
        return d;
    }
    default:
    {
        if (isalpha(t.kind()))
        {
            std::string name = t.name();
            if (symbolic_values.find(name) != symbolic_values.end())
                return symbolic_values[name];
            else if (variables.find(name) != variables.end())
                return variables[name];
            else
                throw std::runtime_error("Undefined variable or symbolic value: " + name);
        }
        throw std::runtime_error("primary expected");
    }
    }
}


double term()
{
    double left = primary();
    while (true)
    {
        token t = ts.get();
        switch (t.kind())
        {
        case '*':
            left *= primary();
            break;
        case '/':
        {
            double d = primary();
            if (d == 0)
                throw std::runtime_error("divide by zero");
            left /= d;
            break;
        }
        case '%':
        {
            int i1 = static_cast<int>(left);
            int i2 = static_cast<int>(term());
            if (i2 == 0)
                throw std::runtime_error("%: divide by zero");
            left = i1 % i2;
            break;
        }
        default:
            ts.putback(t);
            return left;
        }
    }
}

double expression()
{
    double left = term();
    while (true)
    {
        token t = ts.get();
        switch (t.kind())
        {
        case '+':
            left += term();
            break;
        case '-':
            left -= term();
            break;
        default:
            ts.putback(t);
            return left;
        }
    }
}

void clean_up_mess()
{
    ts.ignore(print);
}

void calculate()
{
    std::cout << "Calculator on :)" << std::endl;
    while (std::cin)
    {
        try
        {
            std::cout << "> ";
            token t = ts.get();

            while (t.kind() == print)
                t = ts.get();

            if (t.kind() == quit)
            {
                std::cout << "Calculator off :(" << std::endl;
                return;
            }

            ts.putback(t);

            std::cout << "= " << expression() << std::endl;
        }
        catch (std::runtime_error const& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            clean_up_mess();
        }
    }
}

int main()
{
    try
    {
        calculate();
        return 0;
    }
    catch (...)
    {
        std::cerr << "exception\n";
        return 2;
    }
}
