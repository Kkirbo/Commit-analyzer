#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_command(const char *cmd) {
    char test[128];
    snprintf(test, sizeof(test),
             "command -v %s > /dev/null 2>&1", cmd);
    return system(test) == 0;
}

void fail(const char *msg) {
    fprintf(stderr, "❌ %s\n", msg);
    exit(1);
}

int main() {

    char repo[300];
    char command[512];

    /* ========================= */
    /* CHECK DEPENDENCIES       */
    /* ========================= */
    if (!check_command("git")) {
        fail("git n'est pas installé.");
    }

    if (!check_command("gnuplot")) {
        fail("gnuplot n'est pas installé.");
    }

    /* ========================= */
    /* INPUT REPO               */
    /* ========================= */
    printf("🔗 Entre le lien du repo GitHub : ");

    if (scanf("%299s", repo) != 1) {
        fail("Erreur de lecture du repo.");
    }

    /* ========================= */
    /* RUN ANALYSIS            */
    /* ========================= */
    snprintf(command, sizeof(command),
             "bash scripts/analyze.sh %s", repo);

    if (system(command) != 0) {
        fail("Erreur lors de l'analyse du repo.");
    }

    /* ========================= */
    /* OPEN PHASE FILES        */
    /* ========================= */
    FILE *phases = fopen("output/phases.txt", "r");
    if (!phases) {
        fail("Impossible de lire phases.txt");
    }

    /* ========================= */
    /* OPEN GNUPLOT            */
    /* ========================= */
    FILE *gp = popen("gnuplot", "w");
    if (!gp) {
        fail("Impossible de lancer gnuplot.");
    }

    printf("📊 Génération des graphiques...\n");

    /* ========================= */
    /* GRAPH 1 : COMMITS       */
    /* ========================= */
    fprintf(gp,
        "set terminal png size 900,600\n"
        "set output 'output/graph_commits.png'\n"
        "set title 'Commits par auteur'\n"
        "set style data histograms\n"
        "set style fill solid 1.0 border -1\n"
        "set xtics rotate by -45\n"
        "set ylabel 'Nombre de commits'\n"
        "plot 'output/stats.txt' using 1:xtic(2)\n"
    );

    /* ========================= */
    /* GRAPH 2 : ADDITIONS     */
    /* ========================= */
    fprintf(gp,
        "set output 'output/file_additions.png'\n"
        "set title 'Lignes ajoutees par fichier'\n"
        "set boxwidth 0.9\n"
        "plot 'output/file_additions.txt' using 1:xtic(2)\n"
    );

    /* ========================= */
    /* GRAPH 3 : FREQUENCY     */
    /* ========================= */
    fprintf(gp,
        "set output 'output/graph_frequency.png'\n"
        "set title 'Frequence de modification'\n"
        "set boxwidth 0.9\n"
        "plot 'output/file_frequency.txt' using 1:xtic(2)\n"
    );

    /* ========================= */
    /* PHASES LOOP             */
    /* ========================= */
    char phase[128];

    while (fgets(phase, sizeof(phase), phases)) {

        phase[strcspn(phase, "\n")] = 0;

        if (strlen(phase) == 0)
            continue;

        /* safe name */
        char safe_phase[128];
        snprintf(safe_phase, sizeof(safe_phase), "%s", phase);

        for (int i = 0; safe_phase[i]; i++) {
            if (safe_phase[i] == ' ')
                safe_phase[i] = '_';
        }

        char datafile[256];
        char outfile[256];

        snprintf(datafile, sizeof(datafile),
                 "output/file_frequency_%s.txt",
                 safe_phase);

        snprintf(outfile, sizeof(outfile),
                 "output/graph_frequency_%s.png",
                 safe_phase);

        fprintf(gp,
            "set terminal png size 1000,600\n"
            "set output '%s'\n"
            "set title 'Frequence fichiers - %s'\n"
            "set style data histograms\n"
            "set style fill solid 1.0 border -1\n"
            "set boxwidth 0.9\n"
            "set xtics rotate by -45\n"
            "set ylabel 'Nombre de commits'\n"
            "plot '%s' using 1:xtic(2)\n",
            outfile, phase, datafile
        );
    }

    /* ========================= */
    /* CLEANUP                 */
    /* ========================= */
    fclose(phases);
    pclose(gp);

    printf("✅ Graphiques générés dans output/\n");

    return 0;
}