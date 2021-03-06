#include "util.h"

//Linked list functions
int ll_get_length(LLnode * head)
{
    LLnode * tmp;
    int count = 1;
    if (head == NULL)
        return 0;
    else
    {
        tmp = head->next;
        while (tmp != head)
        {
            count++;
            tmp = tmp->next;
        }
        return count;
    }
}

void ll_append_node(LLnode ** head_ptr, 
                    void * value)
{
    LLnode * prev_last_node;
    LLnode * new_node;
    LLnode * head;

    if (head_ptr == NULL)
    {
        return;
    }
    
    //Init the value pntr
    head = (*head_ptr);
    new_node = (LLnode *) malloc(sizeof(LLnode));
    new_node->value = value;

    new_node->mask = 0;//添加

    //The list is empty, no node is currently present
    if (head == NULL)
    {
        (*head_ptr) = new_node;
        new_node->prev = new_node;
        new_node->next = new_node;
    }
    else
    {
        //Node exists by itself
        prev_last_node = head->prev;
        head->prev = new_node;
        prev_last_node->next = new_node;
        new_node->next = head;
        new_node->prev = prev_last_node;
    }
}


LLnode * ll_pop_node(LLnode ** head_ptr)
{
    LLnode * last_node;
    LLnode * new_head;
    LLnode * prev_head;

    prev_head = (*head_ptr);
    if (prev_head == NULL)
    {
        return NULL;
    }
    last_node = prev_head->prev;
    new_head = prev_head->next;

    //We are about to set the head ptr to nothing because there is only one thing in list
    if (last_node == prev_head)
    {
        (*head_ptr) = NULL;
        prev_head->next = NULL;
        prev_head->prev = NULL;
        return prev_head;
    }
    else
    {
        (*head_ptr) = new_head;
        last_node->next = new_head;
        new_head->prev = last_node;

        prev_head->next = NULL;
        prev_head->prev = NULL;
        return prev_head;
    }
}

//添加

LLnode ** find_serial(LLnode ** head_ptr,uint16_t serial)
{
    LLnode * head;

    if (head_ptr == NULL)
    {
        return NULL;
    }
    
    //Init the value pntr
    head = (*head_ptr);

    //The list is empty, no node is currently present
    if (head == NULL)
    {
        return NULL;
    }
    else
    {
        Frame* frame = (Frame *)head->value;
        if(frame->serial == serial)
        {   
            return head_ptr;
        }
        LLnode * tmp;
        tmp = head->next;
        while (tmp != head)
        {
            frame = (Frame *)tmp->value;
            if(frame->serial == serial)
            {   
                return &tmp;
            }
            tmp = tmp->next;
        }
    }
}

//添加

void ll_destroy_node(LLnode * node)
{
    if (node->type == llt_string)
    {
        free((char *) node->value);
    }
    free(node);
}

//Compute the difference in usec for two timeval objects
long timeval_usecdiff(struct timeval *start_time, 
                      struct timeval *finish_time)
{
  long usec;
  usec=(finish_time->tv_sec - start_time->tv_sec)*1000000;
  usec+=(finish_time->tv_usec- start_time->tv_usec);
  return usec;
}


//Print out messages entered by the user
void print_cmd(Cmd * cmd)
{
    fprintf(stderr, "src=%d, dst=%d, message=%s\n", 
           cmd->src_id,
           cmd->dst_id,
           cmd->message);
}


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

//CRC16查询表生成
unsigned short int GICREN_GeneratorForTableCRC16 (unsigned char data)
{
    unsigned char i;
    unsigned short int Temp = ((unsigned short int)data) << 8;  //源数据流后先补8个0，下方的循环语句等效于源数据流后再补8个0
    for(i = 0; i < 8; i++)  
    {
        if(Temp & 0x8000)
      	    Temp = (Temp << 1) ^ 0x8005;			//1000000000000101B，即CRC生成多项式系数的简式
    	else
      	    Temp <<= 1;
    }
    return Temp;         					//CRC终值
}

// CRC-16/GICREN对应的查询表，共256个。
unsigned short int GICREN_TableCRC16[] = {
	0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011, 0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022, 
	0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072, 0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041, 
	0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2, 0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1, 
	0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1, 0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082, 
	0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192, 0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1, 
	0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1, 0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2, 
	0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151, 0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162, 
	0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132, 0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101, 
	0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312, 0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321, 
	0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371, 0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342, 
	0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1, 0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2, 
	0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2, 0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381, 
	0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291, 0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2, 
	0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2, 0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1, 
	0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252, 0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261, 
	0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231, 0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202};

// *data ：待校验数据的起始地址
// len	 ：待校验数据的长度
unsigned short int GICREN_CalcCRC16 (unsigned char *data, unsigned char len)
{
    unsigned char Index;
    unsigned short int Init = 0xffff;			//1111111111111111B，即CRC初值
    while(len--)	
    {
        Index = (unsigned char)(Init >> 8) ^ (*(data++));	
	    Init = GICREN_TableCRC16[Index] ^ (Init << 8);  //相异或的前后两项分别为式1-1中背景色为白色的项和背景色为蓝色的项
    }
    return Init;					//CRC终值
}

void  create_crc_table(void)
{
    unsigned short i;
    unsigned char j;

    for (i=0; i<=0xFF; i++)
    {
        if (0 == (i%16))
            printf("\n");

        j = i&0xFF;
        printf("0x%.2x, ", cal_table_high_first (j));  /*依次计算每个字节的crc校验值*/
    }
}

static const unsigned char crc_table[] =
{
    0x00,0x31,0x62,0x53,0xc4,0xf5,0xa6,0x97,0xb9,0x88,0xdb,0xea,0x7d,0x4c,0x1f,0x2e,
    0x43,0x72,0x21,0x10,0x87,0xb6,0xe5,0xd4,0xfa,0xcb,0x98,0xa9,0x3e,0x0f,0x5c,0x6d,
    0x86,0xb7,0xe4,0xd5,0x42,0x73,0x20,0x11,0x3f,0x0e,0x5d,0x6c,0xfb,0xca,0x99,0xa8,
    0xc5,0xf4,0xa7,0x96,0x01,0x30,0x63,0x52,0x7c,0x4d,0x1e,0x2f,0xb8,0x89,0xda,0xeb,
    0x3d,0x0c,0x5f,0x6e,0xf9,0xc8,0x9b,0xaa,0x84,0xb5,0xe6,0xd7,0x40,0x71,0x22,0x13,
    0x7e,0x4f,0x1c,0x2d,0xba,0x8b,0xd8,0xe9,0xc7,0xf6,0xa5,0x94,0x03,0x32,0x61,0x50,
    0xbb,0x8a,0xd9,0xe8,0x7f,0x4e,0x1d,0x2c,0x02,0x33,0x60,0x51,0xc6,0xf7,0xa4,0x95,
    0xf8,0xc9,0x9a,0xab,0x3c,0x0d,0x5e,0x6f,0x41,0x70,0x23,0x12,0x85,0xb4,0xe7,0xd6,
    0x7a,0x4b,0x18,0x29,0xbe,0x8f,0xdc,0xed,0xc3,0xf2,0xa1,0x90,0x07,0x36,0x65,0x54,
    0x39,0x08,0x5b,0x6a,0xfd,0xcc,0x9f,0xae,0x80,0xb1,0xe2,0xd3,0x44,0x75,0x26,0x17,
    0xfc,0xcd,0x9e,0xaf,0x38,0x09,0x5a,0x6b,0x45,0x74,0x27,0x16,0x81,0xb0,0xe3,0xd2,
    0xbf,0x8e,0xdd,0xec,0x7b,0x4a,0x19,0x28,0x06,0x37,0x64,0x55,0xc2,0xf3,0xa0,0x91,
    0x47,0x76,0x25,0x14,0x83,0xb2,0xe1,0xd0,0xfe,0xcf,0x9c,0xad,0x3a,0x0b,0x58,0x69,
    0x04,0x35,0x66,0x57,0xc0,0xf1,0xa2,0x93,0xbd,0x8c,0xdf,0xee,0x79,0x48,0x1b,0x2a,
    0xc1,0xf0,0xa3,0x92,0x05,0x34,0x67,0x56,0x78,0x49,0x1a,0x2b,0xbc,0x8d,0xde,0xef,
    0x82,0xb3,0xe0,0xd1,0x46,0x77,0x24,0x15,0x3b,0x0a,0x59,0x68,0xff,0xce,0x9d,0xac
};

unsigned char cal_crc_table(unsigned char *ptr, unsigned char len) 
{
    unsigned char  crc = 0x00;

    while (len--)
    {
        crc = crc_table[crc ^ *ptr++];
    }
    return (crc);
}

char *Mystrcat(char *str1,char *str2)
{
	char *outcome=str1;
	while(*str1)str1++;
	while(*str1++=*str2++);
	return outcome;
}