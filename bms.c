#include <stdio.h>
#include "bms.h"
#include <unistd.h>

void initializeBattery(Battery *battery)
{
    for (int i = 0; i < TOTAL_CELLS; i++)
    {
        battery->cells[i].voltage = 5.0;
        battery->cells[i].temperature = 25.0;
    }

    battery->current = 0.0;
    battery->soc = 0.0;
    battery->soh = 100.0;

    battery->charging = 0;
    battery->fault = 0;

    battery->cycleCount = 0;
    battery->fullyCharged = 0;
}

void displayBatteryStatus(Battery *battery)
{
    float packVoltage = 0.0;

    printf("\n*****************************************\n");
    printf("          BATTERY STATUS\n");
    printf("*****************************************\n");

    for (int i = 0; i < TOTAL_CELLS; i++)
    {
        printf("Cell %d : %.2f V | %.2f C\n",
               i + 1,
               battery->cells[i].voltage,
               battery->cells[i].temperature);

        packVoltage += battery->cells[i].voltage;
    }

    printf("----------------------------------------\n");
    printf("Pack Voltage : %.2f V\n", packVoltage);
    printf("Current      : %.2f A\n", battery->current);
    printf("SOC          : %.2f %%\n", battery->soc);
    printf("SOH          : %.2f %%\n", battery->soh);

    if (battery->fault == 0)
    {
        printf("Status       : NORMAL\n");
    }
    else
    {
        printf("Status       : FAULT DETECTED\n");
    }

    printf("**************************************\n");
}

void monitorCellVoltage(Battery *battery)
{
    int faultFound = 0;

    printf("\n**************************************\n");
    printf("        CELL VOLTAGE MONITOR\n");
    printf("**************************************\n");

    for (int i = 0; i < TOTAL_CELLS; i++)
    {
        float voltage = battery->cells[i].voltage;

        printf("Cell %d : %.2f V", i + 1, voltage);

        if (voltage < MIN_CELL_VOLTAGE)
        {
            printf(" -> UNDERVOLTAGE\n");
            faultFound = 1;
        }
        else if (voltage > MAX_CELL_VOLTAGE)
        {
            printf(" -> OVERVOLTAGE\n");
            faultFound = 1;
        }
        else
        {
            printf(" -> NORMAL\n");
        }
    }

    if (faultFound)
    {
        battery->fault = 1;
        printf("\nWARNING: Cell voltage fault detected!\n");
    }
    else
    {
        printf("\nAll cell voltages are within safe limits.\n");
    }

    printf("**************************************\n");
}

void monitorTemperature(Battery *battery)
{
    int faultFound = 0;

    printf("\n****************************************\n");
    printf("        TEMPERATURE MONITOR\n");
    printf("**************************************\n");

    for (int i = 0; i < TOTAL_CELLS; i++)
    {
        float temperature = battery->cells[i].temperature;

        printf("Cell %d : %.2f C", i + 1, temperature);

        if (temperature < MIN_TEMPERATURE)
        {
            printf(" -> LOW TEMPERATURE\n");
            faultFound = 1;
        }
        else if (temperature > MAX_TEMPERATURE)
        {
            printf(" -> OVERTEMPERATURE\n");
            faultFound = 1;
        }
        else
        {
            printf(" -> NORMAL\n");
        }
    }

    if (faultFound)
    {
        battery->fault = 1;
        printf("\nWARNING: Temperature fault detected!\n");
    }
    else
    {
        printf("\nAll cell temperatures are within safe limits.\n");
    }

    printf("**************************************\n");
}

void monitorCurrent(Battery *battery)
{
    printf("\n**************************************\n");
    printf("          CURRENT MONITOR\n");
    printf("**************************************\n");

    printf("Current : %.2f A\n", battery->current);

    if (battery->current > MAX_CURRENT)
    {
        battery->fault = 1;

        printf("Status  : OVERCURRENT\n");
        printf("WARNING : Current exceeds safe limit!\n");
    }
    else
    {
        printf("Status  : NORMAL\n");
        printf("Current is within safe limits.\n");
    }

    printf("**************************************\n");
}

float calculateSOC(Battery *battery)
{
    float totalVoltage = 0.0;
    float averageVoltage;

    for (int i = 0; i < TOTAL_CELLS; i++)
    {
        totalVoltage += battery->cells[i].voltage;
    }

    averageVoltage = totalVoltage / TOTAL_CELLS;

    battery->soc = ((averageVoltage - MIN_CELL_VOLTAGE) /
                    (MAX_CELL_VOLTAGE - MIN_CELL_VOLTAGE)) * 100.0;

    if (battery->soc > 100.0)
    {
        battery->soc = 100.0;
    }

    if (battery->soc < 0.0)
    {
        battery->soc = 0.0;
    }

    return battery->soc;
}

float calculateSOH(Battery *battery)
{
    float degradation;

    degradation = battery->cycleCount * 0.02;

    battery->soh = 100.0 - degradation;

    if (battery->soh < 0.0)
    {
        battery->soh = 0.0;
    }

    return battery->soh;
}

void startCharging(Battery *battery)
{
    if (battery->soc >= 100.0)
    {
        printf("\nBattery is already fully charged.\n");
        return;
    }

    battery->charging = 1;
    battery->current = 5.0;

    printf("\n**************************************\n");
    printf("           CHARGING BATTERY\n");
    printf("**************************************\n");

    printf("Charging started...\n");

    while (battery->soc < 100.0)
    {
        battery->soc += 5.0;

        if (battery->soc > 100.0)
        {
            battery->soc = 100.0;
        }

        printf("\rCharging... SOC : %.2f %%", battery->soc);

        fflush(stdout);
        usleep(500000);
    }

    battery->current = 0.0;
    battery->charging = 0;
    battery->fullyCharged = 1;

    printf("\n");
    printf("\nBattery fully charged.\n");
    printf("Charging stopped.\n");

    printf("**************************************\n");
}

void startDischarging(Battery *battery)
{
    if (battery->soc <= 0.0)
    {
        printf("\nBattery is already fully discharged.\n");
        return;
    }

    battery->charging = 0;
    battery->current = -5.0;

    printf("\n**************************************\n");
    printf("        DISCHARGING BATTERY\n");
    printf("**************************************\n");

    printf("Discharging started...\n");
    printf("Discharging Current : %.2f A\n", battery->current);

    while (battery->soc > 0.0)
    {
        battery->soc -= 5.0;

        if (battery->soc < 0.0)
        {
            battery->soc = 0.0;
        }

        for (int i = 0; i < TOTAL_CELLS; i++)
        {
            battery->cells[i].voltage -= 0.07;

            if (battery->cells[i].voltage < MIN_CELL_VOLTAGE)
            {
                battery->cells[i].voltage = MIN_CELL_VOLTAGE;
            }

            battery->cells[i].temperature += 0.3;

            if (battery->cells[i].temperature > MAX_TEMPERATURE)
            {
                battery->cells[i].temperature = MAX_TEMPERATURE;
            }
        }

        printf("\rDischarging... SOC : %.2f %%", battery->soc);

        fflush(stdout);
        usleep(500000);
    }

    battery->current = 0.0;

    printf("\nDEBUG: fullyCharged = %d\n", battery->fullyCharged);

    /* Check for a complete charge-discharge cycle */
    if (battery->fullyCharged == 1)
    {
        battery->cycleCount++;

        battery->fullyCharged = 0;

        calculateSOH(battery);

        printf("\n\nCycle completed!");
        printf("\nCycle Count : %d", battery->cycleCount);
        printf("\nSOH         : %.2f %%\n", battery->soh);
    }

    printf("\nBattery fully discharged.\n");
    printf("Discharging stopped.\n");

    printf("**************************************\n");
}

void checkBatteryProtection(Battery *battery)
{
    int faultFound = 0;

    printf("\n**************************************\n");
    printf("          BMS PROTECTION CHECK\n");
    printf("**************************************\n");

    // Voltage check
    for (int i = 0; i < TOTAL_CELLS; i++)
    {
        if (battery->cells[i].voltage < MIN_CELL_VOLTAGE)
        {
            char fault[100];

            printf("Cell %d : UNDERVOLTAGE\n", i + 1);

            sprintf(fault, "Cell %d undervoltage detected", i + 1);
            logFault(fault);

            faultFound = 1;
        }
        else if (battery->cells[i].voltage > MAX_CELL_VOLTAGE)
        {
            char fault[100];

            printf("Cell %d : OVERVOLTAGE\n", i + 1);

            sprintf(fault, "Cell %d overvoltage detected", i + 1);
            logFault(fault);

            faultFound = 1;
        }
    }

    // Temperature check
    for (int i = 0; i < TOTAL_CELLS; i++)
    {
        if (battery->cells[i].temperature < MIN_TEMPERATURE)
        {
            char fault[100];

            printf("Cell %d : LOW TEMPERATURE\n", i + 1);

            sprintf(fault, "Cell %d low temperature detected", i + 1);
            logFault(fault);

            faultFound = 1;
        }
        else if (battery->cells[i].temperature > MAX_TEMPERATURE)
        {
            char fault[100];

            printf("Cell %d : OVERTEMPERATURE\n", i + 1);

            sprintf(fault, "Cell %d overtemperature detected", i + 1);
            logFault(fault);

            faultFound = 1;
        }
    }

    // Current check
    if (battery->current > MAX_CURRENT)
    {
        printf("Current : OVERCURRENT\n");

        logFault("Overcurrent detected");

        faultFound = 1;
    }

    if (faultFound)
    {
        battery->fault = 1;

        printf("----------------------------------------\n");
        printf("BMS STATUS : FAULT DETECTED\n");
        printf("Protection system activated.\n");
    }
    else
    {
        battery->fault = 0;

        printf("----------------------------------------\n");
        printf("BMS STATUS : SAFE\n");
        printf("All protection checks passed.\n");
    }

    printf("**************************************\n");
}

void displayDashboard(Battery *battery)
{
    float packVoltage = 0.0;
    float averageTemperature = 0.0;

    for (int i = 0; i < TOTAL_CELLS; i++)
    {
        packVoltage += battery->cells[i].voltage;
        averageTemperature += battery->cells[i].temperature;
    }

    averageTemperature = averageTemperature / TOTAL_CELLS;

    printf("\n**************************************\n");
    printf("             BMS DASHBOARD\n");
    printf("*****************************************\n");

    for (int i = 0; i < TOTAL_CELLS; i++)
    {
        printf("Cell %d Voltage     : %.2f V\n",
               i + 1,
               battery->cells[i].voltage);
    }

    printf("----------------------------------------\n");
    printf("Pack Voltage       : %.2f V\n", packVoltage);
    printf("Current            : %.2f A\n", battery->current);
    printf("Temperature        : %.2f C\n", averageTemperature);
    printf("SOC                : %.2f %%\n", battery->soc);
    printf("SOH                : %.2f %%\n", battery->soh);
    printf("Cycle Count        : %d\n", battery->cycleCount);

    if (battery->charging)
    {
        printf("Charging           : YES\n");
    }
    else
    {
        printf("Charging           : NO\n");
    }

    if (battery->fault)
    {
        printf("Battery Status     : FAULT\n");
    }
    else
    {
        printf("Battery Status     : SAFE\n");
    }

    printf("**************************************\n");
}

