#include <stdio.h>
#include "file.h"
#include <string.h>

//function to save contacts to the file
void saveContactsToFile(AddressBook *addressBook) {
    FILE *fp;
    // Open the file in write mode
    fp=fopen("addressbook.csv","w");
    if (fp == NULL) {       // Check if the file is opened successfully
        printf("Error opening file");
        return;
    }
    // Write the struct data to the CSV file
    for (int i = 0; i < addressBook->contactCount; i++) {
        fprintf(fp, "%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    fclose(fp);
}

//function to Load contacts from the file
void loadContactsFromFile(AddressBook *addressBook) {

    printf("Loaded contacts from the storage(csv file):\n");
    FILE *fp;
    fp=fopen("addressbook.csv","r");            // Open the file in read mode
    if (fp == NULL) {                                  // Check if the file is opened successfully
        printf("Error opening file\n");
        return;
    }
    addressBook->contactCount = 0;                    // Initialize the contact count to 0
    char temp[121];                                     // Temporary buffer to read the file
    char temp1[51];                                     // Temporary buffer to store the name, phone and email
    
    while (fgets(temp, sizeof(temp), fp))               // Read the file line by line
    {
        int i=0,j=0;
        memset(temp1, '\0', 51);             // Initialize the temporary buffer to 0
        while(temp[i]!=','&&temp[i] != '\0'){    // Read the name from the file
            temp1[i]=temp[i];
            i++;
        }
        strcpy(addressBook->contacts[addressBook->contactCount].name, temp1);   // Copy the name to the address book
        memset(temp1, '\0', 51);            // Initialize the temporary buffer to 0
        i++;
        while(temp[i]!=','&&temp[i] != '\0'){   // Read the phone from the file
            temp1[j]=temp[i];
            i++;
            j++;
        }
        strcpy(addressBook->contacts[addressBook->contactCount].phone, temp1);  // Copy the phone to the address book
        memset(temp1, '\0', 51);            // Initialize the temporary buffer to 0
        i++;
        j=0;
        while(temp[i]!=','&&temp[i] != '\0'&&temp[i]!='\n'){    // Read the email from the file
            temp1[j]=temp[i];
            i++;
            j++;
        }
        strcpy(addressBook->contacts[addressBook->contactCount].email, temp1);  // Copy the email to the address book
        addressBook->contactCount++;    // Increment the contact count
    }
    fclose(fp);   // Close the file
    
}
