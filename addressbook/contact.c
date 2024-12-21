#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"
#include <unistd.h>

//compare function for name
int compareByName(const void *a, const void *b) {
    return strcmp(((Contact *)a)->name, ((Contact *)b)->name);
}

//compare function for phone
int compareByPhone(const void *a, const void *b) {
    return strcmp(((Contact *)a)->phone, ((Contact *)b)->phone);
}

//compare function for email
int compareByEmail(const void *a, const void *b) {
    return strcmp(((Contact *)a)->email, ((Contact *)b)->email);
}

void swapContacts(Contact *a, Contact *b) {     //function to swap two contacts
    Contact temp = *a;
    *a = *b;
    *b = temp;
}

void listContacts(AddressBook *addressBook, int sortCriteria) {//fubnction to list or display contacts
    // Sorting contacts based on the chosen criteria
    switch (sortCriteria){
        case 1:     //sort by name
            for(int i=0;i<addressBook->contactCount-1;i++){
                for(int j=0;j<addressBook->contactCount-i-1;j++){
                    if(compareByName(&addressBook->contacts[j], &addressBook->contacts[j + 1]) > 0){
                        swapContacts(&addressBook->contacts[j], &addressBook->contacts[j + 1]);
                    }
                }
            }
            break;
        case 2:     //sort by phone
            for(int i=0;i<addressBook->contactCount-1;i++){
                for(int j=0;j<addressBook->contactCount-i-1;j++){
                    if(compareByPhone(&addressBook->contacts[j], &addressBook->contacts[j + 1]) > 0){
                        swapContacts(&addressBook->contacts[j], &addressBook->contacts[j + 1]);
                    }

                }
            }
            break;
        case 3:     //sort by email
            for(int i=0;i<addressBook->contactCount-1;i++){
                for(int j=0;j<addressBook->contactCount-i-1;j++){
                    if(compareByEmail(&addressBook->contacts[j], &addressBook->contacts[j + 1]) > 0){
                        swapContacts(&addressBook->contacts[j], &addressBook->contacts[j + 1]);
                    }
                }
            }
            break;
        case 4:     //no sort or print by serial number
            break;
        default:    //invalid choice
            printf("Invalid choice. Please try again.\n");
            return;
    }  
    printf("\nS.no\tName\tMobile\t    Email\n");
    for(int i=0;i<addressBook->contactCount;i++){       //displaying the contacts
        printf("%d. %s ,%s ,%s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
}

void initialize(AddressBook *addressBook) {             //function to initialize the addressbook
    FILE *fp;
    fp=fopen("addressbook.csv","r");                    //opening the file
    if (fp == NULL){                                    //if file is not present populate the addressbook with default contacts
        addressBook->contactCount = 0;
        populateAddressBook(addressBook);               
    } 
    else {
        loadContactsFromFile(addressBook);              //if file is present load the contacts from the file
    }
    fclose(fp);                                    //closing the file
}

//function to save and exit
void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

void createContact(AddressBook *addressBook){        //function to create a new contact OR add a new contact
    int flag1=1,len=0,flag2=0,flag3=0;        //flags to check the validity of the input
    char temp1[50];                         //buffer to store the input
    printf("Enter the name:");
    scanf("%s %[^\n]",addressBook->contacts[addressBook->contactCount].name,temp1); //taking the input for name
    //adding a space at the end of string
    addressBook->contacts[addressBook->contactCount].name[strlen(addressBook->contacts[addressBook->contactCount].name)]=' ';
    //add temp1 to the end of addressBook->contacts[addressBook->contactCount].name
    strcat(addressBook->contacts[addressBook->contactCount].name, temp1);

    printf("Enter the phone number:");
    scanf("%s",addressBook->contacts[addressBook->contactCount].phone); //taking the input for phone number
    while(flag1){    //checking the validity of the phone number
        len=0;
        while(addressBook->contacts[addressBook->contactCount].phone[len]!='\0'){   //checking the length of the phone number
            len++;
        }
        if(len!=10){    //if the length is not 10 then ask the user to enter a valid phone number
            printf("Invalid size of phone number. please enter a valid number :");
            scanf("%s",addressBook->contacts[addressBook->contactCount].phone);
            continue;
        }
        len=0;
        while(len<10){  //checking if the phone number has any characters
            if(addressBook->contacts[addressBook->contactCount].phone[len]>='0'&&addressBook->contacts[addressBook->contactCount].phone[len]<='9'){
                len++;
                continue;
            }
            else{   //if the phone number has characters then ask the user to enter a valid phone number
                printf("Phone number cant have charactersas input. please enter a valid number :");
                scanf("%s",addressBook->contacts[addressBook->contactCount].phone);
                len=99;
            }
        }
        if(len==99){
            len=0;
            continue;
        }
        flag3=0;
        len=0;
        while(len<addressBook->contactCount){   //checking if the phone number already exists
            if(compareByPhone(&addressBook->contacts[addressBook->contactCount],&addressBook->contacts[len])==0){
                flag3=1;
                break;
            }
            len++;
        }
        if(flag3){          //if the phone number already exists then ask the user to enter a new phone number
            printf("phone number already exists. please enter another number :");
            scanf("%s",addressBook->contacts[addressBook->contactCount].phone);
            continue;
        }
        flag1=0;
    }
    printf("Enter the email:");     //taking the input for email
    scanf("%s",addressBook->contacts[addressBook->contactCount].email);
    flag1=1;
    while(flag1){
        len=0;
        flag2=0;
        flag3=0;
        char *result = strstr(addressBook->contacts[addressBook->contactCount].email, "@");
        if(result!=NULL&&result!=addressBook->contacts[addressBook->contactCount].email){   //checking if the email has @
            flag3=1;
        }
        char *result1 = strstr(addressBook->contacts[addressBook->contactCount].email, ".com");
        if(result1!=NULL&&result1!=result+1){           //checking if the email has .com
            flag2=1;
        }
        if(flag2!=1||flag3!=1){    //if the email does not have @ or .com then ask the user to enter a valid email
            printf("email is Invalid. please enter another email :");   
            scanf("%s",addressBook->contacts[addressBook->contactCount].email);
            continue;
        }
        flag2=0;
        flag3=0;
        while(len<addressBook->contactCount){   //checking if the email already exists
            if(compareByEmail(&addressBook->contacts[addressBook->contactCount],&addressBook->contacts[len])==0){
                flag3=1;
                break;
            }
            len++;
        }
        if(flag3){      //if the email already exists then ask the user to enter a new email
            printf("email already exists. please enter another email :");
            scanf("%s",addressBook->contacts[addressBook->contactCount].email);
            continue;
        }
        flag1=0;
    }
    addressBook->contactCount++;    //increment the contact count
}

//function to search a contact
void searchContact(AddressBook *addressBook) {
    int choice;
    printf("\noptions:\n");
    printf("1. search by name\n"); 
    printf("2. search by number\n");
    printf("3. search by email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    char temp[50];  //buffer to store the input
    int flag=0;     //flag to check if the contact is found or not
    
    switch (choice) {
        case 1:             //search by name
            printf("Enter the name:");
            scanf(" %[^\n]",temp);  //taking the input for name
            flag=0;
            for(int i=0;i<addressBook->contactCount;i++){   //searching the contact by name
                if(strcmp(temp,addressBook->contacts[i].name)==0){  //if the contact is found then display the contact
                    printf("The phone number and email with name %s are %s and %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                    flag++;
                    break;
                }
            }
            if(flag==0){            //if the contact is not found then display the message
                printf("not found any contacts with named %s\n",temp);
            }
            break;
        case 2:             //search by phone number
            printf("Enter the phone number:");
            scanf("%s",temp);   //taking the input for phone number
            flag=0;
            for(int i=0;i<addressBook->contactCount;i++){   //searching the contact by phone number
                if(strcmp(temp,addressBook->contacts[i].phone)==0){   //if the contact is found then display the contact
                    printf("The name and email with phone number %s are %s and %s\n",addressBook->contacts[i].phone,addressBook->contacts[i].name,addressBook->contacts[i].email);
                    flag++;
                    break;
                }
            }
            if(flag==0){                //if the contact is not found then display the message
                printf("not found any contacts with this phone number %s\n",temp);
            }
            break;
        case 3:             //search by email
            printf("Enter the email:");
            scanf(" %s",temp);  //taking the input for email
            flag=0;
            for(int i=0;i<addressBook->contactCount;i++){   //searching the contact by email
                if(strcmp(temp,addressBook->contacts[i].email)==0){  //if the contact is found then display the contact
                    printf("The name and phone number with email %s are %s and %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                    flag++;
                    break;
                }
            }
            if(flag==0){            //if the contact is not found then display the message
                printf("not found any contacts with this email %s\n",temp);
            }
            break;

        default:        //if the choice is invalid then display the message
            printf("Invalid choice. Please try again.\n");
            break;
    }
}

//function to edit a contact
void editContact(AddressBook *addressBook){
    int choice;
    printf("\noptions:\n");
    printf("1. Edit name\n");
    printf("2. Edit number\n");
    printf("3. Edit email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    char temp[50];      //buffer to store the search input
    char new[50];       //buffer to store the new input
    int arr[addressBook->contactCount];    //array to store the index of the found contacts
    int flag=0;
    switch (choice) {
        case 1:         //edit name. multiple contacts with same name can be edited
            printf("Enter the name to be edited:");
            scanf(" %[^\n]",temp);      //taking the input for name
            int choice2;
            flag=0;
            for(int i=0;i<addressBook->contactCount;i++){   //searching the contact by name
                if(strcmp(temp,addressBook->contacts[i].name)==0){  //if the contact is found then store the index in the array
                    arr[flag]=i;
                    flag++;
                }
            }
            if(flag==0){        //if the contact is not found then display the message
                printf("not found any contacts with named %s\n",temp);
            }
            else{
                if(flag==1){    //if only one contact is found then edit the contact
                    printf("found %d .%s ,%s ,%s\n",arr[0],addressBook->contacts[arr[0]].name,addressBook->contacts[arr[0]].phone,addressBook->contacts[arr[0]].email);
                    printf("Enter the new name :");
                    scanf("%s",new);
                    strcpy(addressBook->contacts[arr[0]].name,new);//edit the contact
                }
                else{               //if multiple contacts are found then display the contacts and ask the user to select the contact to edit
                    printf("found %d %s in the addressbook\n",flag,temp);
                    for(int i=0;i<flag;i++){
                        printf("%d .%s ,%s ,%s\n",i+1,addressBook->contacts[arr[i]].name,addressBook->contacts[arr[i]].phone,addressBook->contacts[arr[i]].email);
                    }
                    printf("Select one %s to edit:\n",temp);
                    scanf("%d",&choice2);
                    printf("Enter the new name :");
                    scanf("%s",new);
                    strcpy(addressBook->contacts[arr[choice2-1]].name,new);//edit the contact
                }
            }
            break;
        case 2:     //edit phone number. no two contacts can have the same phone number
            printf("Enter the phone number to be edited:");
            scanf("%s",temp);   //taking the input for phone number
            flag=0;
            int flag1=1,flag3,len;
            for(int i=0;i<addressBook->contactCount;i++){   //searching the contact by phone number
                if(strcmp(temp,addressBook->contacts[i].phone)==0){  //if the contact is found then edit the contact
                    printf("Enter the new phone numbers :");
                    scanf("%s",new);        //taking the input for new phone number
                    while(flag1){
                        len=0;
                        while(new[len]!='\0'){
                            len++;
                        }
                        if(len!=10){    //checking the length of the phone number
                            printf("Invalid size of phone number. please enter a valid number :");
                            scanf("%s",new);
                            continue;
                        }
                        len=0;
                        while(len<10){  //checking if the phone number has any characters
                            if(new[len]>='0'&&new[len]<='9'){
                                len++;
                                continue;
                            }
                            else{
                                printf("Phone number cant have charactersas input. please enter a valid number :");
                                scanf("%s",new);
                                len=99;
                            }
                        }
                        if(len==99){    //if the phone number has characters then ask the user to enter a valid phone number
                            len=0;
                            printf("phone number has character. please enter a valid number :");
                            scanf("%s",new);
                            continue;
                        }
                        flag3=0;
                        len=0;
                        while(len<addressBook->contactCount){   //checking if the phone number already exists
                            if(strcmp(new,addressBook->contacts[len].phone)==0){
                                flag3=1;
                                break;
                            }
                            len++;
                        }
                        if(flag3){    //if the phone number already exists then ask the user to enter a new phone number
                            printf("phone number already exists. please enter another number :");
                            scanf("%s",new);
                            continue;
                        }
                        flag1=0;
                    }
                    strcpy(addressBook->contacts[i].phone,new);//edit the contact
                    flag++;
                    break;
                }
            }
            if(flag==0){    //if the contact is not found then display the message
                printf("not found any contacts with this phone number %s\n",temp);
            }
            break;
        case 3:    //edit email. no two contacts can have the same email
            printf("Enter the email to be edited:");
            scanf(" %s",temp);  //taking the input for email
            flag=0;
            int flag2;
            for(int i=0;i<addressBook->contactCount;i++){   //searching the contact by email
                if(strcmp(temp,addressBook->contacts[i].email)==0){
                    printf("Enter the new email :");
                    scanf("%s",new);        //taking the input for new email
                    while(flag1){        //checking the validity of the email
                        len=0;
                        flag2=0;
                        flag3=0;
                        char *result = strstr(new, "@");
                        if(result!=NULL&&result!=new){
                            flag3=1;
                        }
                        char *result1 = strstr(new, ".com");
                        if(result1!=NULL&&result1!=result+1){
                            flag2=1;
                        }
                        if(flag2!=1||flag3!=1){   //if the email does not have @ or .com then ask the user to enter a valid email
                            printf("email is Invalid. please enter another email :");
                            scanf("%s",new);
                            continue;
                        }
                        flag2=0;
                        flag3=0;
                        while(len<addressBook->contactCount){   //checking if the email already exists
                            if(strcmp(new,addressBook->contacts[len].email)==0){
                                flag3=1;
                                break;
                            }
                            len++;
                        }
                        if(flag3){  //if the email already exists then ask the user to enter a new email
                            printf("email already exists. please enter another email :");
                            scanf("%s",new);
                            continue;
                        }
                        flag1=0;
                    }
                    strcpy(addressBook->contacts[i].email,new);  //edit the contact
                    flag++;
                    break;
                }
            }
            if(flag==0){    //if the contact is not found then display the message
                printf("not found any contacts with this email %s\n",temp);
            }
            break;
        default:    //if the choice is invalid then display the message
            printf("Invalid choice. Please try again.\n");
            break;
    }   
}

//function to delete a contact
void deleteContact(AddressBook *addressBook){
    int choice;
    printf("\noptions:\n");
    printf("1. delete by name\n"); 
    printf("2. delete by phone number\n");
    printf("3. delete by email\n");
    printf("4. delete by s.no\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    char temp[50];          //buffer to store the search input
    int choice2;
    int arr[addressBook->contactCount];   //array to store the index of the found contacts
    int flag=0;
    switch (choice) {
            case 1:    //delete by name. multiple contacts with same name can be deleted
                printf("Enter the contact with name to be deleted:");
                scanf(" %[^\n]",temp);
                int choice2;
                flag=0;
                for(int i=0;i<addressBook->contactCount;i++){
                    if(strcmp(temp,addressBook->contacts[i].name)==0){
                        arr[flag]=i;
                        flag++;
                    }
                }
                if(flag==0){
                    printf("not found any contacts with named %s\n",temp);
                }
                else{
                    if(flag==1){
                        printf("%d .%s ,%s ,%s\n",arr[0],addressBook->contacts[arr[0]].name,addressBook->contacts[arr[0]].phone,addressBook->contacts[arr[0]].email);
                        choice2=0;
                    }
                    else{
                        printf("found %d %s in the addressbook\n",flag,temp);
                        for(int i=0;i<flag;i++){
                            printf("%d .%s ,%s ,%s\n",i+1,addressBook->contacts[arr[i]].name,addressBook->contacts[arr[i]].phone,addressBook->contacts[arr[i]].email);
                        }
                        printf("Select one %s to deleted:\n",temp);
                        scanf("%d",&choice2);
                        choice2-=1;
                        printf("deleting %s, %s, %s\n",addressBook->contacts[arr[choice2]].name,addressBook->contacts[arr[choice2]].phone,addressBook->contacts[arr[choice2]].email);
                    }
                    for(int i=arr[choice2];i<addressBook->contactCount-1;i++){
                        addressBook->contacts[i]=addressBook->contacts[i+1];
                    } 
                    addressBook->contactCount--;
                }
                break;
            case 2:    //delete by phone number
                printf("Enter the contact with phone number to be deleted:");
                scanf("%s",temp);
                flag=-1;
                for(int i=0;i<addressBook->contactCount;i++){
                    if(strcmp(temp,addressBook->contacts[i].phone)==0){
                        flag=i;
                        break;
                    }
                }
                if(flag==-1){
                    printf("not found any contacts with this phone number %s\n",temp);
                }
                else{
                    printf("deleting %s, %s, %s\n",addressBook->contacts[flag].name,addressBook->contacts[flag].phone,addressBook->contacts[flag].email);
                    for(int i=flag;i<addressBook->contactCount-1;i++){
                        addressBook->contacts[i]=addressBook->contacts[i+1];
                    }
                    addressBook->contactCount--;
                }
                break;
            case 3:   //delete by email
                printf("Enter the contact with email to be deleted:");
                scanf(" %s",temp);
                flag=-1;
                for(int i=0;i<addressBook->contactCount;i++){
                    if(strcmp(temp,addressBook->contacts[i].email)==0){
                        flag=i;
                        break;
                    }
                }
                if(flag==-1){
                    printf("not found any contacts with this email %s\n",temp);
                }
                else{
                    printf("deleting %s, %s, %s\n",addressBook->contacts[flag].name,addressBook->contacts[flag].phone,addressBook->contacts[flag].email);
                    for(int i=flag;i<addressBook->contactCount-1;i++){
                        addressBook->contacts[i]=addressBook->contacts[i+1];
                    }
                    addressBook->contactCount--;
                }
                break;
            case 4:  //delete by serial number
                for(int i=0;i<addressBook->contactCount;i++){
                    printf("%d. %s ,%s ,%s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                }
                printf("Enter the serial number of the contact that need to be deleted :");
                scanf("%d",&choice2);
                while(choice2<1 || choice2>addressBook->contactCount){
                    printf("Invalid choice. Please try again.\n");
                    printf("Enter the serial number of the contact that need to be deleted :");
                    scanf("%d",&choice2);
                }
                printf("deleting %s, %s, %s\n",addressBook->contacts[choice2-1].name,addressBook->contacts[choice2-1].phone,addressBook->contacts[choice2-1].email);
                for(int i=choice2-1;i<addressBook->contactCount-1;i++){
                    addressBook->contacts[i]=addressBook->contacts[i+1];
                }
                addressBook->contactCount--;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
}
