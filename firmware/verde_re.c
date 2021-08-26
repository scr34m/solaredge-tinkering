#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>

typedef long unsigned int undefined4;
typedef char undefined;

typedef char undefined2;

int verify_arch()
{
  return 1;
}

int extract(char * path,char * param_2,char ** ignore_arr,int ignore_arr_c)
{
  size_t sVar1;
  char *pcVar2;
  int iVar3;
  undefined4 local_4a0;
  undefined auStack1180 [1020];
  undefined4 local_a0;
  undefined auStack156 [124];
  FILE *local_20;
  int local_1c;
  short local_16;
  int file_pos;
  
  local_a0 = 0;
  memset(auStack156,0,0x7c);
  local_4a0 = 0;
  memset(auStack1180,0,0x3fc);
  file_pos = 0;
  snprintf((char *)&local_a0,0x7f,"/bin/cpio -vtF %s | awk {\'print $9\'}",path); // In busybox $6 is the file name, on debian $9
  local_20 = popen((char *)&local_a0,"r");
  do {
    pcVar2 = fgets((char *)&local_4a0,0x3ff,local_20);
    if (pcVar2 == (char *)0x0) {
      return file_pos;
    }
    local_16 = 1;
    for (local_1c = 0; local_1c < ignore_arr_c; local_1c = local_1c + 1) {
      // pcVar2 = *(char **)(local_1c * 4 + ignore_arr); 4 byte is only 32 bit system okay
      pcVar2 = ignore_arr[local_1c];
      // sVar1 = strlen(*(char **)(local_1c * 4 + ignore_arr));
      sVar1 = strlen(pcVar2);
      iVar3 = strncmp((char *)&local_4a0,pcVar2,sVar1);
      if (iVar3 == 0) {
        local_16 = 0;
        break;
      }
    }
    if (local_16 != 0) {
      iVar3 = file_pos * 0x20;
      sVar1 = strlen((char *)&local_4a0);
      strncpy((char *)(param_2 + iVar3),(char *)&local_4a0,sVar1 - 1);
      printf("Added to list: [%s]\n",file_pos * 0x20 + param_2);
      file_pos = file_pos + 1;
    }
  } while( true );
}

int get_file(char *archive_file,char *file_name,undefined4 *param_3,undefined4 *param_4)
{
  int iVar1;
  size_t __n;
  char acStack696 [54];
  unsigned char auStack642 [40];
  unsigned char auStack602 [18];
  char *local_248;
  unsigned char auStack580 [508];
  char *local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined2 local_3c;
  unsigned int local_38;
  unsigned int local_34;
  unsigned long file_size;
  ssize_t readed;
  int fd;
  int local_24;
  int local_20;
  int local_1c;
  unsigned long local_18;
  int file_pos;
  
  local_44 = 0;
  local_40 = 0;
  local_3c = 0;
  local_48 = (char *)0x0;
  *param_3 = 0;
  *param_4 = 0;
  fd = 0xffffffff;
  readed = 0;
  file_pos = 0;
  local_18 = 0;
  file_size = 0;
  local_1c = 0;
  fd = open(archive_file,0);
  if (fd != -1) {
    puts("Get header, open file passed.");
    while( true ) {
      local_34 = 0;
      local_38 = 0;
      memset(acStack696,0,0x6e);
      local_248 = 0;
      memset(auStack580,0,0x1fc);
      readed = read(fd,acStack696,0x6e);
      file_pos = readed + file_pos;
      if (readed != 0x6e) break;
      iVar1 = strncmp(acStack696,"070702",6); // CPIO file magic octal 070702 with CRC
      if (iVar1 != 0) {
         return 0;
      }

      for (local_20 = 0; local_20 < 8; local_20 = local_20 + 1) {
        auStack580[local_20] = (char)acStack696[94 + local_20];
      }
      auStack580[8] = '\0';
      local_18 = strtoul(auStack580, NULL, 0x10);
/*      
      for (local_20 = 0; local_20 < 8; local_20 = local_20 + 1) {
        *(undefined *)((int)&local_44 + local_20) = auStack602[local_20];
      }
      local_18 = strtoul((char *)&local_44,&local_48,0x10);
*/      
      local_38 = local_18 + 0x6e & 3;
      if (local_38 != 0) {
        local_18 = (local_18 - local_38) + 4;
      }
      file_pos = local_18 + file_pos;

      for (local_20 = 0; local_20 < 8; local_20 = local_20 + 1) {
        auStack580[local_20] = (char)acStack696[54 + local_20];
      }
      auStack580[8] = '\0';
      file_size = strtoul(auStack580, NULL,0x10);      
/*      
      for (local_24 = 0; local_24 < 8; local_24 = local_24 + 1) {
        *(undefined *)((int)&local_44 + local_24) = auStack642[local_24];
      }
      file_size = strtoul((char *)&local_44,&local_48,0x10);
*/      
      local_34 = file_size & 3;
      if (local_34 == 0) {
        local_1c = 0;
      }
      else {
        local_1c = 4 - local_34;
      }

      readed = read(fd,&auStack580,local_18);
      __n = strlen(file_name);
      iVar1 = strncmp((char *)&auStack580,file_name,__n);
      if (iVar1 == 0) {
        *param_3 = file_size;
        *param_4 = file_pos;
        close(fd);
        return 1;
      }
      file_pos = local_1c + file_size + file_pos;
      lseek(fd,local_1c + file_size,1);
    }
    puts("Read error");
  }
  return 0;
}

undefined4 decode_write(char *path,int offset,unsigned int file_size,char *name)

{
  size_t sVar1;
  unsigned int local_254;
  undefined4 local_248;
  undefined4 local_244;
  undefined local_240;
  undefined4 local_23c;
  undefined4 local_238;
  undefined local_234;
  undefined4 local_230;
  char auStack556 [252];
  undefined4 local_130;
  char auStack300 [252];
  unsigned int local_30;
  size_t local_2c;
  size_t local_28;
  int local_24;
  unsigned int local_20;
  int f;
  int local_18;
  size_t local_14;
  
  local_130 = 0;
  memset(auStack300,0,0xfb);
  local_230 = 0;
  memset(auStack556,0,0xfb);
  local_23c = 0;
  local_238 = 0;
  local_234 = 0;
  local_248 = 0x30;
  local_244 = 0;
  local_240 = 0;
  f = 0xffffffff;
  auStack300[0] =  0x62;
  auStack300[1] =  0x6a;
  auStack300[2] =  0x77;
  auStack300[3] =  0x7d;
  auStack300[4] =  0x6f;
  auStack300[5] =  0x8c;
  auStack300[6] =  0x94;
  auStack300[7] =  0x9a;
  auStack300[8] =  0x9f;
  auStack300[9] =  0xb7;
  auStack300[10] = 0xd0;
  auStack300[11] = 0xda;
  auStack300[12] = 0x03;
  auStack300[13] = 0x11;
  auStack300[14] = 0x01;
  auStack300[15] = 0x54;
  auStack300[16] = 0x66;
  auStack300[17] = 0x41;
  auStack300[18] = 0xb4;
  auStack300[19] = 0xdb;
  auStack300[20] = 0xff;
  auStack300[21] = 0x1b;
  auStack300[22] = 0x49;
  auStack300[23] = 0x31;
  auStack300[24] = 0x70;
  auStack300[25] = 0xac;
  auStack300[26] = 0xc4;
  auStack300[27] = 0x4c;
  auStack300[28] = 0x76;
  auStack300[29] = 0x69;
  auStack300[30] = 0xe7;
  auStack300[31] = 0x36;
  auStack300[32] = 0x73;
  auStack300[33] = 0xb5;
  auStack300[34] = 0xf3;
  auStack300[35] = 0x36;
  auStack300[36] = 0x6f;
  auStack300[37] = 0xcd;
  auStack300[38] = 0x09;
  auStack300[39] = 0x64;
  auStack300[40] = 0xb4;
  auStack300[41] = 0xcc;
  auStack300[42] = 0xee;
  unsigned long DAT_000220d8 = 0;
//  local_20 = FUN_00010a3c(&local_130); seed
  local_20 = 43; 
  int c = 0;
  if ((0 < (int)local_20) && (f = open(path,2), f != -1)) {
    lseek(f,offset,0);
    local_254 = file_size;
    do {
      if (local_254 == 0) {
/* Write out new checksum because of modified content change
        sVar1 = strlen(name);
        local_24 = 4 - (sVar1 + 0x6e & 3);
        sVar1 = strlen(name);
        lseek(f,(offset - (local_24 + sVar1)) + -8,0);
        snprintf((char *)&local_23c,9,"%X",DAT_000220d8);
        local_28 = strlen((char *)&local_23c);
        local_28 = 8 - local_28;
        if (local_28 != 0) {
          while (sVar1 = strlen((char *)&local_248), sVar1 != local_28) {
            sVar1 = strlen((char *)&local_248);
            *(undefined2 *)((int)&local_248 + sVar1) = 0x30;
          }
          snprintf((char *)((int)&local_248 + local_28),9,"%s",&local_23c);
          strncpy((char *)&local_23c,(char *)&local_248,9);
        }
        local_2c = write(f,&local_23c,8);
*/        
        local_2c = 8;
        if (local_2c == 8) {
          close(f);
          return 1;
        }
        printf("De Error 2 - Stop & Exit\n: read: %d, write %d\n",local_14,local_2c);
        close(f);
        return 0;
      }
      local_30 = local_254;
      if (local_20 <= local_254) {
        local_30 = local_20;
      }
      local_14 = read(f,&auStack556,local_30); // local_230
      local_254 = local_254 - local_30;
/*
      for (local_18 = 0; local_18 < (int)local_14; local_18 = local_18 + 1) {
        auStack556[local_18 + -4] = auStack300[local_18 + -4] ^ auStack556[local_18 + -4];
        DAT_000220d8 = (unsigned int)(char)auStack556[local_18 + -4] + DAT_000220d8;
      }
*/      

      for (local_18 = 0; local_18 < (int)local_14; local_18 = local_18 + 1) {
        auStack556[local_18] = (unsigned char)auStack300[local_18] ^ (unsigned char)auStack556[local_18];
//        DAT_000220d8 = (unsigned int)(char)auStack556[local_18 - 1] + DAT_000220d8;
      }

      lseek(f,-local_14,1);
      local_2c = write(f,&auStack556,local_14);
     
      local_2c = local_14;
    } while (local_2c == local_14);
    printf("De Error 1 - Stop & Exit\n: read: %d, write %d\n",local_14,local_2c);
    close(f);
  }
  return 0;
}

undefined4 UndefinedFunction_0001085c(undefined4 param_1,undefined4 param_2)
{
  int iVar1;
  undefined4 uVar2;
  undefined auStack1140 [1024];
/*  
  char *pcStack116;
  char *pcStack112;
  char *pcStack108;
  char *pcStack104;
  char *pcStack100;
*/  
  char acStack96 [64];
  undefined4 uStack32;
  undefined4 uStack28;
  int iStack24;
  int iStack20;
  uStack28 = 0;
  uStack32 = 0;

  iVar1 = verify_arch();
  if (iVar1 == 0) {
    puts("~~~~~~~~~~~~~~~~~~~~~~~~~");
    puts("|\t!\t|");
    puts("~~~~~~~~~~~~~~~~~~~~~~~~~");
    puts("Verde - Failed");
    uVar2 = 0xffffffff;
  }
  else {
    iVar1 = access("/ztmp/files/firmware0",0);
    if (iVar1 == -1) {
      iVar1 = access("/tmp/files/firmware0",0);
      if (iVar1 == -1) {
        puts("Verde - Didn\'t found any file ");
        puts("Verde - Failed");
        return 0xfffffffe;
      }
      puts("Verde - Found file /tmp/files/firmware0 ");
      // snprintf(acStack96,0x3f,"/tmp/files/firmware0",acStack96,param_2,param_1);
      snprintf(acStack96,0x3f,"/tmp/files/firmware0");
    }
    else {
      puts("Verde - Found file /ztmp/files/firmware0 ");
      // snprintf(acStack96,0x3f,"/ztmp/files/firmware0",acStack96,param_2,param_1);
      snprintf(acStack96,0x3f,"/ztmp/files/firmware0");
    }
/*    
    pcStack116 = "sw-description";
    pcStack112 = "setup.sh";
    pcStack108 = "verde";
    pcStack104 = "se_add.pol";
    pcStack100 = "verde-run.sh";
*/
    char *pcStack116[5] = {"sw-description","setup.sh","verde","se_add.pol","verde-run.sh"};
    memset(auStack1140,0,0x400);
    iStack24 = extract(acStack96,auStack1140,pcStack116,4); // 4 was 3, the elements count possibly, but pcStack has 5 values!?
    for (iStack20 = 0; iStack20 < iStack24; iStack20 = iStack20 + 1) {
      get_file(acStack96,auStack1140 + iStack20 * 0x20,&uStack28,&uStack32);
      printf("Verde - file: [%s], size: %d, offset: %d\n",auStack1140 + iStack20 * 0x20,uStack28,
             uStack32);
      if (iStack20 != 0) {
        continue;
      }
      iVar1 = decode_write(acStack96,uStack32,uStack28,auStack1140 + iStack20 * 0x20);
      if (iVar1 == 0) {
        puts("Verde - Failed");
        return 0xfffffffd;
      }
    }
    puts("Verde - Success");
    // system("touch /tmp/verde_success");
    uVar2 = 0;
  }
  return uVar2;
}


int main(void)
{
    UndefinedFunction_0001085c(0, 0);
    return 0;
}
