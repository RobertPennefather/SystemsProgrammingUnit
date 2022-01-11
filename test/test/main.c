/*
 CITS2002 Project 1 2015 v2
 Name(s):		Gwyn Durbridge, Robert Pennefather
 Student number(s):	21513935, 21511164
 Date:		date-of-submission
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define PI 3.14159265358979323846

struct myStop
{
    int stopID;
    double lat;
    double lon;
    int locType;
    char* name;
    int minTime;
    struct myStop* prev;
    bool checked;
    char* transport;
};

struct myTrip
{
    int tripID;
    int arrive;
    int depart;
    int stopID;
};

struct aTrip
{
    int tripID;
    int routeID;
};

struct myRoute
{
    int routeID;
    char* name;
};

struct myStop start;
struct myStop end;
struct myStop stops[14000];
struct myTrip trips[1200000];
int numberOfStops = sizeof(stops)/sizeof(stops[0]);
int numberOfTimes = sizeof(trips)/sizeof(trips[0]);

int leaveTime = 0;

char *fileNames[9] = {"agency.txt","calendar.txt","calendar_dates.txt","routes.txt","shapes.txt","stops.txt","stop_times.txt","transfers.txt","trips.txt"};
FILE *filePointers[9];

char* getfield(char* line, int num){
    char* tok;
    while ((tok = strsep(&line, ",")) != NULL) {
        if (!--num) {
            return tok;
            break;
        }
    }
    return NULL;
}

double deg2rad(double deg) {
    return (deg * PI / 180);
}

double distance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = deg2rad(lat2-lat1);
    double dLon = deg2rad(lon2-lon1);
    double a = sin(dLat/2) * sin(dLat/2) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * sin(dLon/2) * sin(dLon/2);
    double dist = 2 * atan2(sqrt(a), sqrt(1-a));
    dist = dist * 6371 * 1000 ; // Multiply by radius of earth in km
    return dist;
}

void loadRoutes() {
    printf("# Loading routes...\n");
    
    char line[256];
    
    bool isInitial = true;
    int i = 0;
    
    //Iterate through all the routes
    while (fgets(line, sizeof(line), filePointers[3])) { //Routes.txt
        if (isInitial) {
            isInitial = false;
            continue;
        }
        
        //stops[i].locType = atoi(getfield(strdup(line), 1));
        
        i++;
    }
    
}

void loadStops() {
    printf("# Loading stops...\n");
    char line[256];
    
    bool isInitial = true;
    int i = 0;
    
    //Iterate through all the stops
    while (fgets(line, sizeof(line), filePointers[5])) {
        if (isInitial) {
            isInitial = false;
            continue;
        }
        
        ///Why can't I use this tmp thing multiple times?
        //char* tmp = strdup(line);
        
        stops[i].locType = atoi(getfield(strdup(line), 1));
        stops[i].lat = strtod(getfield(strdup(line), 7),NULL);
        stops[i].lon = strtod(getfield(strdup(line), 8),NULL);
        stops[i].stopID = atoi(getfield(strdup(line), 4));
        stops[i].name = strdup(getfield(strdup(line), 5));
        
        //Can these be defined in structure: Always the same intially?????????????????????????????????????????????????????????
        stops[i].prev = NULL;
        stops[i].minTime = INFINITY;
        stops[i].checked = false;
        stops[i].transport = NULL;
        
        i++;
    }
    
    for (int i=0; i<numberOfStops; i++) {
        stops[i].prev = NULL;
        stops[i].minTime = INFINITY;
        stops[i].checked = false;
    }
    
    printf("Actual Number of Stops: %i\n",i);
    
}

int time2minutes(char* time){
    struct tm tm;
    strptime(time, "%H:%M:%S", &tm);
    int minutes = tm.tm_hour*60+tm.tm_min;
    return minutes;
}

char * minutes2time(int min) {
    struct tm time;
    time.tm_hour = min/60;
    time.tm_min = min%60;
    char timeString[10];
    strftime(timeString, sizeof(timeString), "%H:%M", &time);
    puts(timeString);
    return strdup(timeString);
}

void loadTimes() {
    printf("Loading times...\n");
    char line[256];
    
    bool isInitial = true;
    int i = 0;
    int j = 0;
    
    //Iterate through all times
    while (fgets(line, sizeof(line), filePointers[6])) {
        if (isInitial) {
            isInitial = false;
            continue;
        }
        
        int time = time2minutes(getfield(strdup(line), 2));
        
        if ((start.minTime < time) && (time < (start.minTime + 60))) {
            trips[i].tripID = atoi(getfield(strdup(line), 1));
            trips[i].arrive = time;
            trips[i].depart = time2minutes(getfield(strdup(line), 3));
            trips[i].stopID = atoi(getfield(strdup(line), 4));
            i++;
        }
        
        j++;
        
    }
    
    numberOfTimes = i;
    printf("Actual Number of Stopping Times: %i\n",j);
    printf("Stops within one hour: %i\n",i);
    
}



FILE *openFile(char *name){
    FILE *pointer = fopen(name,"r");
    
    if (pointer == NULL){
        fprintf(stderr,"ERROR opening file: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    else{
        return pointer;
    }
}

void loadFiles(char *dir, char **fileNames){
    
    if(chdir(dir) == 0) {
        printf("# Loading files from %s\n",dir);
    } else { //ERROR
        fprintf(stderr,"ERROR loading directory: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < 9;i++) { //Iterate through each of the nine files
        filePointers[i] = openFile(fileNames[i]);
        //printf("%s\n",fileNames[i]);
    }
    
    loadStops();
    loadTimes();
    loadRoutes();
}



//Find the unchecked stop with shortest travel time
struct myStop* minDist(){
    
    if(start.checked == false){
        start.checked = true;
        return &start;
    }
    
    int minTime = INFINITY;
    struct myStop* closest = NULL;
    
    for (int i=0; i<numberOfStops; i++) {
        if((stops[i].checked == false) && (stops[i].minTime < minTime)){
            minTime = stops[i].minTime;
            closest = &stops[i];
        }
    }
    
    if(end.minTime <= minTime){
        return &end;
    }
    
    closest->checked = true;
    return closest;
    
}



//Check surronding neighbours of stop
void check(struct myStop* currentStop){
    
    
    //If within 1000m of end
    double dist2end = distance(currentStop->lat, currentStop->lon, end.lat, end.lon);
    
    if (dist2end <= 1000) {
        
        int tempMinTime = currentStop->minTime + (dist2end/60) + 1;
        
        if (tempMinTime < end.minTime) {
            end.minTime = tempMinTime;
            end.prev = currentStop;
        }
    }
    
    
    //If at any type of stop
    if (currentStop->stopID != 0) {
        for (int i = 0; i < numberOfTimes; i++) {
            
            int tempStop = trips[i].stopID;
            int tempTime = trips[i].arrive;
            
            //Bus must be within one hour of start for project
            if ((currentStop->stopID == tempStop) && (currentStop->minTime < tempTime)) {
                
                int trip = trips[i].tripID;
                
                for (int j = 0; j < numberOfTimes; j++) {
                    
                    int tempTrip = trips[j].tripID;
                    
                    if ((trip == tempTrip) && (tempTime < trips[j].arrive)) {
                        
                        struct myStop* nextStop = NULL;
                        
                        int k = 0;
                        
                        while (trips[j].stopID != stops[k].stopID) {
                            k++;
                            nextStop = &stops[k];
                        }
                        
                        if (trips[j].arrive < nextStop->minTime) {
                            nextStop->minTime = trips[j].arrive;
                            nextStop->prev = currentStop;
                            if (currentStop->locType == 0) {nextStop->transport = "bus";}
                            else {nextStop->transport = "rail";}
                        }
                    }
                }
            }
        }
    }
    
    //Stops within walking distance
    for (int i=0; i<numberOfStops; i++) {
        
        double dist = distance(currentStop->lat, currentStop->lon, stops[i].lat, stops[i].lon);
        
        if (dist <= 1000) {
            
            int tempMinTime = currentStop->minTime + (dist/60) + 1;
            
            if (tempMinTime < stops[i].minTime) {
                stops[i].minTime = tempMinTime;
                stops[i].prev = currentStop;
                stops[i].transport = "walk";
            }
        }
    }
    
    //Stop now checked
    currentStop->checked = true;
    
}


void readJourney(){
    printf("# READ JOURNEY\n");
    printf("end: %s\n", end.name);
    struct myStop journey[10]; //Can we give array undefined value
    struct myStop* currentStop = &end;
    
    printf("stop: %s\n", currentStop->name);
    
    int i = 9;
    
    while (strcmp(currentStop->name,"START") != false){
        journey[i] = *currentStop;
        currentStop = currentStop->prev;
        i--;
    }
    
    journey[i] = *currentStop;
    
    for (int j = 0; j < 9; j++) {
        
        if (!journey[j].checked) {
            continue;
        }
        
        if (strcmp(journey[j+1].transport, "bus") == 0) {
            printf("%i catch bus NUMBER? to stop %i %s\n",journey[j].minTime, journey[j+1].stopID, journey[j+1].name);
        }
        else if (strcmp(journey[j+1].transport, "rail") == 0){
            printf("%i catch rail NUMBER? to stop %i %s\n",journey[j].minTime, journey[j+1].stopID, journey[j+1].name);
        }
        else if (strcmp(journey[j+1].transport, "walk") == 0){
            
            int dist = (distance(journey[j].lat, journey[j].lon, journey[j+1].lat, journey[j+1].lon))/1;
            
            if (strcmp(journey[j+1].name, "END") == 0) {
                printf("%i walk %im to destination\n",journey[j].minTime, dist);
            }
            
            else {
                printf("%i walk %im to stop %i %s\n",journey[j].minTime, dist, journey[j+1].stopID, journey[j+1].name);
            }
        }
    }
    
    printf("%i arrive\n",end.minTime);
    
}



int main(int argc, const char * argv[]) {
    
    printf("# Initialising Whichbus...\n");
    
    //Check the number of inputs
    if (argc != 6)
    {
        fprintf(stderr, "ERROR: Incorrect number of input arguments\n");
        exit(EXIT_FAILURE);
    }
    
    ///Then check to see if correct types of inputs (like is GPS)
    else {
        char dir[200];
        strcpy(dir, argv[1]);
        
        float startLan  =  atof(argv[2]);
        float startLon  =  atof(argv[3]);
        float endLan    =  atof(argv[4]);
        float endLon    =  atof(argv[5]);
        
        printf("# Calculating route from [%f,%f] to [%f,%f] using directory: %s\n", startLan, startLon, endLan, endLon, dir);
        
        //Get LEAVEHOME environment variable
        const char* s = getenv("LEAVEHOME");
        //printf("LEAVEHOME: %s\n", s);
        struct tm tm;
        strptime(s, "%a %H:%M", &tm);
        //printf("LEAVEHOME: %i:%i on weekday: %i\n", tm.tm_hour, tm.tm_min, tm.tm_wday);
        
        leaveTime = tm.tm_hour*60+tm.tm_min;
        
        start.lat = startLan;
        start.lon = startLon;
        start.name = "START";
        start.prev = NULL;
        start.minTime = leaveTime;
        start.checked = false;
        start.transport = "walk";
        
        end.lat = endLan;
        end.lon = endLon;
        end.name = "END";
        end.prev = NULL;
        end.minTime = INFINITY;
        end.checked = true;
        end.transport = "walk";
        
        
        loadFiles(dir, fileNames);
        
        struct myStop* currentStop = &start;
        
        while (strcmp(currentStop->name,"END") != 0) {
            check(currentStop);
            currentStop = minDist();
        }
        
        if (end.prev == NULL) {
            printf("LEAVEHOME: %i:%i on weekday: %i not possible\n", tm.tm_hour, tm.tm_min, tm.tm_wday);
        }
        
        readJourney();
        
        printf("DONE");
        
        exit(EXIT_SUCCESS);
        
        
        
    }
    
    
    
    return 0;
}
