#!/bin/sh

while inotifywait -e modify ./*tex; do
  pdflatex -output-format pdf tutorials
  pdflatex -output-format pdf practice
done
