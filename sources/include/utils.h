
/* Here are some useful conversions
 * UA: 149 597 870 km
 * al: 9 460 700 000 000 km
 * Pc: 3.26 al = 30856775814672000 m
 */

#define KM2UA 149597870.0
#define KM2LY 9460700000000.0
#define KM2PC 30856775814672.0
#define KM2KLY 9460700000000000.0
#define KM2MLY 9460700000000000000.0
#define LY2PC 3.26

double convert_m_to_ly(double km);

double convert_m_to_kly(double km);

double convert_m_to_mly(double km);

double convert_m_to_au(double km);
