# University Management System (UMS)

A comprehensive offline C++17 console application for managing university operations with role-based authentication and file-based persistence.

## Features

### Core Functionality
- **Role-based Authentication**: Admin, Teacher, Student login system
- **User Management**: Create, view, and delete users
- **Course Management**: CRUD operations for courses
- **Enrollment System**: Student enrollment in courses
- **Grade Management**: Teachers can assign and update grades
- **Attendance Tracking**: Mark and view attendance records
- **Reporting**: Generate various reports and transcripts

### Security Features
- Password hashing using std::hash (with salt)
- Role-based access control
- Input validation and error handling

### Data Persistence
- CSV-based file storage in `data/` directory
- Automatic backup functionality
- Human-readable data formats

## Project Structure

```
University Management System/
├── UMS.cpp              # Main application file (entry point)
├── README.md            # This file
└── data/                # Data directory (created automatically)
    ├── users.csv        # User accounts
    ├── courses.csv      # Course information
    ├── enrollments.csv  # Student enrollments
    └── attendance.csv   # Attendance records
```

## Data Formats

### Users (users.csv)
```
id,username,passwordHash,role,name,email
admin001,admin,1234567890,admin,System Administrator,admin@university.edu
```

### Courses (courses.csv)
```
courseId,courseName,teacherId,credits,semester
CS101,Introduction to Computer Science,TCH001,3,Fall 2025
```

### Enrollments (enrollments.csv)
```
studentId,courseId,grade,status
STU001,CS101,A,enrolled
```

### Attendance (attendance.csv)
```
studentId,courseId,date,status
STU001,CS101,2025-08-15,present
```

## Build Instructions

### Prerequisites
- C++17 compatible compiler (g++, MSVC cl)
- Windows environment

### Compilation

#### Using MinGW g++
```powershell
g++ -std=c++17 UMS.cpp -o UMS.exe
```

#### Using Microsoft Visual C++
```powershell
cl /EHsc UMS.cpp
```

## Usage

### Basic Run
```powershell
./UMS.exe
```

### Initialize with Test Data
```powershell
./UMS.exe --seed
```

### Run Tests
```powershell
./UMS.exe --test
```

## Default Login Credentials

### Admin
- **Username**: admin
- **Password**: admin123

### After Seeding Test Data
#### Teachers
- **Username**: teacher1, **Password**: pass123 (Dr. John Smith)
- **Username**: teacher2, **Password**: pass123 (Prof. Jane Doe)

#### Students
- **Username**: student1, **Password**: pass123 (Alice Johnson)
- **Username**: student2, **Password**: pass123 (Bob Wilson)
- **Username**: student3, **Password**: pass123 (Carol Brown)
- **Username**: student4, **Password**: pass123 (David Lee)

## Sample Run Transcript

```
=== University Management System ===
Welcome to UMS v1.0

=== LOGIN ===
1. Login
2. Exit
Choice: 1
Username: admin
Password: admin123
Login successful! Welcome, System Administrator

=== ADMIN MENU ===
1. Manage Users
2. Manage Courses
3. View Reports
4. Backup Data
5. Logout
Choice: 1

=== USER MANAGEMENT ===
1. Create Teacher
2. Create Student
3. View All Users
4. Delete User
5. Back
Choice: 1
Enter teacher ID: TCH001
Enter username: teacher1
Enter password: pass123
Enter full name: Dr. John Smith
Enter email: john.smith@university.edu
teacher created successfully!
```

## User Roles and Permissions

### Admin
- Create/delete teacher and student accounts
- Manage all courses
- View system reports
- Backup/restore data
- Full system access

### Teacher
- View assigned courses
- Enroll/manage students in their courses
- Enter and update grades
- Mark attendance
- Generate class rosters and grade reports

### Student
- View personal profile
- View enrolled courses
- Check grades and attendance
- Print transcript

## Edge Cases Handled

1. **Empty Datasets**: System works with no initial data
2. **Duplicate IDs**: Validation prevents duplicate user IDs and course IDs
3. **File I/O Errors**: Graceful handling of missing or corrupted files
4. **Invalid Login**: Multiple retry attempts with proper error messages
5. **Concurrent File Usage**: Basic file locking through proper file handling

## Test Checklist

- [ ] Admin can create teacher accounts
- [ ] Admin can create student accounts
- [ ] Teachers can view their assigned courses
- [ ] Teachers can enroll students in courses
- [ ] Teachers can assign grades
- [ ] Teachers can mark attendance
- [ ] Students can view their profile
- [ ] Students can view enrolled courses
- [ ] Students can view grades
- [ ] Students can view attendance
- [ ] Students can print transcript
- [ ] Data persistence works correctly
- [ ] Password authentication works
- [ ] Role-based access control works

## Automated Tests

Run the automated test suite:
```powershell
./UMS.exe --test
```

This will:
1. Seed the database with test data
2. Verify all core functionalities
3. Test authentication and authorization
4. Validate data persistence

## Security Limitations and Future Improvements

### Current Security Limitations
- **Simple Hash Function**: Uses std::hash which is not cryptographically secure
- **Plaintext Salt**: Salt is hardcoded in source code
- **No Password Complexity**: No requirements for strong passwords
- **File-based Storage**: CSV files are readable and modifiable

### Recommended Security Improvements
1. **Implement SHA-256 or bcrypt** for password hashing
2. **Add random salt generation** for each password
3. **Implement password complexity requirements**
4. **Add file encryption** for sensitive data
5. **Implement session management** with timeouts
6. **Add audit logging** for administrative actions

### Future Feature Enhancements
1. **Database Integration**: Migrate from CSV to SQLite/MySQL
2. **GUI Interface**: Develop Qt or web-based interface
3. **Network Support**: Multi-user concurrent access
4. **Advanced Reporting**: Detailed analytics and charts
5. **Email Integration**: Automated notifications
6. **Academic Calendar**: Integration with semester schedules
7. **Financial Module**: Fee management and billing

## Troubleshooting

### Compilation Issues
- Ensure C++17 support: Use `-std=c++17` flag
- Missing includes: Verify all standard library headers

### Runtime Issues
- **Permission Errors**: Run as administrator if data directory creation fails
- **File Not Found**: Run `--seed` to create initial data
- **Login Failed**: Check credentials or reset using admin account

### Data Issues
- **Corrupted Files**: Delete data directory and run `--seed`
- **Missing Data**: Use backup functionality or re-seed

## License and Attribution

This project uses only C++ standard library components. No third-party libraries are included.

## Development Notes

- **Language**: C++17
- **Platform**: Windows (PowerShell)
- **Architecture**: Single-file monolithic design for simplicity
- **Dependencies**: None (standard library only)
- **File Format**: CSV for human readability and easy debugging

---

**Version**: 1.0  
**Last Updated**: August 17, 2025  
**Author**: University Management System Development Team
