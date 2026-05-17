#!/bin/bash

REPO_URL="$1"

BASE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
REPO_DIR="$BASE_DIR/repo"
OUTPUT_DIR="$BASE_DIR/output"
PHASES=$(git -C "$REPO_DIR" log --pretty=%s | grep -i "PHASE" | sort | uniq)
mkdir -p "$OUTPUT_DIR"


if [ -d "$REPO_DIR" ]; then
    echo "🧹 Suppression ancien repo..."
    rm -rf "$REPO_DIR"
fi


echo " Clonage repo..."
git clone "$REPO_URL" "$REPO_DIR" || exit 1


git -C "$REPO_DIR" log \
    --pretty=format:"%an|%ad|%s" --date=short \
    > "$OUTPUT_DIR/commits.txt"


cut -d'|' -f1 "$OUTPUT_DIR/commits.txt" \
| sed 's/^ *//;s/ *$//' \
| sort | uniq -c | sort -nr \
> "$OUTPUT_DIR/stats.txt"
git -C "$REPO_DIR" log --numstat --pretty="" \
| awk '
NF==3 {

    ignored = "\\.(png|json|jpg|jpeg|gif)$"

    if ($3 ~ ignored)
        next

    added[$3] += $1
}

END {
    for (file in added)
        print added[file], file
}' \
| sort -nr \
| head -5 \
> "$OUTPUT_DIR/file_additions.txt"
git -C "$REPO_DIR" log --name-only --pretty="" \
| awk '
NF>0 {
    # ignorer fichiers inutiles
    if ($0 ~ /\.(png|json)$/)
        next

    file_count[$0]++
}

END {
    for (file in file_count)
        print file_count[file], file
}' \
| sort -nr \
| head -10 \
> "$OUTPUT_DIR/file_frequency.txt"
git -C "$REPO_DIR" log --grep="$PHASE" --name-only --pretty="" \
| awk '
NF>0 {
    if ($0 ~ /\.(png|json)$/)
        next

    file_count[$0]++
}

END {
    for (file in file_count)
        print file_count[file], file
}' \

for phase in $PHASES
do
    echo "Analyse de $phase"

    git -C "$REPO_DIR" log --grep="$phase" --name-only --pretty="" \
    | awk '
    NF>0 {
        if ($0 ~ /\.(png|json)$/)
            next

        file_count[$0]++
    }
    END {
        for (file in file_count)
            print file_count[file], file
    }' \
    | sort -nr \
    > "$OUTPUT_DIR/file_frequency_${phase// /_}.txt"

done
echo "OK"