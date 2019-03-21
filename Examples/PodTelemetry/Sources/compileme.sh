#!/usr/bin/env bash

for f in *.proto
do
	protoc --python_out=. $f
done

echo "Done!"