#!/usr/bin/env bash
cd unitTest;

for dir in */; do
  cd $dir;
  echo "============================"
  echo "STARTING TESTS IN DIRECTORY:"
  echo $dir;
  echo "============================"

  arr=( $(find -type f -executable) )
  for unitTest in "${arr[@]}"
  do
    eval $unitTest
  done
  cd ..;
done

