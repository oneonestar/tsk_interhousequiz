#ifndef _UTILITIES_H_
#define _UTILITIES_H_

/* a function to send a message to whatever destination through socket */
int send_message(char*, char*, char*);
int house_to_char(int);
int char_to_house(char);

#endif
