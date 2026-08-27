#include <stdio.h>
#include "bms.h"

int main()
{
    Battery battery;
    int choice;

    initializeBattery(&battery);
    calculateSOC(&battery);
    calculateSOH(&battery);

    do
    {
        printf("\n========================================\n");
        printf("        SMART BMS SIMULATOR\n");
        printf("========================================\n");

        printf("1. Display Battery Status\n");
        printf("2. Monitor Cell Voltages\n");
        printf("3. Monitor Temperature\n");
        printf("4. Monitor Current\n");
        printf("5. Calculate SOC\n");
        printf("6. Calculate SOH\n");
        printf("7. Start Charging\n");
        printf("8. Start Discharging\n");
        printf("9. Check Battery Faults\n");
        printf("10. View Fault Log\n");
        printf("11. BMS Dashboard\n");
        printf("12. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                displayBatteryStatus(&battery);
                break;

            case 2:
                 monitorCellVoltage(&battery);
                break;

            case 3:
                monitorTemperature(&battery);
                break;

            case 4:
                monitorCurrent(&battery);
                break;

            case 5:
                printf("\nSOC: %.2f %%\n", calculateSOC(&battery));
                break;

            case 6:
                printf("\nSOH: %.2f %%\n", calculateSOH(&battery));
                break;

            case 7:
                startCharging(&battery);
                break;

            case 8:
                startDischarging(&battery);
                break;

            case 9:
                checkBatteryProtection(&battery);
                break;

            case 10:
                displayFaultLog();
                break;

            case 11:
                displayDashboard(&battery);
                break;

            case 12:
                printf("\nExiting BMS...\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 12);

    return 0;
}