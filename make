g++ -o run -std=c++11 -pthread main.cpp Sys.cpp ConsoleFunction.cpp Generator/Sys.cpp Generator/Gens/Divide.cpp Generator/Gens/Inc.cpp Generator/Gens/Minus.cpp Generator/Gens/Mod.cpp Generator/Gens/Multiple.cpp Generator/Gens/Parenthesis.cpp Generator/Gens/Sum.cpp Generator/Gens/Triple.cpp ProcessManager/Manager.cpp Generator/Gens/ConstVariable.cpp Generator/TEST.cpp Generator/Gens/UnExistsVariable.cpp

генерация библиотеки:
g++ *.cpp -c -std=c++11 -fPIC
g++ -shared -o libgenerator.so *.o

