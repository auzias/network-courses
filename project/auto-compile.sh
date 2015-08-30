#!/bin/sh

while inotifywait -e modify ./*tex; do
  pdflatex -output-format pdf project-expectations
  rm *aux *log *out *toc *bib
done
