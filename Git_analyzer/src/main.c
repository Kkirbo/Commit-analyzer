#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_command(const char *cmd) {
    char test[128];
    snprintf(test, sizeof(test), "command -v %s > /dev/null 2>&1", cmd);
    return system(test) == 0;
}

void fail(const char *msg) {
    fprintf(stderr, "❌ %s\n", msg);
    exit(1);
}

int main() {
    char repo[300];
    if (!check_command("git")) {
        fail("git n'est pas installé.");
    }

    if (!check_command("gnuplot")) {
        fail("gnuplot n'est pas installé.");
    }

    printf("🔗 Entre le lien du repo GitHub : ");
    if (scanf("%299s", repo) != 1) {
    fail("Erreur de lecture du repo.");
}

char command[512];
snprintf(command, sizeof(command),
         "bash scripts/analyze.sh %s", repo);

if (system(command) != 0) {
    fail("Erreur lors de l'analyse du repo.");
}

    printf("📊 Stats générées.\n");
    FILE *gp = popen("gnuplot", "w");
    if (!gp) {
        fail("Impossible de lancer gnuplot.");
    }

    fprintf(gp,
        "set terminal png size 900,600\n"
        "set output 'graph.png'\n"
        "set title 'Commits par auteur'\n"
        "set style data histograms\n"
        "set style fill solid 1.0 border -1\n"
        "set xtics rotate by -45\n"
        "set ylabel 'Nombre de commits'\n"
        "plot 'output/stats.txt' using 1:xtic(2) title 'Commits'\n"
    );

    pclose(gp);

    printf("Graphique généré : graph.png\n");

    return 0;
}