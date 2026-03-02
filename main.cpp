#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <map>
#include <numeric>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <sstream>

struct Student{
    int id;
    std::string name;
    unsigned short course;
    unsigned short age;
    std::vector<short>estimates;

     friend std::ostream& operator<<(std::ostream& os, const Student& s) {
         return os << s.name << " " << s.age;
     }
};

 bool AddStudent(std::map<int, Student>& students, unsigned int& id){
     Student new_student;

     new_student.id = ++id;

     std::cout << std::endl;

     std::cout << "Enter student name: ";
     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
     std::getline(std::cin, new_student.name);

     while (true) {
     std::cout << "Enter course student: ";
     std::cin >> new_student.course;

     if (std::cin.fail()){
         std::cin.clear();
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         std::cout << "Error! Please enter an integer.\n";
     }

     else if (new_student.course < 1 || new_student.course > 4) {
         std::cout << "Error! The rate must be between 1 and 4.\n";
     }

     else {
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         break;
     }

     }

     std::cout << "Enter the student's age: ";
     if (!(std::cin >> new_student.age)) {
         std::cin.clear();
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         return false;
     }

     unsigned short grade;
     std::cout << "Enter the student's grades (if you want to stop entering, type the symbol): ";
     while (std::cin >> grade){

         if (grade > 5 || grade < 2){
             std::cout << "You've gone beyond the limit. Enter numbers from 2 to 5.\n";
             continue;
         }

         new_student.estimates.push_back(grade);

     }

     students[new_student.id] = std::move(new_student);

     std::cout << "Student added successfully!\n";

     std::cin.clear();
     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

     return true;
 }

 void DeleteStudent(std::map<int, Student>& students){

     std::cout << "Enter the student ID to delete: ";
     unsigned short idToDelete;
     std::cin >> idToDelete;

     if (students.erase(idToDelete)) {
         std::cout << "Student removed.\n";
     } else {
         std::cout << "ID not found!\n";
     }
 }


 void FindStudent(const std::map<int, Student>& students){

     unsigned short target;
     std::cout << "Enter student ID to search: ";
     std::cin >> target;

     auto it = students.find(target);

     if (it != students.end()) {
         const Student& st = it->second;
         std::cout << "\nFound: " << "ID: " << st.id << " | Name: " << st.name << " | Age: " << st.age << " | Grades: ";
         if (st.estimates.empty()){
             std::cout << "No grades yet";
         }
         else{
             for (const auto& grade : st.estimates){
                 std::cout << grade << " ";
             }

             std::cout << std::endl;
         }
     } else {
         std::cout << "Student with ID " << target << " not found.\n";
     }
 }

 void ShowStudent(const std::map<int, Student>& students){

     if (students.empty()) {
         std::cout << "The list is empty.\n";
         return;
     }
     for (const auto& [id, st] : students) {
         std::cout << "ID: " << id << " | Name: " << st.name << " | Age: " << st.age << " | Grade: ";
         if (st.estimates.empty()){
             std::cout << "No grades yet";
         }
         else{
             for (const auto& grade : st.estimates){
                 std::cout << grade << " ";
             }

             std::cout << std::endl;
         }
     }

     std::cout << std::endl;
 }

 void AverageScores(const std::map<int, Student>& students){

     if (students.empty()) {
         std::cout << "The list is empty.\n";
         return;
     }

     for (const auto& [id, st] : students) {
         std::cout << "ID: " << id << " | Name: " << st.name << " | ";
         if (st.estimates.empty()){
             std::cout << "No grades yet";
         }
         else{
             double sum = 0;
             for (int grade : st.estimates) {
                 sum += grade;
             }
             double avg = sum / st.estimates.size();
             std::cout << "Average score: " << avg << std::endl;

             std::cout << std::endl;
         }
     }

     std::cout << std::endl;
 }

 void StudentsCourse(const std::map<int, Student>& students){
     if (students.empty()){
         std::cout << "The list is empty.\n";
         return;
     }

     unsigned short targetCourse;
     std::cout << "Enter course: ";
     if (!(std::cin >> targetCourse)){
         std::cin.clear();
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         return;
     }

     std::cout << "Students on course " << targetCourse << ":\n---\n";

     bool found = false;
     for (const auto& [id, st] : students){
         if (st.course == targetCourse){
             std::cout << "ID: " << st.id << " | Name: " << st.name << std::endl;
             found = true;
         }
     }

     if (!found){
         std::cout << "No students found on this course." << std::endl;
     }
 }

 void BestStudents(const std::map<int, Student>& students){

     std::vector<std::tuple<int, std::string, float >> ranking;
     ranking.reserve(students.size());

     if (students.empty()) {
         std::cout << "The list is empty.\n";
         return;
     }

     for (const auto& [id, st] : students) {
         if (!st.estimates.empty()) {
             double sum = 0;
             for (int grade : st.estimates) {
                 sum += grade;
             }
             float avg = sum / st.estimates.size();
             ranking.emplace_back(id, st.name, avg);
         }
     }

     std::sort(ranking.begin(),ranking.end(), [](const auto& a, const auto& b){
         return std::get<2>(a) > std::get<2>(b);
     });

     std::cout << "--- Student Ranking ---\n";
     for (const auto& [id, name, avg] : ranking){
          std::cout << "ID: " << id << " | Name: " << name << " | Avg: " << avg << "\n";
     }

     std::cout << std::endl;

 }

 void SaveFile(const std::map<int, Student>& students){

     std::ofstream outFile("data.txt");

     if (outFile.is_open()) {
         for (const auto& [id, st] : students) {
             outFile << "ID: " << id << " " << st.name << " " << st.course << " " << st.age << " " << "Grade: ";
             for (unsigned short grade : st.estimates){
                 outFile << grade << " ";
             }

             outFile << '\n';
         }
         outFile.close();
         std::cout << "Data successfully written.\n";
     } else {
         std::cerr << "Error opening file!.\n";
     }
 }

 int LoadFile(std::map<int, Student>& students){

     std::ifstream inFile("data.txt");
     if (!inFile.is_open()){
         std::cerr << "File not found or could not be opened.\n";
         return 0;
     }

     students.clear();
     std::string dummy;
     unsigned int maxId= 0;

     std::string line;

     while(std::getline(inFile, line)){
         if (line.empty()){
             continue;
         }

         std::stringstream ss(line);
         Student st;

         ss >> dummy;
         ss >> st.id;
         ss >> st.name;
         ss >> st.course;
         ss >> st.age;
         ss >> dummy;

         unsigned short grade;
         while (ss >> grade){
             st.estimates.push_back(grade);
         }

         if (st.id > maxId){
             maxId = st.id;
         }

         students[st.id] = std::move(st);
     }

     inFile.close();
     std::cout << "Data loaded. Total students: " << students.size() << "\n";
     return maxId;
 }

int main(){
    std::map<int, Student> students;
    unsigned int id = 0;

    while (true) {
    std::cout << "1. Add a student\n";
    std::cout << "2. Remove student\n";
    std::cout << "3. Find a student\n";
    std::cout << "4. Show all students\n";
    std::cout << "5. Average scores\n";
    std::cout << "6. Course students\n";
    std::cout << "7. Best students\n";
    std::cout << "8. Save to file\n";
    std::cout << "9. Load from file\n";

    std::cout << std::endl;

    unsigned short choose;
    while(true){
    std::cout << "Select action: ";
        if (std::cin >> choose && choose >= 1 && choose <= 9){
        break;
        }
        std::cin.ignore();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error! Enter a number from 1 to 9.\n";
    }

    switch (choose) {
    case 1: {
        char UserChoice;
        do{

        AddStudent(students, id);

            std::cout << "Repeat? (y/n) ";
            std::cin >> UserChoice;


        }while (UserChoice == 'y');

        break;
    }
    case 2: {
        DeleteStudent(students);

        break;
    }

    case 3:{
        FindStudent(students);

        break;
    }

    case 4: {

        ShowStudent(students);

        break;

     }

    case 5: {

        AverageScores(students);

        break;
    }

    case 6: {

        StudentsCourse(students);

        break;
    }

    case 7: {

        BestStudents(students);

        break;
    }

    case 8: {

        SaveFile(students);

        break;
    }

    case 9: {

        std::cout << std::endl;

        id = LoadFile(students);

        std::cout << std::endl;

        break;
    }

    }

}

return 0;
 }
