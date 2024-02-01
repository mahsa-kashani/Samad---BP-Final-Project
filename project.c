#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define approved 0
#define not_approved 1
#define removed 2
#define student 3
#define personel 4
#define not_found -1
#define success 6
#define permission_denied 7
#define invalid 8
#define reserved 9
#define cancled 10
#define served 11

void get_input_ways(void);
void menu_enter_options(void);
void student_menu(void);
void personel_menu(void);

int user_index = 0;
int self_index = 0;
int food_index = 0;
int is_admin = 0;
int is_logged_in = 0;

char curr_user_id[12];
char curr_user_pass[12];
char now_date[12];
char now_time[5];
char** user_array;
int self_array[100];
int food_array[100];
char input_address[50];
char output_address[50];

typedef struct 
{
    char user_password[12];
    char user_id[12];
    int status;
    long money;
    char name[32];
    char family[32];
    char national_id_code[12];
    char gender[6];
    int birthdate_year;
    int birthdate_month;
    int birthdate_day;
    int type;
} user;

typedef struct
{
    char meal_type[10];
    int self_id;
    char name[30];
    char location[50];
    int capacity;
    char type[10];
    char lunch_time_limit[10];
    char dinner_time_limit[10];
} self;

typedef struct
{
    int food_id;
    char name[30];
    char type[10];
    long price;
}food;

typedef struct
{
    int self_id;
    char date[12];
    char meal_type[10];
    int food_id;
    int count;
}meal_plan;

typedef struct
{
    char title[50];
    char content[300];
    char end_date[12];
}news;

typedef struct 
{
    char question[200];
    char option1[30];
    char option2[30];
    char option3[30];
    char option4[30];
    char end_date[12];
}poll;

// typedef struct
// {
//     char user_id[12];
//     int mealplan_index;
//     int status;
// }reserve;

// typedef struct
// {
//     char user_id[12];
//     int mealplan_index;
//     int status;
//     char date[12];
//     char time[5];
// }report;

int logout(char* username)
{
    if(!strcmp(username , curr_user_id))
    {    
        is_admin = 0;
        is_logged_in = 0;
        return success;
    }
    else
    return not_found;
}

void menu_log_out(void)
{
    char logout_user_id[12];
    int ex;
    printf("Please enter your username: ");
    scanf("%s" , &logout_user_id);
    if(logout(logout_user_id) == success)
    {
        printf("\033[0;32mYou've logged out successfully!\033[0m\n");
        get_input_ways();
    }
    else
    {
        printf("\t\t\033[0;31mWRONG USERNAME!\033[0m\nYou can't log out. press 0 to exit: ");
        scanf("%d" , &ex);
        if(is_admin)
        personel_menu();
        else
        student_menu();
    }
}

int is_null(char** tok_line , int cnt)
{
    for (int i = 2; i < cnt; i++)
    {
        if(tok_line[i] == NULL)
        return 1;
    }
    return 0;
}

void file_logout(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 4))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "user") || tok_line[4] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(!is_logged_in)
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        return;
    }
    if(logout(tok_line[3]) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
    else
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        return;
    }
}

int find_user_index(char* user_id)
{
    for (int i = 0; i < 100; i++)
    {
        if(user_array[i] == NULL)
        continue;
        if(!strcmp(user_array[i] , user_id))
        return i;
    }
    return not_found;
}

int find_self_index(int self_id)
{
    for (int i = 0; i < 100; i++)
    {
        if(self_array[i] == self_id)
        return i;
    }
    return not_found;
}

int find_food_index(int food_id)
{
    for (int i = 0; i < 100; i++)
    {
        if(food_array[i] == food_id)
        return i;
    }
    return not_found;
}

int change_user_pass(char* old_pass , char* new_pass , char* username)
{
    if(strcmp(username , curr_user_id))
    return not_found;
    if(strcmp(old_pass , curr_user_pass))
    return permission_denied;
    FILE* fuser = fopen("user.bin" , "rb+");
    fseek(fuser , find_user_index(curr_user_id) * sizeof(user) , SEEK_SET);
    fwrite(new_pass , 12 , 1 , fuser);
    fclose(fuser);
    return success;
}

void file_change_user_pass(char** tok_line)
{
    char old_pass[12];
    char new_pass[12];
    char username[12];
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 8))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "user") || strcmp(tok_line[4] , "old-pass") || strcmp(tok_line[6] , "new-pass") || tok_line[8] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    strcpy(username , tok_line[3]);
    strcpy(old_pass , tok_line[5]);
    strcpy(new_pass , tok_line[7]);
    if(!is_logged_in)
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        return;
    }
    if(change_user_pass(old_pass , new_pass , username) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
    else if(change_user_pass(old_pass , new_pass , username) == not_found)
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        return;
    }
    else
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        return;
    }
}

void exit_options(void)
{
    int ex = -1;
    printf("Press 0 to exit: ");
    scanf("%d" , &ex);
    printf("\n");
    if(is_admin)
    personel_menu();
    else
    student_menu();
}

void menu_change_user_pass()
{
    char username[12];
    char old_pass[12];
    char new_pass[12];
    char new_pass2[12];
    printf("Please enter your user-id: ");
    scanf("%s" , username);
    printf("Please enter your current password: ");
    int i = 0;
    char c = getch();
    while(c != '\r')
    {
        old_pass[i] = c;
        i++;
        c = getch();
    }
    old_pass[i] = '\0';
    printf("\n");
    printf("Please enter your new password: ");
    i = 0;
    c = getch();
    while(c != '\r')
    {
        new_pass[i] = c;
        i++;
        c = getch();
    }
    new_pass[i] = '\0';
    printf("\n");
    printf("Please enter your new password again: ");
    i = 0;
    c = getch();
    while(c != '\r')
    {
        new_pass2[i] = c;
        i++;
        c = getch();
    }
    new_pass2[i] = '\0';
    printf("\n\n");
    if(strcmp(new_pass2 , new_pass))
    {
        printf("\n\t\033[0;31m WRONG PASSWORD!\nplease try again\033[0m\n\n");
        menu_change_user_pass();
        return;
    }
    if(change_user_pass(old_pass , new_pass2 , username) == success)
    {
        printf("\033[0;32mPassword changed successfully!\033[0m\n");
        exit_options();
        return;
    }
    else if(change_user_pass(old_pass , new_pass2 , username) == not_found)
    {
        printf("\t\t\033[0;31mUSER NOT FOUND!\n\033[0m");
        exit_options();
        return;
    }
    else
    {
        printf("\n\t\033[0;31m WRONG PASSWORD!\033[0m\n");
        exit_options();
        return;
    }   
}

void file_change_time(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 6))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "date") || tok_line[3][4] != '-' || tok_line[3][7] != '-' || strcmp(tok_line[4] , "time") || tok_line[5][4] != '\0' || tok_line[6] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    for (int i = 0; i < 4; i++)
    {
        if(!isdigit(tok_line[5][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    for (int i = 0; i < 10; i++)
    {
        if(i == 4 || i == 7)
        continue;
        if(!isdigit(tok_line[3][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    
    
    strcpy(now_date , tok_line[3]);
    strcpy(now_time , tok_line[5]);
    fprintf(f_out , "success\n");
    fclose(f_out);
    return;
}

void menu_change_time()
{
    printf("\nPlease enter new date (in this format: year-month-day): ");
    scanf("%s" , now_date);

    if(now_date[4] != '-' || now_date[7] != '-')
    {
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_change_time();
    }
    for (int i = 0; i < 10; i++)
    {
        if(i == 4 || i == 7)
        continue;
        if(!isdigit(now_date[i]))
        {
            printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
            menu_change_time();
        }
    }
    printf("\nPlease enter new time (in this format: 1130): ");
    scanf("%s" , now_time);
    if(now_time[4] != '\0')
    {
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_change_time();
    }
    for (int i = 0; i < 4; i++)
    {
        if(!isdigit(now_time[i]))
        {
            printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
            menu_change_time();
        }
    }
    printf("\n\t\033[0;32mTime changed successfully!\033[0m\n");
    exit_options();
    return;
}

// int reserve_food(int meal_index)
// {
//     if(is_admin || !is_logged_in)
//     return permission_denied;
//     FILE* fplan = fopen("mealplan.bin" , "rb");
//     meal_plan* meal_temp = (meal_plan*)malloc(sizeof(meal_plan));
//     FILE* ffood = fopen("food.bin" , "rb");
//     food* food_temp = (food*)malloc(sizeof(food));
//     user* curr_user = (user*)malloc(sizeof(user));
//     FILE* fuser = fopen("user.bin" , "rb");
//     fseek(fuser , find_user_index(curr_user_id) * sizeof(user) , SEEK_SET);
//     fread(curr_user , sizeof(user) , 1 , fuser);
//     fclose(fuser);
//     fseek(fplan , meal_index * sizeof(meal_plan) , SEEK_SET);
//     fread(meal_temp , sizeof(meal_plan) , 1 , fplan);
//     fseek(ffood , find_food_index(meal_temp -> food_id) * sizeof(food) , SEEK_SET);
//     fread(food_temp , sizeof(food) , 1 , ffood);
//     if(curr_user -> money < food_temp -> price)
//     {
//         fclose(fplan);
//         free(meal_temp);
//         fclose(ffood);
//         free(food_temp);
//         free(curr_user);
//         return permission_denied;
//     }
//     reserve* new_reserve = (reserve*)malloc(sizeof(reserve));
//     new_reserve -> status = reserved;
//     strcpy(new_reserve -> user_id , curr_user_id);
//     new_reserve -> mealplan_index = meal_index;
//     fclose(fplan);
//     free(meal_temp);
//     fclose(ffood);
//     free(food_temp);
//     free(curr_user);
//     FILE* freserve = fopen("reserve.bin" , "ab");
//     fwrite(new_reserve , sizeof(reserve) , 1 , freserve);
//     fclose(freserve);
//     FILE* freport = fopen("report.bin" , "ab");
//     report* new_report = (report*)malloc(sizeof(report));
//     strcpy(new_report -> user_id , curr_user_id);
//     new_report -> mealplan_index = meal_index;
//     new_report -> status = reserved;
//     strcpy(new_report -> date , now_date);
//     strcpy(new_report -> time , now_time);
//     fwrite(new_report , sizeof(report) , 1 , freport);
//     fclose(freport);
//     return success;
// }

int charge_account(long amount)
{
    if(is_admin || !is_logged_in)
    return permission_denied;
    
    user* temp = (user*)malloc(sizeof(user));
    FILE* fcharge = fopen("user.bin" , "rb+");
    fseek(fcharge , (find_user_index(curr_user_id) * sizeof(user)) , SEEK_SET);
    fread(temp , sizeof(user) , 1 , fcharge);
    temp -> money += amount;
    fseek(fcharge , (find_user_index(curr_user_id) * sizeof(user)) + sizeof(temp -> user_id) + sizeof(temp -> user_password) + sizeof(temp -> status) , SEEK_SET);
    fwrite(&temp -> money , sizeof(temp -> money) , 1 , fcharge);
    fclose(fcharge);
    free(temp);
    return success;
}

// void menu_reserve_food(void)
// {
//     int user_self;
//     FILE* fself = fopen("self.bin" , "rb");
//     self* stemp = (self*)malloc(sizeof(self));
//     while(fread(stemp , sizeof(self) , 1 , fself))
//     {
//         printf("\t\033[0;34mself-id: %d  name: %s  location: %s  self-type: %s  meal-type: %s\n\033[0m" , stemp -> self_id , stemp -> name , stemp -> location 
//         , stemp -> type , stemp -> meal_type);
//     }
//     printf("\n\nPlease enter self-id: ");
//     scanf("%d" , &user_self);
//     if(find_self_index(user_self) == not_found)
//     {
//         printf("\n\t\033[0;31m SELF NOT FOUND!\nplease try again\033[0m\n\n");
//         fclose(fself);
//         free(stemp);
//         menu_reserve_food();
//     }
//     fseek(fself , find_self_index(user_self) * sizeof(self) , SEEK_SET);
//     fread(stemp , sizeof(self) , 1 , fself);
//     fclose(fself);
//     FILE* fuser = fopen("user.bin" , "rb");
//     user* curr_user = (user*)malloc(sizeof(user));
//     fseek(fuser , find_user_index(curr_user_id) * sizeof(user) , SEEK_SET);
//     fread(curr_user , sizeof(user) , 1 , fuser);
//     fclose(fuser);
//     char self_type[10];
//     if(!strcmp(stemp -> type , "boyish"))
//     strcpy(self_type , "male");
//     else
//     strcpy(self_type , "female");
//     if(strcmp(curr_user -> gender , self_type))
//     {
//         printf("\n\t\033[0;31m GENDER DOESN'T MATCH!\nplease try again\033[0m\n\n");
//         fclose(fuser);
//         free(stemp);
//         free(curr_user);
//         menu_reserve_food();
//     }
//     FILE* fplan = fopen("mealplan.bin" , "rb");
//     meal_plan* meal_temp = (meal_plan*)malloc(sizeof(meal_plan));
//     int index = 0 , available_reserve[20] , i = 0;
//     int t_year , t_month , t_day;
//     int year , month , day;
//     sscanf(now_time , "%d-%d-%d" , &year , &month , &day);
//     FILE* ffood = fopen("food.bin" , "rb");
//     food* food_temp = (food*)malloc(sizeof(food));
//     char user_choice[20] , *temp_choices[20]; 
//     while(fread(meal_temp , sizeof(meal_plan) , 1 , fplan))
//     {
//         sscanf(meal_temp -> date , "%d-%d-%d" , &t_year , &t_month , &t_day);
//         if(meal_temp -> self_id == user_self && t_year >= year && t_month >= month && (t_day - day) > 2 && (t_day - day) < 14)
//         {
//             fseek(ffood , find_food_index(meal_temp -> food_id) * sizeof(food) , SEEK_SET);
//             fread(food_temp , sizeof(food) , 1 , ffood);
//             available_reserve[i++] = index;
//             printf("\033[0;34m%d) meal: %s  food-type: %s  food-name: %s  food-price: %d  date: %s\n\033[0m" , i , meal_temp -> meal_type , food_temp -> type
//             , food_temp -> name , food_temp -> price , meal_temp -> date);
//         }
//         index++;
//     }
//     i = 0;
//     printf("Please enter your choices: ");
//     getchar();
//     gets(user_choice);
//     temp_choices[i] = strtok(user_choice , " ");
//     while(temp_choices[i++] != NULL)
//     temp_choices[i] = strtok(NULL , " ");
//     int choices[i];
//     for (int j = 0; j < i; j++)
//     sscanf(temp_choices[j] , "%d" , choices[j]);
//     long total;
//     for (int j = 0; j < i; j++)
//     {
//         fseek(fplan , choices[j] * sizeof(meal_plan) , SEEK_SET);
//         fread(meal_temp , sizeof(meal_plan) , 1 , fplan);
//         fseek(ffood , find_food_index(meal_temp -> food_id) * sizeof(food) , SEEK_SET);
//         fread(food_temp , sizeof(food) , 1 , ffood);
//         total += food_temp -> price;
//     }
//     char choice[5];
//     if(curr_user -> money < total)
//     {
//         printf("\n\t\033[0;31m You don't have enough money!\033[0m\n\nDo you want to charge your account? (yes/no): ");
//         scanf("%s" , choice);
//     }
//     if(!strcmp(choice , "no"))
//     {
//         printf("\033[0;32mOK!\033[0m\n");
//         exit_options();
//         return;
//     }
//     long amount;
//     char card_number[20];
//     int pass;
//     printf("\nPlease enter the amount you want to charge: ");
//     scanf("%d" , &amount);
//     printf("\nPlease enter your card-number (in this format: ****-****-****-****): ");
//     scanf("%s" , card_number);
//     printf("\nPlease enter your pass-code: ");
//     scanf("%d" , &pass);
//     charge_account(amount);
//     printf("\n\t\033[0;32mAccount charged successfully!\033[0m\n");
//     free(stemp);
//     free(curr_user);
//     fclose(fplan);
//     free(meal_temp);
//     fclose(ffood);
//     free(food_temp);
//     for (int j = 0; j < i; j++)
//     reserve_food(choices[j]);
//     printf("\n\t\033[0;32mfood reserved successfully!\033[0m\n");
//     exit_options();
//     return;
// }

int check_news(void)
{
    if(is_admin || !is_logged_in)
    return permission_denied;
    news* temp = (news*)malloc(sizeof(news));
    FILE* fnews = fopen("news.bin" , "rb");
    int news_year , news_month , news_day;
    int year , month , day;
    sscanf(now_date , "%d-%d-%d" , &year , &month , &day);
    while(fread(temp , sizeof(news) , 1 , fnews))
    {
        sscanf(temp -> end_date , "%d-%d-%d" , &news_year , &news_month , &news_day);
        if(news_year > year || (news_year <= year && news_month > month) ||
        (news_year <= year && news_month <= month && news_day >= day))
        {
            free(temp);
            fclose(fnews);
            return success;
        }
    }
    free(temp);
    fclose(fnews);
    return not_found;
}

void file_check_news(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 2))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(tok_line[2] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(check_news() == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
    else if(check_news() == not_found)
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        return;
    }
    else
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        return;
    }
}

void menu_check_news(void)
{
    if(check_news() == not_found)
    {
        printf("\t\t\033[0;31mThere is no news!\033[0m\n");
        exit_options();
    }
    news* temp = (news*)malloc(sizeof(news));
    FILE* fnews = fopen("news.bin" , "rb");
    int news_year , news_month , news_day;
    int year , month , day;
    int index = 0 , choice , available_news[20] , i = 0;
    sscanf(now_date , "%d-%d-%d" , &year , &month , &day);
    while(fread(temp , sizeof(news) , 1 , fnews))
    {
        sscanf(temp -> end_date , "%d-%d-%d" , &news_year , &news_month , &news_day);
        if(news_year > year || (news_year <= year && news_month > month) ||
        (news_year <= year && news_month <= month && news_day >= day))
        {
            available_news[i++] = index;
            printf("\t\033[0;34m%d) %s\033[0m\n" , i , temp -> title);
        }
        index++;
    }
    printf("\nPlease choose a title: ");
    scanf("%d" , &choice);
    fseek(fnews , available_news[choice - 1] * sizeof(news) , SEEK_SET);
    fread(temp , sizeof(news) , 1 , fnews);
    printf("\033[0;36m%s\033[0m\n" , temp -> content);
    fclose(fnews);
    free(temp);
    exit_options();
}

void file_charge_account(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 8))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "amount") || strcmp(tok_line[4] , "card-number") || tok_line[5][4] != '-' || tok_line[5][9] != '-' || tok_line[5][14] != '-' 
    || tok_line[5][19] != '\0' || strcmp(tok_line[6] , "pass-code") || tok_line[8] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    for (int i = 0; i < strlen(tok_line[3]); i++)
    {
        if(!isdigit(tok_line[3][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    for (int i = 0; i < strlen(tok_line[5]); i++)
    {
        if(i == 4 || i == 9 || i == 14)
        continue;
        if(!isdigit(tok_line[5][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    for (int i = 0; i < strlen(tok_line[7]); i++)
    {
        if(!isdigit(tok_line[7][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    long amount;
    sscanf(tok_line[3] , "%d" , &amount);
    if(charge_account(amount) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
    else
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        return;
    }
}

void menu_charge_account(void)
{
    long amount;
    char card_number[20];
    int pass;
    printf("\nPlease enter the amount you want to charge: ");
    scanf("%d" , &amount);
    printf("\nPlease enter your card-number (in this format: ****-****-****-****): ");
    scanf("%s" , card_number);
    if(card_number[4] != '-' || card_number[9] != '-' || card_number[14] != '-' || card_number[19] != '\0')
    {
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_charge_account();
    }
    printf("\nPlease enter your pass-code: ");
    scanf("%d" , &pass);
    charge_account(amount);
    printf("\n\t\033[0;32mAccount charged successfully!\033[0m\n");
    exit_options();
}

int send_charge(long amount , char* username , char* name)
{
    if(is_admin || !is_logged_in)
    return permission_denied;
    long money; 
    if(find_user_index(username) == not_found)
    return not_found;
    user* temp = (user*)malloc(sizeof(user));
    FILE* fcharge = fopen("user.bin" , "rb+");
    fseek(fcharge , (find_user_index(username) * sizeof(user)) , SEEK_SET);
    fread(temp , sizeof(user) , 1 , fcharge);
    if(strcmp(strlwr(temp -> name) , strlwr(name)))
    {    
        free(temp);
        fclose(fcharge);
        return not_found;
    }
    user* curr_user = (user*)malloc(sizeof(user));
    fseek(fcharge , find_user_index(curr_user_id) * sizeof(user) , SEEK_SET);
    fread(curr_user , sizeof(user) , 1 , fcharge);
    if(temp -> type == personel || !strcmp(username , curr_user_id))
    {
        fclose(fcharge);
        free(temp);
        free(curr_user);
        return invalid;
    }
    if(curr_user -> money < amount)
    {
        fclose(fcharge);
        free(temp);
        free(curr_user);
        return permission_denied;
    }
    free(curr_user);
    temp -> money += amount;
    charge_account(-amount);
    fseek(fcharge , (find_user_index(username) * sizeof(user)) + sizeof(temp -> user_id) + sizeof(temp -> user_password) + sizeof(temp -> status) , SEEK_SET);
    fwrite(&temp -> money , sizeof(temp -> money) , 1 , fcharge);
    fclose(fcharge);
    free(temp);
    return success;
}

void file_send_charge(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 8))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "user") || strcmp(tok_line[4] , "amount") || strcmp(tok_line[6] , "name") || tok_line[8] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    for (int i = 0; i < strlen(tok_line[5]); i++)
    {
        if(!isdigit(tok_line[5][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    long amount;
    char username[12];
    char name[12];
    strcpy(username , tok_line[3]);
    sscanf(tok_line[5] , "%d" , &amount);
    strcpy(name , tok_line[7]);
    if(send_charge(amount , username , name) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
    else if(send_charge(amount , username , name) == permission_denied)
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        return;
    }
    else if(send_charge(amount , username , name) == not_found)
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        return;
    }
    else
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
}

void menu_send_charge(void)
{
    char username[12];
    long amount;
    printf("\nPlease enter your friend's user-id: ");
    scanf("%s" , username);
    if(find_user_index(username) == not_found)
    {
        printf("\t\t\033[0;31mUSER NOT FOUND!\n\033[0m");
        exit_options();
        return;
    }
    FILE* fuser = fopen("user.bin" , "rb");
    user* temp = (user*)malloc(sizeof(user));
    fseek(fuser , find_user_index(username) * sizeof(user) , SEEK_SET);
    fread(temp , sizeof(user) , 1 , fuser);
    char user_type[10];
    if(temp -> type == personel)
    strcpy(user_type , "personel");
    else
    strcpy(user_type , "student");
    printf("\n\t\033[0;30muser-id: %s  name: %s  family: %s  gender: %s  type: %s  birthdate: %d-%d-%d  national-id-code : %s\n\n\033[0m"
    ,temp -> user_id , temp -> name , temp -> family , temp -> gender , user_type , temp -> birthdate_year , temp -> birthdate_month ,
    temp -> birthdate_day , temp -> national_id_code);
    char choice[5];
    printf("is this your friend? (yes/no): ");
    scanf("%s" , &choice);
    printf("\n");
    if(!strcmp(choice , "no"))
    {
        fclose(fuser);
        free(temp);
        exit_options();
        return;
    }
    user* curr_user = (user*)malloc(sizeof(user));
    fseek(fuser , find_user_index(curr_user_id) * sizeof(user) , SEEK_SET);
    fread(curr_user , sizeof(user) , 1 , fuser);
    fclose(fuser);
    printf("\n\t\033[0;35mYour account balance is: %d\n\033[0m" , curr_user -> money);
    free(curr_user);
    printf("\nPlease enter the amount you want to charge: ");
    scanf("%d" , &amount);
    if(send_charge(amount , username , temp -> name) == success)
    {
        printf("\n\t\033[0;32mYour friend's account charged successfully!\033[0m\n");
        free(temp);
        exit_options();
        return;
    }
    else if(send_charge(amount , username , temp -> name) == invalid)
    {
        printf("\n\t\033[0;31mINVALID REQUEST!\n\033[0m");
        free(temp);
        exit_options();
        return;
    }
}

int vote(void)
{
    if(is_admin || !is_logged_in)
    return permission_denied;
    poll* temp = (poll*)malloc(sizeof(poll));
    FILE* fpoll = fopen("poll.bin" , "rb");
    int poll_year , poll_month , poll_day;
    int year , month , day;
    sscanf(now_date , "%d-%d-%d" , &year , &month , &day);
    while(fread(temp , sizeof(poll) , 1 , fpoll))
    {
        sscanf(temp -> end_date , "%d-%d-%d" , &poll_year , &poll_month , &poll_day);
        if(poll_year > year || (poll_year <= year && poll_month > month) ||
        (poll_year <= year && poll_month <= month && poll_day >= day))
        {
            free(temp);
            fclose(fpoll);
            return success;
        }
    }
    free(temp);
    fclose(fpoll);
    return not_found;
}

void file_vote(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 2))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(tok_line[2] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(vote() == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
    else if(vote() == not_found)
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        return;
    }
    else
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        return;
    }
}

void menu_vote(void)
{
    if(vote() == not_found)
    {
        printf("\t\t\033[0;31mThere is no poll!\033[0m\n");
        exit_options();
    }
    poll* temp = (poll*)malloc(sizeof(poll));
    FILE* fpoll = fopen("poll.bin" , "rb");
    int poll_year , poll_month , poll_day;
    int year , month , day;
    int index = 0 , choice , available_poll[20] , i = 0;
    sscanf(now_date , "%d-%d-%d" , &year , &month , &day);
    while(fread(temp , sizeof(poll) , 1 , fpoll))
    {
        sscanf(temp -> end_date , "%d-%d-%d" , &poll_year , &poll_month , &poll_day);
        if(poll_year > year || (poll_year <= year && poll_month > month) ||
        (poll_year <= year && poll_month <= month && poll_day >= day))
        {
            available_poll[i++] = index;
            printf("\t\033[0;34m%d) %s\033[0m\n" , i , temp -> question);
        }
        index++;
    }
    printf("\nPlease choose a question: ");
    scanf("%d" , &choice);
    fseek(fpoll , available_poll[choice - 1] * sizeof(poll) , SEEK_SET);
    fread(temp , sizeof(poll) , 1 , fpoll);
    printf("\n\t\033[0;36m1)%s\t\t2)%s\n\t3)%s\t\t4)%s\033[0m\n" , temp -> option1 , temp -> option2 , temp -> option3 , temp -> option4);
    int option;
    printf("please choose one: ");
    scanf("%d" , &option);
    printf("\n\t\033[0;32mYour vote registered successfully!\033[0m\n");
    fclose(fpoll);
    free(temp);
    exit_options();
}

void student_menu(void)
{
    printf("\033[0;36m\t\t1) Logout");
    printf("\t\t\t2) Change password\n");
    printf("\033[0;36m\t\t3) Change Date And Time");
    printf("\t\t4) Rserve Food\n");
    printf("\033[0;36m\t\t5) Check News");
    printf("\t\t\t6) Charge Account\n");
    printf("\033[0;36m\t\t7) Send Charge");
    printf("\t\t\t8) Vote\n");
    printf("\t\t0) Exit\033[0m\n\n");
    printf("Please choose one: ");
    int user_choice;
    scanf("%d" , &user_choice);
    printf("\n");
    if(user_choice == 0)
    {
        printf("\033[0;34mSee you again!\033[0m\n");
        exit(0);
    }
    else if(user_choice == 1)
    menu_log_out();
    else if (user_choice == 2)
    menu_change_user_pass();
    else if(user_choice == 3)
    menu_change_time();
    // else if(user_choice == 4)
    // menu_reserve_food();
    else if(user_choice == 5)
    menu_check_news();
    else if(user_choice == 6)
    menu_charge_account();
    else if(user_choice == 7)
    menu_send_charge();
    else if(user_choice == 8)
    menu_vote();
    else
    {
        printf("\t\t\033[0;31mINVALID CHOICE!\nplease try again\033[0m\n");
        student_menu();
    }
}

int approve(char** approve_list , int cnt)
{
    if(!is_admin)
    return permission_denied;
    user* temp = (user*)malloc(sizeof(user));
    FILE* fuser = fopen("user.bin" , "rb+");
    int new_status = approved;
    for (int i = 0; i < cnt; i++)
    {
        if(find_user_index(approve_list[i]) == not_found)
        {    
            fclose(fuser);
            free(temp);
            return not_found;
        }
    }
    for (int i = 0; i < cnt; i++)
    {
        fseek(fuser , (find_user_index(approve_list[i]) * sizeof(user)) + sizeof(temp -> user_password) + sizeof(temp -> user_id), SEEK_SET);
        fwrite(&new_status , sizeof(int) , 1 , fuser);
    }
    fclose(fuser);
    free(temp);
    return success;
}

void file_approve(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    int i = 2;
    if(is_null(tok_line , 4))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    while(tok_line[i] != NULL)
    {
        if(strcmp(tok_line[i] , "user"))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
        i += 2;
    }
    for(i = 0; tok_line[i + 1] != NULL; i++);
    if(!strcmp(tok_line[i] , "user"))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    char *choices[20];
    for (int j = 0; j < 20; j++)
    choices[j] = (char*)malloc(20);
    i = 3;
    int j = 0;
    while(tok_line[i - 1] != NULL)
    {
        strcpy(choices[j++] , tok_line[i]);
        i += 2;
    }
    choices[j] = NULL;
    if(approve(choices , j) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
    else if(approve(choices , j) == permission_denied)
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        return;
    }
    else
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        return;
    }
}

void menu_approve(void)
{
    user* temp = (user*)malloc(sizeof(user));
    FILE* fuser = fopen("user.bin" , "rb+");
    char user_type[10];
    char *choices[20];
    char user_choice[100];
    for (int i = 0; i < 20; i++)
    choices[i] = (char*)calloc(1 , 20);
    while(fread(temp , sizeof(user) , 1 , fuser))
    {
        if(temp -> status == not_approved)
        {
            if(temp -> type == personel)
            strcpy(user_type , "personel");
            else
            strcpy(user_type , "student");
            printf("\t\033[0;30muser-id: %s  name: %s  family: %s  gender: %s  type: %s  birthdate: %d-%d-%d  user-password: %s  national-id-code : %s\n"
            ,temp -> user_id , temp -> name , temp -> family , temp -> gender , user_type , temp -> birthdate_year , temp -> birthdate_month ,
            temp -> birthdate_day , temp -> user_password , temp -> national_id_code);
        }
    }
    fclose(fuser);
    free(temp);
    int i = 0;
    getchar();
    printf("\033[0m\n\nPlease enter your choices(only user-id): ");
    gets(user_choice);
    choices[i] = strtok(user_choice , " ");
    while(choices[i++] != NULL)
    choices[i] = strtok(NULL , " ");
    if(approve(choices , i - 1) == success)
    {
        printf("\033[0;32mUser approved successfully!\033[0m\n");
        exit_options();
        return;
    }
    else
    {
        printf("\t\t\033[0;31mUSER NOT FOUND!\n\033[0m");
        exit_options();
        return;
    }
    
}

int change_student_pass(char* new_pass , char* username)
{
    if(!is_admin)
    return permission_denied;
    if(find_user_index(username) == not_found)
    return not_found;
    FILE* fuser = fopen("user.bin" , "rb+");
    fseek(fuser , find_user_index(username) * sizeof(user) , SEEK_SET);
    fwrite(new_pass , 12 , 1 , fuser);
    fclose(fuser);
    return success;
}

void file_change_student_pass(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 6))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "user") || strcmp(tok_line[4] , "new-pass") || tok_line[6] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    char username[12];
    char new_pass[12];
    strcpy(username , tok_line[3]);
    strcpy(new_pass , tok_line[5]);
    if(change_student_pass(new_pass , username) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
    else if(change_student_pass(new_pass , username) == permission_denied)
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        return;
    }
    else if(change_student_pass(new_pass , username) == not_found)
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        return;
    }
}

void menu_change_student_pass(void)
{
    char username[12];
    char new_pass[12];
    char new_pass2[12];
    printf("\nPlease enter student's user-id: ");
    scanf("%s" , username);
    printf("\nPlease enter a new password: ");
    int i = 0;
    char c = getch();
    while(c != '\r')
    {
        new_pass[i] = c;
        i++;
        c = getch();
    }
    new_pass[i] = '\0';
    printf("\n");
    printf("Please enter new password again: ");
    i = 0;
    c = getch();
    while(c != '\r')
    {
        new_pass2[i] = c;
        i++;
        c = getch();
    }
    new_pass2[i] = '\0';
    printf("\n\n");
    if(strcmp(new_pass2 , new_pass))
    {
        printf("\n\t\033[0;31m WRONG PASSWORD!\nplease try again\033[0m\n\n");
        menu_change_student_pass();
        return;
    }
    if(change_student_pass(new_pass2 , username) == success)
    {
        printf("\033[0;32mPassword changed successfully!\033[0m\n");
        exit_options();
        return;
    } 
    else
    {
        printf("\t\t\033[0;31mUSER NOT FOUND!\n\033[0m");
        exit_options();
        return;
    }
}

int remove_student(char* username)
{
    if(!is_admin)
    return permission_denied;
    if(find_user_index(username) == not_found)
    return not_found;
    FILE* fuser = fopen("user.bin" , "rb+");
    user* temp = (user*)malloc(sizeof(user));
    fseek(fuser , find_user_index(username) * sizeof(user) , SEEK_SET);
    fread(temp , sizeof(user) , 1 , fuser);
    if(temp -> type == personel)
    {
        fclose(fuser);
        free(temp);
        return permission_denied;
    }
    int new_status = removed;
    fseek(fuser , (find_user_index(username) * sizeof(user)) + sizeof(temp -> user_password) + sizeof(temp -> user_id) , SEEK_SET);
    fwrite(&new_status , sizeof(int) , 1 , fuser);
    fclose(fuser);
    user_array[find_user_index(username)] = '\0';
    free(temp);
    return success;
}

void file_remove_student(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 4))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "user") ||tok_line[4] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    char username[12];
    strcpy(username , tok_line[3]);
    if(remove_student(username) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
    else if(remove_student(username) == not_found)
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        return;
    }
    else
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        return;
    }
}

void menu_remove_student(void)
{
    char option[5];
    char username[12];
    printf("Please enter student's user-id: ");
    scanf("%s" , username);
    printf("are you sure you want to remove this student? (yes/no): ");
    scanf("%s" , option);
    if(!strcmp(option , "no"))
    {
        printf("\033[0;32mOK!\033[0m\n");
        exit_options();
        return;
    }
    if(remove_student(username) == success)
    {
        printf("\033[0;32mStudent removed successfully!\033[0m\n");
        exit_options();
        return;
    }
    else if(remove_student(username) == not_found)
    {
        printf("\t\t\033[0;31mUSER NOT FOUND!\n\033[0m");
        exit_options();
        return;
    }
    else
    {
        printf("\n\t\033[0;31mYOU CAN'T REMOVE A PERSONEL!\n\033[0m");
        exit_options();
        return;
    } 
}

int deactivate_student(char* username)
{
    if(!is_admin)
    return permission_denied;
    if(find_user_index(username) == not_found)
    return not_found;
    FILE* fuser = fopen("user.bin" , "rb+");
    user* temp = (user*)malloc(sizeof(user));
    fseek(fuser , find_user_index(username) * sizeof(user) , SEEK_SET);
    fread(temp , sizeof(user) , 1 , fuser);
    if(temp -> type == personel)
    {
        fclose(fuser);
        free(temp);
        return permission_denied;
    }
    int new_status = not_approved;
    fseek(fuser , (find_user_index(username) * sizeof(user)) + sizeof(temp -> user_password) + sizeof(temp -> user_id) , SEEK_SET);
    fwrite(&new_status , sizeof(int) , 1 , fuser);
    fclose(fuser);
    free(temp);
    return success;
}

void file_deactivate_student(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 4))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "user") || tok_line[4] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    char username[12];
    strcpy(username , tok_line[3]);
    if(deactivate_student(username) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
    else if(deactivate_student(username) == not_found)
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        return;
    }
    else
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        return;
    }
}

void menu_deactivate_student(void)
{
    char username[12];
    printf("Please enter student's user-id: ");
    scanf("%s" , username);
    if(deactivate_student(username) == success)
    {
        printf("\033[0;32mStudent deactivated successfully!\033[0m\n");
        exit_options();
        return;
    }
    else if(deactivate_student(username) == not_found)
    {
        printf("\t\t\033[0;31mUSER NOT FOUND!\n\033[0m");
        exit_options();
        return;
    }
    else
    {
        printf("\n\t\033[0;31mYOU CAN'T DEACTIVATE A PERSONEL!\n\033[0m");
        exit_options();
        return;
    }
}

int define_self(self* new_self)
{
    if(!is_admin)
    return permission_denied;
    FILE* fself = fopen("self.bin" , "ab");
    fwrite(new_self , sizeof(self) , 1 , fself);
    fclose(fself);
    self_array[self_index++] = new_self -> self_id;
    return success;
}

void file_define_self(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 18))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "id") || strcmp(tok_line[4] , "name") || strcmp(tok_line[6] , "location") || strcmp(tok_line[8] , "capacity")
    || strcmp(tok_line[10] , "type") || (strcmp(tok_line[11] , "boyish") && strcmp(tok_line[11] , "girlish")) || strcmp(tok_line[12] , "meal")
    || (strcmp(tok_line[13] , "lunch") && strcmp(tok_line[13] , "dinner") && strcmp(tok_line[13] , "both")) || strcmp(tok_line[14] , "lunch-time-limit")
    || tok_line[15][4] != '-' || tok_line[15][9] != '\0' || strcmp(tok_line[16] , "dinner-time-limit") || tok_line[17][4] != '-' || tok_line[17][9] != '\0'
    || tok_line[18] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    for (int i = 0; i < strlen(tok_line[3]); i++)
    {
        if(!isdigit(tok_line[3][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    for (int i = 0; i < strlen(tok_line[9]); i++)
    {
        if(!isdigit(tok_line[9][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    for (int i = 0; i < strlen(tok_line[15]); i++)
    {
        if(i == 4)
        continue;
        if(!isdigit(tok_line[15][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    for (int i = 0; i < strlen(tok_line[17]); i++)
    {
        if(i == 4)
        continue;
        if(!isdigit(tok_line[17][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    self* temp = (self*)malloc(sizeof(self));
    sscanf(tok_line[3] , "%d" , &temp -> self_id);
    strcpy(temp -> name , tok_line[5]);
    strcpy(temp -> location , tok_line[7]);
    sscanf(tok_line[9] , "%d" , &temp -> capacity);
    strcpy(temp -> type, tok_line[11]);
    strcpy(temp -> meal_type , tok_line[13]);
    strcpy(temp -> lunch_time_limit , tok_line[15]);
    strcpy(temp -> dinner_time_limit , tok_line[17]);

    if(define_self(temp) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        free(temp);
        return;
    }
    else
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        free(temp);
        return;
    }
}

void menu_define_self(void)
{
    self* new_self = (self*)malloc(sizeof(self));
    char choice[5];
    printf("\nPlease enter self-id: ");
    scanf("%d" , &new_self -> self_id);
    printf("\nPlease enter self-name: ");
    scanf("%s" , &new_self -> name);
    printf("\nPlease enter self-location: ");
    scanf("%s" , &new_self -> location);
    printf("\nPlease enter self-capacity: ");
    scanf("%d" , &new_self -> capacity);
    printf("\nPlease enter self-type (boyish/girlish): ");
    scanf("%s" , &new_self -> type);
    if(strcmp(new_self -> type , "boyish") && strcmp(new_self -> type , "girlish"))
    {
        free(new_self);
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_define_self();
    }
    printf("\nPlease enter self-meal-type (lunch/dinner/both): ");
    scanf("%s" , &new_self -> meal_type);
    if(strcmp(new_self -> meal_type , "lunch") && strcmp(new_self -> meal_type , "dinner") && strcmp(new_self -> meal_type , "both"))
    {
        free(new_self);
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_define_self();
    }
    if(!strcmp(new_self -> meal_type , "lunch"))
    {
        printf("\nPlease enter self-lunch-limit (in this format: 1730-1800): ");
        scanf("%s" , &new_self -> lunch_time_limit);
        if(new_self -> lunch_time_limit[4] != '-' || new_self -> lunch_time_limit[9] != '\0')
        {
            free(new_self);
            printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
            menu_define_self();
        }
        strcpy(new_self -> dinner_time_limit , "0000-0000");
    }
    else if(!strcmp(new_self -> meal_type , "dinner"))
    {
        printf("\nPlease enter self-dinner-limit (in this format: 1130-1300): ");
        scanf("%s" , &new_self -> dinner_time_limit);
        if(new_self -> dinner_time_limit[4] != '-' || new_self -> dinner_time_limit[9] != '\0')
        {
            free(new_self);
            printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
            menu_define_self();
        }
        strcpy(new_self -> lunch_time_limit , "0000-0000");
    }
    else
    {
        printf("\nPlease enter self-lunch-limit (in this format: 1730-1800): ");
        scanf("%s" , &new_self -> lunch_time_limit);
        if(new_self -> lunch_time_limit[4] != '-' || new_self -> lunch_time_limit[9] != '\0')
        {
            free(new_self);
            printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
            menu_define_self();
        }
        printf("\nPlease enter self-dinner-limit (in this format: 1130-1300): ");
        scanf("%s" , &new_self -> dinner_time_limit);
        if(new_self -> dinner_time_limit[4] != '-' || new_self -> dinner_time_limit[9] != '\0')
        {
            free(new_self);
            printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
            menu_define_self();
        }
    }
    define_self(new_self);
    free(new_self);
    printf("\n\t\033[0;32mSelf defined successfully!\033[0m\n");
    printf("Do you want to add a new self? (yes/no): ");
    scanf("%s" , &choice);
    if(!strcmp(choice , "yes"))
    menu_define_self();
    else
    {
        printf("\033[0;32mOK!\033[0m\n");
        exit_options();
        return;
    }
}

int define_food(food* new_food)
{
    if(!is_admin)
    return permission_denied;
    FILE* ffood = fopen("food.bin" , "ab");
    fwrite(new_food , sizeof(food) , 1 , ffood);
    fclose(ffood);
    food_array[food_index++] = new_food -> food_id;
    return success;
}

void file_define_food(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 10))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "id") || strcmp(tok_line[4] , "name") || strcmp(tok_line[6] , "type") || (strcmp(tok_line[7] , "food") && strcmp(tok_line[7] , "dessert"))
    || strcmp(tok_line[8] , "price") || tok_line[10] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    for (int i = 0; i < strlen(tok_line[3]); i++)
    {
        if(!isdigit(tok_line[3][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    for (int i = 0; i < strlen(tok_line[9]); i++)
    {
        if(!isdigit(tok_line[9][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    food* temp = (food*)malloc(sizeof(food));
    sscanf(tok_line[3] , "%d" , &temp -> food_id);
    strcpy(temp -> name , tok_line[5]);
    strcpy(temp -> type , tok_line[7]);
    sscanf(tok_line[9] , "%d" , &temp -> price);
    if(define_food(temp) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        free(temp);
        return;
    }
    else
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        return;
    }
}

void menu_define_food(void)
{
    char choice[5];
    food* new_food = (food*)malloc(sizeof(food));
    printf("\nPlease enter food-id: ");
    scanf("%d" , &new_food -> food_id);
    printf("\nPlease enter food-name: ");
    scanf("%s" , &new_food -> name);
    printf("\nPlease enter food-type (food/dessert): ");
    scanf("%s" , &new_food -> type);
    if(strcmp(new_food -> type , "food") && strcmp(new_food -> type , "dessert"))
    {
        free(new_food);
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_define_food();
    }
    printf("\nPlease enter food-price: ");
    scanf("%d" , &new_food -> price);
    define_food(new_food);
    free(new_food);
    printf("\n\t\033[0;32mfood defined successfully!\033[0m\n");
    printf("Do you want to add a new food? (yes/no): ");
    scanf("%s" , &choice);
    if(!strcmp(choice , "yes"))
    menu_define_food();
    else
    {
        printf("\033[0;32mOK!\033[0m\n");
        exit_options();
        return;
    }
}

int define_meal_plan(meal_plan* new_mwal_plan)
{
    if(!is_admin)
    return permission_denied;
    if(find_self_index(new_mwal_plan -> self_id) == not_found)
    return not_found;
    if(find_food_index(new_mwal_plan -> food_id) == not_found)
    return not_found;
    FILE* fself = fopen("self.bin" , "rb");
    self* stemp = (self*)malloc(sizeof(self));
    fseek(fself , find_self_index(new_mwal_plan -> self_id) * sizeof(self) , SEEK_SET);
    fread(stemp , sizeof(self) , 1 , fself);
    if(strcmp(stemp -> meal_type , new_mwal_plan -> meal_type) && strcmp(stemp -> meal_type , "both"))
    return not_found;
    fclose(fself);
    free(stemp);

    FILE* fmeal_plan = fopen("mealplan.bin" , "ab");
    fwrite(new_mwal_plan , sizeof(meal_plan) , 1 , fmeal_plan);
    fclose(fmeal_plan);
    return success;
}

void file_define_meal_plan(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 12))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "self-id") || strcmp(tok_line[4] , "date") || tok_line[5][4] != '-' || tok_line[5][7] != '-' || tok_line[5][10] != '\0'
    || strcmp(tok_line[6] , "type") || (strcmp(tok_line[7] , "lunch") && strcmp(tok_line[7] , "dinner")) || strcmp(tok_line[8] , "food-id") 
    || strcmp(tok_line[10] , "count")  || tok_line[12] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    for (int i = 0; i < strlen(tok_line[3]); i++)
    {
        if(!isdigit(tok_line[3][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    for (int i = 0; i < strlen(tok_line[5]); i++)
    {
        if(i == 4 || i == 7)
        continue;
        if(!isdigit(tok_line[5][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    for (int i = 0; i < strlen(tok_line[9]); i++)
    {
        if(!isdigit(tok_line[9][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    for (int i = 0; i < strlen(tok_line[11]); i++)
    {
        if(!isdigit(tok_line[11][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    meal_plan* temp = (meal_plan*)malloc(sizeof(meal_plan));
    char self_id[15];
    char food_id[15];
    char count[10];
    strcpy(self_id , tok_line[3]);
    sscanf(self_id , "%d" , &temp -> self_id);
    strcpy(temp -> date , tok_line[5]);
    strcpy(temp -> meal_type , tok_line[7]);
    strcpy(food_id , tok_line[9]);
    sscanf(food_id , "%d" , &temp -> food_id);
    strcpy(count , tok_line[11]);
    sscanf(count , "%d" , &temp -> count);
    
    if(define_meal_plan(temp) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        free(temp);
        return;
    }
    else if(define_meal_plan(temp) == permission_denied)
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        free(temp);
        return;
    }
    else
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        free(temp);
        return;
    }
}

void menu_define_meal_plan(void)
{
    char self_type[10];
    char self_date[12];
    meal_plan* new_meal_plan = (meal_plan*)malloc(sizeof(meal_plan));
    self* stemp = (self*)malloc(sizeof(self));
    food* ftemp = (food*)malloc(sizeof(food));
    FILE* fself = fopen("self.bin" , "rb");
    while(fread(stemp , sizeof(self) , 1 , fself))
    printf("\t\033[0;34mself-id: %d  name: %s  location: %s  self-type: %s  meal-type: %s\n\033[0m" , stemp -> self_id , stemp -> name , stemp -> location 
    , stemp -> type , stemp -> meal_type);
    printf("\n\nPlease enter self-id: ");
    scanf("%d" , &new_meal_plan -> self_id);
    free(stemp);
    fclose(fself);
    printf("\nplease enter the date (in this format: year-month-day): ");
    scanf("%s" , self_date);
    if(self_date[4] != '-' || self_date[7] != '-' || self_date[10] != '\0')
    {
        free(new_meal_plan);
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_define_meal_plan();
    }
    strcpy(new_meal_plan -> date , self_date);
    printf("\nPlease enter the type of meal (lunch/dinner): ");
    scanf("%s" , &new_meal_plan -> meal_type);
    printf("\n");
    if(strcmp(new_meal_plan -> meal_type , "lunch") && strcmp(new_meal_plan -> meal_type , "dinner"))
    {
        free(new_meal_plan);
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_define_meal_plan();
    }
    FILE* ffood = fopen("food.bin" , "rb");
    while(fread(ftemp , sizeof(food) , 1 , ffood))
    printf("\t\033[0;34mfood-id: %d  name: %s  type: %s  price: %d\n\033[0m" , ftemp -> food_id , ftemp -> name , ftemp -> type , ftemp -> price);
    printf("\n\nPlease enter food-id: ");
    scanf("%d" , &new_meal_plan -> food_id);
    free(ftemp);
    fclose(ffood);
    printf("\nPlease enter the count of food you want the self to serve: ");
    scanf("%d" , &new_meal_plan -> count);
    char choice[5];
    if(define_meal_plan(new_meal_plan) == success)
    {
        free(new_meal_plan);
        printf("\n\t\033[0;32mself meal-plan defined successfully!\033[0m\n");
        printf("Do you want to add a new meal-plan? (yes/no): ");
        scanf("%s" , &choice);
        if(!strcmp(choice , "yes"))
        menu_define_meal_plan();
        else
        {
            printf("\033[0;32mOK!\033[0m\n");
            exit_options();
            return;
        }
    }
    else
    {
        printf("\n\t\033[0;31mThere is no self/food with such specifications!\n\033[0mDo you want to try again? (yes/no): ");
        scanf("%s" , &choice);
        printf("\n");
        if(!strcmp(choice , "yes"))
        menu_define_meal_plan();
        else
        {
            printf("\033[0;32mOK!\033[0m\n");
            exit_options();
            return;
        }
    }
}

int charge_student_account(int amount , char* username)
{
    if(!is_admin)
    return permission_denied;
    long money; 
    if(find_user_index(username) == not_found)
    return not_found;
    user* temp = (user*)malloc(sizeof(user));
    FILE* fcharge = fopen("user.bin" , "rb+");
    fseek(fcharge , (find_user_index(username) * sizeof(user)) , SEEK_SET);
    fread(temp , sizeof(user) , 1 , fcharge);
    if(temp -> type == personel)
    {
        fclose(fcharge);
        free(temp);
        return invalid;
    }
    temp -> money += amount;
    fseek(fcharge , (find_user_index(username) * sizeof(user)) + sizeof(temp -> user_id) + sizeof(temp -> user_password) + sizeof(temp -> status) , SEEK_SET);
    fwrite(&temp -> money , sizeof(temp -> money) , 1 , fcharge);
    fclose(fcharge);
    free(temp);
    return success;
}

void file_charge_student_account(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 6))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "user") || strcmp(tok_line[4] , "amount") || tok_line[6] != NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    for (int i = 0; i < strlen(tok_line[5]); i++)
    {
        if(!isdigit(tok_line[5][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    char username[12];
    long amount;
    strcpy(username , tok_line[3]);
    sscanf(tok_line[5] , "%d" , &amount);
    if(charge_student_account(amount , username) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
    else if(charge_student_account(amount , username) == permission_denied)
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        return;
    }
    else if(charge_student_account(amount , username) == invalid)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    else
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        return;
    }
}

void menu_charge_student_account(void)
{
    int amount;
    char username[12];
    printf("\nPlease enter student's user-id: ");
    scanf("%s" , username);
    printf("\nPlease enter the amount you want to charge: ");
    scanf("%d" , &amount);
    if(charge_student_account(amount , username) == success)
    {
        printf("\n\t\033[0;32mStudent's account charged successfully!\033[0m\n");
        exit_options();
        return;
    }
    else if(charge_student_account(amount , username) == invalid)
    {
        printf("\n\t\033[0;31mINVALID REQUEST!\nUser is admin!\n\033[0m");
        exit_options();
        return;
    }
    else
    {
        printf("\n\t\033[0;31mUSER NOT FOUND!\n\033[0m");
        exit_options();
        return;
    }
}

int add_news(news* temp)
{
    if(!is_admin)
    return permission_denied;
    FILE* fnews = fopen("news.bin" , "ab");
    fwrite(temp , sizeof(news) , 1 , fnews);
    fclose(fnews);
    return success;
}

void file_add_news(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 8))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "title") || strcmp(tok_line[4] , "content") || strcmp(tok_line[6] , "end-date") || tok_line[7][4] != '-' || tok_line[7][7] != '-' 
    || tok_line[7][10] != '\0' || tok_line[8] !=  NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    for (int i = 0; i < strlen(tok_line[7]); i++)
    {
        if(i == 4 || i == 7)
        continue;
        if(!isdigit(tok_line[7][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    news* temp = (news*)malloc(sizeof(news));
    strcpy(temp -> title , tok_line[3]);
    strcpy(temp -> content , tok_line[5]);
    strcpy(temp -> end_date , tok_line[7]);
    if(add_news(temp) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        free(temp);
        return;
    }
    else
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        free(temp);
        return;
    }
}

void menu_add_news(void)
{
    news* temp = (news*)malloc(sizeof(news));
    printf("\nPlease enter the title of news: ");
    getchar();
    gets(temp -> title);
    printf("\nPlease enter the content of news: ");
    gets(temp -> content);
    printf("\nPlease enter the end-date of showing news (in this format: year-month-date): ");
    scanf("%s" , &temp -> end_date);
    if(temp -> end_date[4] != '-' || temp -> end_date[7] != '-' || temp -> end_date[10] != '\0')
    {
        free(temp);
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_add_news();
    }
    add_news(temp);
    free(temp);
    char choice[5];
    printf("\n\t\033[0;32mNews added successfully!\033[0m\n");
    printf("Do you want to add a new news? (yes/no): ");
    scanf("%s" , &choice);
    if(!strcmp(choice , "yes"))
    menu_add_news();
    else
    {
        printf("\033[0;32mOK!\033[0m\n");
        exit_options();
        return;
    }
}

int add_poll(poll* temp)
{
    if(!is_admin)
    return permission_denied;
    FILE* fpoll = fopen("poll.bin" , "ab");
    fwrite(temp , sizeof(poll) , 1 , fpoll);
    fclose(fpoll);
    return success;
}

void file_add_poll(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 14))
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "question") || strcmp(tok_line[4] , "option") || strcmp(tok_line[6] , "option") || strcmp(tok_line[8] , "option")
    || strcmp(tok_line[10] , "option") || strcmp(tok_line[10] , "option") || strcmp(tok_line[12] , "end-date") || tok_line[13][4] != '-' || tok_line[13][7] != '-' 
    || tok_line[13][10] != '\0' || tok_line[14] !=  NULL)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    for (int i = 0; i < strlen(tok_line[13]); i++)
    {
        if(i == 4 || i == 7)
        continue;
        if(!isdigit(tok_line[13][i]))
        {
            fprintf(f_out , "invalid\n");
            fclose(f_out);
            return;
        }
    }
    poll* temp = (poll*)malloc(sizeof(poll));
    strcpy(temp -> question , tok_line[3]);
    strcpy(temp -> option1 , tok_line[5]);
    strcpy(temp -> option2 , tok_line[7]);
    strcpy(temp -> option3 , tok_line[9]);
    strcpy(temp -> option4 , tok_line[11]);
    strcpy(temp -> end_date , tok_line[13]);
    if(add_poll(temp) == success)
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        free(temp);
        return;
    }
    else
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        free(temp);
        return;
    }
}

void menu_add_poll(void)
{
    poll* temp = (poll*)malloc(sizeof(poll));
    printf("\nPlease enter the question: ");
    getchar();
    gets(temp -> question);
    printf("\nPlease enter option number1: ");
    gets(temp -> option1);
    printf("\nPlease enter option number2: ");
    gets(temp -> option2);
    printf("\nPlease enter option number3: ");
    gets(temp -> option3);
    printf("\nPlease enter option number4: ");
    gets(temp -> option4);
    printf("\nPlease enter the end-date of showing news (in this format: year-month-date): ");
    scanf("%s" , &temp -> end_date);
    if(temp -> end_date[4] != '-' || temp -> end_date[7] != '-' || temp -> end_date[10] != '\0')
    {
        free(temp);
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_add_news();
    }
    add_poll(temp);
    free(temp);
    char choice[5];
    printf("\n\t\033[0;32mpoll added successfully!\033[0m\n");
    printf("Do you want to add a new poll? (yes/no): ");
    scanf("%s" , &choice);
    if(!strcmp(choice , "yes"))
    menu_add_poll();
    else
    {
        printf("\033[0;32mOK!\033[0m\n");
        exit_options();
        return;
    }
}

void personel_menu(void)
{
    printf("\033[0;36m\t\t1) Logout");
    printf("\t\t\t\t2) Change Password\n");
    printf("\t\t3) Approve User's Register");
    printf("\t\t4) Change Student's Password\n");
    printf("\t\t5) Remove Student");
    printf("\t\t\t6) Deactivate Student\n");
    printf("\t\t7) Define Self");
    printf("\t\t\t\t8) Define Food\n");
    printf("\t\t9) Define Meal Plan");
    printf("\t\t\t10) Charge Student Account\n");
    printf("\t\t11) Change Date And Time");
    printf("\t\t12) Add News\n");
    printf("\t\t13) Add Poll");
    printf("\t\t\t\t0) Exit\033[0m\n\n");
    printf("Please choose one: ");
    int user_choice;
    scanf("%d" , &user_choice);
    printf("\n");
    if(user_choice == 0)
    {
        printf("\033[0;34mSee you again!\033[0m\n");
        exit(0);
    }
    else if(user_choice == 1)
    menu_log_out();
    else if (user_choice == 2)
    menu_change_user_pass();
    else if(user_choice == 3)
    menu_approve();
    else if(user_choice == 4)
    menu_change_student_pass();
    else if(user_choice == 5)
    menu_remove_student();
    else if(user_choice == 6)
    menu_deactivate_student();
    else if(user_choice == 7)
    menu_define_self();
    else if(user_choice == 8)
    menu_define_food();
    else if(user_choice == 9)
    menu_define_meal_plan();
    else if(user_choice == 10)
    menu_charge_student_account();
    else if(user_choice == 11)
    menu_change_time();
    else if(user_choice == 12)
    menu_add_news();
    else if(user_choice == 13)
    menu_add_poll();
    else
    {
        printf("\t\t\033[0;31mINVALID CHOICE!\nplease try again\033[0m\n");
        personel_menu();
    }
}

int login(void)
{
    FILE* fuser = fopen("user.bin" , "rb");
    user* temp = (user*)malloc(sizeof(user));
    int ex = -10;
    while(fread(temp , sizeof(user) , 1 , fuser))
    {
        if(!strcmp(temp -> user_id , curr_user_id) && !strcmp(temp -> user_password , curr_user_pass))
        {
            if(temp -> status == not_approved)
            return not_found;
            else if(temp -> status == removed)
            return not_found;
            is_admin = 0;
            if(temp -> type == personel)
            is_admin = 1;
            is_logged_in = 1;
            return success;
        }
    }
    fclose(fuser);
    free(temp);
    return not_found;
}

void menu_login(void)
{
    printf("Please enter your user-id or press 0 to exit: ");
    scanf("%s" , &curr_user_id);
    printf("\n");
    if(!strcmp(curr_user_id , "0"))
    {    
        menu_enter_options();
        return;
    }
    printf("Please enter your password: ");
    int i = 0;
    char c = getch();
    while(c != '\r')
    {
        curr_user_pass[i] = c;
        i++;
        c = getch();
    }
    printf("\n\n");
    curr_user_pass[i] = '\0';
    if(login() == not_found)
    {
        printf("\t\033[0;31mNo user found!\n\033[0m");
        printf("press 0 to exit: ");
        curr_user_id[0] = '\0';
        curr_user_pass[0] = '\0';
        menu_enter_options();
        return;
    }
    else
    {
        printf("\t\t\t\t\033[0;32mWELCOME!\n\n\033[0m");
        if(is_admin)
        personel_menu();
        else
        student_menu();
    }
}

void file_login(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 6))
    {    
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "user") || strcmp(tok_line[4] , "password") ||tok_line[6] != NULL)
    {    
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    strcpy(curr_user_id , tok_line[3]);
    strcpy(curr_user_pass , tok_line[5]);
    if(login() == not_found)
    {
        fprintf(f_out , "not-found\n");
        fclose(f_out);
        curr_user_id[0] = '\0';
        curr_user_pass[0] = '\0';
        return;
    }
    else
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
}

int sign_in(user* new_user)
{
    FILE* fuser = fopen("user.bin" , "ab+");
    user* temp = (user*)malloc(sizeof(user)); 
    while(fread(temp , sizeof(user) , 1 , fuser))
    {
        if(!strcmp(temp -> user_id , new_user -> user_id))
        {    
            fclose(fuser);
            free(temp);
            return permission_denied;
        }
    }
    for (int i = 0; i < strlen(new_user -> user_id); i++)
    {
        if(new_user -> type == student && !isdigit(new_user -> user_id[i]))
        {    
            fclose(fuser);
            free(temp);
            return invalid;
        }
    }
    for (int i = 0; i < strlen(new_user -> national_id_code); i++)
    {
        if(!isdigit(new_user -> national_id_code[i]))
        {    
            fclose(fuser);
            free(temp);
            return invalid;
        }
    }
    new_user -> status = not_approved;
    if(is_logged_in)
    new_user -> status = approved;
    new_user -> money = 0;
    fwrite(new_user , sizeof(user) , 1 , fuser);
    strcpy(user_array[++user_index] , new_user -> user_id);
    fclose(fuser);
    free(temp);
    free(new_user);
    return success;
}

void menu_sign_in(void)
{
    user* temp = (user*)malloc(sizeof(user));
    int ex = -1;
    char birthdate[15];
    char user_type[15];
    printf("\nPlease enter your name: ");
    scanf("%s" , &temp -> name);
    printf("\nPlease enter your family: ");
    scanf("%s" , &temp -> family);
    printf("\nPlease enter your gender (male or female): ");
    scanf("%s" , &temp -> gender);
    if(strcmp(temp -> gender , "male") && strcmp(temp -> gender , "female"))
    {
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_sign_in();
    }
    printf("\nPlease enter your type (student/personel): ");
    scanf("%s" , user_type);
    if(!strcmp(user_type , "student"))
    temp -> type = student;
    else if(!strcmp(user_type , "personel"))
    temp -> type = personel;
    else
    {
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_sign_in();
    }
    printf("\nPlease enter your birthdate (in this format: year-month-day): ");
    scanf("%s" , birthdate);
    if(birthdate[4] != '-' || birthdate[7] != '-' || birthdate[10] != '\0')
    {
        printf("\n\t\033[0;31mINVALID CHOICE!\nPlease try again!\n\033[0m");
        menu_sign_in();
    }
    sscanf(birthdate , "%d-%d-%d" , &temp -> birthdate_year , &temp -> birthdate_month , &temp -> birthdate_day);
    printf("\nPlease enter your national-id-code: ");
    scanf("%s" , &temp -> national_id_code);
    printf("\nPlease enter your user-id: ");
    scanf("%s" , &temp -> user_id);
    printf("\nPlease enter your password: ");
    int i = 0;
    char c = getch();
    while(c != '\r')
    {
        temp -> user_password[i] = c;
        i++;
        c = getch();
    }
    temp -> user_password[i] = '\0';
    if(sign_in(temp) == permission_denied)
    {
        printf("\n\t\033[0;31mUser already exists!\npress 0 to exit:\033[0m ");
        scanf("%d" , &ex);
        menu_enter_options();
    }
    else if(sign_in(temp) == invalid)
    {
        printf("\n\t\033[0;31mINVALID!\nUser-id or national-id-code must be integers\npress 0 to exit:\033[0m ");
        scanf("%d" , &ex);
        menu_enter_options();
    }
    else
    {
        printf("\n\t\033[0;32mYou've signed in successfully!\npress 0 to exit:\033[0m ");
        scanf("%d" , &ex);
        menu_enter_options();
    }
}

void file_sign_in(char** tok_line)
{
    FILE* f_out = fopen(output_address , "a");
    fprintf(f_out , "%s#" , tok_line[0]);
    if(is_null(tok_line , 18))
    {    
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    if(strcmp(tok_line[2] , "name") || strcmp(tok_line[4] , "family") || strcmp(tok_line[6] , "user-id") || strcmp(tok_line[8] , "password")
    || strcmp(tok_line[10] , "national-id-code") || strcmp(tok_line[12] , "birthdate") || tok_line[13][4] != '-' || tok_line[13][7] != '-' || tok_line[13][10] != '\0'
    || strcmp(tok_line[14] , "gender") || (strcmp(tok_line[15] , "male") && strcmp(tok_line[15] ,"female")) ||strcmp(tok_line[16] , "type") 
    || (strcmp(tok_line[17] , "student") && strcmp(tok_line[17] , "admin")) || tok_line[18] !=  NULL)
    {    
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    user* new_user = (user*)malloc(sizeof(user));
    char birthdate[15];
    char user_type[10];
    strcpy(new_user -> name , tok_line[3]);
    strcpy(new_user -> family , tok_line[5]);
    strcpy(new_user -> user_id , tok_line[7]);
    strcpy(new_user -> user_password , tok_line[9]);
    strcpy(new_user -> national_id_code , tok_line[11]);
    strcpy(birthdate , tok_line[13]);
    if(birthdate[4] != '-' || birthdate[7] != '-')
    {    
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    sscanf(birthdate , "%d-%d-%d" , &new_user -> birthdate_year , &new_user -> birthdate_month , &new_user -> birthdate_day);
    strcpy(new_user -> gender , tok_line[15]);
    strcpy(user_type , tok_line[17]);
    if(!strcmp(user_type , "student"))
    new_user -> type = student;
    else if(!strcmp(user_type , "admin"))
    new_user -> type = personel;
    if(sign_in(new_user) == permission_denied)
    {
        fprintf(f_out , "permission-denied\n");
        fclose(f_out);
        return;
    }
    else if(sign_in(new_user) == invalid)
    {
        fprintf(f_out , "invalid\n");
        fclose(f_out);
        return;
    }
    else
    {
        fprintf(f_out , "success\n");
        fclose(f_out);
        return;
    }
}

void menu_enter_options(void)
{
    printf("Please choose one: \n\n1) log-in\n2) sign-in\n0) exit\nenter a number: ");
    int enter_choice;
    scanf("%d" , &enter_choice);
    if(enter_choice == 0)
    {
        printf("\033[0;34mSee you again!\033[0m\n");
        exit(0);
    }
    else if(enter_choice == 1)
    menu_login();
    else if(enter_choice == 2)
    menu_sign_in();
    else
    {
        printf("\t\033[0;31minvalid choice!\n\033[0m");
        menu_enter_options();
    }
}

void main_menu()
{
    menu_enter_options(); 
}

void address_maker(void)
{
    char temp_address[50];
    printf("Please enter your File address: ");
    scanf("%s" , temp_address);
    FILE* faddress = fopen("address.txt" , "w+");
    fprintf(faddress , "%s" , temp_address);
    fseek(faddress , 0 , SEEK_SET);
    char c;
    char c_write = '/';
    int i , j;
    fscanf(faddress , "%c" , &c);
    while(1)
    {
        while(c != '\\')
        {
            if(feof(faddress))
            {    
                fseek(faddress , 0 , SEEK_SET);
                fscanf(faddress , "%s" , input_address);
                input_address[strlen(temp_address)] = '\0';
                fclose(faddress);
                for (i = strlen(input_address) - 1; i >= 0; i--)
                {
                    if(input_address[i] == '/')
                    break;
                }
                for (j = 0; j <= i; j++)
                output_address[j] = input_address[j];
                output_address[j] = '\0';
                strcat(output_address , "40131034.txt");
                output_address[j + 12] = '\0';
                return;
            }
            fscanf(faddress , "%c" , &c);
        }
        fseek(faddress , -1 , SEEK_CUR);
        fprintf(faddress , "%c" , c_write);
        fseek(faddress , 2 , SEEK_CUR);
        fscanf(faddress , "%c" , &c);
    }
}

void tokenizor(void)
{
    char line[300];
    char* tok_line[20];
    int i = 0;
    FILE* f_in = fopen(input_address , "r");
    while(fgets(line , 300 , f_in))
    {
        tok_line[i] = strtok(line , "#|:");
        while(tok_line[i++] != NULL)
        tok_line[i] = strtok(NULL , "#|:");
        i--;
        tok_line[i - 1][strlen(tok_line[i - 1]) - 1] = '\0';
        if(!strcmp(tok_line[1] , "register"))
        file_sign_in(tok_line);
        else if(!strcmp(tok_line[1] , "login"))
        file_login(tok_line);
        else if(!strcmp(tok_line[1] , "logout"))
        file_logout(tok_line);
        else if(!strcmp(tok_line[1] , "change-pass"))
        file_change_user_pass(tok_line);
        else if(!strcmp(tok_line[1] , "approve"))
        file_approve(tok_line);
        else if(!strcmp(tok_line[1] , "change-student-pass"))
        file_change_student_pass(tok_line);
        else if(!strcmp(tok_line[1] , "remove-student"))
        file_remove_student(tok_line);
        else if(!strcmp(tok_line[1] , "deactivate"))
        file_deactivate_student(tok_line);
        else if(!strcmp(tok_line[1] , "define-self"))
        file_define_self(tok_line);
        else if(!strcmp(tok_line[1] , "define-food"))
        file_define_food(tok_line);
        else if(!strcmp(tok_line[1] , "define-meal-plan"))
        file_define_meal_plan(tok_line);
        else if(!strcmp(tok_line[1] , "charge-student-account"))
        file_charge_student_account(tok_line);
        else if(!strcmp(tok_line[1] , "change-datetime"))
        file_change_time(tok_line);
        else if(!strcmp(tok_line[1] , "add-news"))
        file_add_news(tok_line);
        else if(!strcmp(tok_line[1] , "check-news"))
        file_check_news(tok_line);
        else if(!strcmp(tok_line[1] , "charge-account"))
        file_charge_account(tok_line);
        else if(!strcmp(tok_line[1] , "send-charge"))
        file_send_charge(tok_line);
        else if(!strcmp(tok_line[1] , "add-poll"))
        file_add_poll(tok_line);
        else if(!strcmp(tok_line[1] , "vote"))
        file_vote(tok_line);
        i = 0;
    }
}

void get_input_ways(void)
{

    printf("\n\t\t\t\033[0;33mHello!\033[0m\nPlease choose what you want to do: \n\n1) give test case\n2) enter directly\n0) exit\nenter a number: ");
    int choice;
    scanf("%d" , &choice);
    printf("\n\n");
    if(choice == 0)
    {
        printf("\033[0;34mSee you again!\033[0m\n");
        exit(0);
    }
    else if(choice == 1)
    {    
        address_maker();
        tokenizor();
    }
    else if(choice == 2)
    main_menu();
    else
    {    
        printf("\t\033[0;31minvalid choice!\n\033[0m");
        get_input_ways();
    }
}

int main(void)
{
    user* admin = (user*)malloc(sizeof(user));
    strcpy(admin -> user_id , "admin");
    strcpy(admin -> user_password , "admin");
    admin-> type = personel;
    admin -> status = approved;
    user_array = (char**)malloc(100 * sizeof(char*));
    for (int i = 0; i < 100; i++)
    user_array[i] = (char*)malloc(20);
    user_array[0] = "admin";
    FILE* fuser = fopen("user.bin" , "wb");
    fwrite(admin , sizeof(user) , 1 , fuser);
    fclose(fuser);
    free(admin);
    get_input_ways();
}