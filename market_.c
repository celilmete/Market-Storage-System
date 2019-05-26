/*
 * Student Name : Celil Mete
 * Student ID : 150116042
 * Purpose of the program is to read the products and firms from a text file and
 * store them for the market,remove the products that their expiration dates are past
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


struct Firm {//create the firm struct
    int firmId;
    char firmName[100];
    struct Firm *nextFirm;
};
typedef struct Firm firm;//typedef it
typedef firm *firmPtr;//...

struct  Food {//create the food struct
    char prodName[400];
    int expDay;
    int expMonth;
    int expYear;
};
typedef struct Food food;//typedef it

struct  FoodStock {//create the food stock struct
    food food;
    struct FoodStock *nextFood;
};
typedef struct FoodStock foodstock;//typedef it
typedef foodstock *foodstockPtr;//...
//insert firms function
void insertFirms(firmPtr *ptr ,int ID,char name[50]){

    firm *newPtr = malloc(sizeof(firm));//open new space
    if(newPtr != NULL){//if space available
        newPtr -> firmId = ID;//fill the data fields
        strcpy(newPtr -> firmName,name);//...

        firmPtr previousPtr = NULL;
        firmPtr currentPtr = *ptr;

        //loop to find current location in the list
        while (currentPtr != NULL && ID < currentPtr -> firmId) {
            previousPtr = currentPtr;
            currentPtr = currentPtr -> nextFirm;
        }
        //to place first element
        if(previousPtr == NULL){
            newPtr -> nextFirm = *ptr;
            *ptr = newPtr;
        }
        else{
            previousPtr -> nextFirm = newPtr;
            newPtr -> nextFirm = currentPtr;
        }
    }
    else
        printf("No memory available");
}

//read firms function
firmPtr readFirms(char fileName[]){
    firmPtr startPtr = NULL;//create the start of the linked list
    FILE *file= fopen(fileName,"r");
    if(file == NULL)//check if open file failed
        printf("open file failed");

    int id ;
    char name[50];



    while(!feof(file)){//read until nothing left to read
        fscanf(file,"%d ",&id);
        fscanf(file,"%s\n",name);
        insertFirms(&startPtr,id,name);//call the function insert firms with the read datas
    }

    fclose(file);
    return startPtr;
}
//print firms function
void printFirms(firmPtr firmPtr){
    printf("Firms :\n");
    while (firmPtr != NULL) {
        printf("firm no %d \t--> ", firmPtr -> firmId);//print content
        printf("\t%s\n", firmPtr -> firmName);//...
        firmPtr = firmPtr -> nextFirm;//go to next element
    }
}
//insert food function
void insertFoods(foodstockPtr *sptr,char prodName[400],int day,int month , int year) {

    foodstockPtr newPtr = malloc(sizeof(foodstock));//open new space
    strcpy(newPtr -> food.prodName,prodName);//fill the data field
    newPtr ->food.expDay =day;//...
    newPtr ->food.expMonth = month;//...
    newPtr ->food.expYear = year;//...

    foodstockPtr previousPtr = NULL;
    foodstockPtr currentPtr = *sptr;
    //loop to find correct location
    while (currentPtr != NULL){
        previousPtr = currentPtr;
        currentPtr = currentPtr -> nextFood;
    }

    if(previousPtr == NULL){//insert the first element
        newPtr -> nextFood = *sptr;
        *sptr = newPtr;
    }
    else{
        previousPtr -> nextFood = newPtr;
        newPtr -> nextFood = currentPtr;
    }


}
//read foods function
foodstockPtr readFoods(char fileName[50]){
    FILE *file= fopen(fileName,"r");
    foodstockPtr startPtr = NULL;

    time_t t;
    /* Intializes random number generator */
    srand((unsigned) time(&t));




    if(file == NULL)
        printf("open file failed");
    char instructions[400];
    char instructions2[400];
    char instructions3[400];
    int number = 0 ;
    while(!feof(file)){
        fscanf(file,"%s %s %s %d ",instructions,instructions2,instructions3,&number);//read the data from the file
        //concatenate the strings
        strcat(instructions," ");
        strcat(instructions,instructions2);
        strcat(instructions," ");
        strcat(instructions,instructions3);

        int i = 0 ;
        for( ;i < number;i++){//select randomly expire date
            int day = rand() % 14 + 1;
            int month = rand() % 4 + 1;
            int year;
            if(rand() % 2)
                year = 2018;
            else
                year = 2019;
            insertFoods(&startPtr,instructions,day,month,year);//call insert foods function with the read date and random date
        }

    }

    fclose(file);
    return startPtr;
}
//stock out function
foodstockPtr stockOut(foodstockPtr foodPtr,int day , int month, int year){
    foodstockPtr previousptr = NULL;
    foodstockPtr currentptr = foodPtr;
    while(currentptr != NULL){//iterate over the linked list
        if(currentptr -> food.expYear < year){
            if(previousptr == NULL){
                foodstockPtr tempptr = foodPtr;
                foodPtr = foodPtr -> nextFood;
                free(tempptr);
            } else{
                foodstockPtr tempptr = currentptr;
                previousptr -> nextFood = currentptr -> nextFood;
                currentptr = currentptr -> nextFood;
                free(tempptr);
            }

        } else if(currentptr -> food.expYear == year){
            if(currentptr -> food.expMonth < month){
                if(previousptr == NULL){
                    foodstockPtr tempptr = foodPtr;
                    foodPtr = foodPtr -> nextFood;
                    free(tempptr);
                } else{
                    foodstockPtr tempptr = currentptr;
                    previousptr -> nextFood = currentptr -> nextFood;
                    currentptr = currentptr -> nextFood;
                    free(tempptr);
                }

            }else if(currentptr -> food.expMonth == month){
                if(currentptr -> food.expDay <= day){
                    if(previousptr == NULL){
                        foodstockPtr tempptr = foodPtr;
                        foodPtr = foodPtr -> nextFood;
                        free(tempptr);
                    } else{
                        foodstockPtr tempptr = currentptr;
                        previousptr -> nextFood = currentptr -> nextFood;
                        currentptr = currentptr -> nextFood;
                        free(tempptr);
                    }

                }
            }
        }
        previousptr = currentptr;
        currentptr = currentptr -> nextFood;
    }
    return foodPtr;
}
//print food function
void printFood(foodstockPtr stock , char filename[50]) {
    FILE *file = fopen(filename,"w");//open a new file to write
    while(stock != NULL){
        fprintf(file,"%s EXP: --> %d/%d/%d\n",stock->food.prodName,stock->food.expDay,stock->food.expMonth,stock->food.expYear);
        stock = stock -> nextFood;
        //print the content in the file
    }
    fclose(file);
}

int main() {
    firmPtr firms = readFirms("C:\\Users\\celil\\CLionProjects\\untitled3\\firms.txt");
    printFirms(firms);
    printf("\n---------------------------------------\n\n");
    foodstockPtr stocks = readFoods("C:\\Users\\celil\\CLionProjects\\untitled3\\products.txt");
    printFood(stocks,"initial_stock.txt");

    printf("Please enter the day, month and year to be checked\n");
    int day = 0, month = 0 , year = 0;
    scanf("%d %d %d",&day,&month,&year);
    stocks = stockOut(stocks,day,month,year);
    printFood(stocks,"final_stock.txt");


return 0;
}