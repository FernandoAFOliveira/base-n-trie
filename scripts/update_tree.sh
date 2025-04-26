#!/usr/bin/env bash
set -e

# Compute script dir (scripts/) and repo root (one level up)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(dirname "$SCRIPT_DIR")"

TMP="$(mktemp)"

# Generate tree from the repo root
tree -I '.git|build|examples/build|tests/build|*.o|*.exe' -L 2 -N -a "$REPO_ROOT" > "$TMP"

# Replace in the README at the repo root
awk -v new_tree="$(cat "$TMP")" '
BEGIN { in_block=0 }
{
  if ($0 ~ /<!-- TREE START -->/) {
    print
    print "```plaintext"
    print new_tree
    print "```"
    in_block=1
    next
  }
  if ($0 ~ /<!-- TREE END -->/) {
    in_block=0
  }
  if (!in_block)
    print
}' "$REPO_ROOT/README.md" > "$REPO_ROOT/README.tmp" \
   && mv "$REPO_ROOT/README.tmp" "$REPO_ROOT/README.md"

rm "$TMP"
