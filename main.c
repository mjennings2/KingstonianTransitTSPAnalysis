//
// Created by Mathew on 2/15/2018.
//yo its ben.
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// Maximum & Minimum values for truncation and extension (s)
int MIN_TRUNC = 0;
int MAX_TRUNC = 5;
int MIN_EXTE = 0;
int MAX_EXTE = 15;
int CHANGE = 1;
int LOOPS = 1;
double PERCENT_COMPACT = 0.974346885;
double PERCENT_LARGE = 0.02565312;

int DISTANCE = 150;


int * secondsToTime(double seconds){
    static int time[3];

    time[0] = (int) (seconds) / 3600;
    time[1] = (int) (seconds - (time[0] * 3600)) / 60;
    time[2] = (int) (seconds - (time[1] * 60 + time[0] * 3600));

    return time;
}

double average(double seconds){
    return (int) (seconds) / LOOPS;
}

int main() {

    // Input:

    /* We will be Imputing data from an text file, into an intersection array with values for all intersections
     * The array will operate similar to a class.
     * Data will include:
     * number of cars per hour for the 4 different directions in an array
     * Light timing In format TBD.
     *
    */


    int arrivals [4] = {0, 0, 0, 0};
    int extensions[4] = {0, 0, 0, 0};
    int truncations [4] = {0, 0, 0, 0};
    int greenLights [4] = {0, 0, 0, 0};

    int * timeSavedBus;
    int *averageTimeSavedBus;


    int i, j, k, n, hour, maxHours = 24;
    int averageSecondsSavedBus;



    int trafficCount [3][12][4][1];
    int intersections [4] = { };
    int hours [12] = { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    int north [3][24];
    int south [3][24];
    int east [3][24];
    int west [3][24];


    float greenTimes [n];
    float amberTimes [n];
    float allRedTimes [n];
    float redTimes [n];
    float extendGreenTimes [n];
    float speed [4] =  {13.8889, 13.8889, 13.8889, 5.55556};
    float percentGreen [4];
    float percentExtend [4];
    float percentRed [4];
    float totalLightTime[4];
    float truncationTime = 0;
    float extensionTime = 0;
    float secondsSavedOthers;
    float secondsLostOthers;
    float secondsSavedBus;
    float averageSecondsSavedOthers;
    float averageSecondsLostOthers;

    long long compactSudan;
    long long largeSudan;
    long long transitBus;
    long long gasTotal;
    long long dieselTotal;
    long long coTwoTonnes;



    // Scan Traffic count and input number of cars in each direction
    FILE * trafficFile;
    trafficFile = fopen("../trafficCount.txt","r");
    if (trafficFile == NULL){
        printf("Could not open traffic file\n");
    }


    fscanf(trafficFile,"%d",&n);
    printf("%d\n",n);

    for (i = 0; i < n; i++){
        int temp = 0;
        int temp2 = 0;
        fscanf(trafficFile, "%d %d", &temp, &temp2);
        printf("%d %d\n", temp, temp2);
        fscanf(trafficFile," %d %d %d %d",&north[temp][temp2],&south[temp][temp2],&east[temp][temp2],&west[temp][temp2]);
        printf("%d %d %d %d\n", north[temp][temp2], south[temp][temp2], east[temp][temp2], west[temp][temp2]);
    }
    fclose(trafficFile);

    // Scan Intersections and input amount of time for each phase of the light in the buses direction of travel.
    // This is for the direction the bus is traveling (EBL/WBL, E/W, NBL/SBL, N/S)
    // Intersection Green Amber All-Red Red


    FILE * lightFile;
    lightFile = fopen("../Intersections.txt","r");
    if (lightFile == NULL){
        printf("Could not open Light file\n");
    }
    n = 0;
    fscanf(lightFile,"%d",&n);
    printf("%d\n",n);



    for(i = 0; i < n; i++){
        fscanf(lightFile, "%f %f %f %f", &greenTimes[i], &amberTimes[i], &allRedTimes[i], &redTimes[i]);
        printf("%f %f %f %f\n", greenTimes[i], amberTimes[i], allRedTimes[i], redTimes[i]);
    }
    fclose(lightFile);

    // Scan buses file to input how many buses go each hour.
    FILE * busesFile;
    busesFile = fopen("../buses.txt","r");
    if(busesFile == NULL){
        printf("Could not open buses file\n");
    }
    n = 0;
    fscanf(busesFile,"%d",&n);
    printf("%d\n",n);

    int busesPerHour[n];
    for(i = 0; i < n; i++){
        fscanf(busesFile, "%d", &busesPerHour[i]);
        printf("%d\n", busesPerHour[i]);
    }
    fclose(busesFile);

    // Computation

    for(int i = 0; i < 4; i++){

        totalLightTime[i] = greenTimes[i] + amberTimes[i] + allRedTimes[i] + redTimes[i];

        extendGreenTimes[i] = DISTANCE / speed[i];

        if(extendGreenTimes[i] > MAX_EXTE){}
        printf("The bus can't make it to the light when it is extended.");

        percentGreen[i] = (greenTimes[i] - extendGreenTimes[i]) / totalLightTime[i];
        percentExtend[i] = extendGreenTimes[i] / totalLightTime[i];
        percentRed[i] = (redTimes[i] + amberTimes[i] + allRedTimes[i]) / totalLightTime[i];


    }

    srand(time(NULL));


    // Simulate bus loop
    for (int i = 0; i < LOOPS; ++i) {
        for (j = 0; j < 3; j++) {
            for (hour = 0; hour < maxHours; hour++) {
                for (k = 0; k < busesPerHour[hour]; k++) {

                    float r = rand() % 1000;
                    r = r / 1000;
                    //printf("%f\n", r);

                    if (r <= percentExtend[j]) {
                        extensions[j]++;
                    } else if (r <= (percentExtend[j] + percentRed[j])) {
                        truncations[j]++;
                    } else {
                        greenLights[j]++;
                    }
                    arrivals[j]++;
                }
            }
        }
    }



    for(i = 0; i < 3; i++) {
        extensionTime += extensions[i] * (redTimes[i] + amberTimes[i]);
        truncationTime += truncations[i] * MAX_TRUNC;

    }

    /*
     * Calculate total time saved by bus and cars
     * Cars traveling the SAME Direction will save the same amount of time as the bus. And time lost by others is for
     * cars in the oppisite direction of the bus.
     */

    secondsSavedBus = (extensionTime + truncationTime);
    for (int i = 0; i < 3; ++i) {
        for (j = 0; j < 12; j++) {
            secondsSavedOthers += (extensionTime + truncationTime) * east[i][hours[j]] + (extensionTime + truncationTime) * west[i][hours[j]];
            secondsLostOthers += (extensionTime + truncationTime) * north[i][hours[j]] + (extensionTime + truncationTime) * south[i][hours[j]];
        }
    }
    // Calculate used based off of x percent compact sedans, y percent large sedans




    for(i = 0; i < 4; i++) {
        printf("%f\n", percentExtend[i] + percentGreen[i] + percentRed[i]);
    }
    // Output
    for(i = 0; i < 3; i++) {
        printf("The Final values for Intersection # %d are:\n %d Arrivals\n %d Extensions\n %d Truncations\n %d greenLights\n",
               i, arrivals[i], extensions[i], truncations[i], greenLights[i]);
    }

    timeSavedBus = secondsToTime(secondsSavedBus);
    printf("The buses saved a total of %d h %d m %d s = %d s\n", *(timeSavedBus + 0), *(timeSavedBus + 1),
           *(timeSavedBus + 1), (int) (secondsSavedBus));

    // Average Results:
    averageSecondsSavedBus = average(secondsSavedBus);
    averageSecondsSavedOthers = average(secondsSavedOthers);
    averageSecondsLostOthers = average(secondsLostOthers);

    averageTimeSavedBus = secondsToTime(averageSecondsSavedBus);
    printf("The buses saved a total of %d h %d m %d s = %d s\n", *(averageTimeSavedBus + 0), *(averageTimeSavedBus + 0),
           *(averageTimeSavedBus + 0), averageSecondsSavedBus);

    compactSudan = 0.0002020485333 * averageSecondsSavedOthers * PERCENT_COMPACT;
    //time in seconds
    largeSudan = 0.0004100863889 * averageSecondsSavedOthers * PERCENT_LARGE;
    transitBus = 0.001019958056 * averageSecondsSavedBus;
    gasTotal = compactSudan + largeSudan;
    dieselTotal = transitBus;
    coTwoTonnes = ((1/424)*gasTotal) + ((1/333)*dieselTotal);


    return 0;
}




// 1tonne of CO2 e = 424L of gasoline or 333L of diesel


//0.72737472 Litres per hour of idling for compact sedan
//when calculating money saved for city, look at updated gas buddy prices/
// 3.671849 Litres per hour of idling for transit bus
// 1.476311 Litres per hour of idling for large sedan

/*
double compactSudan = 0.0002020485333* timeSavedOther
//time in seconds
largeSudan = 0.0004100863889 * timeSavedOther

transitBus = 0.001019958056 * timeSavedBus
gasTotal = compactSudan + largeSudan
dieselTotal = transitTotal
coTwoTonnes = ((1/424)*gasTotal) + ((1/333)*dieselTotal)
*/













































