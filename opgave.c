#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_NAME_LGT 30
#define MAX_NUMBER_OF_RACERS 790 
#define OTL 999999
#define DNF 9999999
#define MAX 99999999

typedef struct {
    char name[MAX_NAME_LGT], last_name[MAX_NAME_LGT], race_name[MAX_NAME_LGT], team[4], nationality[4];
    int age, position, hours, minutes, secounds, time_total_secs, points, total_points;
} racer;

typedef struct {
    char name[MAX_NAME_LGT], last_name[MAX_NAME_LGT];
    int number_of_completed_races;
} danish_racer;


void load_racers(racer*);
void calc_points(racer*);
int sort_racers_by_race_name(const void*, const void*);
void italians_over_30(racer*);
int sort_italians_over_30(const void*, const void*);
void print_italians_over_30(racer*);
void danes_who_has_completed_a_race(racer*);
int sort_danes_who_has_completed_a_race(const void*, const void*);
danish_racer* make_danish_racer_array(racer*);
int sort_racers_by_name(const void*, const void*);
void calc_total_points(racer*);
void most_points_total(racer*);
int sort_max_points(const void*, const void*);
void best_time_in_paris_amstel(racer*);
int sort_paris_amstel(const void*, const void*);
int is_amstel_or_paris(racer*);
void avrage_age_of_top_10(racer*);
int sort_top_10(const void*, const void*);



int main(int number_of_terminal_inputs, char **terminal_input){
    
   /* int x; */
   int input;
    
    /* inisilase racer array */
    racer racers[MAX_NUMBER_OF_RACERS];
   
    /* loads racers from file to array */
    load_racers(racers);  
    calc_points(racers);
    
    if(strcmp(terminal_input[1], "--print") == 0){
        italians_over_30(racers);
        printf("___________________________________________________________________________________________________ \n\n");
        danes_who_has_completed_a_race(racers);
        printf("___________________________________________________________________________________________________ \n\n");
        most_points_total(racers);
        printf("___________________________________________________________________________________________________ \n\n");
        best_time_in_paris_amstel(racers);
        printf("___________________________________________________________________________________________________ \n\n");
        avrage_age_of_top_10(racers);
        
        return 0;
    }
        
    
    do{
        printf("\ntype in the number corrosponding to the info you wish displayed \n\n");
        
        printf("(1) Italians over 30 \n");
        printf("(2) Danish riders who has completed 1 or more races \n");
        printf("(3) Top 10 point scores \n");
        printf("(4) fastest time in Amstel Gold Race and Paris Roubaix combined \n");
        printf("(5) Avarage age of top 10 races \n\n");
        printf("(0) Exit \n");
    

        scanf(" %d", &input);
        if(input == 1)
            italians_over_30(racers);
        else if(input == 2)
            danes_who_has_completed_a_race(racers);
        else if(input == 3)
            most_points_total(racers);
        else if(input == 4)
            best_time_in_paris_amstel(racers);
        else if(input == 5)
            avrage_age_of_top_10(racers);
        else if(input == 0)
            return 0;
        else
            printf("\n ##### Didn't recognise input ##### \n");
    } while(input != 0);

    return 0;
}

void load_racers(racer* racers){
    
    char first_letter, secound_letter, third_letter;
    int loop = 0;
    
    FILE *data = fopen("cykelloeb", "r");
    
    racer* racer_pointer = racers;
    
    /* load racers loop */
    while(feof(data) == 0){
        fscanf(data, " %s \"", racer_pointer->race_name);
        
        fscanf(data, " %c", &first_letter);
        fscanf(data, " %c", &secound_letter);
        
        /* load in first name */
        loop = 0;
        while(!(isupper(first_letter) && (isupper(secound_letter)))){
            racer_pointer->name[loop] = first_letter;
            first_letter = secound_letter;
            loop++;
            fscanf(data, "%c", &secound_letter);
            /* checks if the charater is a special charater  */
            if(!isalpha(secound_letter)){
                fscanf(data, "%c", &third_letter);
                ungetc(third_letter, data);
                if(isupper(first_letter) && (isupper(third_letter)))
                    break;
            }
                
        }
        racer_pointer->name[--loop] = '\0';
        
        /* load in last name and convert letters to lowercase other than first in a given name */
        loop = 1;
        racer_pointer->last_name[0] = first_letter;
        while(secound_letter != 34 && !(first_letter == 32 && secound_letter == 32 ) && (loop<MAX_NAME_LGT) ){
            if(!isalpha(first_letter))
                racer_pointer->last_name[loop] = secound_letter;
            else
                racer_pointer->last_name[loop] = tolower(secound_letter);
            first_letter = secound_letter;
            loop++;
            fscanf(data, "%c", &secound_letter);
            if (loop > MAX_NAME_LGT){
                printf("error");
                break;
            }
        }
        racer_pointer->last_name[loop] = '\0';
        
        /* loads age, team and nationality */       
        fscanf(data, " | %d", &racer_pointer->age);
        fscanf(data, " %s", racer_pointer->team);
        fscanf(data, " %s |", racer_pointer->nationality);   
        
        /* scans in position */
        fscanf(data, " %c", &first_letter);
        if(isdigit(first_letter)){
            ungetc(first_letter, data);
            fscanf(data, " %d", &racer_pointer->position);
        }
        else if (first_letter == 'O'){
            racer_pointer->position = OTL;
            while(first_letter != ' ')
                fscanf(data, "%c", &first_letter);
        }
        else{
            racer_pointer->position = DNF;
            while(first_letter != ' ')
                fscanf(data, "%c", &first_letter);
        }
        
        /* loads in time to finish race */
        fscanf(data, " %c ", &first_letter);
        if(isdigit(first_letter)){
            ungetc(first_letter, data);
            fscanf(data, " %d:", &racer_pointer->hours);
            fscanf(data, " %d:", &racer_pointer->minutes);
            fscanf(data, " %d", &racer_pointer->secounds);
            racer_pointer->time_total_secs = racer_pointer->hours * 3600 + racer_pointer->minutes * 60 + racer_pointer->secounds;
        }
        else{
            racer_pointer->hours = MAX;
            racer_pointer->minutes = MAX;
            racer_pointer->secounds = MAX;
            racer_pointer->time_total_secs = MAX;
        }                
        racer_pointer++;
    }
    fclose(data);        
}

void calc_points(racer* racers){

    int total_racers, loop, start_from = 0;
    racer* racer_pointer = racers;
    
    /* sorts racers by race name */
    /* qsort(racers, MAX_NUMBER_OF_RACERS, sizeof(racer), sort_racers_by_race_name); */
    

    while(start_from < MAX_NUMBER_OF_RACERS){
        
        /* counts number of racers in the different races */
        total_racers = 1;
        while(strcmp(racer_pointer[start_from + total_racers - 1].race_name, racer_pointer[start_from + total_racers].race_name) == 0){
            total_racers++;
            if(start_from + total_racers > MAX_NUMBER_OF_RACERS){
                printf("error");
                break;
            }
        }
        
        /* calculates points for the racers in each race */
        loop = 0;
        while(loop < total_racers){
            racer_pointer[start_from + loop].points = 0;
            
            if(!(racer_pointer[start_from + loop].position == DNF)){
                racer_pointer[start_from + loop].points += 1;
                
                if(!(racer_pointer[start_from + loop].position == OTL)){
                    racer_pointer[start_from + loop].points += 2;
                
                        switch(racer_pointer[start_from + loop].position){
                            case 1:
                                racer_pointer[start_from + loop].points += 10; break;
                            case 2:
                                racer_pointer[start_from + loop].points += 5; break;
                            case 3:
                                racer_pointer[start_from + loop].points += 2; break;
                        }
                            
                    racer_pointer[start_from + loop].points += (total_racers - loop) / 13;
                }
            }    
            loop++;
        }
        start_from += total_racers;   
    }
    
    calc_total_points(racers);
}

void italians_over_30(racer* racers){
    
    qsort(racers, MAX_NUMBER_OF_RACERS, sizeof(racer), sort_italians_over_30);
    
    print_italians_over_30(racers);   
}

int sort_italians_over_30(const void* a, const void* b){
    
    racer *pa = (racer*)a, *pb = (racer*)b;
    int result;
    
    if((strcmp(pa->nationality, "ITA") == 0) && !(strcmp(pb->nationality, "ITA") == 0))
        return -1;
    else if (!(strcmp(pa->nationality, "ITA") == 0) && (strcmp(pb->nationality, "ITA") == 0))
        return 1;
    else if (pa->age >= 30 && pb->age < 30)
        return -1;
    else if(pa->age < 30 && pb->age >= 30)
        return 1;
    
    result = strcmp(pa->last_name, pb->last_name);
    if (result == 0)
        return strcmp(pa->name, pb->name);
    
    return result;
}

void print_italians_over_30(racer* racers){
    
    int loop, is_former_same_person;
    racer* racer_pointer = racers;
    
    
    
    for(loop = 0; strcmp(racer_pointer[loop].nationality, "ITA") == 0 && racer_pointer[loop].age >= 30; loop++){
        
        if (strcmp(racer_pointer[loop].last_name, racer_pointer[loop -1].last_name) == 0 && strcmp(racer_pointer[loop].name, racer_pointer[loop -1].name) == 0)
            is_former_same_person = 1;
        else
            is_former_same_person = 0;
        
        /* prints sepetator line if its a new person */
        if(is_former_same_person == 0)
            printf("___________________________________________________________________________________________________ \n\n");
        
        if(is_former_same_person == 0)
            printf("%s, %s    age: %d    nationality: %s    total points Earned: %d \n\n", racer_pointer[loop].last_name, racer_pointer[loop].name, racer_pointer[loop].age, racer_pointer[loop].nationality, racer_pointer[loop].total_points);
        
        if(racer_pointer[loop].position == OTL)
            printf("%-30s    position: OTL   time:  OTL       points earned: %d \n", racer_pointer[loop].race_name, racer_pointer[loop].points);
        if(racer_pointer[loop].position == DNF)
            printf("%-30s    position: DNF   time:  DNF       points earned: %d \n", racer_pointer[loop].race_name, racer_pointer[loop].points); 
        else    
            printf("%-30s    position: %3d   time: %2d:%2d:%2d   points earned: %d \n", racer_pointer[loop].race_name, racer_pointer[loop].position, racer_pointer[loop].hours, racer_pointer[loop].minutes, racer_pointer[loop].secounds, racer_pointer[loop].points);
        
    }
}

void danes_who_has_completed_a_race(racer* racers){
    
    danish_racer* danish_racers;
    int loop = -1;
    
    danish_racers = (danish_racer*) make_danish_racer_array(racers);

    printf("\n Danish racers who has finished 1 or more races: \n\n");
    printf("     First name          | Last name           | Completed races \n");
    printf("    ---------------------|---------------------|---------------------\n");
    
    while(strcmp(danish_racers[++loop].name, "end of list") != 0)
        printf("     %-20s| %-20s| %d races completed \n", danish_racers[loop].name, danish_racers[loop].last_name, danish_racers[loop].number_of_completed_races);

    free(danish_racers);
}

danish_racer* make_danish_racer_array(racer *racers){
        
    int x = 0, y = 0, number_of_races;
    racer *racer_pointer = racers; 
    danish_racer *danish_racers;
    
    danish_racers = (danish_racer*) calloc(30, sizeof(danish_racer));
   
    qsort(racers, MAX_NUMBER_OF_RACERS, sizeof(racer), sort_danes_who_has_completed_a_race);
    
    while(strcmp(racer_pointer[x].nationality, "DEN") == 0){
        
        number_of_races = 0;
        
        do{
            x++;
            if(racer_pointer[x].position != DNF)
                number_of_races++;
        } while(strcmp(racer_pointer[x].name, racer_pointer[x + 1].name) == 0 && strcmp(racer_pointer[x].last_name, racer_pointer[x + 1].last_name) == 0);
        
        if(number_of_races != 0){
            strcpy(danish_racers[y].name, racer_pointer[x].name);
            strcpy(danish_racers[y].last_name, racer_pointer[x].last_name);
            danish_racers[y].number_of_completed_races = number_of_races;
            
            y++;
        } 
    }
    
    if(y > 30){
        printf("number of danish racers to large \n");
        exit(EXIT_FAILURE);
    }

    strcpy(danish_racers[y].name, "end of list");  
    
    return danish_racers; 
}

int sort_danes_who_has_completed_a_race(const void* a, const void *b){
    
    racer *pa = (racer*)a, *pb = (racer*)b;
    int result;
    
    if((strcmp(pa->nationality, "DEN") == 0) && !(strcmp(pb->nationality, "DEN") == 0))
        return -1;
    else if (!(strcmp(pa->nationality, "DEN") == 0) && (strcmp(pb->nationality, "DEN") == 0))
        return 1;
    
    result = strcmp(pa->last_name, pb->last_name);
    if (result == 0)
        return strcmp(pa->name, pb->name);
    
    return result;
}

int sort_racers_by_name(const void* a, const void* b){
    
    racer *pa = (racer*)a, *pb = (racer*)b;
    int result;
     
    result = strcmp(pa->last_name, pb->last_name);
    if (result == 0)
        return strcmp(pa->name, pb->name);
    
    return result; 
}

void calc_total_points(racer* racers){
    
    int total_points, number_of_times_listed, loop;
    racer* racer_pointer = racers;
    
    qsort(racers, MAX_NUMBER_OF_RACERS, sizeof(racer), sort_racers_by_name);
        
    for(loop = 0; loop < MAX_NUMBER_OF_RACERS; loop++){
            
        total_points = racer_pointer[loop].points;
        number_of_times_listed = 0;
                
        while((strcmp(racer_pointer[loop].name, racer_pointer[loop + 1].name) == 0) && (strcmp(racer_pointer[loop].last_name, racer_pointer[loop + 1].last_name) == 0)){
            loop++;
            total_points += racer_pointer[loop].points;
            number_of_times_listed++;
        }
            
        while(number_of_times_listed >= 0){
            racer_pointer[loop - number_of_times_listed].total_points = total_points;
            --number_of_times_listed;
        }  
    }
}

void most_points_total(racer* racers){
    
    int number_of_racers = 1, x = 0;
    racer* racer_pointer = racers;
    
    qsort(racers, MAX_NUMBER_OF_RACERS, sizeof(racer), sort_max_points);

    printf("\n Highest point scores in all races combined: \n\n");
    printf("     First name          | Last name           | Total points earned \n");
    printf("    ---------------------|---------------------|---------------------\n");
    
    while(number_of_racers <= 10){
        
        if(!(strcmp(racer_pointer[x].name, racer_pointer[x - 1].name) == 0) && !(strcmp(racer_pointer[x].last_name, racer_pointer[x - 1].last_name) == 0)){
            printf("     %-20s| %-20s|         %-4d \n", racer_pointer[x].last_name, racer_pointer[x].name, racer_pointer[x].total_points);
            number_of_racers++;
        }
        x++;
    }
}

int sort_max_points(const void* a, const void* b){
    
    racer *pa = (racer*)a, *pb = (racer*)b;
    int result;
    
    if(pa->total_points > pb->total_points)
        return -1;    
    if(pa->total_points < pb->total_points)
        return 1;
    

    result = strcmp(pa->last_name, pb->last_name);
    if (result == 0)
        return strcmp(pa->name, pb->name);
    
    return result;
}

void best_time_in_paris_amstel(racer* racers){
    
    int loop = 1, best_result = MAX, current_result, posistion_in_array;
    racer* racer_pointer = racers;
    
    qsort(racers, MAX_NUMBER_OF_RACERS, sizeof(racer), sort_paris_amstel);
    
    while(is_amstel_or_paris(&racer_pointer[loop])){
        
        if((strcmp(racer_pointer[loop].name, racer_pointer[loop - 1].name) == 0) && (strcmp(racer_pointer[loop].last_name, racer_pointer[loop - 1].last_name) == 0))
            current_result = racer_pointer[loop].time_total_secs + racer_pointer[loop - 1].time_total_secs;
            
        if(current_result < best_result){
            best_result = current_result;
            posistion_in_array = loop;
        }
        loop++;
    }
    
    printf("Fastest time in Paris Roubaix and Amstel Gold Race combined: %s %s   time: ", racer_pointer[posistion_in_array].name, racer_pointer[posistion_in_array].last_name);
    printf("%d:%d:%d \n", best_result / 3600, (best_result % 3600) / 60, best_result % 60);
}

int sort_paris_amstel(const void* a, const void* b){
    
    int result;
    racer *pa = (racer*)a, *pb = (racer*)b;
    
    if(is_amstel_or_paris(pa) && !is_amstel_or_paris(pb))
        return -1;
    else if (!is_amstel_or_paris(pa) && is_amstel_or_paris(pb))
        return 1;
    
    result = strcmp(pa->last_name, pb->last_name);
    if (result == 0)
        return strcmp(pa->name, pb->name);
    
    return result;
    
}

int is_amstel_or_paris(racer* a){
    return ((strcmp(a->race_name, "AmstelGoldRace") == 0) || (strcmp(a->race_name, "ParisRoubaix") == 0));
}

void avrage_age_of_top_10(racer* racers){
    
    int loop = 0, number_of_people = 0;
    double avarage_age = 0;
    racer* racer_pointer = racers;
    
    qsort(racers, MAX_NUMBER_OF_RACERS, sizeof(racer), sort_top_10);
    
    for(loop = 0; racer_pointer[loop].position <= 10; loop++){
        if((strcmp(racer_pointer[loop].name, racer_pointer[loop - 1].name) != 0)){
            number_of_people++;
            avarage_age += racer_pointer[loop].age;
        }
    }

    avarage_age = avarage_age / number_of_people;
    
    printf("The avarage age of the top 10 racers is: %.2lf \n", avarage_age);
    
}

int sort_top_10(const void* a, const void* b){
    
    int result;
    racer *pa = (racer*)a, *pb = (racer*)b;
    
    if((pa->position <= 10) && !(pb->position <= 10))
        return -1;
    if(!(pa->position <= 10) && (pb->position <= 10))
        return 1;
    
    result = strcmp(pa->last_name, pb->last_name);
    if (result == 0)
        return strcmp(pa->name, pb->name);
    
    return result;
}

