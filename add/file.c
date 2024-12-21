#include <stdio.h>
#include "file.h"
#include <string.h>

void saveContactsToFile(AddressBook *addressBook) {
    FILE *fp;
    // Open the file in write mode
    fp=fopen("addressbook.csv","w");
    if (fp == NULL) {
        printf("Error opening file");
        return;
    }
    // Write the struct data to the CSV file

    for (int i = 0; i < addressBook->contactCount; i++) {
        fprintf(fp, "%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    fclose(fp);
}//works fine

void loadContactsFromFile(AddressBook *addressBook) {

    printf("Loaded contacts from the storage(csv file):\n");
    FILE *fp;
    fp=fopen("addressbook.csv","r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }
    addressBook->contactCount = 0;
    char temp[121];
    char temp1[51];
    
    while (fgets(temp, sizeof(temp), fp))
    {
        int i=0,j=0;
        memset(temp1, '\0', 51);
        while(temp[i]!=','&&temp[i] != '\0'){
            temp1[i]=temp[i];
            i++;
        }
        strcpy(addressBook->contacts[addressBook->contactCount].name, temp1);
        memset(temp1, '\0', 51);
        i++;
        while(temp[i]!=','&&temp[i] != '\0'){
            temp1[j]=temp[i];
            i++;
            j++;
        }
        strcpy(addressBook->contacts[addressBook->contactCount].phone, temp1);
        memset(temp1, '\0', 51);
        i++;
        j=0;
        while(temp[i]!=','&&temp[i] != '\0'&&temp[i]!='\n'){
            temp1[j]=temp[i];
            i++;
            j++;
        }
        strcpy(addressBook->contacts[addressBook->contactCount].email, temp1);
        addressBook->contactCount++;
    }
    fclose(fp);
    
}
