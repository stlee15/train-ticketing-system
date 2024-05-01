#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma warning(disable : 4996)

struct Ticket {
    char destination[50];
    char trainID[6];
    char departureDate[11];
    char departureTime[6];
    double price;
};

typedef struct {
    char bookingID[6];
    char memberID[7];
    char bookingDate[11];
    struct Ticket ticket;
    char seats[4][5];
    int quantity;
    double total;
    char status[20];
} MemberTickets;

void staff();
void staffAdd();
void staffEdit();
void staffDelete();

void member();
void memberBuy(char memberID[7]);
void memberView(char memberID[7]);
void memberEdit();
void memberCancel();

void generateID(char, int, char *);
void ticketHeader();
void displayTickets();

void main() {
    int choice;
    do {
        printf("\nTicket Booking Module:\n1. Staff\n2. Member\n3. Exit\n\nEnter option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                staff();
                break;

            case 2:
                member();
                break;
            case 3:
                printf("\nHave a good day!\n");
                break;
            default:
                printf("\nInvalid option!\n");
                break;
        }
    } while (choice != 3);

    system("pause");
}

void staff() {
    int choice;
    do {
        printf("\nStaff Menu:\n1. View Tickets\n2. Add Ticket\n3. Edit Ticket\n4. Delete Ticket\n5. Back\n\nEnter option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayTickets();
                break;
            case 2:
                staffAdd();
                break;
            case 3:
                staffEdit();
                break;
            case 4:
                staffDelete();
                break;
            default:
                printf("\nInvalid option!\n");
                break;
        }
    } while (choice != 5);
}

void displayTickets() {
    struct Ticket ticket;
    FILE *ptrView = fopen("tickets.txt", "r");
    if (ptrView == NULL) {
        printf("An error occurred, please try again!\n");
        exit(-1);
    }

    displayHeader();

    int count = 0;
    while (fscanf(ptrView, "%[^|]|%[^|]|%[^|]|%[^|]|%lf\n", &ticket.destination, &ticket.trainID, &ticket.departureDate, &ticket.departureTime, &ticket.price) != EOF) {
        count++;
        printf("%-3d %-15s %-10s %-15s %-15s %.2lf\n", count, ticket.destination, ticket.trainID, ticket.departureDate, ticket.departureTime, ticket.price);
    }

    if (count == 0) {
        printf("No records found.\n\n");
    }
    fclose(ptrView);
}

void staffAdd() {
    struct Ticket ticket;
    FILE *ptr = fopen("tickets.txt", "a");
    if (ptr == NULL) {
        printf("An error occurred, please try again!\n");
        exit(-1);
    }

    char prompt;

    do {
        printf("\nEnter destination (eg. Kuala Lumpur): ");
        rewind(stdin);
        gets(ticket.destination);

        printf("Enter train ID (eg. T1001): ");
        rewind(stdin);
        gets(ticket.trainID);

        printf("Enter departure date (eg. 21/01/2024): ");
        rewind(stdin);
        gets(ticket.departureDate);

        printf("Enter departure time (eg. 07:00): ");
        rewind(stdin);
        gets(ticket.departureTime);

        printf("Enter price (eg. 100.00): ");
        rewind(stdin);
        scanf("%lf", &ticket.price);

        printf("\nConfirm ticket (y/n)? ");
        rewind(stdin);
        char confirm = tolower(getchar());

        if (confirm == 'y') {
            fprintf(ptr, "%s|%s|%s|%.2lf\n", ticket.destination, ticket.trainID, ticket.departureDate, ticket.price);
        } else {
            printf("No changes made!\n");
        }

        printf("\nAdd more ticket (y/n)? ");
        rewind(stdin);
        prompt = tolower(getchar());

    } while (prompt != 'n');
    fclose(ptr);
}

void staffEdit() {
    struct Ticket ticket[50];

    FILE *ptr = fopen("tickets.txt", "r");
    if (ptr == NULL) {
        printf("An error occurred, please try again!\n");
        exit(-1);
    }

    int count = 0;
    while (fscanf(ptr, "%[^|]|%[^|]|%[^|]|%lf\n", &ticket[count].destination, &ticket[count].trainID, &ticket[count].departureDate, &ticket[count].price) != EOF) {
        count++;
    }
    fclose(ptr);

    char prompt;
    do {
        char destination[50], trainID[6], departureDate[11], departureTime[6];
        double price;
        int id;

        displayTickets();

        printf("\nEnter No. to edit (0 to exit): ");
        rewind(stdin);
        scanf("%d", &id);

        if (id == 0) {
            break;
        }

        if (id < 0 || id > count) {
            printf("\nNo record found!\n");
        } else {
            printf("\nEnter destination (eg. Kuala Lumpur): ");
            rewind(stdin);
            gets(destination);

            printf("Enter train ID (eg. T1001): ");
            rewind(stdin);
            gets(trainID);

            printf("Enter departure date (eg. 21/01/2024): ");
            rewind(stdin);
            gets(departureDate);

            printf("Enter departure time (eg. 07:00): ");
            rewind(stdin);
            gets(departureTime);

            printf("Enter price (eg. 100.00): ");
            rewind(stdin);
            scanf("%lf", &price);

            printf("Confirm update (y/n)? ");
            rewind(stdin);
            char confirm = tolower(getchar());
            if (confirm == 'y') {
                strcpy(ticket[id - 1].destination, destination);
                strcpy(ticket[id - 1].trainID, trainID);
                strcpy(ticket[id - 1].departureDate, departureDate);
                strcpy(ticket[id - 1].departureTime, departureTime);
                ticket[id - 1].price = price;
            } else {
                printf("\nUpdate cancelled!\n");
            }
        }

        printf("\nContinue editing (y/n)? ");
        rewind(stdin);
        prompt = tolower(getchar());

    } while (prompt != 'n');

    FILE *ptrWrite = fopen("tickets.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(ptr, "%s|%s|%s|%.2lf\n", ticket[i].destination, ticket[i].trainID, ticket[i].departureDate, ticket[i].price);
    }

    fclose(ptrWrite);
}

void staffDelete() {
    char prompt;
    int id;
    do {
        struct Ticket ticket[50];

        FILE *ptr = fopen("tickets.txt", "r");
        if (ptr == NULL) {
            printf("An error occurred, please try again!\n");
            exit(-1);
        }

        int count = 0;
        while (fscanf(ptr, "%[^|]|%[^|]|%[^|]|%lf\n", &ticket[count].destination, &ticket[count].trainID, &ticket[count].departureDate, &ticket[count].price) != EOF) {
            count++;
        }
        fclose(ptr);

        displayTickets();

        printf("\nEnter No. to delete (0 to exit): ");
        rewind(stdin);
        scanf("%d", &id);

        if (id != 0) {
            if (id < 0 || id > count) {
                printf("\nNo record found!\n");
            } else {
                FILE *ptrDelete = fopen("tickets.txt", "w");

                ticketHeader();
                printf("%2d  %-15s %-10s %-15s %.2lf\n", id, ticket[id - 1].destination, ticket[id - 1].trainID, ticket[id - 1].departureDate, ticket[id - 1].price);

                printf("\nConfirm delete (y/n)? ");
                rewind(stdin);
                char confirm = tolower(getchar());
                if (confirm == 'y') {
                } else {
                    printf("\nUpdate cancelled!\n");
                    fprintf(ptr, "%s|%s|%s|%.2lf\n", ticket[id - 1].destination, ticket[id - 1].trainID, ticket[id - 1].departureDate, ticket[id - 1].price);
                }
                fclose(ptrDelete);
            }
        }

        printf("\nContinue deleting (y/n)? ");
        rewind(stdin);
        prompt = tolower(getchar());
    } while (prompt == 'y' || id == 0);
}

void member() {
    int choice;
    do {
        printf("\nMember Menu:\n1. Buy Tickets\n2. My Ticket\n3. Back\n\nEnter option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                memberBuy("M12345");
                break;
            case 2:
                memberView("M12345");
                break;
            default:
                printf("\nInvalid option!\n");
                break;
        }
    } while (choice != 3);
}

void memberBuy(char memberID[7]) {
    struct Ticket ticket[50];
    MemberTickets memberTicket;

    displayTickets();

    FILE *ptrView = fopen("tickets.txt", "r");

    if (ptrView == NULL) {
        printf("An error occurred, please try again!\n");
        exit(-1);
    }

    int count = 0;
    while (fscanf(ptrView, "%[^|]|%[^|]|%[^|]|%[^|]|%lf\n", &ticket[count].destination, &ticket[count].trainID, &ticket[count].departureDate, &ticket[count].departureTime, &ticket[count].price) != EOF) {
        count++;
    }
    fclose(ptrView);

    if (count > 0) {
        int id;

        printf("\nEnter ID of ticket you'd like to buy: ");
        rewind(stdin);
        scanf("%d", &id);

        if (id < 1 || id > count) {
            printf("\nNo record found!\n");
        } else {
            do {
                printf("Enter quantity of tickets to buy (max 5): ");
                rewind(stdin);
                scanf("%d", &memberTicket.quantity);
            } while (memberTicket.quantity > 5);

            memberTicket.total = ticket[id - 1].price * memberTicket.quantity;

            char confirm;
            printf("\nGrand total: %.2lf\n", memberTicket.total);
            printf("Proceed (y/n)? ");
            rewind(stdin);
            confirm = tolower(getchar());

            if (confirm == 'y') {
                FILE *ptrWrite = fopen("memberTickets.txt", "a+");

                time_t now = time(NULL);
                struct tm *local = localtime(&now);

                int day, month, year;
                char date[11];

                day = local->tm_mday;
                month = local->tm_mon + 1;
                year = local->tm_year + 1900;

                sprintf(date, "%02d/%02d/%d", day, month, year);

                generateID('B', 5, memberTicket.bookingID);
                memberTicket.ticket = ticket[id - 1];

                strcpy(memberTicket.bookingDate, date);

                for (int i = 0; i < memberTicket.quantity; i++) {
                    char coach;
                    int seat;

                    do {
                        printf("\nEnter desired seat for ticket %d (coach: A-F, seat: 1-48) eg. A14: ", i + 1);
                        rewind(stdin);
                        scanf("%c%d", &coach, &seat);

                        coach = toupper(coach);
                    } while ((coach < 'A' || coach > 'F') || (seat < 1 || seat > 48));

                    sprintf(memberTicket.seats[i], "%c%02d", coach, seat);
                }

                strcpy(memberTicket.status, "Booked");

                fprintf(ptrWrite, "%s|%s|%s|%s|%s|%s|", memberID, memberTicket.bookingID, memberTicket.ticket.destination, memberTicket.ticket.trainID, memberTicket.ticket.departureDate, memberTicket.ticket.departureTime, memberTicket.bookingDate);

                for (int i = 0; i < memberTicket.quantity; i++) {
                    fprintf(ptrWrite, "%s", memberTicket.seats[i]);
                    if (i < memberTicket.quantity - 1) {
                        fprintf(ptrWrite, ",");
                    }
                }

                fprintf(ptrWrite, "|%d|%.2lf|%s\n", memberTicket.quantity, memberTicket.total, memberTicket.status);

                printf("Booking Successful!\n");
                fclose(ptrWrite);
            }
        }
    }
}

void memberView(char memberID[7]) {
    MemberTickets memberTicket;

    printf("\n%-15s %-15s %-10s %-10s %-15s %-15s %-15s %-10s %s\n", "Booking ID", "Destination", "Train ID", "Seat(s)", "Departure Date", "Departure Time", "Booking Date", "Total", "Status");
    printf("========================================================================================================================\n");

    FILE *ptr = fopen("memberTickets.txt", "r");
    if (ptr == NULL) {
        printf("An error occurred, please try again!\n");
        exit(-1);
    }

    while (fscanf(ptr, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%lf|%s\n", &memberTicket.memberID, &memberTicket.bookingID, &memberTicket.ticket.destination, &memberTicket.ticket.trainID, &memberTicket.ticket.departureDate, &memberTicket.ticket.departureTime, &memberTicket.bookingDate, &memberTicket.seats, &memberTicket.quantity, &memberTicket.total, &memberTicket.status) != EOF) {
        if (strcmp(memberTicket.memberID, memberID) == 0) {
            printf("%-15s %-15s %-10s %-10s %-15s %-15s %-15s %-10.2lf %s\n", memberTicket.bookingID, memberTicket.ticket.destination, memberTicket.ticket.trainID, memberTicket.seats, memberTicket.ticket.departureDate, memberTicket.ticket.departureTime, memberTicket.bookingDate, memberTicket.total, memberTicket.status);
        } else {
            printf("No records found.\n");
        }
    }
    fclose(ptr);
    int choice;
    do {
        printf("\nWhat would you like to do? \n\n");
        printf("1. Change Time\n2. Cancel Ticket\n3. Back\n\n");

        printf("Enter option: ");
        rewind(stdin);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                printf("\nInvalid option!\n");
                system("pause");
                break;
        }
    } while (choice != 3);
}

void memberEdit() {
    printf("\nUnder construction!\n");
}

void memberCancel() {
    printf("\nUnder construction!\n");
}

void displayHeader() {
    printf("\n%-3s %-15s %-10s %-15s %-15s %s\n", "ID", "Destination", "Train ID", "Departure Date", "Departure Time", "Price");
    printf("=======================================================================\n");
}

void generateID(char head, int length, char *id) {
    id[0] = head;
    srand(time(NULL));
    for (int i = 1; i < length; i++) {
        id[i] = '0' + rand() % 10;
    }
    id[length] = '\0';
}