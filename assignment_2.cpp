// Assignment 2 - Physics Course Database - Tom Melichar - 19/02/2024
// Code to read a courselist file and turn it to usable data, with a menu system to find various pieces of information about the data

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

std::ifstream open_file()
{
  while(true)
  {
    std::string file_name;
    std::cout<<"Enter file name (including extension)"<<std::endl<<"Input : ";
    std::cin>>file_name;
    std::cout<<std::endl;
    if(file_name == "c") file_name = "courselist.dat"; // shortcut to speed up testing
    std::ifstream file{file_name};
    if (!file.good()) std::cerr<<"Error : File could not be opened"<<std::endl;
    else
    {
        if (file.fail()) std::cerr<<"Error : File could not be read"<<std::endl;
        else return file;
    }
    std::cin.clear(); // reset input
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // empty input
  }
}

int multiple_choice(std::vector<std::string> prompts, std::vector<std::string> options, bool one_prompt = false)
{
  std::string input;
  while(true)
  {
    std::vector<std::string>::iterator prompt{prompts.begin()}, opt{options.begin()};
    if(!one_prompt) for(; opt < options.end(); prompt++, opt++) std::cout<<"Enter < "<<*opt<<" > "<<*prompt<<std::endl; // print prompts
    else std::cout<<prompts[0]<<std::endl; // print single prompt if one_prompt is true
    std::cout<<"Input : ";
    std::cin>>input;
    std::cout<<std::endl;
    for(int i = 0; i < options.size(); i++) if(input == options[i] && std::cin.peek() == '\n') return i+1; // check for each option and return accordingly
    std::cin.clear(); // reset input
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // empty input
    std::cerr<<"Error : Input must be one of the options"<<std::endl;
  }
}

double calculate_mean(std::vector<double>& values)
{
  double sum{0};
  for(int i = 0; i < values.size(); i++) sum += values[i];
  return sum / values.size();
}

double calculate_standard_deviation(std::vector<double> values, double mean)
{
  double diff_sum{0};
  for(std::vector<double>::iterator val = values.begin(); val < values.end(); val++) diff_sum += pow((*val-mean), 2);
  return pow((pow((values.size()-1),-1) * diff_sum), 0.5);
}

bool compare_names(std::string a, std::string b) {return a.substr(19) < b.substr(19);} // compares courses by their title

bool compare_codes(std::string a, std::string b) {return a.substr(12,17) < b.substr(12,17);} // ranks two strings

bool compare_marks(std::string a, std::string b) {return a.substr(0,4) > b.substr(0,4);} // ranks two strings

void print_sorted(std::vector<std::string> titles, int sort_type) 
{
  if(sort_type == 1) std::sort(titles.begin(), titles.end(), compare_names); // sorts the courses alphabetically
  else if(sort_type == 2) std::sort(titles.begin(), titles.end(), compare_codes); // sorts the courses by their course code
  else std::sort(titles.begin(), titles.end(), compare_marks); // sorts the courses by their average mark
  std::cout<<"-----------------------------------------------------------------"<<std::endl;
  for (std::vector<std::string>::iterator title = titles.begin(); title < titles.end(); ++title) std::cout<<*title<<std::endl;
  std::cout<<"-----------------------------------------------------------------"<<std::endl;
}

void print_means(std::vector<double> marks)
{
  double mean{calculate_mean(marks)}, standard_dev{calculate_standard_deviation(marks, mean)};
  std::cout<<"Average mark = "<<std::setprecision(1)<<std::fixed<<mean;
  std::cout<<" \u00b1 "<<std::setprecision(1)<<std::fixed<<standard_dev*(pow(marks.size(),-0.5))<<std::endl;
  std::cout<<"Standard deviation = "<<std::setprecision(1)<<std::fixed<<standard_dev<<"\n\n";
}

void year_means(std::vector<double> marks, std::vector<std::string> courses, int year)
{
  std::vector<double> year_marks;
  std::vector<double>::iterator mark = marks.begin();
  std::vector<std::string>::iterator course = courses.begin();
  for(; mark < marks.end(); ++mark, ++course) if((*course)[12] == char(year + '0')) year_marks.push_back(*mark);
  print_means(year_marks);
}

void print_year(std::vector<std::string> courses, int year)
{
  std::vector<std::string> year_courses;
  std::vector<std::string>::iterator course;
  for(course = courses.begin(); course < courses.end(); ++course) if((*course)[12] == char(year + '0')) year_courses.push_back(*course);
  print_sorted(year_courses, multiple_choice({"to sort alphabetically", "to sort numerically", "to sort by mark"}, {"a", "n", "m"}));
}

int main()
{
  std::vector<double> marks;
  std::vector<std::string> courses;
  std::vector<int> year_num{0, 0, 0, 0};
  double mark;
  int code, repeat, choice, index;
  std::string line, title;
  std::ifstream file = open_file();
  while (std::getline(file, line))
  {
    std::stringstream line_stream(line);
    line_stream>>mark>>code>>std::ws; // extracting the mark and the course code, and then discarding whitespaces
    std::getline(line_stream, title); // extracting the remainder as a string
    std::stringstream new_title; // defined here to clear and reset it each time the code loops
    new_title<<std::setprecision(1)<<std::fixed<<mark<<" | PHYS "<<code<<" | "<<title;
    year_num[floor(code/10000)-1]++;
    marks.push_back(mark);
    courses.push_back(new_title.str());
  }
  std::vector<std::string> prompts
  {
    "for all course data", "for the mean mark and its standard deviation", "for the total number of courses in the file",
    "for all data from a specific year", "for the mean mark and its standard deviation from a specific year", 
    "for the total number of courses in a specific year"
  };
  while(true)
  {
    choice = multiple_choice(prompts, {"1", "2", "3", "4", "5", "6"});
    if(choice == 1) print_sorted(courses, multiple_choice({"to sort alphabetically", "to sort numerically", "to sort by mark"}, {"a", "n", "m"}));
    else if(choice == 2) print_means(marks);
    else if(choice == 3) std::cout<<"There are "<<marks.size()<<" courses available overall"<<"\n\n";
    else
    {
      index = multiple_choice({"Enter the desired year number (1/2/3/4)"}, {"1", "2", "3", "4"}, true);
      if(choice == 4) print_year(courses, index);
      else if(choice == 5) year_means(marks, courses, index);
      else if(choice == 6) std::cout<<"There are "<<year_num[index - 1]<<" courses available in year "<<index<<"\n\n";
    }
    if(multiple_choice({"Would you like to continue (y/n)"}, {"y", "n"}, true) == 2) return 0;
  }
}