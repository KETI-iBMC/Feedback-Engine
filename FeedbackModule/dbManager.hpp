#include <sqlite3.h>
#include <iostream>
#include <string>

enum log_Type{
    FOFL_log = 1,
    predictLog = 2,
    feedbackComplete = 3,

}

class dbManager {
    private:

    static bool only_one;
    static dbManager *phoneixInstance;

    static void Create(){
        static dbManager db_instance;
        phoneixInstance = &db_instance;
    }

    static void distroy_db() {phoneixInstance-> ~dbManager();} 
    bool insert_Feedback_Field (string des, );
    bool insert_Feedback ();//
};