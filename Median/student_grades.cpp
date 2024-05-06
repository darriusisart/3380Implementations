
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// hold all information related to a single student
struct student_info
{
    std::string name;                // students name
    double midterm, final;           // midterm and final exam grades
    std::vector<double> homework;    // all homework grades
};

// read all information related to one student
std::istream& read_hw(std::istream& in, std::vector<double>& hw)
{
    if (in)
    {
        hw.clear();    // get rid of previous contents

        // read homework grades
        double x;
        while (in >> x)
            hw.push_back(x);

        // clear the stream so that input will work for
        // the next student
        in.clear();
    }
    return in;
}

std::istream& operator>>(std::istream& in, student_info& s)
{
    // read the students name, midterm and final exam grades
    in >> s.name >> s.midterm >> s.final;

    // read all homework grades for this student
    return read_hw(in, s.homework);
}

// compute the median of a std::vector<double>
// note: calling this function copies the whole std::vector
double median(const std::vector<double>& vec)
{
    auto size = vec.size();
    if (size == 0)
        throw std::domain_error("std::vector is empty, median undefined");

    auto mid = size / 2;
    std::vector<double> sorted_vec = vec; // Make a copy to avoid modifying the original vector
    std::nth_element(sorted_vec.begin(), sorted_vec.begin() + mid, sorted_vec.end());

    if (size % 2 == 0)
    {
        auto it1 = sorted_vec.begin() + mid;
        std::nth_element(sorted_vec.begin(), it1 - 1, sorted_vec.end());
        return (*it1 + *(it1 - 1)) / 2.0;
    }
    else
    {
        return sorted_vec[mid];
    }
}



// Calculate the final grade for one student
// compute a student's overall grade from midterm and final exam
// grades and homework grade
double grade(double midterm, double final, double homework)
{
    return 0.2 * midterm + 0.4 * final + 0.4 * homework;
}

// Compute a student's overall grade from midterm and
// final exam grades and all homework grades.
// This function does not copy the std::vector argument
// (as median does so for us).
double grade(double midterm, double final, std::vector<double> const& hw)
{
    if (hw.size() == 0)
        throw std::domain_error("student has done no homework");
    return grade(midterm, final, median(hw));
}

double grade(student_info const& s)
{
    return grade(s.midterm, s.final, s.homework);
}

int main()
{
    std::ifstream in("student_grades.txt");

    // read and store all the records, find the length of
    // the longest name
    std::vector<student_info> students;    // all student records
    std::string::size_type maxlen = 0;     // length of longest name

    student_info record;
    while (in >> record)
    {
        maxlen = std::max(maxlen, record.name.size());
        students.push_back(record);
    }

    // sorting the student data using a lambda function
    std::sort(students.begin(), students.end(),
        [](student_info const& x, student_info const& y) {
            return x.name < y.name;
        });

    // generating the report
    for (student_info const& si : students)
    {
        // write the name, padded on the right side to maxlen + 1 characters
        std::cout << si.name << std::string(maxlen + 1 - si.name.size(), ' ');

        // compute and write the grade
        try
        {
            // first calculate the grade
            double final_grade = grade(si);

            // now, do actual output
            std::streamsize prec = std::cout.precision();
            std::cout << std::setprecision(3) << final_grade
                      << std::setprecision(prec);
        }
        catch (std::domain_error e)
        {
            std::cout << e.what();
        }
        std::cout << std::endl;
    }

    return 0;
}
