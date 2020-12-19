#include<iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;
// #define FRAME_PAYLOAD_SIZE 5
// char *Mystrcat(char *str1,char *str2)
// {
// 	char *outcome=str1;
// 	while(*str1)str1++;
// 	while(*str1++=*str2++);
// 	return outcome;
// }

// int main()
// {
// 	// char str1[101] = "asdad";
//     // char str2[50] = "asdfgg";
    
//     // strcat(str1, str2);
//     // char data[FRAME_PAYLOAD_SIZE];
//     // data[0] = 'a';
//     // data[1] = 'b';
//     // char * char_buffer = (char *) malloc(10);
//     // char_buffer[0] = 'w';
//     // memcpy(char_buffer+1, 
//     //        data,
//     //        FRAME_PAYLOAD_SIZE);
//     // cout<<char_buffer<<endl;


//     return 0;

//  } 

#define MAX_FRAME_SIZE 48

//TODO: You should change this!
//Remember, your frame can be AT MOST 48 bytes!
#define FRAME_PAYLOAD_SIZE 48-5-6
struct Frame_t
{
    char data[FRAME_PAYLOAD_SIZE];
    uint16_t ack;
    unsigned char CRC8;
    uint16_t serial;
    uint16_t send;
    uint16_t receiver;
    friend bool operator < (Frame_t f1, Frame_t f2){//重新定义bool类型操作符‘<’，
                                //‘<’的两边分别是fruit类型的f1和f2
        return f1.serial < f2.serial;//操作符返回f1.price是否小鱼f2.price
    }

};
typedef struct Frame_t Frame;

char * convert_frame_to_char(Frame * frame)
{
    //TODO: You should implement this as necessary
    char * char_buffer = (char *) malloc(MAX_FRAME_SIZE);
    memset(char_buffer,
           0,
           MAX_FRAME_SIZE);
    
    char send = (char)frame->send;
    char receiver = (char)frame->receiver;
    char CRC8 = (char)frame->CRC8;
    char serial = (char)frame->serial;
    char ack = (char)frame->ack;

    int i = 0;
    // char_buffer[0] = send;
    // char_buffer[1] = '|';
    // char_buffer[2] = receiver;
    // char_buffer[3] = '|';
    // char_buffer[4] = CRC8;
    // char_buffer[5] = '|';
    // char_buffer[6] = serial;
    // char_buffer[7] = '|';
    // char_buffer[8] = ack;
    // char_buffer[9] = '|';

    if(frame->send !=0)
    {
        char_buffer[i++] = send;
    }
    char_buffer[i++] = '|';

    if(frame->receiver !=0)
    {
        char_buffer[i++] = receiver;
    }
    char_buffer[i++] = '|';

    if(frame->CRC8 !=0)
    {
        char_buffer[i++] = CRC8;
    }
    char_buffer[i++] = '|';

    if(frame->serial !=0)
    {
        char_buffer[i++] = serial;
    }
    char_buffer[i++] = '|';

    if(frame->ack !=0)
    {
        char_buffer[i++] = ack;
    }
    char_buffer[i++] = '|';

    memcpy(char_buffer+i, 
           frame->data,
           FRAME_PAYLOAD_SIZE);
    return char_buffer;
}

Frame * convert_char_to_frame(char * char_buf)
{
    //TODO: You should implement this as necessary
    Frame * frame = (Frame *) malloc(sizeof(Frame));

    char *p;
    char *buff;
    buff=char_buf;
    p = strsep(&buff, "|");
    int i=0;
    char temp;
    while(i<5)
    {
        if(i==0){temp = *p;frame->send = (uint16_t)temp;}
        else if(i==1){temp = *p;frame->receiver = (uint16_t)temp;}
        else if(i==2){frame->CRC8 = (unsigned char)*p;}
        else if(i==3){temp = *p;frame->serial = (uint16_t)temp;}
        else if(i==4){temp = *p;frame->ack = (uint16_t)temp;}
        p = strsep(&buff, "|");
        i++;
    }
    memset(frame->data,
           0,
           sizeof(char)*sizeof(frame->data));
    memcpy(frame->data, 
           p,
           sizeof(char)*sizeof(frame->data));
    return frame;
}


int main()
{
    Frame * f = (Frame *) malloc (sizeof(Frame));

    memcpy(f->data, 
           "frame->data",
           FRAME_PAYLOAD_SIZE);
    f->ack = 0;
    f->CRC8 = 68;
    f->serial = 69;
    f->send = 70;
    f->receiver = 71;


    char* temp = convert_frame_to_char(f);
    cout<<temp<<endl;

    Frame * f2 = convert_char_to_frame(temp);

    cout<<f2->data<<endl;
    cout<<f2->ack<<endl;
    cout<<f2->CRC8<<endl;
    cout<<f2->serial<<endl;
    cout<<f2->send<<endl;
    cout<<f2->receiver<<endl;
    

}