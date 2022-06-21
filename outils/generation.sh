#!/bin/bash



for i in `ls generation*.txt`; do
    echo $i | sed 's/generation\([0-9]\+\).txt/\1/'
    echo $i | sed 's/.*\([0-9]\+\).*/\1/'
    echo $i
    echo test
done
echo aabbabbaab | sed 's/[ab]*/x/'
echo aabbabbaab | sed 's/a.*b/y/'
echo aabbabbaab | sed 's/a.*bb/z/'
echo aabbabbaab | sed 's/a\?b/z/g'
echo aabbabbaab | sed 's/aab\|ba\|bba/ab/g'


echo "sssssejkf fed 158e fd" | sed "s/\(aaaa\)[^0-9]*\([0-9]+\)[^0-9]*/\2\1/"
echo "ejkf fed 158e fd" | sed -E "s/.*\([0-9]+\).*/a/"

#fichier=generation
#echo dim: > $fichier