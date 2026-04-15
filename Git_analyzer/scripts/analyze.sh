#!/bin/bash

REPO_URL="$1"

BASE_DIR="$(cd "$(dirname "$0")/.." && pwd)"

mkdir -p "$BASE_DIR/output"

# 🧹 reset repo
rm -rf "$BASE_DIR/repo"

echo "📥 Clonage repo..."
git clone "$REPO_URL" "$BASE_DIR/repo" || exit 1

# 📊 commits propres
git -C "$BASE_DIR/repo" log \
    --pretty=format:"%an|%ad|%s" --date=short \
    > "$BASE_DIR/output/commits.txt"

# ❌ sécurité
if [ ! -s "$BASE_DIR/output/commits.txt" ]; then
    echo "ERREUR: commits.txt vide"
    exit 1
fi

# 📊 stats propres
cut -d'|' -f1 "$BASE_DIR/output/commits.txt" \
| sed 's/^ *//;s/ *$//' \
| sort | uniq -c | sort -nr \
> "$BASE_DIR/output/stats.txt"

echo "OK"