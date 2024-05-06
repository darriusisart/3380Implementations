#include <iostream>

// Function to create a std::string consisting of N equal characters using the power algorithm
std::string create_string(char ch, int N)
{
    // Check if N is non-negative
    if (N < 0)
        throw std::invalid_argument("N must be non-negative");

    // Edge case: if N is 0, return an empty string
    if (N == 0)
        return "";

    // Calculate the power of the character using std::pow
    char result = std::pow(ch, N);

    // Create a string of length N with the character
    return std::string(N, result);
}

int main()
{
    char ch;
    int N;

    // Input character and integer
    std::cout << "Enter a character: ";
    std::cin >> ch;
    std::cout << "Enter an integer: ";
    std::cin >> N;

    try
    {
        // Create string and print
        std::string result = create_string(ch, N);
        std::cout << "Resulting string: " << result << std::endl;
    }
    catch (const std::invalid_argument& e)
    {
        // Handle invalid argument exception
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    }

    return 0;
}
