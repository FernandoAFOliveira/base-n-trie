#!/usr/bin/env bash
set -e

TMP=$(mktemp)
# Generate depth-2 tree, ignore .git and build artifacts
tree -I '.git|build|examples/build|tests/build|*.o|*.exe' -L 2 -N -a > "$TMP"

# Replace the block between markers in README.md
awk -v new_tree="$(cat "$TMP")" '
BEGIN{start=0}
{
  if ($0 ~ /<!-- TREE START -->/) {print; print "```plaintext"; print new_tree; print "```"; start=1; next}
  if ($0 ~ /<!-- TREE END -->/) {start=0}
  if (start==0) print
}' README.md > README.tmp && mv README.tmp README.md

rm "$TMP"
