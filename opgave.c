#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_NAME_LGT 30
#define MAX_NUMBER_OF_RACERS 800 
#define OTL 9999999
#define DNF 99999999
#define MAX 999999999

typedef struct {
    char name[MAX_NAME_LGT], last_name[MAX_NAME_LGT], race_name[MAX_NAME_LGT], team[3], nationality[3];
    int age, position, hours, minutes, secounds, time_total_secs, points;
} racer;

void load_racers(racer*);
void calc_points(racer*);
int sort_racers_by_race_name(const void*, const void*);



int main(void){
    
    /* inisilase racer array */
    racer racers[MAX_NUMBER_OF_RACERS];
   
    /* loads racers from file to array */
    load_racers(racers);
    calc_points(racers);
   
    printf("name: %s last name: %s \n", racers[220 - 1].name, racers[220 - 1].last_name);
    printf("position: %d points: %d \n", racers[220 - 1].position, racers[220 - 1].points);
  
    return 0;
}

void load_racers(racer* racers){
    
    char first_letter, secound_letter, third_letter;
    int loop = 0, x = 0;
    
    FILE *data = fopen("cykelloeb", "r");
    
    racer* racer_pointer = racers;
    
    /* loads in first and last name */
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
        racer_pointer->name[loop--] = '\0';
        
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
        
        /* scans in posistion */
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
        fscanf(data, " %c", &first_letter);
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

        /* printf("name: %s last name: %s \n", racer_pointer->name, racer_pointer->last_name); */
        
        x++;
        /* printf("%d \n", x); */
        
        /* FIND BETTER SOLUTION! */
        if(x == 790)
            break;
        
        racer_pointer++;
    }
    fclose(data);  
}

void calc_points(racer* racers){

    int total_racers, loop, start_from = 0;
    racer* racer_pointer = racers;
    
    /* sorts racers by race name */
    /* qsort(racers, MAX_NUMBER_OF_RACERS, sizeof(racer), sort_racers_by_race_name); */
    
    while(start_from <= MAX_NUMBER_OF_RACERS){
        total_racers = 1;
        while(strcmp(racer_pointer[start_from + total_racers - 1].race_name, racer_pointer[start_from + total_racers].race_name) == 0){
            total_racers++;
            if(start_from + total_racers > MAX_NUMBER_OF_RACERS){
                printf("error");
                break;
            }
        }
        total_racers++;
        
        loop = 0;
        while(loop < total_racers){
            if(racer_pointer[start_from + loop].position == OTL)
                racer_pointer[start_from + loop].points = (total_racers - loop) / 13 + 1;
            else if(racer_pointer[start_from + loop].position == DNF)
                racer_pointer[start_from + loop].points = 0;
            else
                racer_pointer[start_from + loop].points = (total_racers - loop) / 13 + 3;
            loop++;
        }
        start_from += total_racers;
            
        /* printf("%d \n", total_racers); */
    }
    
}

int sort_racers_by_race_name(const void* a, const void* b){
    
    racer *pa = (racer*)a, *pb = (racer*)b;
    
    return strcmp(pa->race_name, pb->race_name);
}


