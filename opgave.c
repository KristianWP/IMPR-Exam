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
void take_terminal_input(char**, racer*);
void take_user_input(racer*);
void italians_over_30_compact(racer*);
void print_all_info(racer*);
void load_names(FILE*, racer*);
void load_position(FILE*, racer*);
void load_time(FILE*, racer*);
int count_racers_in_race(racer*, int);
void calc_points_per_racer(racer*, int, int);
int is_same_person(racer*, racer*);



int main(int number_of_terminal_inputs, char **terminal_input){
    
    /* inisilase racer array */
    racer racers[MAX_NUMBER_OF_RACERS];
   
    /* loads racers from file to array */
    load_racers(racers);  
	
	take_terminal_input(terminal_input, racers);
	
	take_user_input(racers);

    return 0;
}

void load_racers(racer* racers){
        
    FILE *data = fopen("cykelloeb", "r");    
    racer* racer_pointer = racers;
    
    /* load racers loop */
    while(feof(data) == 0){
        fscanf(data, " %s \"", racer_pointer->race_name);
        
        load_names(data, racer_pointer);
        
        /* loads age, team and nationality */       
        fscanf(data, " | %d", &racer_pointer->age);
        fscanf(data, " %s", racer_pointer->team);
        fscanf(data, " %s |", racer_pointer->nationality);   
        
        load_position(data, racer_pointer);
        
        load_time(data, racer_pointer);
        
        racer_pointer++;
    }
    fclose(data);

	calc_points(racers);	
}

void load_time(FILE* data, racer* racer_pointer){
        
    char test_letter;    
        
    fscanf(data, " %c ", &test_letter);
    if(isdigit(test_letter)){
        ungetc(test_letter, data);
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
}        

void load_position(FILE* data,racer* racer_pointer){

    char test_letter;

    fscanf(data, " %c", &test_letter);
    if(isdigit(test_letter)){
        ungetc(test_letter, data);
        fscanf(data, " %d", &racer_pointer->position);
    }
    else if (test_letter == 'O'){
        racer_pointer->position = OTL;
        while(test_letter != ' ')
            fscanf(data, "%c", &test_letter);
    }
    else{
        racer_pointer->position = DNF;
        while(test_letter != ' ')
            fscanf(data, "%c", &test_letter);    
    }
}

void load_names(FILE *data, racer* racer_pointer){
    
    char first_letter, secound_letter, third_letter;
    int loop;
    
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
}

void calc_points(racer* racers){

    int total_racers, start_from = 0;

    while(start_from < MAX_NUMBER_OF_RACERS){
        
        total_racers = count_racers_in_race(racers, start_from);
        
        calc_points_per_racer(racers, start_from, total_racers);
        
        start_from += total_racers;   
    }
    
    calc_total_points(racers);
}

void calc_points_per_racer(racer* racer_pointer, int start_from, int total_racers){

    int loop = 0;
    
    while(loop <= total_racers){
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
                        
                racer_pointer[start_from + loop].points += (total_racers - loop - 1) / 13;
            }
        }    
    loop++;
    }    
}

int count_racers_in_race(racer* racer_pointer, int start_from){
    
    int total_racers = 1;
    
    while(strcmp(racer_pointer[start_from + total_racers - 1].race_name, racer_pointer[start_from + total_racers].race_name) == 0){
        total_racers++;
        if(start_from + total_racers > MAX_NUMBER_OF_RACERS){
            printf("error");
            break;
        }
    }
        
    return total_racers;
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

void italians_over_30(racer* racers){
    
    int loop, is_former_same_person;
    racer* racer_pointer = racers;
    
    qsort(racers, MAX_NUMBER_OF_RACERS, sizeof(racer), sort_italians_over_30);
    
    /* prints racers one by one, includeing all the races competed in and seperated by lines */
    for(loop = 0; strcmp(racer_pointer[loop].nationality, "ITA") == 0 && racer_pointer[loop].age >= 30; loop++){
        
        is_former_same_person = is_same_person(&racer_pointer[loop], &racer_pointer[loop -1]);
        
        /* prints sepetator line if its a new person */
        if(is_former_same_person == 0)
            printf("___________________________________________________________________________________________________ \n\n");
        
        if(is_former_same_person == 0)
            printf("Name: %s, %s    age: %d    nationality: %s    total points Earned: %d \n\n", racer_pointer[loop].last_name, racer_pointer[loop].name, racer_pointer[loop].age, racer_pointer[loop].nationality, racer_pointer[loop].total_points);
        
        if(racer_pointer[loop].position == OTL)
            printf("%-30s    position: OTL   time:  OTL       points earned: %d \n", racer_pointer[loop].race_name, racer_pointer[loop].points);
        if(racer_pointer[loop].position == DNF)
            printf("%-30s    position: DNF   time:  DNF       points earned: %d \n", racer_pointer[loop].race_name, racer_pointer[loop].points); 
        else    
            printf("%-30s    position: %3d   time: %2d:%2d:%2d   points earned: %d \n", racer_pointer[loop].race_name, racer_pointer[loop].position, racer_pointer[loop].hours, racer_pointer[loop].minutes, racer_pointer[loop].secounds, racer_pointer[loop].points);
        
    }
    printf("___________________________________________________________________________________________________ \n\n");
}

void italians_over_30_compact(racer* racers){
	
    int loop, number_of_times_listed;
    racer* racer_pointer = racers;
    
    qsort(racers, MAX_NUMBER_OF_RACERS, sizeof(racer), sort_italians_over_30);
    
    /* prints header */
	printf("\n Racers from italy, older then 30: \n\n");
    printf("     First name          | Last name           | age  | Races competed in \n");
    printf("    ---------------------|---------------------|------|-------------------\n");
	
    /* prints racers who fit the critiria */
    for(loop = 0; strcmp(racer_pointer[loop].nationality, "ITA") == 0 && racer_pointer[loop].age >= 30; loop++){
        
        number_of_times_listed = 1;
		
		while(strcmp(racer_pointer[loop].last_name, racer_pointer[loop +1].last_name) == 0 && strcmp(racer_pointer[loop].name, racer_pointer[loop +1].name) == 0){
            number_of_times_listed += 1;
			loop++;
		}
    
        printf("     %-20s| %-20s|  %d  |          %d \n", racer_pointer[loop].name, racer_pointer[loop].last_name, racer_pointer[loop].age, number_of_times_listed);
        
	}
}

void danes_who_has_completed_a_race(racer* racers){
    
    danish_racer* danish_racers;
    int loop = -1;
    
    danish_racers = (danish_racer*) make_danish_racer_array(racers);

    /* prints header */
    printf("\n Danish racers who has finished 1 or more races: \n\n");
    printf("     First name          | Last name           | Completed races \n");
    printf("    ---------------------|---------------------|---------------------\n");
    
    /* prints all info in the array, until racer named "end of list" */
    while(strcmp(danish_racers[++loop].name, "end of list") != 0)
        printf("     %-20s| %-20s| %d races completed \n", danish_racers[loop].name, danish_racers[loop].last_name, danish_racers[loop].number_of_completed_races);

    /* free's array inisiated in make_danish_racer_array */
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
        
        /* counts number of races any given racer has been a part of and finished*/
        do{
            x++;
            if(racer_pointer[x].position != DNF)
                number_of_races++;
        } while(is_same_person(&racer_pointer[x], &racer_pointer[x + 1]));
        
        /* saves any racer who has completed 1 or more races to an array */
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

    /* adds a racer named "end of list" below the last */
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
                
        /* counts if any charater is listed multiple times in a row, and adds the points together */
        while(is_same_person(&racer_pointer[loop], &racer_pointer[loop + 1])){
            loop++;
            total_points += racer_pointer[loop].points;
            number_of_times_listed++;
        }
          
        /* adds the total value of points to all the posistions where the racer is listed  */
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
        
        if(!is_same_person(&racer_pointer[x], &racer_pointer[x - 1])){
            printf("     %-20s| %-20s|         %-4d \n", racer_pointer[x].name, racer_pointer[x].last_name, racer_pointer[x].total_points);
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
    
    int loop = 1, best_result = MAX, current_result = MAX, posistion_in_array;
    racer* racer_pointer = racers;
    
    qsort(racers, MAX_NUMBER_OF_RACERS, sizeof(racer), sort_paris_amstel);
    
    /* checks if any person is listed twice in a row, signifying they have partisipated in both races, then adding the times together and saving the fastest time */
    while(is_amstel_or_paris(&racer_pointer[loop])){
        
        if(is_same_person(&racer_pointer[loop], &racer_pointer[loop - 1]))
            current_result = racer_pointer[loop].time_total_secs + racer_pointer[loop - 1].time_total_secs;
            
        if(current_result < best_result){
            best_result = current_result;
            posistion_in_array = loop;
        }
        loop++;
    }
    
    printf("Fastest time in Paris Roubaix and Amstel Gold Race combined: %s %s   time: ", racer_pointer[posistion_in_array].name, racer_pointer[posistion_in_array].last_name);
    
    /* devides the total time in secounds into hours, minutes and secounds */
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
    
    /* adds all ages together for every individual */
    for(loop = 0; racer_pointer[loop].position <= 10; loop++){
        if(!is_same_person(&racer_pointer[loop], &racer_pointer[loop - 1])){
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

void take_terminal_input(char** terminal_input, racer* racers){
	
	if(strcmp(terminal_input[1], "--print") == 0){
        print_all_info(racers);
		
		exit(EXIT_SUCCESS);
    }
}

void take_user_input(racer* racers){
	
	int input;
	
	do{
        printf("\ntype in the number corrosponding to the info you wish displayed \n\n");
        
        printf("(1) Italians over 30 (Full list) \n");
		printf("(2) Italians over 30 (Compact) \n");
        printf("(3) Danish riders who has completed 1 or more races \n");
        printf("(4) Top 10 point scores \n");
        printf("(5) fastest time in Amstel Gold Race and Paris Roubaix combined \n");
        printf("(6) Avarage age of top 10 races \n\n");
        printf("(7) Print all info (2-6) \n\n");
        printf("(0) Exit \n");
    

        scanf(" %d", &input);
        if(input == 1)
            italians_over_30(racers);
		else if(input == 2)
            italians_over_30_compact(racers);
        else if(input == 3)
            danes_who_has_completed_a_race(racers);
        else if(input == 4)
            most_points_total(racers);
        else if(input == 5)
            best_time_in_paris_amstel(racers);
        else if(input == 6)
            avrage_age_of_top_10(racers);
        else if(input == 7)
            print_all_info(racers);
        else if(input != 0)
            printf("\n ##### Didn't recognise input ##### \n");
    } while(input != 0);
}

void print_all_info(racer* racers){
    
    italians_over_30_compact(racers);
    printf("___________________________________________________________________________________________________ \n");
    danes_who_has_completed_a_race(racers);
    printf("___________________________________________________________________________________________________ \n");
    most_points_total(racers);
    printf("___________________________________________________________________________________________________ \n\n");
    best_time_in_paris_amstel(racers);
    printf("___________________________________________________________________________________________________ \n\n");
    avrage_age_of_top_10(racers);
    printf("___________________________________________________________________________________________________ \n");
}

int is_same_person(racer* person_1, racer* person_2){
    return (strcmp(person_1->name, person_2->name) == 0) && (strcmp(person_1->last_name, person_2->last_name) == 0);
}



