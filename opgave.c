#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NAME_LGT 30
#define MAX_NUMBER_OF_RACERS 800 

typedef struct {
    char name[MAX_NAME_LGT], last_name[MAX_NAME_LGT], race_name[MAX_NAME_LGT], team[3], nationality[3];
    int age, position, hours, minutes, secounds, time_total_secs;
} racer;

void load_racers(racer*);




int main(void){
    
    
    /* inisilase racer array */
    racer racers[MAX_NUMBER_OF_RACERS];
   
    /* loads racers from file to array */
    load_racers(racers);
   
    printf("name: %s last name: %s \n", racers[0].name, racers[0].last_name);
  
    return 0;
}

void load_racers(racer* racers){
    
    char first_letter, secound_letter, third_letter;
    int loop = 0;
    
    FILE *data = fopen("cykelloeb", "r");
    
    racer* racer_pointer = racers;
    
    /* loads in first and last name !feof(data) */
    while(!feof(data)){
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
        
        /* load in last name and convert letters to lowercase other than first in a given name */
        loop = 1;
        racer_pointer->last_name[0] = first_letter;
        
        while(secound_letter != 34 && !(first_letter == 32 && secound_letter == 32 ) && (loop<MAX_NAME_LGT)){
            if(!isalpha(first_letter))
                racer_pointer->last_name[loop] = secound_letter;
            else
                racer_pointer->last_name[loop] = tolower(secound_letter);
            first_letter = secound_letter;
            loop++;
            fscanf(data, "%c", &secound_letter);
            if (loop > MAX_NAME_LGT)
                printf("error");
        }
        
        /* printf("name: %s last name: %s \n", racer_pointer->name, racer_pointer->last_name); */
        racer_pointer++;
    }
    
    
    fclose(data);
    
    
}
