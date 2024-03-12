#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_USERS 10

// Structure to represent a user
struct User {
    char username[50];
    char password[50];
};

// Structure to store course information
struct Course {
    int courseId;
    char courseName[50];
    float assignmentMarks;
    float internalMarks;
    float externalMarks;
    float gpa; // New field to store GPA
};

// Structure to store student information
struct Student {
    int id;
    char name[50];
    unsigned long long int contact;
    char email[50];
    int numCourses;
    struct Course courses[10]; // Assuming a maximum of 10 courses
};

// Function prototypes
bool login(struct User* users, int userCount);
bool signup(struct User* users, int* userCount);
void addStudent(struct Student *students, int *count);
void displayStudent(struct Student *students, int count, int studentID);
void displayAllStudents(struct Student *students, int count);
unsigned long long int getValidContact();
void getValidName(char *name);
float calculateGPA(float percentage);
float calculateCGPA(struct Student *student);

int main() {
    struct User users[MAX_USERS];
    int userCount = 0;

    struct Student students[50]; // Assuming a maximum of 50 students
    int studentCount = 0;
    int choice;
    int studentID;

    while (1) {
        printf("\n\nStudent Database Management System\n");
        printf("1. Login\n");
        printf("2. Sign Up\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (login(users, userCount)) {
                    printf("Login successful!\n");
                    // Proceed to student management menu
                    do {
                        printf("\nStudent Management Menu\n");
                        printf("1. Add Student\n");
                        printf("2. Display Student\n");
                        printf("3. Display All Students\n");
                        printf("4. Logout\n");
                        printf("Enter your choice: ");
                        scanf("%d", &choice);

                        switch (choice) {
                            case 1:
                                addStudent(students, &studentCount);
                                break;
                            case 2:
                                printf("Enter student ID to display: ");
                                scanf("%d", &studentID);
                                displayStudent(students, studentCount, studentID);
                                break;
                            case 3:
                                displayAllStudents(students, studentCount);
                                break;
                            case 4:
                                printf("Logging out...\n");
                                break;
                            default:
                                printf("Invalid choice. Please enter a valid option.\n");
                        }
                    } while (choice != 4);
                } else {
                    printf("Login failed. Please try again.\n");
                }
                break;
            case 2:
                if (signup(users, &userCount)) {
                    printf("Sign up successful! You can now login.\n");
                } else {
                    printf("Sign up failed. Please try again.\n");
                }
                break;
            case 3:
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    }

    return 0;
}

bool login(struct User* users, int userCount) {
    char username[50];
    char password[50];

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return true;
        }
    }

    return false;
}

bool signup(struct User* users, int* userCount) {
    if (*userCount < MAX_USERS) {
        printf("Enter a new username: ");
        scanf("%s", users[*userCount].username);

        printf("Enter a new password: ");
        scanf("%s", users[*userCount].password);

        (*userCount)++;
        return true;
    } else {
        printf("User limit reached. Cannot sign up more users.\n");
        return false;
    }
}

void addStudent(struct Student *students, int *count) {
    int newStudentID;

    // Check for duplicate IDs
    do {
        printf("Enter student ID: ");
        scanf("%d", &newStudentID);
        int duplicateID = 0;
        for (int i = 0; i < *count; i++) {
            if (students[i].id == newStudentID) {
                duplicateID = 1;
                printf("Error: Student with the same ID already exists. Please enter a unique ID.\n");
                break;
            }
        }
        if (!duplicateID) {
            break; // Unique ID, exit the loop
        }
    } while (1);
    students[*count].id = newStudentID;

    // Get a valid name
    getValidName(students[*count].name);

    // Validate and get contact
    students[*count].contact = getValidContact();

    // Validate and get email
    while (1) {
        printf("Enter email (with @gmail.com extension): ");
        scanf("%s", students[*count].email);
        if (strstr(students[*count].email, "@gmail.com") != NULL) {
            break;
        }
        printf("Error: Email must have @gmail.com extension. Please try again.\n");
    }

    // Get number of courses
    printf("Enter the number of courses: ");
    scanf("%d", &students[*count].numCourses);

    // Get details for each course
    for (int i = 0; i < students[*count].numCourses; i++) {
        printf("Enter details for Course %d:\n", i + 1);
        printf("Enter course ID: ");
        scanf("%d", &students[*count].courses[i].courseId);
        printf("Enter course name: ");
        scanf("%s", students[*count].courses[i].courseName);

        // Enter marks for the course
        printf("Enter assignment marks for Course %d (-1 if not applicable): ", i + 1);
        scanf("%f", &students[*count].courses[i].assignmentMarks);
        if (students[*count].courses[i].assignmentMarks == -1) {
            // Set other marks to -1 if assignment marks are not applicable
            students[*count].courses[i].internalMarks = -1;
            students[*count].courses[i].externalMarks = -1;
        } else {
            // Enter internal marks for the course
            printf("Enter internal marks for Course %d (-1 if not applicable): ", i + 1);
            scanf("%f", &students[*count].courses[i].internalMarks);

            // Enter external marks for the course
            printf("Enter external marks for Course %d (-1 if not applicable): ", i + 1);
            scanf("%f", &students[*count].courses[i].externalMarks);
        }

        // Calculate GPA and store it
        float totalMarks = students[*count].courses[i].assignmentMarks +
                           students[*count].courses[i].internalMarks +
                           students[*count].courses[i].externalMarks;

        students[*count].courses[i].gpa = calculateGPA(totalMarks);
    }
    (*count)++;
}

void displayStudent(struct Student *students, int count, int studentID) {
    for (int i = 0; i < count; i++) {
        if (students[i].id == studentID) {
            printf("\n\nStudent Profile:\n");
            printf("ID: %d\n", students[i].id);
            printf("Name: %s\n", students[i].name);
            printf("Contact: %llu\n", students[i].contact);
            printf("Email: %s\n", students[i].email);
            printf("Number of Courses: %d\n", students[i].numCourses);

            printf("\nCourses:\n");
            for (int j = 0; j < students[i].numCourses; j++) {
                printf("Course %d:\n", j + 1);
                printf("ID: %d\n", students[i].courses[j].courseId);
                printf("Name: %s\n", students[i].courses[j].courseName);
                printf("Assignment Marks: %.2f\n", students[i].courses[j].assignmentMarks);
                printf("Internal Marks: %.2f\n", students[i].courses[j].internalMarks);
                printf("External Marks: %.2f\n", students[i].courses[j].externalMarks);
                printf("GPA: %.2f\n", students[i].courses[j].gpa);
            }

            // Calculate and display CGPA
            float cgpa = calculateCGPA(&students[i]);
            printf("\nCGPA: %.2f\n", cgpa);

            return;
        }
    }
    printf("Error: Student with ID %d not found.\n", studentID);
}

void displayAllStudents(struct Student *students, int count) {
    if (count == 0) {
        printf("No students to display.\n");
        return;
    }
    printf("\n\nAll Students:\n");
    for (int i = 0; i < count; i++) {
        printf("\nStudent %d:\n", i + 1);
        printf("ID: %d\n", students[i].id);
        printf("Name: %s\n", students[i].name);
        printf("Contact: %llu\n", students[i].contact);
        printf("Email: %s\n", students[i].email);
        printf("Number of Courses: %d\n", students[i].numCourses);

        printf("\nCourses:\n");
        for (int j = 0; j < students[i].numCourses; j++) {
            printf("Course %d:\n", j + 1);
            printf("ID: %d\n", students[i].courses[j].courseId);
            printf("Name: %s\n", students[i].courses[j].courseName);
            printf("Assignment Marks: %.2f\n", students[i].courses[j].assignmentMarks);
            printf("Internal Marks: %.2f\n", students[i].courses[j].internalMarks);
            printf("External Marks: %.2f\n", students[i].courses[j].externalMarks);
            printf("GPA: %.2f\n", students[i].courses[j].gpa);
        }

        // Calculate and display CGPA
        float cgpa = calculateCGPA(&students[i]);
        printf("\nCGPA: %.2f\n", cgpa);
    }
}

unsigned long long int getValidContact() {
    unsigned long long int contact;
    char input[20];
    do {
        printf("Enter contact number (up to 10 digits): ");
        if (scanf("%llu", &contact) == 1) {
            sprintf(input, "%llu", contact);
            // Check if the entered string contains any non-numeric character
            bool hasNonNumeric = false;
            for (int i = 0; i < strlen(input); i++) {
                if (!isdigit(input[i])) {
                    hasNonNumeric = true;
                    break;
                }
            }
            // Check if the contact has only numbers
            if (!hasNonNumeric && strlen(input) <= 10) {
                break;
            } else {
                printf("Error: Invalid input. Contact number should only contain up to 10 numeric digits. Please try again.\n");
            }
        } else {
            // Clear the input buffer
            while (getchar() != '\n');
            printf("Error: Invalid input. Contact number should only contain numeric digits. Please try again.\n");
        }
    } while (1);
    return contact;
}

void getValidName(char *name) {
    int i;
    do {
        printf("Enter name: ");
        scanf("%s", name);
        // Check if the name contains only characters
        for (i = 0; i < strlen(name); i++) {
            if (!isalpha(name[i])) {
                printf("Error: Invalid input. Name should only contain characters. Please try again.\n");
                break;
            }
        }
    } while (i != strlen(name)); // Repeat until a valid name is entered
}

float calculateGPA(float percentage) {
    // Implement your GPA calculation logic here
    // This is a general example; you can modify this based on your grading system
    if (percentage >= 90) {
        return 4.0;
    } else if (percentage >= 80) {
        return 3.5;
    } else if (percentage >= 70) {
        return 3.0;
    } else if (percentage >= 60) {
        return 2.5;
    } else if (percentage >= 50) {
        return 2.0;
    } else {
        return 1.0;
    }
}

float calculateCGPA(struct Student *student) {
    float totalCGPA = 0.0;
    for (int i = 0; i < student->numCourses; i++) {
        totalCGPA += student->courses[i].gpa;
    }
    return (totalCGPA / student->numCourses);
}
