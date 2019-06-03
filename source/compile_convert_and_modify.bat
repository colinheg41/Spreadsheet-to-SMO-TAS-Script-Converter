g++ -o convert tsv_to_script.cpp parse.cpp -static-libgcc -static-libstdc++
g++ -o modify prep_modify.cpp parse.cpp -static-libgcc -static-libstdc++