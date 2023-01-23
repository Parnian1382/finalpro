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
    printf("crap");
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
    for (int i = 0; i < 32; i++)
    {
        fprintf(file, "%d ", s[i]);
    }
    fprintf(file, "\n");
    for (int i = 0; i < 8; i++)
    {
        fprintf(file, "%d ", status[i]);
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
void WRONG_COMMAND()
{
    printf("command not available!\n");
}
void WRONG_ARGOMAN()
{
}
int main()
{   char chr;
    char name[200];
    printf("enter file name: ");
    scanf("%s", name);
    FILE *fptr;
    int line = 0, i = 0, rs, rd, rt, imm, step;
    fptr = fopen(name, "r");
    char content[1000];
    while (fgets(content, 1000, fptr))
    { 
        for (int j = 0; j < sizeof(content); j++)
        {
            content[j] = toupper(content[j]);
        }
        i = 0;
        char command[150] = {'\0'};
        // line++;
        while (content[i] != ' ')
        {
            command[i] = content[i];
            i++;
            // if(content[i+1]='\n')
            //     command[i+1]=' ';
        }
        if (strcmp(command, "ADD") == 0)
        {
            sscanf(content, "ADD S%d, S%d, S%d", &rd, &rs, &rt);
            ADD(rd, rs, rt);
        }
        else if (strcmp(command, "SUB") == 0)
        {
            sscanf(content, "SUB S%d, S%d, S%d", &rd, &rs, &rt);
            SUB(rd, rs, rt);
        }
        else if (strcmp(command, "AND") == 0)
        {
            sscanf(content, "AND S%d, S%d, S%d", &rd, &rs, &rt);
            AND(rd, rs, rt);
        }
        else if (strcmp(command, "XOR") == 0)
        {
            sscanf(content, "XOR S%d, S%d, S%d", &rd, &rs, &rt);
            XOR(rd, rs, rt);
        }
        else if (strcmp(command, "OR") == 0)
        {
            sscanf(content, "OR S%d, S%d, S%d", &rd, &rs, &rt);
            OR(rd, rs, rt);
        }
        else if (strcmp(command, "ADDI") == 0)
        {
            sscanf(content, "ADDI S%d, S%d, %d", &rd, &rs, &imm);
            ADDI(rd, rs, imm);
        }
        else if (strcmp(command, "SUBI") == 0)
        {
            sscanf(content, "SUBI S%d, S%d, %d", &rd, &rs, &imm);
            SUBI(rd, rs, imm);
        }
        else if (strcmp(command, "ANDI") == 0)
        {
            sscanf(content, "ANDI S%d, S%d, %d", &rd, &rs, &imm);
            ANDI(rd, rs, imm);
        }
        else if (strcmp(command, "XORI") == 0)
        {
            sscanf(content, "XORI S%d, S%d, %d", &rd, &rs, &imm);
            XORI(rd, rs, imm);
        }
        else if (strcmp(command, "ORI") == 0)
        {
            sscanf(content, "ORI S%d, S%d, %d", &rd, &rs, &imm);
            ORI(rd, rs, imm);
        }
        else if (strcmp(command, "MOV") == 0)
        {
            if (content[8] == 'S' || content[9] == 'S')
            {

                sscanf(content, "MOV S%d, S%d", &rt, &rs);
                MOV(rt, s[rs]);
            }
            else
            {
                sscanf(content, "MOV S%d, %d", &rt, &imm);
                MOV(rt, imm);
            }
        }
        else if (strcmp(command, "SWP") == 0)
        {
            sscanf(content, "SWP S%d, S%d", &rt, &rs);
            SWP(rt, rs);
        }
        else if (strcmp(command, "DUMP_REGS") == 0)
        {
            DUMP_REGS();
        }
        else if (strcmp(command, "DUMP_REGS_F") == 0)
        {
            DUMP_REGS_F();
        }
        else if (strcmp(command, "INPUT") == 0)
        {
            INPUT();
        }
        else if (strcmp(command, "OUTPUT") == 0)
        {
            OUTPUT();
        }
        else if (strcmp(command, "MULL") == 0)
        {
            sscanf(content, "MULL S%d, S%d", &rt, &rd);
            MULL(rt, rd);
        }
        else if (strcmp(command, "DIV") == 0)
        {
            sscanf(content, "DIV S%d, S%d", &rt, &rs);
            DIV(rt, rs);
        }
        else if (strcmp(command, "PUSH") == 0)
        {
            sscanf(content, "PUSH S%d", &rt);
            push(rt);
        }
        else if (strcmp(command, "PUP") == 0)
        {
            sscanf(content, "PUP S%d", &rt);
            pup(rt);
        }
        else if (strcmp(command, "JUMP") == 0)
        {
            printf("hhh");
            sscanf(content, "JUMP %d", &imm);
            line = 0;
            fseek(fptr, 0, SEEK_SET);
            while (line + 1 < imm)
            {
                char c;
                c = fgetc(fptr);
                if (c == '\n')
                {
                    line++;
                }
            }
        }
        else if (strcmp(command, "SKIE") == 0)
        {
            if (s[rt] = s[rs])

            {
                while (chr = fgetc(fptr) != '\n')
                {
                }
                line++;
            }
        }
        else
        {
            WRONG_COMMAND();
        }
    }

    fclose(fptr);
}
