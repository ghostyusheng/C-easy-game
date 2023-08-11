#include <graphics_lib.h>
#include <amio.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <struct.h>
#include <global.h>
#include <motion.h>

/*----------------------------------------------------------------------------*/
// Function: initAudio
// Params: void
// Return: void
// Describe:
//   Pre-loading different audio resource, and set audio play loop.
/*----------------------------------------------------------------------------*/
void initAudio() {
    amio_init_audio();
    amio_load_sample("main", "./samples/bg.wav");
    amio_load_sample("spectra", "./samples/collide.wav");
    amio_load_sample("oye", "./samples/oye.wav");
    amio_load_sample("laugh", "./samples/laugh.wav");
    amio_add_sample_instance("main", PLAY_LOOP, 1.0);
}

/*----------------------------------------------------------------------------*/
// Function: initBallsAttribute
// Params: Ball balls[]
// Return: void
// Describe:
//   Initing C struct balls' attributes including colors(use 0~4 to represent),coordinate-x,coordinate-y,
//   coordinate-old-x,coordinate-old-y
/*----------------------------------------------------------------------------*/
void initBallsAttribute(Ball balls[]) {
    for(int i = 0; i < MAX_NUMBER_BALLS; i++)
    {
        balls[i].color = (uint8_t) rand_number(1, NUMBER_COLORS-1) % 5;
        balls[i].vx = (double) (rand_number(MIN_VEL, MAX_VEL) * generate_sign());
        balls[i].vy = 1;
        balls[i].x_old = 0;
        balls[i].y_old = (double) rand_number(YMIN, YMAX) / 2.2;
    }
}

/*----------------------------------------------------------------------------*/
// Function: drawGift
// Params: Ball ball, BITMAP gifts[]
// Return: void
// Describe:
//   Drawing the canvas frames with gifts' motion.
/*----------------------------------------------------------------------------*/
void drawGift(Ball ball, BITMAP gifts[])
{

    draw_bitmap(gifts[ball.color], (int)ball.x_new, (int)ball.y_new);
}

/*----------------------------------------------------------------------------*/
// Function: initMenu
// Params: void
// Return: void
// Describe:
//   Init the games' menu
/*---------------------------------.------------------------------------------*/
void initMenu() {
    initwindow(XWINDOW_MENU, YWINDOW_MENU); // set window size
    create_event_queue();
    initkeyboard();
    reg_display_events();// register callback function and async display function
    reg_keyboard_events();// register callback function and async keyboard function
    initfont();
    BITMAP bkg_img = load_bitmap("./assets/img/menu.png");
    char text[200] = {};
    char text2[200] = {};
    do {
        draw_bitmap(bkg_img, (int)XWINDOW_MENU/2, (int)YWINDOW_MENU/2); // draw menu backgrouds
        outtextxy(100,150,"Press q to start game"); // draw some game guide texts
        outtextxy(100,170,"Press w check game rules");
        outtextxy(100,190,"Press e check author info");
        outtextxy(100,220,text);
        outtextxy(100,240,text2);
        update_display();
        pausefor(50);
        if (check_if_event()) // periodically check event loop to check if there is a new event
        {
            wait_for_event();
            if (event_key_down()) // detect keyboard event
            {
                if(event_key('q')) { // press 'q' to start game
                    printf("start game\n");
                    event_close_display();
                    closekeyboard();
                    closegraph();
                    break;
                } else if (event_key('w')) {
                    sprintf(text, "1.[Catch Gift]If catch santa,gain 5 more marks.Otherwise,gain 1");
                    sprintf(text2, "2.Every 2 marks level up 1 difficulty(speed up,more ball)");
                } else if (event_key('e')) {
                    sprintf(text, "Yusheng Zhang, Exam No: Y3915588, Major: Robotics");
                    sprintf(text2, "");
                }
            }
        }
    } while(1);

}

/*----------------------------------------------------------------------------*/
// Function: main
// Params: void
// Return: int
// Describe:
//   Game application entrance, responsible for init image resource, some handler and c struct.
/*---------------------------------.------------------------------------------*/
int main(void)
{
    int number_of_balls = 1; // initing balls count
    int count=0;
    G g = {0, 1}; // G is equal to struct Global, G is abbreviation for quick coding.
    Ball balls[MAX_NUMBER_BALLS];
    initBallsAttribute(balls);
    initAudio();
    initMenu();

    initwindow(XWINDOW, YWINDOW);
    BITMAP bkg_img = load_bitmap("./assets/img/bg3.png");
    BITMAP role_img = load_bitmap("./assets/img/role.png");
    BITMAP gifts[5] = { // initing a array of gift resources
        load_bitmap("./assets/img/gift2/gift_0.png"),
        load_bitmap("./assets/img/gift2/gift_1.png"),
        load_bitmap("./assets/img/gift2/gift_2.png"),
        load_bitmap("./assets/img/gift2/gift_3.png"),
        load_bitmap("./assets/img/gift2/gift_4.png"),
    };
    Role role = {role_img, 75, (int)YWINDOW - 75}; // init the santa(stick man) role, images' radius is 75(75 in case overflow the screen)
    initmouse();
    initkeyboard();
    create_event_queue();
    reg_display_events();
    reg_mouse_events();
    reg_keyboard_events();
    initfont();
    int x = 130;
    int y = YWINDOW - 120;
    int diff = 0;
    int *pdiff = &diff;

    do
    {
        setcolor(WHITE);
        draw_bitmap(bkg_img, (int)XWINDOW/2, (int)YWINDOW/2);
        draw_bitmap(role.img, role.x, role.y);
        outtextxy(5,80,"Press 'q' to quit. Press 'p' to pause. Left/right click to add/delete gift.");
        outtextxy(5,100,"Press a(A)/d(D) to move santa.");
        char str[80];
        sprintf(str, "Level: %d  Score: %d", g.level, g.score);
        outtextxy(5, 140, str);
        setcolor(BROWN);
        circle(x + *pdiff, y + 67, 13, 1.5);
        line(x + *pdiff, y + 80 , x + *pdiff, y + 100 , 1);
        line(x -30 + *pdiff, y +70, x + *pdiff, y +100, 1);
        line(x + *pdiff, y +100, x +30 + *pdiff, y +70, 1);
        line(x + *pdiff, y +100, x +30 + *pdiff, y +140, 1);
        line(x -30 + *pdiff, y +140, x + *pdiff, y +100, 1);

        for(int i = 0; i < number_of_balls; i++) // in every frames, update gifts' position
        {
            ball_update_new_pos(&balls[i]);
            ball_hit_boundary(&balls[i], &role, &number_of_balls, &g); // collidation
            drawGift(balls[i], gifts);
            ball_update_old_pos(&balls[i]);

        }
        update_display();
        amio_update_audio();
        count++;
        pausefor(10); /* wait 8 miliseconds */
        if (check_if_event())
        {
            wait_for_event();
            if (event_close_display())
            {
                break;
            }
            else if (event_mouse_button_down()) // left click add a new gift
            {
                if (event_mouse_left_button_down())
                {

                    if (number_of_balls < MAX_NUMBER_BALLS)
                    {
                        number_of_balls++;
                    }
                }
                else if (event_mouse_right_button_down()) // right click delete a gift
                {
                    if (number_of_balls > 0)
                    {
                        number_of_balls = number_of_balls - 1;
                    }
                }
            }
            else if (event_key_down())
            {
                controlRole(&role, &diff); // use keyboard('a', 'd') to control the santa
                if  (event_key('q'))
                {
                    break;
                }

                if (event_key('p'))
                {
                    bool pause = true;
                    while(pause)
                    {
                        wait_for_event();
                        if (event_key_up() && event_key('p'))pause = false;
                    }
                }
            }
        }
    }
    while (count < MAXCOUNT);
    amio_destroy_audio(); // release all the opened resources/hanlders
    closemouse();
    closekeyboard();
    closegraph();

    return 0;
}

