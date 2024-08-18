#include <concepts>
#include <filesystem>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <ostream>
#include <string>
#include <sys/types.h>





enum LoggerArgs {
    FileWrite,
    Default
};

class Logger {


    public:

        static Logger& GetInstance() {
            static Logger s;
            return s;
        }


        ~Logger() {
            if (buffercount)
                flush();
        }

        template<typename... Args>
            void Warn(std::string str, Args... args) {

                std::string formated = std::vformat(str, std::make_format_args(std::forward<Args>(args)...)); 

                const auto now =  std::chrono::steady_clock::now();

                const std::chrono::duration<double> time_elapsed = now - _time_origin;

                buffer << std::fixed << std::setprecision(8) << std::left;


                buffer << std::setw(15) << time_elapsed;
                buffer << std::setw(22) << "\033[32m Warning: \033[0m"<< formated << std::endl;

                buffer.flush();

                buffercount++;

                if(buffercount > buffer_line_size)
                    flush();


            }

        template<typename... Args>
            void Info(std::string str, Args... args) {
                std::string formated = std::vformat(str, std::make_format_args(std::forward<Args>(args)...)); 

                const auto now =  std::chrono::steady_clock::now();

                const std::chrono::duration<double> time_elapsed = now - _time_origin;

                buffer << std::fixed << std::setprecision(8) << std::left;


                buffer << std::setw(15) << time_elapsed;
                buffer  << std::setw(13) << " Info: "<< formated << std::endl;

                buffer.flush();


                buffercount++;
                if(buffercount > buffer_line_size)
                    flush();


            }
        template<typename...Args>
            void Error(std::string str, Args... args) {

                std::string formated = std::vformat(str, std::make_format_args(std::forward<Args>(args)...)); 

                const auto now =  std::chrono::steady_clock::now();

                const std::chrono::duration<double> time_elapsed = now - _time_origin;

                buffer << std::fixed << std::setprecision(8) << std::left;


                buffer << std::setw(15) << time_elapsed;
                buffer << std::setw(22) << "\033[31m Error: \033[0m"<< formated << std::endl;

                buffer.flush();

                if constexpr (flushonError) {
                    flush();
                } else {
                    buffercount++;

                    if(buffercount > buffer_line_size)
                        flush();

                }

            } 

        void flush() {

            if (outputpath.empty()) {
                setOutputfile( "/home/xenia/Projects/Wind2/test/Logs");
            }

            if (la != FileWrite || outputpath.empty()) {

                std::cout << buffer.str() << std::flush;
                buffer.str("");
                buffer.clear();
            }
            else if (la == FileWrite) {

                std::ofstream f;
                f.open(outputpath.string(), std::fstream::app);

                if (f.good()) {

                    f << buffer.str() << std::flush;
                    buffer.str("");
                    buffer.clear();
                }


                f.close();
            }

            buffercount = 0;
        }


        void setOutputfile(std::string filepath) {

            outputpath = filepath;

            if (outputpath.is_relative()) {
                outputpath = std::filesystem::absolute(outputpath);
            }

            if (!outputpath.has_extension()) {
                outputpath.replace_extension(".log");
            }

            if (std::filesystem::exists(outputpath)) {
                std::filesystem::path old = outputpath;
                old.concat(".old");
                std::filesystem::rename(outputpath, old);
            }
        }




    private:


        LoggerArgs la = FileWrite;
        std::ofstream output;


        u_int16_t buffercount;


        std::ostringstream buffer;


        std::filesystem::path outputpath;


        u_int buffer_line_size = 20;

        const std::chrono::time_point<std::chrono::steady_clock> _time_origin;


        Logger() : _time_origin(std::chrono::steady_clock::now()){

        }


        static constexpr bool flushonError = false;


};
