#!/bin/sh

while inotifywait -e modify ./*tex; do
  pdflatex -output-format pdf 2015.fr
  pdflatex -output-format pdf 2015
  pdflatex -output-format pdf 2015.fr
  pdflatex -output-format pdf 2015
  rm *aux *log *out *toc *bib
done
