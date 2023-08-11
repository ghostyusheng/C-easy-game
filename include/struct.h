/*----------------------------------------------------------------------------*/
// File: struct.h
// Describe:
//   Define all the c struct including the ball(gift), Role(santa), Global(G)
/*----------------------------------------------------------------------------*/

typedef struct
{
    double x_old, y_old;
    double x_new, y_new;
    double vx, vy;
    uint8_t color;
    BITMAP img;
} Ball;

/* make a data type hold position and velocity */
typedef struct
{
    double    pos;
    double    vel;
} posvel;

typedef struct
{
    BITMAP img;
    int x;
    int y;
} Role;


typedef struct // used to save game score and level
{
    int score;
    int level;
} Global;
typedef Global G;
