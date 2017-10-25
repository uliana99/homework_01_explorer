#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <cstring>

//  export EXPLORER_LOG_PATH="log.txt"
//  g++-7 main.cpp -lstdc++fs  -o open && ./open explorer/file.dat

namespace fs = std::experimental::filesystem;
using namespace std;

int main(int argc, char const *argv[]) {
    try {
        string path;

        //  Исключение - вызов программы без параметра
        if(argc < 2) throw logic_error("do not have any adress");

        //  Случай если адрес передали example: explorer file.dat
        if(argc > 2) {
            for(int i = 1; i < argc; i++) {
                path += argv[i];
                if(i != (argc - 1))
                    path += "/";
            }
        } else {
        //  Случай если адрес передали example: explorer/file.dat
            path += argv[1];
        }

        //  Проверка на существование файла по заданному адресу
        if(!fs::exists(path)) throw logic_error("do not found any files");

        //  Проверка существования файла log.txt
        
        if(!fs::exists(getenv("EXPLORER_LOG_PATH"))) {
            //  Создаем файл log.txt
            ofstream ofs(getenv("EXPLORER_LOG_PATH"));
            ofs << "the record of modifications:" << endl << endl;
            ofs.close();
        }

        const fs::path filePath = path;

        //  Получаем имя файла file.dat.
        const fs::path textFilename = filePath.filename();
        //  Получаем время последннего изменения файла file.dat.
        auto ftime = fs::last_write_time(filePath);
        //  Интерпритируем данные времени в строку для вывода.
        std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);

        //  Расчитываем размер файла
        double fileSize = fs::file_size(filePath);
        fileSize /= 1000;

        //  Вывод в конец файла.
        ofstream session((getenv("EXPLORER_LOG_PATH")), ios_base::app);

        session << "name:\t" << textFilename << endl;
        session << "size:\t" << fileSize << " Kb" << endl;
        session << "date:\t" << std::asctime(std::localtime(&cftime)) << endl;

        session.close();

    } catch(const exception& e) {
        cout << e.what() << endl;
    } 

    return 0;
}

