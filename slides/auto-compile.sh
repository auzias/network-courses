#!/bin/sh

while inotifywait -e modify ./*tex; do
  pdflatex -output-format pdf -jobname=slides all
done
