#!/bin/bash

if [ $# != 1 ]; then
  echo "Usage: run <file_name>"
  exit -1;
fi

file_name=$1 
echo "$file_name"
file_type=""
if [ "${1##*.}" != "$file_name" ]; then
  file_type="${1##*.}"
else
  echo "input has no extension, so can't detect";
  exit -1;
fi

echo "$file_type file detected"

exec_file="${file_name%.*}";

if [ "${exec_file:0:1}" == "/" ];then
  exec_file="${file_name%.*}";
else
  exec_file="./${file_name%.*}";
fi

if [ "$file_type" == "cpp" ]; then
  g++ "$file_name" -o "${file_name%.*}" && "${exec_file}"
elif [ "$file_type" == "py" ]; then
  python3 "$file_name"
elif [ "$file_type" == "js" ]; then
  node "$file_name"
else
  echo "File type not configured"
fi
