#include "gen.h"
#define PBSTR "************************************************************"
#define PBWIDTH 60

double temps_CPU()
{
    struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_utime.tv_sec + (usage.ru_utime.tv_usec / 1000000.0);
}

bool verify_dna_filename(char *filename)
{
    char extension[] = ".txt";
    int extension_len = strlen(extension);
    int filename_len = strlen(filename);

    // Verify that the file name has at least 8 characters, including the extension
    if (filename_len < 8)
    {
        return false;
    }

    // Verify that the last 4 characters of the file name are ".txt"
    if (strcmp(filename + filename_len - extension_len, extension) != 0)
    {
        return false;
    }

    // Verify that the first 3 characters of the file name are "adn"
    if (strncmp(filename, "adn", 3) != 0)
    {
        return false;
    }

    // Verify that the fourth character of the file name is a digit
    if (!isdigit(filename[3]))
    {
        return false;
    }

    // Verify that the fifth character of the file name is a dot
    if (filename[4] != '.')
    {
        return false;
    }

    // All checks passed, file name is valid
    return true;
}

bool verify_output_filename(char *filename)
{
    char extension[] = ".txt";
    int extension_len = strlen(extension);
    int filename_len = strlen(filename);

    // Verify that the file name has at least 5 characters, including the extension
    if (filename_len < 5)
    {
        return false;
    }

    // Verify that the last 4 characters of the file name are ".txt"
    if (strcmp(filename + filename_len - extension_len, extension) != 0)
    {
        return false;
    }

    // All checks passed, file name is valid
    return true;
}

void printProgress(double START, int GEN, data_t *data)
{
    double ACTUAL = temps_CPU();
    double percentage = ((double)GEN) / (double)data->ERA;
    int etime = (int)((ACTUAL - START) * ((double)data->ERA / (double)GEN))- (int)ACTUAL;
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r\033[0;32m%3d%% [%.*s%*s] Estimated Time : %d min %d s \033[0m", val, lpad, PBSTR, rpad, "", etime / 60, etime % 60);
    fflush(stdout);
}

data_t defineConst()
{
    data_t data;
    data.POP = 1000;
    data.ERA = 10000;
    data.IOTA = 21;
    data.SOL = 0;
    strcpy(data.file, "adn1.txt");
    printf("\n     Default Settings [POP : %d, ERA : %d, IOTA : %d, FILE : %s]\n", data.POP, data.ERA, data.IOTA, data.file);

    struct tm *timenow;
    time_t now = time(NULL);
    timenow = localtime(&now);
    strftime(data.output, sizeof(data.output), "output/%H_%M_%S.txt", timenow);
    char answer;
    printf("\nDo you want to use the defaults values ? (y/n)\n");
    answer = getchar();
    if (answer == 'y' || answer == 'Y')
    {
        struct tm *timenow;
        time_t now = time(NULL);
        timenow = localtime(&now);
        strftime(data.output, sizeof(data.output), "output/%H_%M_%S.txt", timenow);
        system("clear");
        dnasequencer();
        printf("\n     Default Settings [POP : %d, ERA : %d, IOTA : %d, FILE : %s]\n", data.POP, data.ERA, data.IOTA, data.file);
    }
    else
    {
        data.SOL = 0;
        int pop, era, iota;

        printf("Enter the POP value between 1 and 1.000.000 : \n");
        while (scanf("%d", &pop) != 1 || pop < 1 || pop > 1000000)
        {
            printf("Invalid input, please enter a number\n");
            while (getchar() != '\n')
                ;
        }

        printf("Enter the ERA value value between 1 and 1.000.000 : \n");
        while (scanf("%d", &era) != 1 || era < 1 || era > 1000000)
        {
            printf("Invalid input, please enter a number\n");
            while (getchar() != '\n')
                ;
        }

        printf("Enter the IOTA value between 1 and 100 : \n");
        while (scanf("%d", &iota) != 1 || iota < 1 || iota > 100)
        {
            printf("Invalid input, please enter a number\n");
            while (getchar() != '\n')
                ;
        }

        data.POP = pop;
        data.ERA = era;
        data.IOTA = iota;

        printf("Enter the .txt file of DNA :\n");
        do
        {
            scanf("%s", data.file);
        } while (verify_dna_filename(data.file) == false);
        while (getc(stdin) != '\n')
            continue;

        char filename[32];
        char filepath[32] = "output/";

        printf("Enter the .txt file for OUTPUT :\n");
        do
        {
            scanf("%s", filename);
        } while (verify_output_filename(filename) == false);

        strcpy(data.output, filepath);
        strcat(data.output, filename);

        system("clear");
        dnasequencer();
        printf("\n     Modified Settings [POP : %d, ERA : %d, IOTA : %d, FILE : %s]\n", data.POP, data.ERA, data.IOTA, data.file);
    }
    return data;
}

void dnasequencer()
{

    FILE *image = fopen("image.txt", "r");
    char buffer[256];
    while (fgets(buffer, 256, image) != NULL)
    {
        printf("%s", buffer);
    }
    fclose(image);
}

FILE *modifierFichier(data_t *data)
{
    FILE *g = fopen(data->output, "w");
    fclose(g);

    FILE *f = fopen(data->output, "a+");
    return f;
}

data_t lireFichier(data_t data)
{
    FILE *f = fopen(data.file, "r");

    fscanf(f, "%d", &(data.nb_brins));

    fscanf(f, "%d", &(data.adn_size));

    /*
    char answer;
    while (getchar() != '\n')
        ;
    printf("\nDo you want to adapt values ? (y/n)\n");
    answer = getchar();
    if (answer == 'y' || answer == 'Y')
    {
        data.POP = 25 * (int)(exp((double)data.nb_brins));
        data.ERA = 15 * (int)(exp((double)data.adn_size));
        data.IOTA = (int)(exp((double)data.nb_brins));
        printf("\nSettings [POP : %d, ERA : %d, IOTA : %d]\n", data.POP, data.ERA, data.IOTA);
        sleep(2);
    }*/

    data.seq = (adn_t *)malloc(data.POP * sizeof(adn_t));

    int i, j;

    i = 1;
    while (i <= (data.nb_brins))
    {
        fscanf(f, "%d", &(data.seq[0].brin[i].taille));
        // printf("%d", data.seq[0].brin[i].taille);
        data.seq[0].brin[i].compl = false;

        j = 0;
        while (j <= data.seq[0].brin[i].taille)
        {
            fscanf(f, "%c", &(data.seq[0].brin[i].bases[j]));
            // printf("%c", data.seq[0].brin[i].bases[j]);
            j++;
        }
        i++;
        // printf("\n");
    }
    printf("\n");
    fclose(f);
    return data;
}

data_t rand_adn(data_t *data)
{
    int temp;
    for (int i = 1; i < data->POP; i++)
    {
        temp = arc4random_uniform(data->nb_brins) + 1;

        data->seq[i].brin[temp].colonne = 1;

        for (int j = 1; j <= (data->nb_brins); j++)
        {
            memset(data->seq[i].brin[j].bases, 0, sizeof(data->seq[i].brin[j].bases));
            if (j != temp)
            {
                data->seq[i].brin[j].colonne = arc4random_uniform(data->adn_size) + 1;
                while (data->seq[i].brin[j].colonne + data->seq[0].brin[j].taille - 1 > (data->adn_size))
                {
                    data->seq[i].brin[j].colonne = arc4random_uniform(data->adn_size) + 1;
                }
            }
        }
    }
    return *data;
}

data_t selection(data_t *data)
{
    for (int i = 1; i < data->POP; i++)
    {
        for (int j = 1; j <= data->adn_size; j++)
        {
            int A = 0, C = 0, T = 0, G = 0;

            for (int k = 1; k <= data->nb_brins; k++)
            {
                for (int l = 1; l <= data->seq[0].brin[k].taille; l++)
                {
                    if ((data->seq[i].brin[k].colonne + l - 1) == j)
                    {
                        if (data->seq[0].brin[k].bases[l] == 'A')
                            A++;
                        if (data->seq[0].brin[k].bases[l] == 'C')
                            C++;
                        if (data->seq[0].brin[k].bases[l] == 'T')
                            T++;
                        if (data->seq[0].brin[k].bases[l] == 'G')
                            G++;
                    }
                }
            }

            // printf("A = %d, C = %d, T = %d, G = %d\n", A, C, T, G);

            int temp = A;
            data->seq[i].S[j] = 'A';

            if (temp < C)
            {
                temp = C;
                data->seq[i].S[j] = 'C';
            }

            if (temp < T)
            {
                temp = T;
                data->seq[i].S[j] = 'T';
            }

            if (temp < G)
            {
                temp = G;
                data->seq[i].S[j] = 'G';
            }

            if ((A == C) || (A == G) || (A == T) || (C == T) || (C == G) || (T == G))
            {
                int x = arc4random_uniform(4);
                if (x == 0 && A > 0)
                {
                    data->seq[i].S[j] = 'A';
                }
                x = arc4random_uniform(3) + 1;
                if (x == 1 && C > 0)
                {
                    data->seq[i].S[j] = 'C';
                }
                x = arc4random_uniform(2) + 2;
                if (x == 2 && T > 0)
                {
                    data->seq[i].S[j] = 'T';
                }
                if (x == 3 && G > 0)
                {
                    data->seq[i].S[j] = 'G';
                }
            }

            if (temp == 0)
            {
                data->seq[i].S[j] = 'x';
            }
        }
        /*
        printf("\n");
        for (int p = 1; p <= data->adn_size; p++)
            printf("%c", data->seq[i].S[p]);
        printf("\n");
        */
    }
    return *data;
}

data_t evaluation(data_t *data, FILE *f)
{
    for (int i = 1; i < data->POP; i++)
    {
        data->seq[i].score = 0;
        for (int j = 1; j <= data->adn_size; j++)
        {
            if (data->seq[i].S[j] == 'x')
                data->seq[i].score += 100;
            for (int k = 1; k <= data->nb_brins; k++)
            {
                for (int l = 1; l <= data->seq[0].brin[k].taille; l++)
                {
                    if ((data->seq[i].brin[k].colonne + l - 1) == j)
                    {
                        if (data->seq[i].S[j] != data->seq[0].brin[k].bases[l])
                            data->seq[i].score++;
                    }
                }
            }
        }
        data->seq[i].life = exp(-((double)data->seq[i].score) / data->IOTA); // function to reduce the score to a survival probability between 0 and 1
        display_solutions(data, i, f);
    }
    return *data;
}

data_t hades(data_t *data)
{
    double OMEGA;
    int temp;
    for (int i = 1; i < data->POP; i++)
    {
        OMEGA = exp(-(double)(arc4random_uniform(100) + 1) / data->IOTA);
        // printf("OMEGA %.4f\n", OMEGA);
        if ((data->seq[i].life < OMEGA)) // Séquence remplacée si .life < OMEGA
        {
            temp = arc4random_uniform(data->nb_brins) + 1;
            data->seq[i].brin[temp].colonne = 1;

            for (int j = 1; j <= (data->nb_brins); j++)
            {
                if (j != temp)
                {
                    data->seq[i].brin[j].colonne = arc4random_uniform(data->adn_size) + 1;
                    while (data->seq[i].brin[j].colonne + data->seq[0].brin[j].taille - 1 > (data->adn_size))
                    {
                        data->seq[i].brin[j].colonne = arc4random_uniform(data->adn_size) + 1;
                    }
                }
            }
        }
    }
    return *data;
}

data_t mutation(data_t *data)
{
    double BETA;
    BETA = exp(-(double)(arc4random_uniform(50) + 1) / data->IOTA);
    if (BETA > exp(-(double)(arc4random_uniform(100) + 1) / data->IOTA)) // Probability of a mutation appears
    {

        int temp = arc4random_uniform(data->nb_brins + 1);
        data->seq[0].brin[temp].compl = (data->seq[0].brin[temp].compl ^true); // XOR returns 0 if already mutated 1 otherwise

        for (int j = 1; j <= (data->seq[0].brin[temp].taille); j++)
        {
            int f = 0;
            if (data->seq[0].brin[temp].bases[j] == 'A' && f == 0)
            {
                data->seq[0].brin[temp].bases[j] = 'T';
                f++;
            }
            if (data->seq[0].brin[temp].bases[j] == 'T' && f == 0)
            {
                data->seq[0].brin[temp].bases[j] = 'A';
                f++;
            }
            if (data->seq[0].brin[temp].bases[j] == 'G' && f == 0)
            {
                data->seq[0].brin[temp].bases[j] = 'C';
                f++;
            }
            if (data->seq[0].brin[temp].bases[j] == 'C' && f == 0)
            {
                data->seq[0].brin[temp].bases[j] = 'G';
                f++;
            }
        }

        int i, j;
        char aux[10];

        // Copy the elements in the array aux[] from the end of the array bases[]
        for (i = data->seq[0].brin[temp].taille, j = 1; i > 0; i--, j++)
            aux[j] = data->seq[0].brin[temp].bases[i];

        // Copy the inverted table into the original
        for (i = 1; i <= data->seq[0].brin[temp].taille; i++)
            data->seq[0].brin[temp].bases[i] = aux[i];
    }
    return *data;
}

data_t display_solutions(data_t *data, int i, FILE *f)
{
    //double RATE = exp(-temps_CPU());
    double RATE = 0.9;
    if (data->seq[i].score == 0 || (data->SOL == 0 && data->seq[i].life > RATE))
    {
        /*
        printf("Success rate : %.2f%% , Time : %.2f s\n", data->seq[i].life * 100, temps_CPU());
        for (int p = 1; p <= data->adn_size; p++)
        {
            printf("%c", data->seq[i].S[p]);
        }
        printf("\n");
        for (int j = 1; j <= (data->nb_brins); j++)
        {
            for (int s = 1; s <= data->seq[i].brin[j].colonne - 1; s++)
            {
                printf("*");
            }
            for (int p = 1; p <= data->seq[0].brin[j].taille; p++)
            {
                printf("%c", data->seq[0].brin[j].bases[p]);
            }
            for (int s = (data->seq[i].brin[j].colonne + data->seq[0].brin[j].taille - 1); s <= data->adn_size - 1; s++)
            {
                printf("*");
            }
            if (data->seq[0].brin[j].compl == 1)
            {
                printf(" MUTÉ");
            }
            printf("\n");
        } */

        if (readFile(data, i, f) == false)
        {
            data->SOL++;
            fputc('\n', f);
            fprintf(f, "Rating : %.2f%% , Time : %.2f s\n", data->seq[i].life * 100, temps_CPU());
            for (int p = 1; p <= data->adn_size; p++)
            {
                fputc(data->seq[i].S[p], f);
            }
            fputc('\n', f);
            for (int j = 1; j <= (data->nb_brins); j++)
            {
                for (int s = 1; s <= data->seq[i].brin[j].colonne - 1; s++)
                {
                    fputc('*', f);
                }
                for (int p = 1; p <= data->seq[0].brin[j].taille; p++)
                {
                    fputc(data->seq[0].brin[j].bases[p], f);
                }
                for (int s = (data->seq[i].brin[j].colonne + data->seq[0].brin[j].taille - 1); s <= data->adn_size - 1; s++)
                {
                    fputc('*', f);
                }
                if (data->seq[0].brin[j].compl == 1)
                {
                    fputs(" MUTE", f);
                }
                fputc('\n', f);
            }
        }
    }
    return *data;
}

bool readFile(data_t *data, int s, FILE *f)
{
    fseek(f, 0, SEEK_SET);
    int nblocks = data->SOL;
    char line[100];
    int flag = 0;

    // loop through all blocks
    for (int i = 1; i <= nblocks; i++)
    {
        // skip the first line of the block
        fgets(line, sizeof(line), f);
        fgets(line, sizeof(line), f);
        // read the second line of the block
        fgets(line, sizeof(line), f);
        if (strncmp(line, data->seq[s].S + 1, data->adn_size) == 0)
        {
            flag = 1;
            // skip the remaining lines of the block
            for (int j = 1; j <= data->nb_brins; j++)
            {
                char buffer[32];
                fgets(line, sizeof(line), f);
                for (int m = 1; m < data->seq[s].brin[j].colonne; m++)
                {
                    buffer[m] = '*';
                }
                for (int p = data->seq[s].brin[j].colonne; p <= (data->seq[s].brin[j].colonne + data->seq[0].brin[j].taille - 1); p++)
                {
                    buffer[p] = data->seq[0].brin[j].bases[p - data->seq[s].brin[j].colonne + 1];
                }
                for (int t = (data->seq[s].brin[j].colonne + data->seq[0].brin[j].taille); t <= data->adn_size; t++)
                {
                    buffer[t] = '*';
                }
                if (strncmp(line, buffer + 1, data->adn_size) != 0)
                {
                    flag = 0;
                }
            }
        }
        else
        {
            for (int j = 1; j <= data->nb_brins; j++)
                fgets(line, sizeof(line), f);
        }
    }
    if (flag == 1)
    {
        return true;
    }
    return false;
}
