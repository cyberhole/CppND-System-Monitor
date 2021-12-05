#include <string>
#include "format.h"
using std::string;
//Everything here is done

string Format::ElapsedTime(long seconds) { 
    /*Accepts long as input returns a string;
      Converts seconds into hours:minutes:seconds
    */
   //Conversion of the seconds into the different "fields"
int hours_=seconds/3600;
int minutes_=(seconds/60-hours_*60);
int seconds_=(seconds-minutes_*60-hours_*3600);
    //Creation of the string to output the time
std::string time=convertTime(hours_)+":"+convertTime(minutes_)+":"+convertTime(seconds_);
return time;}

string Format::convertTime(int number){
    /*Function accepts an int and returns a string.
    Verifies if the int is bigger than 10. If it isn't it adds a 0 on the left. 
    To format the output of the time on the elapsed time function.
    */
std::string number_; 
if (number>=10)
number_=std::to_string(number);
else 
number_="0"+std::to_string(number);
return number_;
}