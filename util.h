#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <math.h>
#include <sys/time.h>
#include "common.h"

//Linked list functions
int ll_get_length(LLnode *);
void ll_append_node(LLnode **, void *);
LLnode * ll_pop_node(LLnode **);
void ll_destroy_node(LLnode *);

//Print functions
void print_cmd(Cmd *);

//Time functions
long timeval_usecdiff(struct timeval *, 
                      struct timeval *);

//TODO: Implement these functions
char * convert_frame_to_char(Frame *);
Frame * convert_char_to_frame(char *);

unsigned short int GICREN_GeneratorForTableCRC16 (unsigned char data);//CRC-16生成表函数
unsigned short int GICREN_CalcCRC16 (unsigned char *data, unsigned char len);//返回CRC16校验码

void  create_crc_table(void);//CRC-8生成表函数
unsigned char cal_crc_table(unsigned char *ptr, unsigned char len);//返回CRC8校验码
char *Mystrcat(char *str1,char *str2);//字符串拼接
LLnode ** find_serial(LLnode ** head_ptr,uint16_t serial);//寻找对应序列号的帧

#endif
