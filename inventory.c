#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>


//data structure
typedef struct {
    char name[50];
    int quantity;
    float value;
} Item;

typedef struct {
    Item* items;
    int size;
    int capacity;
} Inventory;

//encryption function to use with password and username 
void encryptCaesar(char *text, int key) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = 'a' + (text[i] - 'a' + key) % 26;
        } else if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = 'A' + (text[i] - 'A' + key) % 26;
        }
    }
}


// decryption funciton
void dencryptCaesar(char *text, int key) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = 'a' - (text[i] - 'a' + key) % 26;
        } else if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = 'A' - (text[i] - 'A' + key) % 26;
        }
    }
}

// function to check if the password and username is correct
bool login(char* username, char* password){
    char user[7] = "eabtqm";
    int key = 27364;
    char pass[255] = "fqmotqddazmz";
    encryptCaesar(username,key);
    encryptCaesar(password,key);
    // pass teacherronan username sophea
    if (strcmp(user,username )== 0 && strcmp(pass,password)==0){
        system("clear");
        return true;
    } else {
        return false;
    }
}

// initialize the inventory
void initializeInventory(Inventory* inv) {
    inv->size = 0;
    inv->capacity = 2;
    inv->items = malloc(inv->capacity * sizeof(Item));
    if (inv->items == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
}

// save the inventory data to a text file
void saveInventoryToFile(const Inventory* inv, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%d\n", inv->size); // Save size first
    for (int i = 0; i < inv->size; i++) {
        fprintf(file, "%s\n%d\n%f\n", inv->items[i].name, inv->items[i].quantity, inv->items[i].value);
    }

    fclose(file);
}

// get the inventory data from a text file 
void loadInventoryFromFile(Inventory* inv, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    fscanf(file, "%d\n", &inv->size);
    inv->capacity = inv->size + 5;
    inv->items = malloc(inv->capacity * sizeof(Item));

    for (int i = 0; i < inv->size; i++) {
        fgets(inv->items[i].name, sizeof(inv->items[i].name), file);
        inv->items[i].name[strcspn(inv->items[i].name, "\n")] = 0; // Remove newline
        fscanf(file, "%d\n%f\n", &inv->items[i].quantity, &inv->items[i].value);
    }

    fclose(file);
}

// a new item to inventory
void addItem(Inventory* inv) {
    if (inv->size >= inv->capacity) {
        inv->capacity *= 2;
        Item* temp = realloc(inv->items, inv->capacity * sizeof(Item));
        if (temp == NULL) {
            printf("Memory reallocation failed.\n");
            exit(1);
        }
        inv->items = temp;
    }

    printf("Enter item name: ");
    scanf(" %s", inv->items[inv->size].name);

    printf("Enter quantity: ");
    scanf("%d", &inv->items[inv->size].quantity);

    printf("Enter value: ");
    scanf("%f", &inv->items[inv->size].value);

    inv->size++;
    printf("Item added successfully!\n");
}

void removeItem(Inventory* inv) {
    char itemName[50];
    printf("Enter the name of the item to remove: ");
    scanf("%s", itemName);  // Read user input

    int found = 0;
    for (int i = 0; i < inv->size; i++) {
        if (strcmp(inv->items[i].name, itemName) == 0) {
            found = 1;
            // Shift items to the left to overwrite the removed item
            for (int j = i; j < inv->size - 1; j++) {
                inv->items[j] = inv->items[j + 1];
            }
            inv->size--;
            printf("Item '%s' removed from inventory.\n", itemName);
            break;
        }
    }

    if (!found) {
        printf("Item '%s' not found in inventory.\n", itemName);
    }
}

//update the information of an item
void updateItem(Inventory* inv) {
    char itemName[50];
    printf("Enter the name of the item to update: ");
    scanf("%49s", itemName);

    int found = 0;
    for (int i = 0; i < inv->size; i++) {
        if (strcmp(inv->items[i].name, itemName) == 0) {
            found = 1;
            printf("Enter new quantity: ");
            scanf("%d", &inv->items[i].quantity);
            printf("Enter new value: ");
            scanf("%f", &inv->items[i].value);
            printf("Item '%s' updated successfully.\n", itemName);
            break;
        }
    }

    if (!found) {
        printf("Item '%s' not found in inventory.\n", itemName);
    }
}

// print all the items in inventory
void viewInventory(const Inventory* inv) {
    printf("\n--- Inventory List ---\n");
    for (int i = 0; i < inv->size; i++) {
        printf("Item %d: %s | Quantity: %d | Value: %.2f\n",
               i + 1, inv->items[i].name, inv->items[i].quantity, inv->items[i].value);
    }
    printf("----------------------\n");
}

// get the name and output all information about the items
void searchItem(const Inventory* inv) {
    char searchName[50];
    printf("Enter item name to search: ");
    // scan until new line
    scanf(" %s", searchName);

    int found = 0;
    for (int i = 0; i < inv->size; i++) {
        if (strcmp(inv->items[i].name, searchName) == 0) {
            printf("Found: %s | Quantity: %d | Value: %.2f\n",
                   inv->items[i].name, inv->items[i].quantity, inv->items[i].value);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Item not found.\n");
    }
}

//free the memory of inventory
void freeInventory(Inventory* inv) {
    free(inv->items);
    inv->items = NULL;
    inv->size = 0;
    inv->capacity = 0;
}

//write all the information of an inventory to a text file
void export(const Inventory* inv) {
    char header[100] = "\n---------Inventory management-----------\n";
    char body[200];
    FILE* ptr;
    ptr = fopen("./text.txt", "w");
    if (ptr == NULL) {
        printf("memory allocation fault.");
        return;
    }
    fputs(header, ptr);
    for (int i = 0; i < inv->size; i++) {
        snprintf(body, sizeof(body), "item:%s | quantity:%d | value:%.2f\n", inv->items[i].name, inv->items[i].quantity, inv->items[i].value);
        fputs(body, ptr);
    }
    fclose(ptr);
}

// write a recript in a text file
// use for the purchase
void writeReceipt(const Inventory* inv) {
    time_t currenttime;
    time(&currenttime);

    int receiptnum = 10001;

    char header[] =
        "--------------------------------------------------\n"
        "\t\t\t\tRECEIPT\n"
        "--------------------------------------------------\n"
        "Business Name: Sigma Management\n"
        "Address:\tSIGMA MALE house\n"
        "Phone:\t\t01023424\n"
        "Email:\t\tsigma@sigmail.com\n"
        "--------------------------------------------------\n";

    char title[200];
    sprintf(title, "\tReceipt No: %d\n\tDate: %s\n", receiptnum, ctime(&currenttime));

    FILE* ptr = fopen("./text.txt", "w");
    if (ptr == NULL) {
        printf("Could not open file for writing.\n");
        return;
    }

    fputs(header, ptr);
    fputs(title, ptr);

    fprintf(ptr, "Items Purchased:\n");
    float total = 0.0;
    for (int i = 0; i < inv->size; i++) {
        fprintf(ptr, "%d. %s\tQty: %d\tPrice: %.2f\tSubtotal: %.2f\n",
                i + 1,
                inv->items[i].name,
                inv->items[i].quantity,
                inv->items[i].value,
                inv->items[i].quantity * inv->items[i].value);
        total += inv->items[i].quantity * inv->items[i].value;
    }

    fprintf(ptr, "--------------------------------------------------\n");
    fprintf(ptr, "Total Amount: %.2f\n", total);
    fprintf(ptr, "--------------------------------------------------\n");
    fprintf(ptr, "Thank you for your purchase!\n");

    fclose(ptr);
}
// Sorting the inventory
// sort by name
int compareByNameCaseInsensitive(const void* a, const void* b) {
    const Item* item1 = (const Item*)a;
    const Item* item2 = (const Item*)b;
    return strcasecmp(item1->name, item2->name);
}

void sortByName(Inventory* inv) {
    if (inv->size < 2) {
        printf("Not enough items to sort.\n");
        return;
    }
    qsort(inv->items, inv->size, sizeof(Item), compareByNameCaseInsensitive);
    printf("Inventory sorted alphabetically (Aa Bb Cc).\n");
}


//sort by quantity
int compareByQuantityDesc(const void* a, const void* b) {
    const Item* item1 = (const Item*)a;
    const Item* item2 = (const Item*)b;
    return item2->quantity - item1->quantity;
}

void sortByQuantity(Inventory* inv) {
    if (inv->size < 2) {
        printf("Not enough items to sort.\n");
        return;
    }
    qsort(inv->items, inv->size, sizeof(Item), compareByQuantityDesc);
    printf("Inventory sorted by quantity (highest price).\n");
}

//sort by price/value
int compareByPriceDesc(const void* a, const void* b) {
    const Item* item1 = (const Item*)a;
    const Item* item2 = (const Item*)b;
    if (item2->value > item1->value) return 1;
    else if (item2->value < item1->value) return -1;
    else return 0;
}

void sortByPrice(Inventory* inv) {
    if (inv->size < 2) {
        printf("Not enough items to sort.\n");
        return;
    }
    qsort(inv->items, inv->size, sizeof(Item), compareByPriceDesc);
    printf("Inventory sorted by price (highest price).\n");
}

// print the most valueable item
Item getMostValuableItem(const Inventory* inv) {
    if (inv->size == 0) {
        Item dummy = {"", 0, 0.0f};
        return dummy;
    }
    int maxIdx = 0;
    float maxValue = inv->items[0].quantity * inv->items[0].value;
    for (int i = 1; i < inv->size; i++) {
        float totalValue = inv->items[i].quantity * inv->items[i].value;
        if (totalValue > maxValue) {
            maxValue = totalValue;
            maxIdx = i;
        }
    }
    return inv->items[maxIdx];
}

//print the item that have the least quantity
Item getLeastStockedItem(const Inventory* inv) {
    if (inv->size == 0) {
        Item dummy = {"", 0, 0.0f};
        return dummy;
    }
    int minIdx = 0;
    int minQty = inv->items[0].quantity;
    for (int i = 1; i < inv->size; i++) {
        if (inv->items[i].quantity < minQty) {
            minQty = inv->items[i].quantity;
            minIdx = i;
        }
    }
    return inv->items[minIdx];
}

// find the average value/price
// use in purchase mode
float getAverageItemValue(const Inventory* inv) {
    if (inv->size == 0) return 0.0f;
    float sum = 0.0f;
    for (int i = 0; i < inv->size; i++) {
        sum += inv->items[i].value;
    }
    return sum / inv->size;
}

// print the menu for sorting
void showSort(Inventory* inv){
    printf("1. sort item by name\n2. sort by quantity\n3. sort by Price\n");
    int choice = 0;
    scanf("%d",&choice);
    switch (choice){
        case 1: sortByName(inv); viewInventory(inv); break;
        case 2: sortByQuantity(inv); viewInventory(inv); break;
        case 3: sortByPrice(inv); viewInventory(inv); break;
    }
}

// enter the purchase mode
void purchase(Inventory* inv) {
    // init ann inventory for the purchased items
    Inventory pur_items;
    pur_items.capacity = 5;
    pur_items.size = 0;
    pur_items.items = malloc(sizeof(Item) * pur_items.capacity);

    // get current time and date
    time_t currenttime;
    time(&currenttime); 
    printf("\n------------------Purchase Mode------------------\n");

    // ask the user to input the name of item that they want to purchase
    while (1) {
        char searchName[50];
        printf("\nEnter item name to search (or type 'total' to finish): ");
        scanf(" %s", searchName);
        // check if they typed "total"
        // if yes we sum up the purchase and write them a reciept
        if (strcmp(searchName, "total") == 0) {
            printf("\n-------- Purchase Summary --------\n");
            float totalValue = 0.0;
            for (int i = 0; i < pur_items.size; i++) {
                printf("%d. %s | Value: %.2f | Quantity: %d\n", i + 1,
                       pur_items.items[i].name,
                       pur_items.items[i].value,
                       pur_items.items[i].quantity);
                totalValue += pur_items.items[i].value * pur_items.items[i].quantity;
            }
            printf("Total items: %d\n", pur_items.size);
            printf("Total value: %.2f\n", totalValue);

            // Write to file
            FILE* file = fopen("purchase_history.txt", "a");
            if (file != NULL) {
                fprintf(file, "Purchase History:\n");
                for (int i = 0; i < pur_items.size; i++) {
                    fprintf(file, "%d. %s | Value: %.2f | Quantity: %d\n", i + 1,
                            pur_items.items[i].name,
                            pur_items.items[i].value,
                            pur_items.items[i].quantity);
                }
                fprintf(file, "\nTotal items: %d\n", pur_items.size);
                fprintf(file, "Total price: %.2f\n", totalValue);
                fprintf(file,"Date/Time: %s",ctime(&currenttime));
                fprintf(file,"---------------------------------------------------------------\n");
                fclose(file);
                printf("saved to 'purchase_history.txt'.\n");
                writeReceipt(&pur_items);
            } else {
                printf("Could not write to file.\n");
            }
            
            break;
        }
        // search for the item and then ask them how many they want to purchase
        int found = 0;
        for (int i = 0; i < inv->size; i++) {
            if (strcmp(inv->items[i].name, searchName) == 0) {
                printf("Item found: %s | Value: %.2f | Stock: %d\n",inv->items[i].name, inv->items[i].value, inv->items[i].quantity);
                found = 1;

                int quantity;
                printf("Enter quantity to purchase: ");
                scanf("%d", &quantity);

                if (quantity > inv->items[i].quantity) {
                    printf("Requested quantity exceeds available stock.\n");
                } else {
                    char YN[10];
                    printf("Confirm purchase [Y/N]: ");
                    scanf(" %s", YN);

                    if (strcmp(YN, "Y") == 0 || strcmp(YN, "y") == 0) {
                        if (pur_items.size < pur_items.capacity) {
                            Item purchased = inv->items[i];
                            purchased.quantity = quantity;
                            pur_items.items[pur_items.size++] = purchased;
                            inv->items[i].quantity-=quantity;
                            printf("Item added to cart.\n");
                        } else {
                            printf("Cart is full.\n");
                        }
                    } else {
                        printf("Purchase cancelled.\n");
                    }
                }
                break;
            }
        }

        if (!found) {
            printf("Item not found.\n");
        }

        // Show current cart
        if (pur_items.size > 0) {
            printf("\nCurrent Cart:\n");
            for (int i = 0; i < pur_items.size; i++) {
                printf("%d. %s | Value: %.2f | Quantity: %d\n", i + 1,
                       pur_items.items[i].name,
                       pur_items.items[i].value,
                       pur_items.items[i].quantity);
            }
        }
    }

    free(pur_items.items);
}



// print the item in a good format
void printItem(const Item* item) {
    printf("Item: %s | Quantity: %d | Value: %.2f\n", item->name, item->quantity, item->value);
    
}

int main() {
    Inventory inv;
    loadInventoryFromFile(&inv, "inventory.txt");

    // ask for username and password
    int choice;
    char username[100];
    char password[100];
    printf("enter username:");
    scanf("%s",username);
    printf("enter password:");
    scanf("%s",password);
    bool auth = login(username,password);


    if (auth){
    do {
        printf("\n1. Add item\n2. View inventory\n3. Search item\n4. Export to file\n5. Remove item \n6. Update item\n7. Sort inventory\n");
        printf("8. Show most valuable item\n9. Show least stocked item\n10. Show average item value\n11. Purcahse mode\n12. Save\n13. exit\nChoose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addItem(&inv);saveInventoryToFile(&inv, "inventory.txt"); break;
            case 2: viewInventory(&inv); break;
            case 3: searchItem(&inv); break;
            case 4: export(&inv); break;
            case 5: removeItem(&inv); break;
            case 6: updateItem(&inv); break;
            case 7: showSort(&inv); break;
            case 8: {
                Item maxItem = getMostValuableItem(&inv);
                printf("\nMost Valuable Item:\n");
                printItem(&maxItem);
                break;
            }
            case 9: {
                Item minItem = getLeastStockedItem(&inv);
                printf("\nLeast Stocked Item:\n");
                printItem(&minItem);
                break;
            }
            case 10: {
                float avg = getAverageItemValue(&inv);
                printf("\nAverage item value: %.2f\n", avg);
                break;
            }
            case 11 : purchase(&inv); break;
            case 12 : saveInventoryToFile(&inv,"inventory.txt"); printf("saved.\n"); break;
            case 13 : saveInventoryToFile(&inv, "inventory.txt"); freeInventory(&inv); printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }

    } while (choice != 13);
    } else {
        printf("Acess dined.");
        freeInventory(&inv);
    }
    return 0;
}