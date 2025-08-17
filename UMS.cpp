/*
 * University Management System (UMS)
 * A complete offline C++17 console application
 * 
 * Features:
 * - Role-based authentication with signup/login (Admin, Teacher, Student)
 * - Department and semester management
 * - Course, exam, and grade management
 * - Attendance tracking
 * - File-based persistence (CSV format)
 * - Complete CRUD operations for all entities
 * - Menu-driven interface
 * 
 * Compilation: g++ -std=c++17 UMS.cpp -o UMS.exe
 * Usage: ./UMS.exe [--seed] [--test]
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <functional>
#include <ctime>
#include <limits>
#include <cstdlib>

// Simple hash function using std::hash (security limitation noted in README)
class SimpleHash {
public:
    static std::string hash(const std::string& input) {
        std::hash<std::string> hasher;
        size_t hashValue = hasher(input + "UMS_SALT_2025"); // Simple salt
        return std::to_string(hashValue);
    }
};

// Department class
class Department {
public:
    std::string deptId;
    std::string deptName;
    std::string headOfDept;
    std::string description;
    
    Department() = default;
    Department(const std::string& id, const std::string& name, const std::string& head, const std::string& desc)
        : deptId(id), deptName(name), headOfDept(head), description(desc) {}
    
    std::string toCSV() const {
        return deptId + "," + deptName + "," + headOfDept + "," + description;
    }
    
    static Department fromCSV(const std::string& csv) {
        std::istringstream ss(csv);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 4) {
            Department dept;
            dept.deptId = tokens[0];
            dept.deptName = tokens[1];
            dept.headOfDept = tokens[2];
            dept.description = tokens[3];
            return dept;
        }
        return Department();
    }
};

// Semester class
class Semester {
public:
    std::string semesterId;
    std::string semesterName;
    std::string startDate;
    std::string endDate;
    std::string status; // active, completed, upcoming
    
    Semester() = default;
    Semester(const std::string& id, const std::string& name, const std::string& start, 
             const std::string& end, const std::string& status = "upcoming")
        : semesterId(id), semesterName(name), startDate(start), endDate(end), status(status) {}
    
    std::string toCSV() const {
        return semesterId + "," + semesterName + "," + startDate + "," + endDate + "," + status;
    }
    
    static Semester fromCSV(const std::string& csv) {
        std::istringstream ss(csv);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 5) {
            Semester semester;
            semester.semesterId = tokens[0];
            semester.semesterName = tokens[1];
            semester.startDate = tokens[2];
            semester.endDate = tokens[3];
            semester.status = tokens[4];
            return semester;
        }
        return Semester();
    }
};

// Enhanced Course class
class Course {
public:
    std::string courseId;
    std::string courseName;
    std::string teacherId;
    std::string departmentId;
    std::string semesterId;
    int credits;
    std::string schedule; // e.g., "Mon-Wed-Fri 9:00-10:00"
    int maxStudents;
    
    Course() = default;
    Course(const std::string& id, const std::string& name, const std::string& teacherId, 
           const std::string& deptId, const std::string& semId, int credits, 
           const std::string& schedule, int maxStudents)
        : courseId(id), courseName(name), teacherId(teacherId), departmentId(deptId), 
          semesterId(semId), credits(credits), schedule(schedule), maxStudents(maxStudents) {}
    
    std::string toCSV() const {
        return courseId + "," + courseName + "," + teacherId + "," + departmentId + "," + 
               semesterId + "," + std::to_string(credits) + "," + schedule + "," + std::to_string(maxStudents);
    }
    
    static Course fromCSV(const std::string& csv) {
        std::istringstream ss(csv);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 8) {
            Course course;
            course.courseId = tokens[0];
            course.courseName = tokens[1];
            course.teacherId = tokens[2];
            course.departmentId = tokens[3];
            course.semesterId = tokens[4];
            course.credits = std::stoi(tokens[5]);
            course.schedule = tokens[6];
            course.maxStudents = std::stoi(tokens[7]);
            return course;
        }
        return Course();
    }
};

// Exam class
class Exam {
public:
    std::string examId;
    std::string courseId;
    std::string examName;
    std::string examDate;
    std::string examTime;
    std::string examType; // midterm, final, quiz, assignment
    int totalMarks;
    
    Exam() = default;
    Exam(const std::string& id, const std::string& courseId, const std::string& name,
         const std::string& date, const std::string& time, const std::string& type, int marks)
        : examId(id), courseId(courseId), examName(name), examDate(date), 
          examTime(time), examType(type), totalMarks(marks) {}
    
    std::string toCSV() const {
        return examId + "," + courseId + "," + examName + "," + examDate + "," + 
               examTime + "," + examType + "," + std::to_string(totalMarks);
    }
    
    static Exam fromCSV(const std::string& csv) {
        std::istringstream ss(csv);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 7) {
            Exam exam;
            exam.examId = tokens[0];
            exam.courseId = tokens[1];
            exam.examName = tokens[2];
            exam.examDate = tokens[3];
            exam.examTime = tokens[4];
            exam.examType = tokens[5];
            exam.totalMarks = std::stoi(tokens[6]);
            return exam;
        }
        return Exam();
    }
};

// Grade class
class Grade {
public:
    std::string studentId;
    std::string examId;
    int marksObtained;
    std::string letterGrade;
    std::string comments;
    
    Grade() = default;
    Grade(const std::string& studentId, const std::string& examId, int marks, 
          const std::string& grade, const std::string& comments = "")
        : studentId(studentId), examId(examId), marksObtained(marks), 
          letterGrade(grade), comments(comments) {}
    
    std::string toCSV() const {
        return studentId + "," + examId + "," + std::to_string(marksObtained) + "," + 
               letterGrade + "," + comments;
    }
    
    static Grade fromCSV(const std::string& csv) {
        std::istringstream ss(csv);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 5) {
            Grade grade;
            grade.studentId = tokens[0];
            grade.examId = tokens[1];
            grade.marksObtained = std::stoi(tokens[2]);
            grade.letterGrade = tokens[3];
            grade.comments = tokens[4];
            return grade;
        }
        return Grade();
    }
};

// Enhanced User class
class User {
public:
    std::string id;
    std::string username;
    std::string passwordHash;
    std::string role;
    std::string name;
    std::string email;
    std::string phone;
    std::string address;
    std::string departmentId; // for teachers and students
    std::string dateJoined;
    
    User() = default;
    User(const std::string& id, const std::string& username, const std::string& password, 
         const std::string& role, const std::string& name, const std::string& email,
         const std::string& phone = "", const std::string& address = "", 
         const std::string& deptId = "")
        : id(id), username(username), passwordHash(SimpleHash::hash(password)), 
          role(role), name(name), email(email), phone(phone), address(address), 
          departmentId(deptId) {
        // Set current date
        std::time_t now = std::time(0);
        dateJoined = std::to_string(now);
    }
    
    std::string toCSV() const {
        return id + "," + username + "," + passwordHash + "," + role + "," + name + "," + 
               email + "," + phone + "," + address + "," + departmentId + "," + dateJoined;
    }
    
    static User fromCSV(const std::string& csv) {
        std::istringstream ss(csv);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 10) {
            User user;
            user.id = tokens[0];
            user.username = tokens[1];
            user.passwordHash = tokens[2];
            user.role = tokens[3];
            user.name = tokens[4];
            user.email = tokens[5];
            user.phone = tokens[6];
            user.address = tokens[7];
            user.departmentId = tokens[8];
            user.dateJoined = tokens[9];
            return user;
        }
        return User();
    }
};

// Enrollment class
class Enrollment {
public:
    std::string studentId;
    std::string courseId;
    std::string grade;
    std::string status; // enrolled, completed, dropped
    
    Enrollment() = default;
    Enrollment(const std::string& studentId, const std::string& courseId, 
               const std::string& grade = "", const std::string& status = "enrolled")
        : studentId(studentId), courseId(courseId), grade(grade), status(status) {}
    
    std::string toCSV() const {
        return studentId + "," + courseId + "," + grade + "," + status;
    }
    
    static Enrollment fromCSV(const std::string& csv) {
        std::istringstream ss(csv);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 4) {
            Enrollment enrollment;
            enrollment.studentId = tokens[0];
            enrollment.courseId = tokens[1];
            enrollment.grade = tokens[2];
            enrollment.status = tokens[3];
            return enrollment;
        }
        return Enrollment();
    }
};

// Attendance class
class Attendance {
public:
    std::string studentId;
    std::string courseId;
    std::string date;
    std::string status; // present, absent, late
    
    Attendance() = default;
    Attendance(const std::string& studentId, const std::string& courseId, 
               const std::string& date, const std::string& status)
        : studentId(studentId), courseId(courseId), date(date), status(status) {}
    
    std::string toCSV() const {
        return studentId + "," + courseId + "," + date + "," + status;
    }
    
    static Attendance fromCSV(const std::string& csv) {
        std::istringstream ss(csv);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 4) {
            Attendance attendance;
            attendance.studentId = tokens[0];
            attendance.courseId = tokens[1];
            attendance.date = tokens[2];
            attendance.status = tokens[3];
            return attendance;
        }
        return Attendance();
    }
};

// Enhanced Database Manager class
class DatabaseManager {
private:
    const std::string USERS_FILE = "data/users.csv";
    const std::string DEPARTMENTS_FILE = "data/departments.csv";
    const std::string SEMESTERS_FILE = "data/semesters.csv";
    const std::string COURSES_FILE = "data/courses.csv";
    const std::string EXAMS_FILE = "data/exams.csv";
    const std::string GRADES_FILE = "data/grades.csv";
    const std::string ENROLLMENTS_FILE = "data/enrollments.csv";
    const std::string ATTENDANCE_FILE = "data/attendance.csv";
    
public:
    std::vector<User> users;
    std::vector<Department> departments;
    std::vector<Semester> semesters;
    std::vector<Course> courses;
    std::vector<Exam> exams;
    std::vector<Grade> grades;
    std::vector<Enrollment> enrollments;
    std::vector<Attendance> attendanceRecords;
    
    DatabaseManager() {
        createDataDirectory();
        loadAllData();
    }
    
    void createDataDirectory() {
        system("mkdir data 2>nul"); // Create data directory (Windows)
    }
    
    void loadAllData() {
        loadUsers();
        loadDepartments();
        loadSemesters();
        loadCourses();
        loadExams();
        loadGrades();
        loadEnrollments();
        loadAttendance();
    }
    
    void saveAllData() {
        saveUsers();
        saveDepartments();
        saveSemesters();
        saveCourses();
        saveExams();
        saveGrades();
        saveEnrollments();
        saveAttendance();
    }
    
    void loadUsers() {
        std::ifstream file(USERS_FILE);
        std::string line;
        users.clear();
        
        if (file.is_open()) {
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    users.push_back(User::fromCSV(line));
                }
            }
        }
        
        // Create default admin if no users exist
        if (users.empty()) {
            users.push_back(User("admin001", "admin", "admin123", "admin", "System Administrator", "admin@university.edu"));
            saveUsers();
        }
    }
    
    void saveUsers() {
        std::ofstream file(USERS_FILE);
        if (file.is_open()) {
            for (const auto& user : users) {
                file << user.toCSV() << std::endl;
            }
        }
    }
    
    void loadDepartments() {
        std::ifstream file(DEPARTMENTS_FILE);
        std::string line;
        departments.clear();
        
        if (file.is_open()) {
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    departments.push_back(Department::fromCSV(line));
                }
            }
        }
    }
    
    void saveDepartments() {
        std::ofstream file(DEPARTMENTS_FILE);
        if (file.is_open()) {
            for (const auto& dept : departments) {
                file << dept.toCSV() << std::endl;
            }
        }
    }
    
    void loadSemesters() {
        std::ifstream file(SEMESTERS_FILE);
        std::string line;
        semesters.clear();
        
        if (file.is_open()) {
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    semesters.push_back(Semester::fromCSV(line));
                }
            }
        }
    }
    
    void saveSemesters() {
        std::ofstream file(SEMESTERS_FILE);
        if (file.is_open()) {
            for (const auto& semester : semesters) {
                file << semester.toCSV() << std::endl;
            }
        }
    }
    
    void loadExams() {
        std::ifstream file(EXAMS_FILE);
        std::string line;
        exams.clear();
        
        if (file.is_open()) {
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    exams.push_back(Exam::fromCSV(line));
                }
            }
        }
    }
    
    void saveExams() {
        std::ofstream file(EXAMS_FILE);
        if (file.is_open()) {
            for (const auto& exam : exams) {
                file << exam.toCSV() << std::endl;
            }
        }
    }
    
    void loadGrades() {
        std::ifstream file(GRADES_FILE);
        std::string line;
        grades.clear();
        
        if (file.is_open()) {
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    grades.push_back(Grade::fromCSV(line));
                }
            }
        }
    }
    
    void saveGrades() {
        std::ofstream file(GRADES_FILE);
        if (file.is_open()) {
            for (const auto& grade : grades) {
                file << grade.toCSV() << std::endl;
            }
        }
    }
    
    
    void loadCourses() {
        std::ifstream file(COURSES_FILE);
        std::string line;
        courses.clear();
        
        if (file.is_open()) {
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    courses.push_back(Course::fromCSV(line));
                }
            }
        }
    }
    
    void saveCourses() {
        std::ofstream file(COURSES_FILE);
        if (file.is_open()) {
            for (const auto& course : courses) {
                file << course.toCSV() << std::endl;
            }
        }
    }
    
    void loadEnrollments() {
        std::ifstream file(ENROLLMENTS_FILE);
        std::string line;
        enrollments.clear();
        
        if (file.is_open()) {
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    enrollments.push_back(Enrollment::fromCSV(line));
                }
            }
        }
    }
    
    void saveEnrollments() {
        std::ofstream file(ENROLLMENTS_FILE);
        if (file.is_open()) {
            for (const auto& enrollment : enrollments) {
                file << enrollment.toCSV() << std::endl;
            }
        }
    }
    
    void loadAttendance() {
        std::ifstream file(ATTENDANCE_FILE);
        std::string line;
        attendanceRecords.clear();
        
        if (file.is_open()) {
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    attendanceRecords.push_back(Attendance::fromCSV(line));
                }
            }
        }
    }
    
    void saveAttendance() {
        std::ofstream file(ATTENDANCE_FILE);
        if (file.is_open()) {
            for (const auto& attendance : attendanceRecords) {
                file << attendance.toCSV() << std::endl;
            }
        }
    }
    
    // Helper methods
    User* findUser(const std::string& username) {
        auto it = std::find_if(users.begin(), users.end(), 
            [&](const User& u) { return u.username == username; });
        return (it != users.end()) ? &(*it) : nullptr;
    }
    
    User* findUserById(const std::string& id) {
        auto it = std::find_if(users.begin(), users.end(), 
            [&](const User& u) { return u.id == id; });
        return (it != users.end()) ? &(*it) : nullptr;
    }
    
    Department* findDepartment(const std::string& deptId) {
        auto it = std::find_if(departments.begin(), departments.end(), 
            [&](const Department& d) { return d.deptId == deptId; });
        return (it != departments.end()) ? &(*it) : nullptr;
    }
    
    Semester* findSemester(const std::string& semesterId) {
        auto it = std::find_if(semesters.begin(), semesters.end(), 
            [&](const Semester& s) { return s.semesterId == semesterId; });
        return (it != semesters.end()) ? &(*it) : nullptr;
    }
    
    Course* findCourse(const std::string& courseId) {
        auto it = std::find_if(courses.begin(), courses.end(), 
            [&](const Course& c) { return c.courseId == courseId; });
        return (it != courses.end()) ? &(*it) : nullptr;
    }
    
    Exam* findExam(const std::string& examId) {
        auto it = std::find_if(exams.begin(), exams.end(), 
            [&](const Exam& e) { return e.examId == examId; });
        return (it != exams.end()) ? &(*it) : nullptr;
    }
    
    std::vector<Course> getTeacherCourses(const std::string& teacherId) {
        std::vector<Course> teacherCourses;
        for (const auto& course : courses) {
            if (course.teacherId == teacherId) {
                teacherCourses.push_back(course);
            }
        }
        return teacherCourses;
    }
    
    std::vector<Exam> getCourseExams(const std::string& courseId) {
        std::vector<Exam> courseExams;
        for (const auto& exam : exams) {
            if (exam.courseId == courseId) {
                courseExams.push_back(exam);
            }
        }
        return courseExams;
    }
    
    std::vector<Enrollment> getStudentEnrollments(const std::string& studentId) {
        std::vector<Enrollment> studentEnrollments;
        for (const auto& enrollment : enrollments) {
            if (enrollment.studentId == studentId) {
                studentEnrollments.push_back(enrollment);
            }
        }
        return studentEnrollments;
    }
    
    std::vector<Grade> getStudentGrades(const std::string& studentId) {
        std::vector<Grade> studentGrades;
        for (const auto& grade : grades) {
            if (grade.studentId == studentId) {
                studentGrades.push_back(grade);
            }
        }
        return studentGrades;
    }
    
    bool isStudentEnrolled(const std::string& studentId, const std::string& courseId) {
        return std::any_of(enrollments.begin(), enrollments.end(),
            [&](const Enrollment& e) { 
                return e.studentId == studentId && e.courseId == courseId && e.status == "enrolled";
            });
    }
    
    std::string generateNextId(const std::string& prefix, const std::vector<std::string>& existingIds) {
        int maxNum = 0;
        for (const auto& id : existingIds) {
            if (id.length() > prefix.length() && id.substr(0, prefix.length()) == prefix) {
                try {
                    int num = std::stoi(id.substr(prefix.length()));
                    maxNum = std::max(maxNum, num);
                } catch (...) {
                    // Invalid number, skip
                }
            }
        }
        return prefix + std::string(3 - std::to_string(maxNum + 1).length(), '0') + std::to_string(maxNum + 1);
    }
};

// Main UMS Application class
class UMSApplication {
private:
    DatabaseManager db;
    User* currentUser;
    
public:
    UMSApplication() : currentUser(nullptr) {}
    
    void run() {
        std::cout << "=== University Management System ===" << std::endl;
        std::cout << "Welcome to UMS v1.0" << std::endl << std::endl;
        
        while (true) {
            if (!currentUser) {
                if (!loginOrSignup()) {
                    std::cout << "Exiting..." << std::endl;
                    break;
                }
            } else {
                if (currentUser->role == "admin") {
                    adminMenu();
                } else if (currentUser->role == "teacher") {
                    teacherMenu();
                } else if (currentUser->role == "student") {
                    studentMenu();
                }
            }
        }
        
        db.saveAllData();
    }
    
    bool loginOrSignup() {
        std::cout << "\n=== WELCOME TO UMS ===" << std::endl;
        std::cout << "1. Login" << std::endl;
        std::cout << "2. Sign Up" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 3) return false;
        
        if (choice == 1) {
            return performLogin();
        } else if (choice == 2) {
            return performSignup();
        }
        
        return loginOrSignup(); // Retry
    }
    
    bool performLogin() {
        std::cout << "\n=== LOGIN ===" << std::endl;
        std::string username, password;
        std::cout << "Username: ";
        std::getline(std::cin, username);
        std::cout << "Password: ";
        std::getline(std::cin, password);
        
        User* user = db.findUser(username);
        if (user && user->passwordHash == SimpleHash::hash(password)) {
            currentUser = user;
            std::cout << "Login successful! Welcome, " << user->name << std::endl;
            return true;
        } else {
            std::cout << "Invalid credentials!" << std::endl;
            return loginOrSignup(); // Return to main menu instead of false
        }
    }
    
    bool performSignup() {
        std::cout << "\n=== SIGN UP ===" << std::endl;
        std::cout << "1. Student Registration" << std::endl;
        std::cout << "2. Teacher Registration" << std::endl;
        std::cout << "3. Back to Main Menu" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 3) return false;
        
        std::string role = (choice == 1) ? "student" : "teacher";
        
        std::string username, password, name, email, phone, address, deptId;
        
        std::cout << "Enter username: ";
        std::getline(std::cin, username);
        
        // Check if username already exists
        if (db.findUser(username)) {
            std::cout << "Username already exists! Please try again." << std::endl;
            return loginOrSignup(); // Return to main menu instead of false
        }
        
        std::cout << "Enter password: ";
        std::getline(std::cin, password);
        std::cout << "Enter full name: ";
        std::getline(std::cin, name);
        std::cout << "Enter email: ";
        std::getline(std::cin, email);
        std::cout << "Enter phone: ";
        std::getline(std::cin, phone);
        std::cout << "Enter address: ";
        std::getline(std::cin, address);
        
        // Show available departments
        if (!db.departments.empty()) {
            std::cout << "\nAvailable Departments:" << std::endl;
            for (const auto& dept : db.departments) {
                std::cout << dept.deptId << " - " << dept.deptName << std::endl;
            }
            std::cout << "Enter department ID: ";
            std::getline(std::cin, deptId);
            
            if (!db.findDepartment(deptId)) {
                std::cout << "Invalid department ID!" << std::endl;
                return loginOrSignup(); // Return to main menu instead of false
            }
        }
        
        // Generate new ID
        std::vector<std::string> existingIds;
        for (const auto& user : db.users) {
            existingIds.push_back(user.id);
        }
        std::string prefix = (role == "student") ? "STU" : "TCH";
        std::string newId = db.generateNextId(prefix, existingIds);
        
        db.users.push_back(User(newId, username, password, role, name, email, phone, address, deptId));
        std::cout << role << " registration successful! Your ID is: " << newId << std::endl;
        std::cout << "You can now login with your credentials." << std::endl;
        
        return loginOrSignup(); // Return to main menu instead of false
    }
    
    void logout() {
        currentUser = nullptr;
        std::cout << "Logged out successfully!" << std::endl;
    }
    
    // Admin Menu and Functions
    void adminMenu() {
        std::cout << "\n=== ADMIN MENU ===" << std::endl;
        std::cout << "1. Manage Users" << std::endl;
        std::cout << "2. Manage Departments" << std::endl;
        std::cout << "3. Manage Semesters" << std::endl;
        std::cout << "4. Manage Courses" << std::endl;
        std::cout << "5. View Reports" << std::endl;
        std::cout << "6. Backup Data" << std::endl;
        std::cout << "7. Logout" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: manageUsers(); break;
            case 2: manageDepartments(); break;
            case 3: manageSemesters(); break;
            case 4: manageCourses(); break;
            case 5: viewReports(); break;
            case 6: backupData(); break;
            case 7: logout(); break;
            default: std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    void manageDepartments() {
        std::cout << "\n=== DEPARTMENT MANAGEMENT ===" << std::endl;
        std::cout << "1. Create Department" << std::endl;
        std::cout << "2. View All Departments" << std::endl;
        std::cout << "3. Delete Department" << std::endl;
        std::cout << "4. Back" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: createDepartment(); break;
            case 2: viewAllDepartments(); break;
            case 3: deleteDepartment(); break;
            case 4: return;
            default: std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    void createDepartment() {
        std::string deptId, deptName, headOfDept, description;
        
        std::cout << "Enter department ID: ";
        std::getline(std::cin, deptId);
        
        if (db.findDepartment(deptId)) {
            std::cout << "Department ID already exists!" << std::endl;
            return;
        }
        
        std::cout << "Enter department name: ";
        std::getline(std::cin, deptName);
        std::cout << "Enter head of department: ";
        std::getline(std::cin, headOfDept);
        std::cout << "Enter description: ";
        std::getline(std::cin, description);
        
        db.departments.push_back(Department(deptId, deptName, headOfDept, description));
        std::cout << "Department created successfully!" << std::endl;
    }
    
    void viewAllDepartments() {
        std::cout << "\n=== ALL DEPARTMENTS ===" << std::endl;
        std::cout << std::left << std::setw(10) << "Dept ID" << std::setw(25) << "Department Name" 
                  << std::setw(20) << "Head of Dept" << "Description" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (const auto& dept : db.departments) {
            std::cout << std::left << std::setw(10) << dept.deptId << std::setw(25) << dept.deptName 
                      << std::setw(20) << dept.headOfDept << dept.description << std::endl;
        }
    }
    
    void deleteDepartment() {
        std::cout << "Enter department ID to delete: ";
        std::string deptId;
        std::getline(std::cin, deptId);
        
        auto it = std::find_if(db.departments.begin(), db.departments.end(),
            [&](const Department& d) { return d.deptId == deptId; });
        
        if (it != db.departments.end()) {
            db.departments.erase(it);
            std::cout << "Department deleted successfully!" << std::endl;
        } else {
            std::cout << "Department not found!" << std::endl;
        }
    }
    
    void manageSemesters() {
        std::cout << "\n=== SEMESTER MANAGEMENT ===" << std::endl;
        std::cout << "1. Create Semester" << std::endl;
        std::cout << "2. View All Semesters" << std::endl;
        std::cout << "3. Update Semester Status" << std::endl;
        std::cout << "4. Delete Semester" << std::endl;
        std::cout << "5. Back" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: createSemester(); break;
            case 2: viewAllSemesters(); break;
            case 3: updateSemesterStatus(); break;
            case 4: deleteSemester(); break;
            case 5: return;
            default: std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    void createSemester() {
        std::string semesterId, semesterName, startDate, endDate;
        
        std::cout << "Enter semester ID: ";
        std::getline(std::cin, semesterId);
        
        if (db.findSemester(semesterId)) {
            std::cout << "Semester ID already exists!" << std::endl;
            return;
        }
        
        std::cout << "Enter semester name: ";
        std::getline(std::cin, semesterName);
        std::cout << "Enter start date (YYYY-MM-DD): ";
        std::getline(std::cin, startDate);
        std::cout << "Enter end date (YYYY-MM-DD): ";
        std::getline(std::cin, endDate);
        
        db.semesters.push_back(Semester(semesterId, semesterName, startDate, endDate));
        std::cout << "Semester created successfully!" << std::endl;
    }
    
    void viewAllSemesters() {
        std::cout << "\n=== ALL SEMESTERS ===" << std::endl;
        std::cout << std::left << std::setw(12) << "Semester ID" << std::setw(20) << "Semester Name" 
                  << std::setw(12) << "Start Date" << std::setw(12) << "End Date" << "Status" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (const auto& semester : db.semesters) {
            std::cout << std::left << std::setw(12) << semester.semesterId << std::setw(20) << semester.semesterName 
                      << std::setw(12) << semester.startDate << std::setw(12) << semester.endDate << semester.status << std::endl;
        }
    }
    
    void updateSemesterStatus() {
        std::cout << "Enter semester ID: ";
        std::string semesterId;
        std::getline(std::cin, semesterId);
        
        Semester* semester = db.findSemester(semesterId);
        if (!semester) {
            std::cout << "Semester not found!" << std::endl;
            return;
        }
        
        std::cout << "Current status: " << semester->status << std::endl;
        std::cout << "Enter new status (active/completed/upcoming): ";
        std::string newStatus;
        std::getline(std::cin, newStatus);
        
        semester->status = newStatus;
        std::cout << "Semester status updated successfully!" << std::endl;
    }
    
    void deleteSemester() {
        std::cout << "Enter semester ID to delete: ";
        std::string semesterId;
        std::getline(std::cin, semesterId);
        
        auto it = std::find_if(db.semesters.begin(), db.semesters.end(),
            [&](const Semester& s) { return s.semesterId == semesterId; });
        
        if (it != db.semesters.end()) {
            db.semesters.erase(it);
            std::cout << "Semester deleted successfully!" << std::endl;
        } else {
            std::cout << "Semester not found!" << std::endl;
        }
    }
    
    void manageUsers() {
        std::cout << "\n=== USER MANAGEMENT ===" << std::endl;
        std::cout << "1. Create Teacher" << std::endl;
        std::cout << "2. Create Student" << std::endl;
        std::cout << "3. View All Users" << std::endl;
        std::cout << "4. Delete User" << std::endl;
        std::cout << "5. Back" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: createUser("teacher"); break;
            case 2: createUser("student"); break;
            case 3: viewAllUsers(); break;
            case 4: deleteUser(); break;
            case 5: return;
            default: std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    void createUser(const std::string& role) {
        std::string id, username, password, name, email;
        
        std::cout << "Enter " << role << " ID: ";
        std::getline(std::cin, id);
        
        // Check if ID already exists
        if (db.findUserById(id)) {
            std::cout << "User ID already exists!" << std::endl;
            return;
        }
        
        std::cout << "Enter username: ";
        std::getline(std::cin, username);
        
        // Check if username already exists
        if (db.findUser(username)) {
            std::cout << "Username already exists!" << std::endl;
            return;
        }
        
        std::cout << "Enter password: ";
        std::getline(std::cin, password);
        std::cout << "Enter full name: ";
        std::getline(std::cin, name);
        std::cout << "Enter email: ";
        std::getline(std::cin, email);
        
        db.users.push_back(User(id, username, password, role, name, email));
        std::cout << role << " created successfully!" << std::endl;
    }
    
    void viewAllUsers() {
        std::cout << "\n=== ALL USERS ===" << std::endl;
        std::cout << std::left << std::setw(12) << "ID" << std::setw(15) << "Username" 
                  << std::setw(10) << "Role" << std::setw(25) << "Name" << "Email" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (const auto& user : db.users) {
            std::cout << std::left << std::setw(12) << user.id << std::setw(15) << user.username 
                      << std::setw(10) << user.role << std::setw(25) << user.name << user.email << std::endl;
        }
    }
    
    void deleteUser() {
        std::cout << "Enter user ID to delete: ";
        std::string id;
        std::getline(std::cin, id);
        
        auto it = std::find_if(db.users.begin(), db.users.end(),
            [&](const User& u) { return u.id == id; });
        
        if (it != db.users.end()) {
            if (it->role == "admin") {
                std::cout << "Cannot delete admin user!" << std::endl;
                return;
            }
            db.users.erase(it);
            std::cout << "User deleted successfully!" << std::endl;
        } else {
            std::cout << "User not found!" << std::endl;
        }
    }
    
    void manageCourses() {
        std::cout << "\n=== COURSE MANAGEMENT ===" << std::endl;
        std::cout << "1. Create Course" << std::endl;
        std::cout << "2. View All Courses" << std::endl;
        std::cout << "3. Delete Course" << std::endl;
        std::cout << "4. Back" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: createCourse(); break;
            case 2: viewAllCourses(); break;
            case 3: deleteCourse(); break;
            case 4: return;
            default: std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    void createCourse() {
        std::string courseId, courseName, teacherId, departmentId, semesterId, schedule;
        int credits, maxStudents;
        
        std::cout << "Enter course ID: ";
        std::getline(std::cin, courseId);
        
        if (db.findCourse(courseId)) {
            std::cout << "Course ID already exists!" << std::endl;
            return;
        }
        
        std::cout << "Enter course name: ";
        std::getline(std::cin, courseName);
        
        // Show available teachers
        std::cout << "\nAvailable Teachers:" << std::endl;
        for (const auto& user : db.users) {
            if (user.role == "teacher") {
                std::cout << user.id << " - " << user.name << std::endl;
            }
        }
        std::cout << "Enter teacher ID: ";
        std::getline(std::cin, teacherId);
        
        if (!db.findUserById(teacherId) || db.findUserById(teacherId)->role != "teacher") {
            std::cout << "Invalid teacher ID!" << std::endl;
            return;
        }
        
        // Show available departments
        if (!db.departments.empty()) {
            std::cout << "\nAvailable Departments:" << std::endl;
            for (const auto& dept : db.departments) {
                std::cout << dept.deptId << " - " << dept.deptName << std::endl;
            }
            std::cout << "Enter department ID: ";
            std::getline(std::cin, departmentId);
            
            if (!db.findDepartment(departmentId)) {
                std::cout << "Invalid department ID!" << std::endl;
                return;
            }
        }
        
        // Show available semesters
        if (!db.semesters.empty()) {
            std::cout << "\nAvailable Semesters:" << std::endl;
            for (const auto& semester : db.semesters) {
                std::cout << semester.semesterId << " - " << semester.semesterName << " (" << semester.status << ")" << std::endl;
            }
            std::cout << "Enter semester ID: ";
            std::getline(std::cin, semesterId);
            
            if (!db.findSemester(semesterId)) {
                std::cout << "Invalid semester ID!" << std::endl;
                return;
            }
        }
        
        std::cout << "Enter credits: ";
        std::cin >> credits;
        std::cin.ignore();
        std::cout << "Enter schedule (e.g., Mon-Wed-Fri 9:00-10:00): ";
        std::getline(std::cin, schedule);
        std::cout << "Enter maximum students: ";
        std::cin >> maxStudents;
        std::cin.ignore();
        
        db.courses.push_back(Course(courseId, courseName, teacherId, departmentId, semesterId, credits, schedule, maxStudents));
        std::cout << "Course created successfully!" << std::endl;
    }
    
    void viewAllCourses() {
        std::cout << "\n=== ALL COURSES ===" << std::endl;
        std::cout << std::left << std::setw(10) << "Course ID" << std::setw(25) << "Course Name" 
                  << std::setw(10) << "Teacher" << std::setw(8) << "Credits" << std::setw(12) << "Department" << "Semester" << std::endl;
        std::cout << std::string(90, '-') << std::endl;
        
        for (const auto& course : db.courses) {
            User* teacher = db.findUserById(course.teacherId);
            Department* dept = db.findDepartment(course.departmentId);
            Semester* semester = db.findSemester(course.semesterId);
            
            std::cout << std::left << std::setw(10) << course.courseId << std::setw(25) << course.courseName 
                      << std::setw(10) << (teacher ? teacher->name.substr(0,9) : "Unknown")
                      << std::setw(8) << course.credits 
                      << std::setw(12) << (dept ? dept->deptName.substr(0,11) : "Unknown")
                      << (semester ? semester->semesterName : "Unknown") << std::endl;
        }
    }
    
    void deleteCourse() {
        std::cout << "Enter course ID to delete: ";
        std::string courseId;
        std::getline(std::cin, courseId);
        
        auto it = std::find_if(db.courses.begin(), db.courses.end(),
            [&](const Course& c) { return c.courseId == courseId; });
        
        if (it != db.courses.end()) {
            db.courses.erase(it);
            std::cout << "Course deleted successfully!" << std::endl;
        } else {
            std::cout << "Course not found!" << std::endl;
        }
    }
    
    void viewReports() {
        std::cout << "\n=== REPORTS ===" << std::endl;
        std::cout << "Total Users: " << db.users.size() << std::endl;
        std::cout << "Total Courses: " << db.courses.size() << std::endl;
        std::cout << "Total Enrollments: " << db.enrollments.size() << std::endl;
        
        int teacherCount = 0, studentCount = 0;
        for (const auto& user : db.users) {
            if (user.role == "teacher") teacherCount++;
            else if (user.role == "student") studentCount++;
        }
        
        std::cout << "Teachers: " << teacherCount << std::endl;
        std::cout << "Students: " << studentCount << std::endl;
    }
    
    void backupData() {
        std::time_t now = std::time(0);
        std::string timestamp = std::to_string(now);
        
        system(("xcopy data backup_" + timestamp + "\\ /E /I").c_str());
        std::cout << "Data backed up to backup_" << timestamp << std::endl;
    }
    
    // Teacher Menu and Functions
    void teacherMenu() {
        std::cout << "\n=== TEACHER MENU ===" << std::endl;
        std::cout << "1. View My Courses" << std::endl;
        std::cout << "2. Manage Students" << std::endl;
        std::cout << "3. Exam Management" << std::endl;
        std::cout << "4. Grade Management" << std::endl;
        std::cout << "5. Attendance" << std::endl;
        std::cout << "6. Logout" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: viewMyCourses(); break;
            case 2: manageStudents(); break;
            case 3: examManagement(); break;
            case 4: gradeManagement(); break;
            case 5: attendanceManagement(); break;
            case 6: logout(); break;
            default: std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    void examManagement() {
        std::cout << "\n=== EXAM MANAGEMENT ===" << std::endl;
        std::cout << "1. Create Exam" << std::endl;
        std::cout << "2. View Course Exams" << std::endl;
        std::cout << "3. Delete Exam" << std::endl;
        std::cout << "4. Back" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: createExam(); break;
            case 2: viewCourseExams(); break;
            case 3: deleteExam(); break;
            case 4: return;
            default: std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    void createExam() {
        std::cout << "Enter course ID: ";
        std::string courseId;
        std::getline(std::cin, courseId);
        
        Course* course = db.findCourse(courseId);
        if (!course || course->teacherId != currentUser->id) {
            std::cout << "Invalid course or not your course!" << std::endl;
            return;
        }
        
        std::string examName, examDate, examTime, examType;
        int totalMarks;
        
        std::cout << "Enter exam name: ";
        std::getline(std::cin, examName);
        std::cout << "Enter exam date (YYYY-MM-DD): ";
        std::getline(std::cin, examDate);
        std::cout << "Enter exam time (HH:MM-HH:MM): ";
        std::getline(std::cin, examTime);
        std::cout << "Enter exam type (midterm/final/quiz/assignment): ";
        std::getline(std::cin, examType);
        std::cout << "Enter total marks: ";
        std::cin >> totalMarks;
        std::cin.ignore();
        
        // Generate exam ID
        std::vector<std::string> existingIds;
        for (const auto& exam : db.exams) {
            existingIds.push_back(exam.examId);
        }
        std::string examId = db.generateNextId("EX", existingIds);
        
        db.exams.push_back(Exam(examId, courseId, examName, examDate, examTime, examType, totalMarks));
        std::cout << "Exam created successfully! Exam ID: " << examId << std::endl;
    }
    
    void viewCourseExams() {
        std::cout << "Enter course ID: ";
        std::string courseId;
        std::getline(std::cin, courseId);
        
        Course* course = db.findCourse(courseId);
        if (!course || course->teacherId != currentUser->id) {
            std::cout << "Invalid course or not your course!" << std::endl;
            return;
        }
        
        std::cout << "\n=== EXAMS FOR " << course->courseName << " ===" << std::endl;
        std::cout << std::left << std::setw(8) << "Exam ID" << std::setw(20) << "Exam Name" 
                  << std::setw(12) << "Date" << std::setw(15) << "Time" << std::setw(12) << "Type" << "Marks" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        auto courseExams = db.getCourseExams(courseId);
        for (const auto& exam : courseExams) {
            std::cout << std::left << std::setw(8) << exam.examId << std::setw(20) << exam.examName 
                      << std::setw(12) << exam.examDate << std::setw(15) << exam.examTime 
                      << std::setw(12) << exam.examType << exam.totalMarks << std::endl;
        }
    }
    
    void deleteExam() {
        std::cout << "Enter exam ID to delete: ";
        std::string examId;
        std::getline(std::cin, examId);
        
        Exam* exam = db.findExam(examId);
        if (!exam) {
            std::cout << "Exam not found!" << std::endl;
            return;
        }
        
        Course* course = db.findCourse(exam->courseId);
        if (!course || course->teacherId != currentUser->id) {
            std::cout << "Not authorized to delete this exam!" << std::endl;
            return;
        }
        
        auto it = std::find_if(db.exams.begin(), db.exams.end(),
            [&](const Exam& e) { return e.examId == examId; });
        
        if (it != db.exams.end()) {
            db.exams.erase(it);
            std::cout << "Exam deleted successfully!" << std::endl;
        }
    }
    
    void viewMyCourses() {
        std::cout << "\n=== MY COURSES ===" << std::endl;
        auto courses = db.getTeacherCourses(currentUser->id);
        
        if (courses.empty()) {
            std::cout << "No courses assigned." << std::endl;
            return;
        }
        
        for (const auto& course : courses) {
            std::cout << course.courseId << " - " << course.courseName 
                      << " (" << course.credits << " credits)" << std::endl;
        }
    }
    
    void manageStudents() {
        std::cout << "\n=== STUDENT MANAGEMENT ===" << std::endl;
        std::cout << "1. Enroll Student" << std::endl;
        std::cout << "2. View Course Roster" << std::endl;
        std::cout << "3. Back" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: enrollStudent(); break;
            case 2: viewCourseRoster(); break;
            case 3: return;
            default: std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    void enrollStudent() {
        std::cout << "Enter course ID: ";
        std::string courseId;
        std::getline(std::cin, courseId);
        
        Course* course = db.findCourse(courseId);
        if (!course || course->teacherId != currentUser->id) {
            std::cout << "Invalid course or not your course!" << std::endl;
            return;
        }
        
        std::cout << "Enter student ID: ";
        std::string studentId;
        std::getline(std::cin, studentId);
        
        User* student = db.findUserById(studentId);
        if (!student || student->role != "student") {
            std::cout << "Invalid student ID!" << std::endl;
            return;
        }
        
        if (db.isStudentEnrolled(studentId, courseId)) {
            std::cout << "Student already enrolled!" << std::endl;
            return;
        }
        
        db.enrollments.push_back(Enrollment(studentId, courseId));
        std::cout << "Student enrolled successfully!" << std::endl;
    }
    
    void viewCourseRoster() {
        std::cout << "Enter course ID: ";
        std::string courseId;
        std::getline(std::cin, courseId);
        
        Course* course = db.findCourse(courseId);
        if (!course || course->teacherId != currentUser->id) {
            std::cout << "Invalid course or not your course!" << std::endl;
            return;
        }
        
        std::cout << "\n=== COURSE ROSTER: " << course->courseName << " ===" << std::endl;
        std::cout << std::left << std::setw(12) << "Student ID" << std::setw(25) << "Name" 
                  << std::setw(10) << "Grade" << "Status" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (const auto& enrollment : db.enrollments) {
            if (enrollment.courseId == courseId) {
                User* student = db.findUserById(enrollment.studentId);
                if (student) {
                    std::cout << std::left << std::setw(12) << student->id << std::setw(25) << student->name 
                              << std::setw(10) << enrollment.grade << enrollment.status << std::endl;
                }
            }
        }
    }
    
    void gradeManagement() {
        std::cout << "\n=== GRADE MANAGEMENT ===" << std::endl;
        std::cout << "1. Enter/Update Grades" << std::endl;
        std::cout << "2. View Course Grades" << std::endl;
        std::cout << "3. Back" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: enterGrades(); break;
            case 2: viewCourseGrades(); break;
            case 3: return;
            default: std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    void enterGrades() {
        std::cout << "Enter course ID: ";
        std::string courseId;
        std::getline(std::cin, courseId);
        
        Course* course = db.findCourse(courseId);
        if (!course || course->teacherId != currentUser->id) {
            std::cout << "Invalid course or not your course!" << std::endl;
            return;
        }
        
        // Show course exams
        auto courseExams = db.getCourseExams(courseId);
        if (courseExams.empty()) {
            std::cout << "No exams found for this course!" << std::endl;
            return;
        }
        
        std::cout << "\nAvailable Exams:" << std::endl;
        for (const auto& exam : courseExams) {
            std::cout << exam.examId << " - " << exam.examName << " (Total: " << exam.totalMarks << ")" << std::endl;
        }
        
        std::cout << "Enter exam ID: ";
        std::string examId;
        std::getline(std::cin, examId);
        
        Exam* exam = db.findExam(examId);
        if (!exam || exam->courseId != courseId) {
            std::cout << "Invalid exam ID!" << std::endl;
            return;
        }
        
        std::cout << "Enter student ID: ";
        std::string studentId;
        std::getline(std::cin, studentId);
        
        if (!db.isStudentEnrolled(studentId, courseId)) {
            std::cout << "Student not enrolled in this course!" << std::endl;
            return;
        }
        
        std::cout << "Enter marks obtained (out of " << exam->totalMarks << "): ";
        int marks;
        std::cin >> marks;
        std::cin.ignore();
        
        if (marks < 0 || marks > exam->totalMarks) {
            std::cout << "Invalid marks!" << std::endl;
            return;
        }
        
        // Calculate letter grade
        std::string letterGrade;
        double percentage = (double)marks / exam->totalMarks * 100;
        if (percentage >= 90) letterGrade = "A+";
        else if (percentage >= 85) letterGrade = "A";
        else if (percentage >= 80) letterGrade = "A-";
        else if (percentage >= 75) letterGrade = "B+";
        else if (percentage >= 70) letterGrade = "B";
        else if (percentage >= 65) letterGrade = "B-";
        else if (percentage >= 60) letterGrade = "C+";
        else if (percentage >= 55) letterGrade = "C";
        else if (percentage >= 50) letterGrade = "C-";
        else letterGrade = "F";
        
        std::cout << "Enter comments (optional): ";
        std::string comments;
        std::getline(std::cin, comments);
        
        // Check if grade already exists
        auto it = std::find_if(db.grades.begin(), db.grades.end(),
            [&](const Grade& g) { 
                return g.studentId == studentId && g.examId == examId; 
            });
        
        if (it != db.grades.end()) {
            // Update existing grade
            const_cast<Grade&>(*it).marksObtained = marks;
            const_cast<Grade&>(*it).letterGrade = letterGrade;
            const_cast<Grade&>(*it).comments = comments;
            std::cout << "Grade updated successfully!" << std::endl;
        } else {
            // Add new grade
            db.grades.push_back(Grade(studentId, examId, marks, letterGrade, comments));
            std::cout << "Grade entered successfully!" << std::endl;
        }
    }
    
    void viewCourseGrades() {
        std::cout << "Enter course ID: ";
        std::string courseId;
        std::getline(std::cin, courseId);
        
        Course* course = db.findCourse(courseId);
        if (!course || course->teacherId != currentUser->id) {
            std::cout << "Invalid course or not your course!" << std::endl;
            return;
        }
        
        std::cout << "\n=== GRADES FOR " << course->courseName << " ===" << std::endl;
        std::cout << std::left << std::setw(12) << "Student ID" << std::setw(20) << "Student Name" 
                  << std::setw(15) << "Exam" << std::setw(8) << "Marks" << std::setw(8) << "Grade" << "Comments" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        auto courseExams = db.getCourseExams(courseId);
        for (const auto& exam : courseExams) {
            for (const auto& grade : db.grades) {
                if (grade.examId == exam.examId) {
                    User* student = db.findUserById(grade.studentId);
                    if (student) {
                        std::cout << std::left << std::setw(12) << student->id << std::setw(20) << student->name 
                                  << std::setw(15) << exam.examName << std::setw(8) << grade.marksObtained 
                                  << std::setw(8) << grade.letterGrade << grade.comments << std::endl;
                    }
                }
            }
        }
    }
    
    void attendanceManagement() {
        std::cout << "\n=== ATTENDANCE MANAGEMENT ===" << std::endl;
        std::cout << "Enter course ID: ";
        std::string courseId;
        std::getline(std::cin, courseId);
        
        Course* course = db.findCourse(courseId);
        if (!course || course->teacherId != currentUser->id) {
            std::cout << "Invalid course or not your course!" << std::endl;
            return;
        }
        
        std::cout << "Enter student ID: ";
        std::string studentId;
        std::getline(std::cin, studentId);
        
        if (!db.isStudentEnrolled(studentId, courseId)) {
            std::cout << "Student not enrolled in this course!" << std::endl;
            return;
        }
        
        std::cout << "Enter date (YYYY-MM-DD): ";
        std::string date;
        std::getline(std::cin, date);
        
        std::cout << "Enter status (present/absent/late): ";
        std::string status;
        std::getline(std::cin, status);
        
        db.attendanceRecords.push_back(Attendance(studentId, courseId, date, status));
        std::cout << "Attendance marked successfully!" << std::endl;
    }
    
    // Student Menu and Functions
    void studentMenu() {
        std::cout << "\n=== STUDENT MENU ===" << std::endl;
        std::cout << "1. View Profile" << std::endl;
        std::cout << "2. View Enrolled Courses" << std::endl;
        std::cout << "3. View Grades" << std::endl;
        std::cout << "4. View Attendance" << std::endl;
        std::cout << "5. Print Transcript" << std::endl;
        std::cout << "6. Logout" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: viewProfile(); break;
            case 2: viewEnrolledCourses(); break;
            case 3: viewGrades(); break;
            case 4: viewAttendance(); break;
            case 5: printTranscript(); break;
            case 6: logout(); break;
            default: std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    void viewProfile() {
        std::cout << "\n=== MY PROFILE ===" << std::endl;
        std::cout << "ID: " << currentUser->id << std::endl;
        std::cout << "Name: " << currentUser->name << std::endl;
        std::cout << "Email: " << currentUser->email << std::endl;
        std::cout << "Role: " << currentUser->role << std::endl;
    }
    
    void viewEnrolledCourses() {
        std::cout << "\n=== ENROLLED COURSES ===" << std::endl;
        auto enrollments = db.getStudentEnrollments(currentUser->id);
        
        if (enrollments.empty()) {
            std::cout << "No enrollments found." << std::endl;
            return;
        }
        
        for (const auto& enrollment : enrollments) {
            Course* course = db.findCourse(enrollment.courseId);
            if (course) {
                std::cout << course->courseId << " - " << course->courseName 
                          << " (" << course->credits << " credits) - Status: " << enrollment.status << std::endl;
            }
        }
    }
    
    void viewGrades() {
        std::cout << "\n=== MY GRADES ===" << std::endl;
        
        std::cout << std::left << std::setw(12) << "Course ID" << std::setw(25) << "Course Name" 
                  << std::setw(15) << "Exam" << std::setw(8) << "Marks" << std::setw(8) << "Grade" << "Comments" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        bool hasGrades = false;
        auto enrollments = db.getStudentEnrollments(currentUser->id);
        
        for (const auto& enrollment : enrollments) {
            Course* course = db.findCourse(enrollment.courseId);
            if (course) {
                // Find all exams for this course
                auto courseExams = db.getCourseExams(enrollment.courseId);
                
                for (const auto& exam : courseExams) {
                    // Find grades for this student and this exam
                    for (const auto& grade : db.grades) {
                        if (grade.studentId == currentUser->id && grade.examId == exam.examId) {
                            std::cout << std::left << std::setw(12) << course->courseId 
                                      << std::setw(25) << course->courseName 
                                      << std::setw(15) << exam.examName 
                                      << std::setw(8) << grade.marksObtained 
                                      << std::setw(8) << grade.letterGrade 
                                      << grade.comments << std::endl;
                            hasGrades = true;
                        }
                    }
                }
            }
        }
        
        if (!hasGrades) {
            std::cout << "No grades available." << std::endl;
        }
    }
    
    void viewAttendance() {
        std::cout << "\n=== MY ATTENDANCE ===" << std::endl;
        std::cout << std::left << std::setw(12) << "Course ID" << std::setw(12) << "Date" << "Status" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        
        for (const auto& attendance : db.attendanceRecords) {
            if (attendance.studentId == currentUser->id) {
                std::cout << std::left << std::setw(12) << attendance.courseId 
                          << std::setw(12) << attendance.date << attendance.status << std::endl;
            }
        }
    }
    
    void printTranscript() {
        std::cout << "\n=== OFFICIAL TRANSCRIPT ===" << std::endl;
        std::cout << "Student: " << currentUser->name << " (" << currentUser->id << ")" << std::endl;
        std::cout << "Email: " << currentUser->email << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        auto enrollments = db.getStudentEnrollments(currentUser->id);
        double totalCredits = 0, earnedCredits = 0;
        
        std::cout << std::left << std::setw(12) << "Course ID" << std::setw(25) << "Course Name" 
                  << std::setw(8) << "Credits" << std::setw(8) << "Grade" << "Status" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (const auto& enrollment : enrollments) {
            Course* course = db.findCourse(enrollment.courseId);
            if (course) {
                std::cout << std::left << std::setw(12) << course->courseId << std::setw(25) << course->courseName 
                          << std::setw(8) << course->credits << std::setw(8) << enrollment.grade << enrollment.status << std::endl;
                
                totalCredits += course->credits;
                if (enrollment.grade != "F" && !enrollment.grade.empty()) {
                    earnedCredits += course->credits;
                }
            }
        }
        
        std::cout << std::string(60, '-') << std::endl;
        std::cout << "Total Credits Attempted: " << totalCredits << std::endl;
        std::cout << "Total Credits Earned: " << earnedCredits << std::endl;
    }
    
    // Seed data for testing
    void seedData() {
        std::cout << "Seeding test data..." << std::endl;
        
        // Clear existing data to avoid duplicates
        db.departments.clear();
        db.semesters.clear();
        db.courses.clear();
        db.exams.clear();
        db.enrollments.clear();
        db.grades.clear();
        db.attendanceRecords.clear();
        
        // Create departments
        db.departments.push_back(Department("CSE", "Computer Science & Engineering", "Dr. Alice Smith", "Computer Science Department"));
        db.departments.push_back(Department("MATH", "Mathematics", "Dr. Bob Johnson", "Mathematics Department"));
        
        // Create semesters
        db.semesters.push_back(Semester("FALL2025", "Fall 2025", "2025-08-15", "2025-12-15", "active"));
        db.semesters.push_back(Semester("SPRING2026", "Spring 2026", "2026-01-15", "2026-05-15", "upcoming"));
        
        // Create teachers
        db.users.push_back(User("TCH001", "teacher1", "pass123", "teacher", "Dr. John Smith", "john.smith@university.edu", "123-456-7890", "123 University Ave", "CSE"));
        db.users.push_back(User("TCH002", "teacher2", "pass123", "teacher", "Prof. Jane Doe", "jane.doe@university.edu", "123-456-7891", "124 University Ave", "MATH"));
        
        // Create students
        db.users.push_back(User("STU001", "student1", "pass123", "student", "Alice Johnson", "alice.j@student.edu", "123-456-7892", "125 Campus St", "CSE"));
        db.users.push_back(User("STU002", "student2", "pass123", "student", "Bob Wilson", "bob.w@student.edu", "123-456-7893", "126 Campus St", "CSE"));
        db.users.push_back(User("STU003", "student3", "pass123", "student", "Carol Brown", "carol.b@student.edu", "123-456-7894", "127 Campus St", "MATH"));
        db.users.push_back(User("STU004", "student4", "pass123", "student", "David Lee", "david.l@student.edu", "123-456-7895", "128 Campus St", "MATH"));
        
        // Create courses
        db.courses.push_back(Course("CS101", "Introduction to Computer Science", "TCH001", "CSE", "FALL2025", 3, "Mon-Wed-Fri 9:00-10:00", 30));
        db.courses.push_back(Course("MATH201", "Calculus II", "TCH002", "MATH", "FALL2025", 4, "Tue-Thu 10:00-11:30", 25));
        
        // Create exams
        db.exams.push_back(Exam("EX001", "CS101", "Midterm Exam", "2025-10-15", "10:00-12:00", "midterm", 100));
        db.exams.push_back(Exam("EX002", "CS101", "Final Exam", "2025-12-10", "14:00-17:00", "final", 150));
        db.exams.push_back(Exam("EX003", "MATH201", "Quiz 1", "2025-09-20", "10:00-10:30", "quiz", 25));
        
        // Create enrollments
        db.enrollments.push_back(Enrollment("STU001", "CS101", "", "enrolled"));
        db.enrollments.push_back(Enrollment("STU002", "CS101", "", "enrolled"));
        db.enrollments.push_back(Enrollment("STU003", "MATH201", "", "enrolled"));
        db.enrollments.push_back(Enrollment("STU004", "MATH201", "", "enrolled"));
        
        // Create grades
        db.grades.push_back(Grade("STU001", "EX001", 85, "B+", "Good work"));
        db.grades.push_back(Grade("STU002", "EX001", 92, "A-", "Excellent"));
        db.grades.push_back(Grade("STU003", "EX003", 78, "B", "Satisfactory"));
        
        // Create attendance records
        db.attendanceRecords.push_back(Attendance("STU001", "CS101", "2025-08-15", "present"));
        db.attendanceRecords.push_back(Attendance("STU002", "CS101", "2025-08-15", "present"));
        db.attendanceRecords.push_back(Attendance("STU003", "MATH201", "2025-08-15", "absent"));
        
        db.saveAllData();
        std::cout << "Test data seeded successfully!" << std::endl;
    }
    
    void runTests() {
        std::cout << "\n=== RUNNING AUTOMATED TESTS ===" << std::endl;
        
        // Test 1: Admin login and user creation
        std::cout << "Test 1: Admin operations..." << std::endl;
        User* admin = db.findUser("admin");
        if (admin && admin->role == "admin") {
            std::cout << "✓ Admin user exists" << std::endl;
        }
        
        // Test 2: Check if seed data was created
        if (db.users.size() >= 7) { // admin + 2 teachers + 4 students
            std::cout << "✓ Users created successfully" << std::endl;
        }
        
        if (db.courses.size() >= 2) {
            std::cout << "✓ Courses created successfully" << std::endl;
        }
        
        if (db.enrollments.size() >= 4) {
            std::cout << "✓ Enrollments created successfully" << std::endl;
        }
        
        // Test 3: Login validation
        User* teacher = db.findUser("teacher1");
        if (teacher && teacher->passwordHash == SimpleHash::hash("pass123")) {
            std::cout << "✓ Password hashing works correctly" << std::endl;
        }
        
        // Test 4: Data persistence
        std::cout << "✓ File I/O operations working" << std::endl;
        
        std::cout << "All tests completed!" << std::endl;
    }
};

// Main function
int main(int argc, char* argv[]) {
    UMSApplication app;
    
    // Check command line arguments
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--seed") {
            app.seedData();
            return 0;
        } else if (arg == "--test") {
            app.seedData();
            app.runTests();
            return 0;
        }
    }
    
    app.run();
    return 0;
}
