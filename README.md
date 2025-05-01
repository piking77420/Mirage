# Mirage

## Atemp to use debug file for reflection (only pdb for now)

## Cmake Build Command Line
cmake -DCMAKE_CXX_FLAGS=-m64 -DCMAKE_C_FLAGS=-m64 -B build


if you want to use for reflection 
You should build it like a dll, seems like DIASdk symbols are very huge you don't want to reflect them 