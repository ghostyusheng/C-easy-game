/*----------------------------------------------------------------------------*/
// File: motion.h
// Describe:
//   Responsible for ball(gift) motion including collide, move.
/*----------------------------------------------------------------------------*/

posvel hit_boundary(double pos, double vel, int isx);

/* handle what happens to balls at boundaries
   of the graphics window */
posvel hit_boundary(double pos, double vel, int isx)
{
    posvel z;

    z.pos = pos;
    z.vel = vel;

    if (isx)
    {
        if (pos < XMIN)
        {
            z.pos = XMIN;
            z.vel = -vel;
        }
        else if (pos > XMAX)
        {
            z.pos = XMAX;
            z.vel = -vel;
        }
    }
    else
    {
        if (pos < YMIN)
        {
            z.pos = YMIN;
            z.vel = -vel;
        }

        if (pos > YMAX)
        {
            z.pos = 0;
            z.vel = vel;
        }
    }

    return z;
}

void ball_update_new_pos(Ball *ball)
{
    /* calculate new ball positions */
    ball->x_new = ball->x_old + ball->vx*TICK;
    ball->y_new = ball->y_old + ball->vy*TICK;

}

void ball_update_old_pos(Ball *ball)
{
    /* update old ball positions */
    ball->x_old = ball->x_new;
    ball->y_old = ball->y_new;
}

/*----------------------------------------------------------------------------*/
// Function: ball_hit_boundary
// Params: Ball * ball, Role * role, int * number_of_balls, G * g
// Return: void
// Describe:
//   Check if the ball/gift have collided with the santa
/*----------------------------------------------------------------------------*/
void ball_hit_boundary(Ball * ball, Role * role, int * number_of_balls, G * g)
{
    if (ball->x_new > (role->x - 75) && ball->x_new < (role->x + 75) && ball->y_new > (role->y - 75)) // Use superposition of coordinates to judge colide
    {
        printf("collide \n");
        amio_add_sample_instance("spectra", PLAY_ONCE, 1.0);
        ball->x_new = 0;
        ball->y_new = (double) rand_number(YMIN, YMAX) / 3;
        ball->vy = rand_number(1,3);
        printf("color: %d \n", ball->color);
        if (ball->color == 3) { // ball/gift type 3, get 5 more marks, and play special laugh audio
            printf("yummy! \n");
            amio_add_sample_instance("laugh", PLAY_ONCE, 1.0);
            g->score += 5;
        } else { // normally , a gift get 1 mark
            g->score += 1;
        }

        if (g->score > 0 && (g->score % 2 == 0)) { // get 2 more marks, level up(more difficult), speed up, add more balls
            printf("level up, speed up, oye! \n");
            amio_add_sample_instance("oye", PLAY_ONCE, 1.0);
            g->level += 1;
            ball->vy += 1;
            if ((*number_of_balls < MAX_NUMBER_BALLS)) {
               *number_of_balls += 1;
            }
        }
    }

    posvel pv;

    pv = hit_boundary(ball->x_new, ball->vx, 1);

    ball->x_new = pv.pos;
    ball->vx = pv.vel;

    pv = hit_boundary(ball->y_new, ball->vy, 0);

    ball->y_new = pv.pos;
    ball->vy = pv.vel;
}

int generate_sign(void)
{

    if(rand_number(0, 1) == 0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

/*----------------------------------------------------------------------------*/
// Function: XboardControl
// Params: int x
// Return: void
// Describe:
//   Check if the santa's position exceed the window, if exceed , reset position
/*----------------------------------------------------------------------------*/
int XboardControl(int x) {
    if (x < 0) {
        return 75;
    } else if (x > XWINDOW) {
        return XWINDOW-75;
    }
}


/*----------------------------------------------------------------------------*/
// Function: controlRole
// Params: Role *role
// Return: void
// Describe:
//   use keyboard 'a','d' to control the motion of santa
/*----------------------------------------------------------------------------*/
void controlRole(Role *role, int *pdiff)
{

    if  (event_key('d'))
    {
        role->x += 50;
        *pdiff += 50;
        role->x = XboardControl(role->x);
        *pdiff = XboardControl(role->x);
        draw_bitmap(role->img, role->x, role->y);
    } else if (event_key('a'))
    {
        role->x -= 50;
        *pdiff -= 50;
        role->x = XboardControl(role->x);
        *pdiff = XboardControl(role->x);
        draw_bitmap(role->img, role->x, role->y);
    }

}
