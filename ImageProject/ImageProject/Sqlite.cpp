#include "Logging.h"
#include  "Sqlite.h"

void Sqlite::save(double avgR, double avgG, double avgB, float x, float y, float width, float height)
{
    char* messageErorr;
    //numFrame = (duration_cast<milliseconds>(numFrame.time_since_epoch()).count());// system_clock::now();
    int exit = sqlite3_open(((string)SAVE_TO_DB_PATH).c_str(), &SaveData::dataBase);

    //time_t t_c = time(nullptr); // Assuming t_c is your time value
    //tm timeinfo;
    //localtime_s(&timeinfo, &t_c);
    //char buffer[80];
    //strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    //string formattedTime(buffer);
    string sqlstatement = "INSERT INTO AVERAGES (NUM_FRAME,AVG_R,AVG_G,AVG_B ,X ,Y ,WIDTH,HEIGTH) VALUES(" +
        to_string(system_clock::to_time_t(Sqlite::SaveData::numFrame)) + ", " + // (duration_cast<milliseconds>(numFrame.time_since_epoch()).count()) + ", " +
        to_string(avgR) + ", " +
        to_string(avgG) + ", " +
        to_string(avgB) + ", " +
        to_string(x) + ", " +
        to_string(y) + ", " +
        to_string(width) + ", " +
        to_string(height) + ");";
    cout << "sqlstatement ::" << sqlstatement << endl;
    exit = sqlite3_exec(SaveData::dataBase, sqlstatement.c_str(), NULL, NULL, &messageErorr);
    if (exit != SQLITE_OK)
    {
        Logging::getFile()->error("Error Insert");
        //cout << "Error Insert" << endl;
        sqlite3_free(messageErorr);
    }
    else
        Logging::getFile()->info("Records create Successfully!\n");
    //cout << "Records create Successfully!\n";
    sqlite3_close(SaveData::dataBase);

}

int Sqlite::createDB()
{

    int exit = 0;
    exit = sqlite3_open(((string)SAVE_TO_DB_PATH).c_str(), &SaveData::dataBase);
    if (exit)
    {
        cerr << "Error open DB " << sqlite3_errmsg(SaveData::dataBase) << endl;
        return (-1);
    }
    else
        Logging::getFile()->info("Opened Database Successfully!");
    //cout << "Opened Database Successfully!" << endl;
    sqlite3_close(SaveData::dataBase);
    return (0);
}

int Sqlite::createTable()
{
    string sql = "CREATE TABLE  AVERAGES("
        \
        "NUM_FRAME        TEXT      NOT NULL," \
        "AVG_R DOUBLE     NOT NULL," \
        "AVG_G  DOUBLE      NOT NULL," \
        "AVG_B             DOUBLE      NOT NULL," \
        "X           FLOAT NOT NULL," \
        "Y           FLOAT NOT NULL," \
        "WIDTH       FLOAT     NOT NULL," \
        "HEIGTH      FLOAT      NOT NULL);";
    int exit = 0;
    exit = sqlite3_open(((string)SAVE_TO_DB_PATH).c_str(), &SaveData::dataBase);
    char* messaggeError;
    exit = sqlite3_exec(SaveData::dataBase, sql.c_str(), NULL, NULL, &messaggeError);
    if (exit != SQLITE_OK)
    {
        Logging::getFile()->error("Error Create Table");
        sqlite3_free(messaggeError);
    }
    else
        Logging::getFile()->info("Table created Successfully");
    //std::cout << "Table created Successfully" << std::endl;
    sqlite3_close(SaveData::dataBase);
    return (0);
}

Sqlite::Sqlite()
{
    if (isFirstInstance) {
        int db = createDB();
        int table = createTable();
        isFirstInstance = false;
    }
}