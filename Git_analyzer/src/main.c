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

    if (!check_command("git")) fail("git n'est pas installé.");
    if (!check_command("gnuplot")) fail("gnuplot n'est pas installé.");

    printf("🔗 Entre le lien du repo GitHub : ");

    if (scanf("%299s", repo) != 1)
        fail("Erreur de lecture du repo.");

    snprintf(command, sizeof(command),
             "bash scripts/analyze.sh %s", repo);

    if (system(command) != 0)
        fail("Erreur analyse repo");

    FILE *gp = popen("gnuplot", "w");
    if (!gp) fail("gnuplot error");

    printf("📊 Génération des graphiques...\n");

    fprintf(gp,
        "set terminal png size 900,600\n"
        "set output 'output/graph_commits.png'\n"
        "set title 'Commits par auteur'\n"
        "set style data histograms\n"
        "set style fill solid 1.0 border -1\n"
        "set xtics rotate by -45\n"
        "set ylabel 'Commits'\n"
        "plot 'output/stats.txt' using 1:xtic(2)\n"
    );

    fprintf(gp,
        "set output 'output/file_additions.png'\n"
        "set title 'Lignes ajoutées'\n"
        "set xtics rotate by -45\n"
        "set ylabel 'Ajouts'\n"
        "plot 'output/file_additions.txt' using 1:xtic(2)\n"
    );

    fprintf(gp,
        "set output 'output/file_frequency.png'\n"
        "set title 'Fréquence modifications fichiers'\n"
        "set xtics rotate by -45\n"
        "set ylabel 'Modifications'\n"
        "plot 'output/file_frequency.txt' using 1:xtic(2)\n"
    );

    pclose(gp);

    printf("✅ Graphiques générés\n");

    return 0;
}