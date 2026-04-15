#!/bin/bash

REPO_URL="$1"

BASE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
REPO_DIR="$BASE_DIR/repo"
OUTPUT_DIR="$BASE_DIR/output"

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

echo "OK"