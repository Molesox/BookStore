//
// Created by Bastou on 22/05/2019.
//

#ifndef BOOKSTORE_LOGGER_H
#define BOOKSTORE_LOGGER_H

#include <fstream>
#include <sys/time.h>

using namespace std;

class FileLogger {

public:
    enum e_logType {
        LOG_ERROR, LOG_WARNING, LOG_INFO
    };

    // source contains the author of the log, file is the actual filename used to save the log.
    FileLogger(const string source, const string file) : WarnCounter(0U), ErrorCounter(0U) {
        myFile.open(file);

        if (myFile.is_open()) {
            myFile << "Bookstore log : " << source << endl;
        }
    }

    ~FileLogger() {
        if (myFile.is_open()) {
            myFile << endl << endl;

            // log error and warning counts
            myFile << WarnCounter << " warnings" << endl;
            myFile << ErrorCounter << " errors" << endl;

            myFile.close();
        }
    }

    friend FileLogger &operator <<(FileLogger &logger, const e_logType l_type) {
        switch (l_type) {
            case FileLogger::e_logType::LOG_ERROR:
                logger.myFile << "[ERROR]: ";
                ++logger.ErrorCounter;
                break;

            case FileLogger::e_logType::LOG_WARNING:
                logger.myFile << "[WARNING]: ";
                ++logger.WarnCounter;
                break;

            default:
                logger.myFile << "[INFO]: ";
                break;
        }
        return logger;
    }

    friend FileLogger &operator <<(FileLogger &logger, const string text) {
        logger.myFile << text << endl;
        return logger;
    }

    void log(string logstr){
        myFile << timeStamp() << logstr << endl;
    }

    string timeStamp(){
        time_t now;
        struct tm *current;
        now = time(0);
        current = localtime(&now);

        struct timeval detail_time;
        gettimeofday(&detail_time,NULL);
        return to_string(current->tm_hour) + ":" + to_string(current->tm_min) + ":" +
            to_string(current->tm_sec) + "." + to_string(detail_time.tv_usec) + " ";
    }

    //FileLogger FileLogger(const FileLogger &) = delete;

    FileLogger &operator=(const FileLogger &) = delete;

private:
    ofstream myFile;
    unsigned int WarnCounter;
    unsigned int ErrorCounter;
};

#endif //BOOKSTORE_LOGGER_H
