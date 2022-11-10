#ifndef MAIN_H_
#define MAIN_H

#define GLY_DATA_SIZE   23
#define GLY_HOURS_SIZE  63
#define EXEC            1
#define NO_EXEC         0
#define TOL_ANALOG      50
#define GLY_MAX_LIM     180
#define GLY_MIN_LIM     80
#define GLY_NO_SENSOR   30

#define ALERT_NO        0
#define ALERT_HIPO      1
#define ALERT_HIPER     2
#define ALERT_NO_SENSOR 3

#define S0              A14
#define S1              A12
#define A_BOARD         A9
#define E0              45

#define RX              7
#define TX              6
#define DEVICE_ID       "010203"

#define A               0.0007829
#define B               4.949e-6
#define C               0.000778
#define D               0.3365
#define E               0.6237
#define Ub              0.0275
#define K               0.00002

#define GLY_CONV            0.9775
#define CONV_ANAL_GLY(X)   {X*GLY_CONV - 111} 



#endif //MAIN_H_