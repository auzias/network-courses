#!/bin/sh

while inotifywait -e modify ./*tex; do
  pdflatex -output-format pdf tutorials
  pdflatex -output-format pdf tutorials
  pdflatex -output-format pdf practice
  pdflatex -output-format pdf practice
  rm *aux *log *out *toc *bib
done
