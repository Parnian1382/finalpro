#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
int s[32], Imm;
int stack[100];
int status[8];
void push(int n)
{
    for (int i = 0; i < 100; i++)
    {
        stack[i + 1] = stack[i];
    }
    stack[0] = s[n];
}
void pup(int n)
{
    stack[0] = s[n];
    for (int i = 0; i < 100; i++)
    {
        stack[i] = stack[i + 1];
    }
}
void parity_flag(int result)
{
    int m = 0;
    while (result > 0)
    {
        if (result % 2 == 1)
            m++;
        result /= 2;
    }
    if (m % 2 == 1)
        status[0] = 1;
    else
        status[0] = 0;
}
void zero_flag(int result)
{
    if (result == 0)
        status[1] = 1;
    else
        status[1] = 0;
}
void sign_flag(int result)
{
    if (result < 0)
        status[2] = 1;
    else
        status[2] = 0;
}
void overflow_flag_add(int a, int b, int c)
{
    if (b > 0 && c > 0 && a < 0 || b < 0 && c < 0 && a > 0)
        status[5] = 1;
    else
        status[5] = 0;
}
void overflow_flag_sub(int a, int b, int c)
{
    if (b > 0 && c < 0 && a < 0 || b < 0 && c > 0 && a > 0)
        status[5] = 1;
    else
        status[5] = 0;
}
void overflow_flag_mull(int a, int b, int c)
{
    if (b != 0 && (a / b) != c)
        status[5] = 1;
    else
        status[5] = 0;
}
void ADD(int rd, int rs, int rt)
{
    s[rd] = s[rs] + s[rt];
    parity_flag(s[rd]);
    zero_flag(s[rd]);
    sign_flag(s[rd]);
    overflow_flag_add(s[rd], s[rs], s[rt]);
}
void SUB(int rd, int rs, int rt)
{
    s[rd] = s[rs] - s[rt];
    parity_flag(s[rd]);
    zero_flag(s[rd]);
    sign_flag(s[rd]);
    overflow_flag_sub(s[rd], s[rs], s[rt]);
}
void AND(int rd, int rs, int rt)
{
    s[rd] = s[rs] & s[rt];
    parity_flag(s[rd]);
    zero_flag(s[rd]);
    sign_flag(s[rd]);
}
void OR(int rd, int rs, int rt)
{
    s[rd] = s[rs] | s[rt];
    parity_flag(s[rd]);
    zero_flag(s[rd]);
    sign_flag(s[rd]);
}
void XOR(int rd, int rs, int rt)
{
    s[rd] = s[rs] ^ s[rt];
    parity_flag(s[rd]);
    zero_flag(s[rd]);
    sign_flag(s[rd]);
}
void DIV(int rt, int rs)
{
    int quotient, remain;
    quotient = s[rt] / s[rs];
    remain = s[rt] % s[rs];
    s[rt] = quotient;
    s[rs] = remain;
    parity_flag(s[rt]);
    zero_flag(s[rt]);
    sign_flag(s[rt]);
}
void MULL(int rt, int rs)
{
    int multi;
    multi = s[rt] * s[rs];
    s[rs] = multi >> 4;
    s[rt] = multi % 16;
    parity_flag(multi);
    zero_flag(multi);
    sign_flag(multi);
    overflow_flag_mull(s[rt], s[rs], Imm);
    // s[rt] = multi & 15;
    // s[rs] = multi & 240;
    // s[rt] = multi & 00001111;
    // s[rs] = multi & 11110000;
}
void ADDI(int rt, int rs, int Imm)
{
    s[rt] = s[rs] + Imm;
    parity_flag(s[rt]);
    zero_flag(s[rt]);
    sign_flag(s[rt]);
    overflow_flag_add(s[rt], s[rs], Imm);
}
void SUBI(int rt, int rs, int Imm)
{
    s[rt] = s[rs] - Imm;
    parity_flag(s[rt]);
    zero_flag(s[rt]);
    sign_flag(s[rt]);
    overflow_flag_sub(s[rt], s[rs], Imm);
}
void ANDI(int rt, int rs, int Imm)
{
    s[rt] = s[rs] & Imm;
    parity_flag(s[rt]);
    zero_flag(s[rt]);
    sign_flag(s[rt]);
}
void ORI(int rt, int rs, int Imm)
{
    s[rt] = s[rs] | Imm;
    parity_flag(s[rt]);
    zero_flag(s[rt]);
    sign_flag(s[rt]);
}
void XORI(int rt, int rs, int Imm)
{
    s[rt] = s[rs] ^ Imm;
    parity_flag(s[rt]);
    zero_flag(s[rt]);
    sign_flag(s[rt]);
}
void MOV(int rt, int Imm)
{
    s[rt] = Imm;
}
void SWP(int rt, int rs)
{
    int help1, help2;
    help1 = s[rt];
    help2 = s[rs];
    MOV(rt, help2);
    MOV(rs, help1);
    // printf("aaaa");
    //  s[rt] = s[rs];
    //  s[rs] = c;
}
void DUMP_REGS()
{
    for (int i = 0; i < 32; i++)
    {
        printf("%d  ", s[i]);
    }
    printf("\n");
    for (int i = 0; i < 8; i++)
    {
        printf("%d  ", status[i]);
    }
}
void DUMP_REGS_F()
{
    FILE *file;
    file = fopen("regs.txt", "w");
    for(int i=0;i<32;i++)
    {
        fprintf(file,"%d ",s[i]);
    }
    fprintf(file,"\n");
    for(int i=0;i<8;i++)
    {
        fprintf(file,"%d ",status[i]);
    }
    fclose(file);
}
void INPUT()
{
    scanf("%d", &s[0]);
}
void OUTPUT()
{
    printf("\n%d", s[0]);
}
void WRONG_COMMAND(int num)
{
    printf("command %d not available!\n",num);
}
void WRONG_ARGOMAN()
{
    
}