#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 100

//note that between student data in the file there is a comma

typedef struct Student
{
    //student info
    int studentID;
    char studentName[50];
    char homeAddress[50];
    int classID;
    char enrollmentDate[50];
    struct Student* right; // Pointer to the right child
    struct Student* left;   // Pointer to the left child

} Student;

typedef struct Node
{
    Student* student;   // Pointer to a student structure
    struct Node* next;  // Pointer to the next node in the linked list
} Node;

//GLOBAL Array to store class IDs
int classes[MAXSIZE];
// Current number of elements in the classes array
int currentSize = 0;


void addClass (int classID)
{

    int flag = 0;
    // Check if classID already exists in the classes array
    for (int i = 0; i < currentSize; i++)
    {
        if (classes[i] == classID)
        {
            flag = 1; // Set flag to 1 if classID is found
            break; // Exit the loop since there's no need to continue searching
        }
    }

    if (!flag)
    {
        classes[currentSize++] = classID; // Add classID to the classes array if not already present
    }

}
Student* createStudent(int studentID, char* studentName, char* homeAddress, int classID, char* enrollmentDate)
{
    // Allocate memory for a new Student structure
    Student* newStudent = (Student*)malloc(sizeof(Student));
// Set the properties of the new student
    newStudent->studentID = studentID;
    strcpy(newStudent->studentName, studentName);
    strcpy(newStudent->homeAddress, homeAddress);
    newStudent->classID = classID;
    strcpy(newStudent->enrollmentDate, enrollmentDate);
    newStudent->left = NULL;
    newStudent->right = NULL;

    addClass(classID); // Add the classID to the classes array

    return newStudent; // Return the newly created student
}


Student* insertStudent(Student* root, int studentID, char* studentName, char* homeAddress, int classID, char* enrollmentDate)
{
    // If the root is NULL, create a new student and return it
    if (root == NULL)
    {
        return createStudent(studentID, studentName, homeAddress, classID, enrollmentDate);
    }
    // If the studentID is less than the root's studentID, recursively insert the student in the left subtree
    if (studentID < root->studentID)
    {
        root->left = insertStudent(root->left, studentID, studentName, homeAddress, classID, enrollmentDate);
    }
    // If the studentID is less than the root's studentID, recursively insert the student in the left subtree
    else if (studentID > root->studentID)
    {
        root->right = insertStudent(root->right, studentID, studentName, homeAddress, classID, enrollmentDate);
    }

    return root; // Return the root of the modified binary tree
}


Student* Find(Student* root, int studentID)
{
// If the root is NULL, the student with the given ID doesn't exist in the tree
    if(root==NULL)
        return NULL;
// If the studentID is less than the root's studentID, recursively search in the left subtree
    if(studentID < root->studentID)
        return Find(root->left,studentID);
// If the studentID is greater than the root's studentID, recursively search in the right subtree
    else if(studentID > root->studentID)
        return Find(root->right,studentID);

    else

        return root;// Return the root since it matches the desired studentID

}
void removeClass(int id, Node* sortedList)
{
    int classFound = 0;

    Node* current = sortedList;
// Traverse the sorted list and count the occurrences of the classID
    while (current != NULL)
    {
        if (current->student->classID == id)
        {
            classFound++;
        }
        current = current->next;
    }

    if (classFound == 1)
    {
        // Traverse the sorted list and count the occurrences of the classID
        for (int i = 0; i < currentSize; i++)
        {
            if (classes[i] == id)
            {
                // Shift the elements after the removed classID to fill the gap
                for (int j = i; j < currentSize - 1; j++)
                {
                    classes[j] = classes[j + 1];
                }
                currentSize--;// Decrement the current size of the classes array
                break;
            }
        }
    }
}


Student* find_min(Student* root)
{
// If the root is NULL, return NULL
    if(root==NULL)
        return NULL;
    // If the left child is NULL, the current node has the minimum value
    else if(root->left==NULL)
        return root;
    else
        return (find_min(root->left)); // Recursively search in the left subtree


}

Student* deleteStudent (Student* root,int id,Node* sortedList)
{

    Student* temp;
// If the root is NULL, the student with the given ID doesn't exist
    if(root==NULL)
    {
        printf("Student not found\n\n");
        return NULL;
    }
    // If the ID is less than the root's studentID, recursively search in the left subtree
    else if(id<root->studentID)
        root->left=deleteStudent(root->left,id,sortedList);
    // If the ID is greater than the root's studentID, recursively search in the right subtree
    else if(id>root->studentID)
        root->right=deleteStudent(root->right,id,sortedList);
    //If the node have both right and left nodes
    else if(root->left && root->right)
    {
        temp=find_min(root->right);// Find the minimum node in the right subtree
        // Replace the root's values with the minimum node's values
        root->studentID=temp->studentID;
        root->classID = temp->classID;
        strcpy(root->studentName, temp->studentName);
        strcpy(root->homeAddress, temp->homeAddress);
        // Delete the minimum node from the right subtree
        root->right=deleteStudent(root->right,root->studentID,sortedList);
    }
    else
    {
        if (root->left == NULL)
        {
            temp = root->right;
            int Id =root->classID;
            // Remove the classID from the classes array
            removeClass(Id,sortedList);
            free(root);
            printf("Student has been deleted successfully\n\n");

            return temp;
        }
        else if (root->right == NULL)
        {
            temp = root->left;
            int Id =root->classID;
            // Remove the classID from the classes array
            removeClass(Id,sortedList);
            free(root);
            printf("Student has been deleted successfully\n\n");

            return temp;
        }
    }


    return root; // Return the modified root of the binary tree
}
Student* updateStudent(Student* root, int id,Node* sortedList)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (id < root->studentID)
    {
        root->left = updateStudent(root->left, id,sortedList);
    }
    else if (id > root->studentID)
    {
        root->right = updateStudent(root->right, id,sortedList);
    }
    else
    {
        int choice,classID;
        int exitFlag = 1;

        while (exitFlag)
        {
            //Note that id is unique and we can not change it
            printf("Select an attribute to update or -1 to finish updating:\n");
            printf("1. Student Name\n");
            printf("2. Home Address\n");
            printf("3. Class ID\n");
            printf("4. Enrollment Date\n");

            scanf("%d", &choice);

            switch (choice)
            {

            case 1:
                printf("Enter updated student name:\n");
                scanf(" %[^\n]s", root->studentName);
                printf("Student name updated successfully!\n\n");
                break;
            case 2:
                printf("Enter updated home address:\n");
                scanf(" %[^\n]s", root->homeAddress);
                printf("Home address updated successfully!\n\n");
                break;
            case 3:
                classID=root->classID;
                // Remove the old class ID from the classes array
                removeClass(classID,sortedList);
                printf("Enter updated class ID:\n");
                scanf("%d", &(root->classID));
                // Add the updated class ID to the classes array
                addClass(root->classID);
                printf("Class ID updated successfully!\n\n");

                break;
            case 4:
                printf("Enter updated enrollment date:\n");
                scanf(" %[^\n]s", root->enrollmentDate);
                printf("Enrollment date updated successfully!\n\n");
                break;
            case -1:
                exitFlag = 0;
                break;
            default:
                printf("\nOops! That doesn't seem like a valid choice. Please try again.\n\n");
                break;
            }
        }

    }

    return root;
}

Node* insertNode(Student* root,Node * list)
{
    // Create a new node
    Node* newNode=(Node*)malloc(sizeof(Node));
    newNode->student=root;

    // If the list is empty or the root's studentName is smaller than the first node's studentName
    // Insert the new node at the beginning of the list
    if (list == NULL || strcmp(root->studentName, list->student->studentName) < 0)
    {
        newNode->next = list;
        return newNode;
    }


    Node * current = list;
    // Traverse the list to find the appropriate position to insert the new node
    while(current->next!=NULL && strcmp(root->studentName, current->next->student->studentName) > 0)
    {

        current=current->next;
    }
// Insert the new node after the current node
    newNode->next = current->next;
    current->next = newNode;

    return list;
}
void inorderTraversalByName(Student* root,Node** sortedList)
{
    if (root != NULL)
    {
        // Traverse the left subtree
        inorderTraversalByName(root->left,sortedList);
        // Insert the current student into the sorted list based on name
        *sortedList=insertNode(root,*sortedList);
        // Traverse the right subtree
        inorderTraversalByName(root->right,sortedList);
    }
}
//print student info
void printData(Student* student)
{
    if (student != NULL)
    {
        printf("Student ID: %d\n", student->studentID);
        printf("Student Name: %s\n", student->studentName);
        printf("Home Address: %s\n", student->homeAddress);
        printf("Class ID: %d\n", student->classID);
        printf("Enrollment Date: %s\n", student->enrollmentDate);
    }
}
void preorderTraversal(Student* root, FILE* file)
{
    if (root != NULL)
    {
        // Write the current student's data to the file
        //note there is a comma
        fprintf(file, "%d,%s,%s,%d,%s\n", root->studentID, root->studentName, root->homeAddress, root->classID, root->enrollmentDate);
        // Traverse the left subtree
        preorderTraversal(root->left, file);
        // Traverse the right subtree
        preorderTraversal(root->right, file);
    }
}
void saveToFile(char* filename, Student* root)
{

    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    // Write student data to the file using preorder traversal
    preorderTraversal(root, file);


    fclose(file);

}
Student* readFromFile(char* fileName)
{
    Student* root = NULL;
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return NULL;
    }

    char line[200];
    int studentCount = 0;
    //read each line
    while (fgets(line, sizeof(line), file))
    {
        int studentID, classID;
        char studentName[50], homeAddress[50], enrollmentDate[50];
        if (sscanf(line, "%d,%[^,],%[^,],%d,%s\n", &studentID, studentName, homeAddress, &classID, enrollmentDate) == 5)
        {
            // Insert the student read from the file into the binary tree
            root = insertStudent(root, studentID, studentName, homeAddress, classID, enrollmentDate);
            studentCount++;
        }
    }

    fclose(file);
//if file was empty
    if (studentCount == 0)
    {
        printf("No student records found in the file\nInsert students then continue\n\n");
    }

    return root;
}

int main()
{
    char* filename = "students.data.txt";
    Node* sortedList=NULL;


    int studentID, classID;
    char studentName[50], homeAddress[50], enrollmentDate[50];
    int choice;
    int flag=1;
    printf("Welcome----------------------------------\n\n");
    // Read student records from the file and build the binary tree
    //note that between student data there is a comma
    Student *BST = readFromFile(filename);

    printf("Enter your choice\n\n1-Insert a student\n2-Find a student by his/her student ID and update the student Info\n3-List all students in lexicographic order of their names\n4-Search for a city and list all students from that city\n5-List all students by their class\n6-Delete a student given his/her student ID\n7-Save all students in file\n8-Exit\n\n");
    scanf("%d",&choice);
    printf("\n");
    while(choice!=8)
    {
        switch(choice)
        {

        case 1:
            // Insert a new student
            printf("Enter student ID: ");
            scanf("%d", &studentID);
            printf("Enter student name: ");
            scanf(" %[^\n]", studentName);
            printf("Enter home address: ");
            scanf(" %[^\n]", homeAddress);
            printf("Enter class ID: ");
            scanf("%d", &classID);
            printf("Enter enrollment date: ");
            scanf(" %[^\n]", enrollmentDate);

            BST = insertStudent(BST, studentID, studentName, homeAddress, classID, enrollmentDate);

            break;
        case 2:
            // Find a student by ID and update their information
            sortedList = NULL;
            inorderTraversalByName(BST, &sortedList);
            printf("Enter Student ID\n");
            scanf("%d",&studentID);
            Student *student = Find(BST,studentID);
            if (student != NULL)
            {
                printf("\n");
                printData(student);
                printf("\n");
                BST = updateStudent(BST,studentID,sortedList);

            }
            else
            {
                printf("Student with ID %d not found.\n\n",studentID);
            }

            break;

        case 3:
            // List all students in lexicographic order of their names
            if(BST==NULL)
            {
                printf("No Students were found\n\n");
            }
            else
            {
                sortedList = NULL;
                inorderTraversalByName(BST, &sortedList);

                printf("All students (list in lexicographic order of their names):\n\n");
                while (sortedList != NULL)
                {
                    printData(sortedList->student);
                    printf("\n");
                    sortedList = sortedList->next;
                }
            }
            break;

        case 4:
            // Search for students from a specific city
            printf("Enter a city name\n");
            scanf(" %[^\n]", homeAddress);
            printf("\n");
            sortedList=NULL;
            inorderTraversalByName(BST,&sortedList);
            while (sortedList != NULL)
            {
                if(strcmp(sortedList->student->homeAddress,homeAddress) == 0)
                {
                    flag=0;
                    printData(sortedList->student);
                    printf("\n");
                }
                sortedList = sortedList->next;
            }

            if(flag==1)
            {
                printf("No Students in %s\n\n",homeAddress);
            }
            break;

        case 5:
            // Search for students from a specific city
            if(BST==NULL)
            {
                printf("No Students were found\n\n");
            }
            else
            {
                sortedList=NULL;
                inorderTraversalByName(BST,&sortedList);

                for (int i = 0; i < currentSize; i++)
                {
                    printf("Students in class %d:\n\n", classes[i]);


                    Node* current = sortedList;

                    while (current != NULL)
                    {
                        if (current->student->classID == classes[i])
                        {
                            printData(current->student);
                            printf("\n");
                        }
                        current = current->next;
                    }

                }

            }
            break;

        case 6:
            // Delete a student given their ID
            sortedList=NULL;
            inorderTraversalByName(BST,&sortedList);
            printf("Enter Student ID to delete\n");
            scanf("%d",&studentID);
            BST=deleteStudent(BST,studentID,sortedList);
            break;

        case 7:
            // Save all students to the file
            saveToFile(filename, BST);
            printf("Students saved to file successfully!\n");

            break;
        default:
            printf("Oops! That doesn't seem like a valid choice. Please try again.\n\n");
            break;

        }

// Prompt for the next choice
        printf("\nEnter your choice\n\n1-Insert a student\n2-Find a student by his/her student ID and update the student Info\n3-List all students in lexicographic order of their names\n4-Search for a city and list all students from that city\n5-List all students by their class\n6-Delete a student given his/her student ID\n7-Save all students in file\n8-Exit\n\n");
        scanf("%d",&choice);
        printf("\n");
    }

    return 0;
}
