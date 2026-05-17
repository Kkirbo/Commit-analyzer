#!/bin/bash

REPO_URL="$1"

BASE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
REPO_DIR="$BASE_DIR/repo"
OUTPUT_DIR="$BASE_DIR/output"

mkdir -p "$OUTPUT_DIR"

rm -rf "$REPO_DIR"

echo "Clonage repo..."
git clone "$REPO_URL" "$REPO_DIR" || exit 1

echo "Stats commits..."

git -C "$REPO_DIR" log --since="2026-04-06" \
--pretty=format:"%an" \
| sort | uniq -c | sort -nr \
> "$OUTPUT_DIR/stats.txt"

echo "Frequency fichiers..."

git -C "$REPO_DIR" log --since="2026-04-06" --name-only --pretty="" \
| awk '
NF>0 {
    if ($0 ~ /\.(png|json|jpg|jpeg|gif|svg|mp4|pdf)$/)
        next
    count[$0]++
}
END {
    for (f in count)
        print count[f], f
}' \
| sort -nr \
> "$OUTPUT_DIR/file_frequency.txt"

echo "Additions..."

git -C "$REPO_DIR" log --since="2026-04-06" --numstat --pretty="" \
| awk '
NF==3 {
    if ($3 ~ /\.(png|json|jpg|jpeg|gif|svg|mp4|pdf)$/)
        next
    added[$3] += $1
}
END {
    for (file in added)
        print added[file], file
}' \
| sort -nr \
| head -10 \
> "$OUTPUT_DIR/file_additions.txt"

echo "OK"