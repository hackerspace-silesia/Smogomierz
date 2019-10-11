#ifndef Smogomierz_h
#define Smogomierz_h

void takeTHPMeasurements();
void takeNormalnPMMeasurements();
void takeSleepPMMeasurements();
void sendDataToExternalServices();
void sendDataToExternalDBs();
void averagePM();
void pm_calibration();

#endif