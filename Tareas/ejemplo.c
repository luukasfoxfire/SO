int main()
{

int i, fin=0, pid, PipeForPlayer1[2], PipeForPlayer2[2];
int count=3, numb[]={0,0,0,0,0,0};
char control[1];
pipe(PipeForPlayer1);
pipe(PipeForPlayer2);

if((pid = fork() ) == 0){
    close(PipeForPlayer1[1]);
    close(PipeForPlayer2[0]);
    write(PipeForPlayer2[1] ,"0", 1);
}
else{
    close(PipeForPlayer2[1]);
    close(PipeForPlayer1[0]);
}

if(pid>0){
    read(PipeForPlayer2[0],Turno,1);
    if(Turno[0]=='0'){
        //printf("turno %c\n",Turno[0]);
    write(PipeForPlayer1[1],"1",1);
    }
}
else{
    read(PipeForPlayer1[0],Turno,1);
    if(Turno[0]== '1'){
        //printf("turno %c\n",Turno[0]);
        write(PipeForPlayer2[1],"0",1);
    }
}




    return 0;
}




if (p < 0)
    {
        fprintf(stderr, "fork Failed" );
        return 1;
    }

    // Parent process
    else if (p > 0)
    {
        char concat_str[100];

        close(PipeForPlayer1[0]);  // Close reading end of first pipe

        // Write input string and close writing end of first
        // pipe.
        write(PipeForPlayer1[1], input_str, strlen(input_str)+1);
        close(PipeForPlayer1[1]);

        // Wait for child to send a string
        wait(NULL);

        close(PipeForPlayer2[1]); // Close writing end of second pipe

        // Read string from child, print it and close
        // reading end.
        read(PipeForPlayer2[0], concat_str, 100);
        printf("Concatenated string %s\n", concat_str);
        close(PipeForPlayer2[0]);
    }

    // child process
    else
    {
        close(PipeForPlayer1[1]);  // Close writing end of first pipe

        // Read a string using first pipe
        char concat_str[100];
        read(PipeForPlayer1[0], concat_str, 100);

        // Concatenate a fixed string with it
        int k = strlen(concat_str);
        int i;
        for (i=0; i<strlen(fixed_str); i++)
            concat_str[k++] = fixed_str[i];

        concat_str[k] = '\0';   // string ends with '\0'

        // Close both reading ends
        close(PipeForPlayer1[0]);
        close(PipeForPlayer2[0]);

        // Write concatenated string and close writing end
        write(PipeForPlayer2[1], concat_str, strlen(concat_str)+1);
        close(PipeForPlayer2[1]);

        exit(0);
    }
