#include <stdio.h>

int main(void)
{
    FILE * in = NULL;
    FILE * out = NULL;

    in = fopen("fcopy.in", "r");
    out = fopen("fcopy.out", "w");

    int c = 0;
    int blank = 0;
    while ((c = fgetc(in)) != EOF)
    {
        if ((c == ' ') || (c == '\t'))
        {
            if (blank == 0)
            {
                fputc(c, out);
                blank = 1;
            }
        } 
        else
        {
            blank = 0;
            fputc(c, out);
        }
    }

    fclose(in);
    fclose(out);

    return 0;
}