#include "gen.h"

int main()
{
    system("clear");
    dnasequencer();

    data_t data;
    FILE *f;

    double START, END;

    START = temps_CPU();
    data = defineConst(); // Choose values and input/output file
    data = lireFichier(data);
    rand_adn(&data); // creer POP sequences aleatoires avec tous les brins
    f = modifierFichier(&data);
    int GEN = 1;

    while (GEN <= data.ERA)
    {
        printProgress(START, GEN, &data);
        // printf("GEN : %d\n", GEN);
        //  selection : on range dans le tableau S les solutions aléatoires
        selection(&data);
        // evaluation : on regarde le score de chaque sequence solution proposées en comptant le nombre d'erreurs et en prenant les n meilleurs avec répétitions possible
        evaluation(&data, f);
        // selection naturelle
        hades(&data);
        // mutation au complémentaire
        mutation(&data);

        GEN++;
    }

    END = temps_CPU();

    printf("\n\nSPECS : \n");

    printf("\n      Different valid sequences : %d\n", data.SOL);

    printf("\n      Settings [POP : %d, ERA : %d, IOTA : %d, FILE : %s, OUTPUT : %s]\n", data.POP, data.ERA, data.IOTA, data.file, data.output);

    printf("\n      CPU time : %.3f ms, %.2f s, %.0f min\n", (END - START) * 1000, (END - START), (END - START) / 60);

    printf("\n      MEMORY used : %zu mo, %zu ko\n", malloc_size(data.seq) / (1 << 20), malloc_size(data.seq) / (1 << 10));

    fwrite("\nSPECS : \n", sizeof(char), sizeof("\nSPECS : \n"), f);

    fprintf(f, "\n    Different valid sequences : %d\n", data.SOL);

    fprintf(f, "    Settings [POP : %d, ERA : %d, IOTA : %d, FILE : %s, OUTPUT : %s]\n", data.POP, data.ERA, data.IOTA, data.file, data.output);

    fprintf(f, "    CPU time : %d min %d s \n", (int)(END - START) / 60, (int)(END - START) % 60);

    fprintf(f, "    MEMORY used : %zu mo, %zu ko\n", malloc_size(data.seq) / (1 << 20), malloc_size(data.seq) / (1 << 10));

    free(data.seq);
    fclose(f);

    char choice;
    while (getc(stdin) != '\n')
        ;
    printf("\nDo you want to restart the program? (y/n)\n");
    choice = getchar();
    if (choice == 'y' || choice == 'Y')
    {
        system("gcc -lm main.c gen.c -o main && ./main");
    }
    else
    {
        printf("Exiting program...\n");
        return EXIT_SUCCESS;
    }
}
