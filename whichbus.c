/*
 CITS2002 Project 1 2015 v4
 Name(s):		Gwyn Durbridge, Robert Pennefather
 Student number(s):	21513935, 21511164
 Date:		18-09-15
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

#define LARGE_NUMBER 2000
#define JOURNEY_LENGTH 10
#define PI 3.14159265358979323846


//Create structures to store fields
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
    int tripID;
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


//Create myStop structures for start, end and other stops inbetween
//Create arrays for other data to later import into
struct myStop start;
struct myStop end;
struct myStop stops[14000];
struct myTrip trips[1200000];
struct aTrip someTrips[35000];
struct myRoute routes[500];

int numberOfStops = sizeof(stops)/sizeof(stops[0]);
int numberOfTimes = sizeof(trips)/sizeof(trips[0]);

int leaveTime = 0;

//Store list of filenames to load from given directory
char *fileNames[9] = {"agency.txt","calendar.txt","calendar_dates.txt","routes.txt","shapes.txt","stops.txt","stop_times.txt","transfers.txt","trips.txt"};
FILE *filePointers[9];


//Function to split transperth data into various fields based on commas
//Effectively a CSV parser
//Will return the field value for the given line and field number
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

//Converts degrees into radians
double deg2rad(double deg) {
    return (deg * PI / 180);
}


//Find the distance of two longitude and latitude points, taking into account curvature of Earth
double distance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = deg2rad(lat2-lat1);
    double dLon = deg2rad(lon2-lon1);
    double a = sin(dLat/2) * sin(dLat/2) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * sin(dLon/2) * sin(dLon/2);
    double dist = 2 * atan2(sqrt(a), sqrt(1-a));
    dist = dist * 6371 * 1000 ; // Multiply by radius of earth in km
    return dist;
}


//Find the the name of a trip ie. Bus 98 or Blue CAT
char *getRouteName(int tripID) {
    
    int i = 0;
    int j = 0;
    
    while (someTrips[i].tripID != tripID) {
        i++;
    }
    
    while (routes[j].routeID != someTrips[i].routeID) {
        j++;
    }
    
    return routes[j].name;
    
}


//Import transperth file route names
void loadRoutes() {
    
    char line[256];
    
    bool isInitial = true;
    int i = 0;
    
    //Iterate through all the routes
    while (fgets(line, sizeof(line), filePointers[3])) { //Routes.txt
        if (isInitial) {
            isInitial = false;
            continue;
        }
        
        routes[i].routeID = atoi(getfield(strdup(line), 1));
        
        char * shortName = strdup(getfield(strdup(line), 3));
        char * longName = strdup(getfield(strdup(line), 4));
        
        //Only use name that isn't empty
        if (strcmp(shortName, "") != 0) {
            routes[i].name = shortName;
        }
        else if (strcmp(longName, "") != 0) {
            sprintf(longName,"\"%s\"",longName);
            routes[i].name = strdup(longName);
        }
        
        i++;
        
    }
}


//Import transperth trips
void loadTrips() {
    
    char aline[256];
    
    bool isInitial = true;
    int i = 0;
    
    //Iterate through all the trips
    while (fgets(aline, sizeof(aline), filePointers[8])) { //Trips.txt
        if (isInitial) {
            isInitial = false;
            continue;
        }
        
        someTrips[i].routeID = atoi(getfield(strdup(aline), 1));
        someTrips[i].tripID = atoi(getfield(strdup(aline), 3));
        
        i++;
    }
    
}


//Import transperth information for different stops
void loadStops() {
    
    char line[256];
    
    bool isInitial = true;
    int i = 0;
    
    //Iterate through all the stops
    while (fgets(line, sizeof(line), filePointers[5])) {
        if (isInitial) {
            isInitial = false;
            continue;
        }
        
        //Find if a stop is either a bus or train stop
        if (strcmp(strdup(getfield(strdup(line), 2)), "") != 0) {
            stops[i].locType = 1;
        }
        else {
            stops[i].locType = atoi(getfield(strdup(line), 1));
        }
        
        stops[i].lat = strtod(getfield(strdup(line), 7),NULL);
        stops[i].lon = strtod(getfield(strdup(line), 8),NULL);
        stops[i].tripID = atoi(getfield(strdup(line), 3));
        stops[i].stopID = atoi(getfield(strdup(line), 4));
        stops[i].name = strdup(getfield(strdup(line), 5));
        
        i++;
    }
    
    //Same for all stops intially
    for (int i=0; i<numberOfStops; i++) {
        stops[i].prev = NULL;
        stops[i].minTime = LARGE_NUMBER;
        stops[i].checked = false;
        stops[i].transport = NULL;
    }
}


//Function to convert a given 24 hour clock time into minutes past midnight
int time2minutes(char* time){
    struct tm tm;
    strptime(time, "%H:%M:%S", &tm);
    int minutes = tm.tm_hour*60+tm.tm_min;
    return minutes;
}


//Convert the number of minutes past midnight back to a 24 hour clock time
char * minutes2time(int min) {
    struct tm time;
    time.tm_hour = min/60;
    time.tm_min = min%60;
    char timeString[10];
    strftime(timeString, sizeof(timeString), "%H:%M", &time);
    return strdup(timeString);
}


//Import transperth public transport arrival times
void loadTimes() {
    
    char line[256];
    
    bool isInitial = true;
    int i = 0;
    
    //Iterate through all times
    while (fgets(line, sizeof(line), filePointers[6])) {
        if (isInitial) {
            isInitial = false;
            continue;
        }
        
        int time = time2minutes(getfield(strdup(line), 2));
        
        //Only import times that are within one hour of after leaving home (as stated by project brief)
        if ((start.minTime < time) && (time < (start.minTime + 60))) {
            trips[i].tripID = atoi(getfield(strdup(line), 1));
            trips[i].arrive = time;
            trips[i].depart = time2minutes(getfield(strdup(line), 3));
            trips[i].stopID = atoi(getfield(strdup(line), 4));
            i++;
        }
    }
    
    numberOfTimes = i; //Reduce number of times that will now need to be looked through
    
}

//Return file pointer for given file name
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


//Import all required files
void loadFiles(char *dir, char **fileNames){
    
    if(chdir(dir) != 0) { //ERROR
        fprintf(stderr,"ERROR loading directory: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < 9;i++) { //Iterate through each of the nine files
        //Create file pointers to each of the nine transit data files
        filePointers[i] = openFile(fileNames[i]);
    }
   
    //Load transit data into memory from given directory
    loadStops();
    loadTimes();
    loadRoutes();
    loadTrips();
}


//Find the unchecked stop with shortest travel time
struct myStop* minDist(){
    
    if(start.checked == false){
        start.checked = true;
        return &start;
    }
    
    int minTime = LARGE_NUMBER;
    struct myStop* closest = NULL;
    
    for (int i=0; i<numberOfStops; i++) {
        if((stops[i].checked == false) && (stops[i].minTime < minTime)){
            minTime = stops[i].minTime;
            closest = &stops[i];
        }
    }
    
    //Return end if it is the shortest stop
    if(end.minTime <= minTime){
        return &end;
    }

    return closest;
    
}


//Check all neighbours of stop and change their minimum travel time if lower than currently
void check(struct myStop* currentStop){
    
    
    //If within 1000m of end, test if faster than travel time to end
    double dist2end = distance(currentStop->lat, currentStop->lon, end.lat, end.lon);
    
    if (dist2end <= 1000) {
        
        int tempMinTime = currentStop->minTime + (dist2end/60) + 1;
        
        if (tempMinTime < end.minTime) {
            end.minTime = tempMinTime;
            end.prev = currentStop;
        }
    }
    
    
    //If at any type of stop, test time taken to reach stop
    if (currentStop->stopID != 0) {
        for (int i = 0; i < numberOfTimes; i++) {
            
            int tempStop = trips[i].stopID;
            int tempTime = trips[i].arrive;
            
            //Check all transport times, must travel to current stop and arrive after traveller arrives
            if ((currentStop->stopID == tempStop) && (currentStop->minTime < tempTime)) {
                
                int trip = trips[i].tripID;
                
                for (int j = 0; j < numberOfTimes; j++) {
                    
                    int tempTrip = trips[j].tripID;
                    
                    //Find stops this route travels to, arrival times at other stops must be after arrival at current stop
                    if ((trip == tempTrip) && (tempTime < trips[j].arrive)) {
                        
                        struct myStop* nextStop = NULL;
                        
                        int k = 0;
                        
                        //Create a pointer to next stop bus can travel to
                        while (trips[j].stopID != stops[k].stopID) {
                            k++;
                            nextStop = &stops[k];
                        }
                        
                        //Check if time to travel to next stop is faster than current minimum time to stop
                        if (trips[j].arrive < nextStop->minTime) {
                            nextStop->minTime = trips[j].arrive;
                            nextStop->prev = currentStop;
                            nextStop->tripID = trip;
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
            
            //Check if time to walk to stop is faster than current minimum time to stop
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

//Print output for journey
void readJourney(){
    
    struct myStop journey[JOURNEY_LENGTH];
    struct myStop* currentStop = &end;
    
    for (int k = 0; k < JOURNEY_LENGTH; k++) {
        journey[k].checked = false;
    }
    
    int i = JOURNEY_LENGTH - 1;
    
    //Put all stops into journey array working back from end to start
    while (strcmp(currentStop->name,"START") != 0){
        journey[i] = *currentStop;
        currentStop = currentStop->prev;
        i--;
    }
    
    journey[i] = *currentStop;
    
    for (int j = 0; j < JOURNEY_LENGTH - 1; j++) {
        
        //Skips over journey elements with nothing in it
        if (!journey[j].checked) {
            continue;
        }
        
        //Prints if bus was used to travel to stop
        if (strcmp(journey[j+1].transport, "bus") == 0) {
            printf("%s catch bus %s to stop %i %s\n", minutes2time(journey[j].minTime), getRouteName(journey[j+1].tripID), journey[j+1].stopID, journey[j+1].name);
        }
        
        //Prints if train was used to travel to stop
        else if (strcmp(journey[j+1].transport, "rail") == 0){
            printf("%s catch rail %s to stop %i %s\n", minutes2time(journey[j].minTime), getRouteName(journey[j+1].tripID), journey[j+1].stopID, journey[j+1].name);
        }
        
        //Prints if stop was walked to
        else if (strcmp(journey[j+1].transport, "walk") == 0){
            
            int dist = (distance(journey[j].lat, journey[j].lon, journey[j+1].lat, journey[j+1].lon))/1;
            
            //Prints final walking distance to end destination, if end is next stop
            if (strcmp(journey[j+1].name, "END") == 0) {
                printf("%s walk %im to destination\n", minutes2time(journey[j].minTime), dist);
            }
            else {
                printf("%s walk %im to stop %i %s\n", minutes2time(journey[j].minTime), dist, journey[j+1].stopID, journey[j+1].name);
            }
        }
    }
    
    //Prints arrival time
    printf("%s arrive\n", minutes2time(end.minTime));
    
}


int main(int argc, const char * argv[]) {
    fprintf(stderr, "Starting...\n");
    
    //Check the number of inputs
    if (argc != 6)
    {
        fprintf(stderr, "ERROR: Incorrect number of input arguments\n");
        exit(EXIT_FAILURE);
    }
    
    else {
        //Get argument parameters and set them as variables
        
        //directory location parameter
        char dir[200];
        strcpy(dir, argv[1]);
        
        //GPS location parameters
        float startLan  =  atof(argv[2]);
        float startLon  =  atof(argv[3]);
        float endLan    =  atof(argv[4]);
        float endLon    =  atof(argv[5]);
        
        //Get LEAVEHOME environment variable
        const char* s = getenv("LEAVEHOME");
        struct tm tm;
        strptime(s, "%a %H:%M", &tm);

        //Calculate the leave time in terms of minutes from midnight
        leaveTime = tm.tm_hour*60+tm.tm_min;
        
        //Intialise starting location as myStop
        start.lat = startLan;
        start.lon = startLon;
        start.name = "START";
        start.prev = NULL;
        start.minTime = leaveTime;
        start.checked = false;
        start.transport = "walk";
        
        //Intialise end location as myStop
        end.lat = endLan;
        end.lon = endLon;
        end.name = "END";
        end.prev = NULL;
        end.minTime = LARGE_NUMBER;
        end.checked = true;
        end.transport = "walk";
        
        //Import files from transperth
        loadFiles(dir, fileNames);
        
        struct myStop* currentStop = &start;
        
        //Check stops with minimum travel time until end stop is reached
        while (strcmp(currentStop->name,"END") != 0) {
            check(currentStop);
            currentStop = minDist();
        }
        
        if (end.prev == NULL) {
            printf("LEAVEHOME: %i:%i on weekday: %i not possible\n", tm.tm_hour, tm.tm_min, tm.tm_wday);
        }
        
        //Print journey if end location can be reached with given conditions
        else {
            readJourney();
        }
        
        exit(EXIT_SUCCESS);
        
    }
    
    return 0;
    
}
