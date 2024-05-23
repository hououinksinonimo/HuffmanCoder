Команды для компиляции файлов и линкования консольного приложения:  
g++ -c Binary_Tree.cpp -O3 -o bt.o  
g++ -c Compress.cpp -O3 -o c.o  
g++ -c Decompress.cpp -O3 -o d.o  
g++ -c main.cpp -O3 -o main.o  
g++ c.o d.o main.o bt.o -O3 -o HuffmanCoder  
  
  
Кодер представляет из себя консольное приложение написанное на языке C++ для работы необходимо передать два параметра в консоли, режим работы (‘c’ – для сжатия, ‘d’ – для разжатия) и имя файла(в случае сжатия – это имя\путь до сжимаего файла, разжатия – это имя\путь для файла, куда будут размещены декодируемые данные). После работы программы создаются два файла – compressed_data.bin(сжатые данные), и data_for_decompress.bin(данные необходимые для декодирования сжатых данных).
