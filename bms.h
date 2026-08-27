#ifndef BMS_H
#define BMS_H

#define TOTAL_CELLS 4

#define MIN_CELL_VOLTAGE 2.8
#define MAX_CELL_VOLTAGE 4.2

#define MIN_TEMPERATURE 0.0
#define MAX_TEMPERATURE 45.0

#define MAX_CURRENT 10.0


typedef struct
{
    float voltage;
    float temperature;

} BatteryCell;


typedef struct
{
    BatteryCell cells[TOTAL_CELLS];

    float current;
    float soc;
    float soh;

    int charging;
    int fault;
    int cycleCount;
    int fullyCharged;

} Battery;


void initializeBattery(Battery *battery);

void displayBatteryStatus(Battery *battery);

void monitorCellVoltage(Battery *battery);
void monitorTemperature(Battery *battery);
void monitorCurrent(Battery *battery);

float calculateSOC(Battery *battery);
float calculateSOH(Battery *battery);

void startCharging(Battery *battery);
void startDischarging(Battery *battery);

void checkBatteryProtection(Battery *battery);

void logFault(const char *message);
void displayFaultLog(void);
void clearFaultLog(void);

void displayDashboard(Battery *battery);

#endif