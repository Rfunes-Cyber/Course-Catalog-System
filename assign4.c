#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char course_Name[84];
    char course_Sched[4];
    unsigned course_Size;
    unsigned course_Hours;
    char padding[20];
} COURSE;

#define DATAFILE "courses.dat"

void displayMenu() {
    printf("Enter one of the following actions or press CTRL-D to exit.\n");
    printf("C - create a new course record\n");
    printf("U - update an existing course record\n");
    printf("R - read an existing course record\n");
    printf("D - delete an existing course record\n");
}

int main() {
    FILE *fp = fopen(DATAFILE, "rb+");

    if (!fp) {
        fp = fopen(DATAFILE, "wb+");
        if (!fp) {
            perror("Could not open or create data file");
            exit(EXIT_FAILURE);
        }
    }

    char command;

    displayMenu();
    while (scanf(" %c", &command) == 1) {
        switch (command) {
            case 'C':
            case 'c': {
                int courseNum;
                COURSE c;
                memset(&c, 0, sizeof(COURSE));  

                printf("Course number:\n");
                scanf("%d", &courseNum);
                getchar(); 

                printf("Course name:\n");
                fgets(c.course_Name, sizeof(c.course_Name), stdin);
                c.course_Name[strcspn(c.course_Name, "\n")] = '\0';  

                printf("Course schedule:\n");
                fgets(c.course_Sched, sizeof(c.course_Sched), stdin);
                c.course_Sched[strcspn(c.course_Sched, "\n")] = '\0';

                printf("Course credit hours:\n");
                scanf("%u", &c.course_Hours);

                printf("Course enrollment:\n");
                scanf("%u", &c.course_Size);

                long offset = courseNum * sizeof(COURSE);
                fseek(fp, offset, SEEK_SET);

                COURSE existing;
                if (fread(&existing, sizeof(COURSE), 1, fp) == 1 && existing.course_Hours > 0) {
                    printf("ERROR: course already exists\n");
                } else {
                    fseek(fp, offset, SEEK_SET);
                    fwrite(&c, sizeof(COURSE), 1, fp);
                }

                break;
            }

            case 'U':
            case 'u': {
                int courseNum;
                printf("Enter a CS course number to update:\n");
                if (scanf("%d", &courseNum) != 1) {
                    printf("ERROR: invalid input\n");
                    while (getchar() != '\n'); 
                    break;
                }

                long offset = courseNum * sizeof(COURSE);
                fseek(fp, offset, SEEK_SET);

                COURSE c;
                if (fread(&c, sizeof(COURSE), 1, fp) != 1 || c.course_Hours == 0) {
                    printf("ERROR: course not found\n");
                    break;
                }

                printf("Enter new course name:\n");
                getchar();
                fgets(c.course_Name, sizeof(c.course_Name), stdin);
                c.course_Name[strcspn(c.course_Name, "\n")] = '\0';

                printf("Enter new schedule:\n");
                fgets(c.course_Sched, sizeof(c.course_Sched), stdin);
                c.course_Sched[strcspn(c.course_Sched, "\n")] = '\0';

                printf("Enter new credit hours:\n");
                scanf("%u", &c.course_Hours);

                printf("Enter new enrollment size:\n");
                scanf("%u", &c.course_Size);

                fseek(fp, offset, SEEK_SET);
                fwrite(&c, sizeof(COURSE), 1, fp);
                printf("Course updated successfully.\n");
                break;
            }

            case 'R':
            case 'r': {
                int courseNum;
                printf("Enter a CS course number:\n");
                if (scanf("%d", &courseNum) != 1) {
                    printf("ERROR: invalid input\n");
                    while (getchar() != '\n'); 
                    break;
                }

                COURSE c;
                long offset = courseNum * sizeof(COURSE);
                fseek(fp, offset, SEEK_SET);

                if (fread(&c, sizeof(COURSE), 1, fp) != 1 || c.course_Hours == 0) {
                    printf("ERROR: course not found\n");
                } else {
                    printf("Course number: %d\n", courseNum);
                    printf("Course name: %s\n", c.course_Name);
                    printf("Scheduled days: %s\n", c.course_Sched);
                    printf("Credit hours: %u\n", c.course_Hours);
                    printf("Enrolled Students: %u\n", c.course_Size);
                }
                break;
            }

            case 'D':
            case 'd': {
                int courseNum;
                printf("Enter a CS course number to delete:\n");
                if (scanf("%d", &courseNum) != 1) {
                    printf("ERROR: invalid input\n");
                    while (getchar() != '\n');
                    break;
                }

                long offset = courseNum * sizeof(COURSE);
                fseek(fp, offset, SEEK_SET);

                COURSE c;
                if (fread(&c, sizeof(COURSE), 1, fp) != 1 || c.course_Hours == 0) {
                    printf("ERROR: course not found\n");
                } else {
                    COURSE blank = {0};  
                    fseek(fp, offset, SEEK_SET);
                    fwrite(&blank, sizeof(COURSE), 1, fp);
                    printf("course number was successfully deleted.\n");
                }
                break;
            }

            default:
                printf("ERROR: invalid option\n");
                break;
        }

        displayMenu(); 
    }

    fclose(fp);
    return 0;
}
