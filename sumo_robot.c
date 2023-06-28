#include<16F628A.h>
#FUSES XT, NOWDT, PUT, BROWNOUT, NOPROTECT, NOMCLR, NOLVP, NOCPD
//
//
#use fast_io (a)
#use fast_io (b)
#use delay (clock=4000000)
//
#byte porta = 0x05
#byte portb = 0x06
//
INT c,d,e;
long time, dist;
//
short int y,z;
//
#define left_edge_detector  PIN_B6
#define right_edge_detector  PIN_B7
#bit right_motorA = portb.0
#bit right_motorB = portb.1
#bit detector_opponent_right_trigger = porta.0
#bit detector_opponent_right_echo = porta.1
#bit left_motorA = portb.2
#bit left_motorB = portb.3
#bit detector_opponent_left_trigger = porta.2
#bit detector_opponent_left_echo = porta.3
//
void comand_back ()
{
   for (c=0; c<50; c++) {
   left_motorA=1; right_motorA=1;
   left_motorB=0; right_motorB=0;
   delay_ms (15);       //robot detects the line and return after 0.75 s
   }
   return;
}
void linha ()
{
    output_high (left_edge_detector);
    delay_us (50);
    output_float (left_edge_detector);
    y=input (left_edge_detector);
    output_high (right_edge_detector);
    delay_us (50);
    output_float (right_edge_detector);
    z=input (right_edge_detector);
    porta=(0b00000000);
    //pulse for 50 us, Set the pin as input and wait the signal
}
void obstacles_left ()
{
      detector_opponent_left_trigger=1;
      delay_us (10);
      detector_opponent_left_trigger=0;
}
void obstacles_right ()
{
      detector_opponent_right_trigger=1;
      delay_us (10);
      detector_opponent_right_trigger=0;
}
void distance ()
{
      time = get_rtcc ();
      dist = (time*340);   //Distance in m/s; Sound Speed: 340 m/s
      set_rtcc (60);       //restart the counter
}
VOID main ()
{
   delay_ms (5000);
   SET_TRIS_A(0b00001010);
   SET_TRIS_B(0b00000000);
   setup_counters (RTCC_INTERNAL, RTCC_DIV_128);
   
   while (true)
   {
      linha ();
      obstacles_right ();
      obstacles_left ();
      IF (z==0)
      {
      comand_back ();
        for (d=0; d<=10; d++) {
         left_motorA=0; right_motorA=0;
         left_motorB=0; right_motorB=1;
         delay_ms (15);    //after returning, robot turn left, and returns to the center of the arena
         }
      }
      else if (y==0) {
      comand_back ();
         for (e=0; e<=25; e++) {
         left_motorA=0; right_motorA=0;
         left_motorB=1; right_motorB=0;
         delay_ms (15);    //after returning, robot turn right, and returns to the center of the arena
         }
      }
         else if (detector_opponent_right_echo) {
            distance ();
            if (dist<1) {
            left_motorA=0; right_motorA=0;
            left_motorB=1; right_motorB=0;
            }
            }             //turn right in the direction of the opponent
         else if (detector_opponent_left_echo) {
            distance ();
            if (dist<1) {
            left_motorA=0; right_motorA=0;
            left_motorB=0; right_motorB=1;
            }
            }             //turn left in the direction of the opponent
      else {
      left_motorA=0; right_motorA=0;
      left_motorB=1; right_motorB=1;
      }
      }                   //moving straight
   }

