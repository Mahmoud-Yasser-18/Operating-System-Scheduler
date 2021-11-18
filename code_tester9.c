///
#include <stdio.h>

int main()
{
    FILE *out_file = fopen("output.txt", "w"); // write only

    // test for files not existing.
    if (out_file == NULL)
    {
        printf("Error! Could not open file\n");
       
    }

    // write to file vs write to screen
    int integer =5;
    fprintf(out_file, "this is a test %d\n", integer); // write to file
fclose(out_file);
}
