#include <Arduino.h>

#include <SPI.h>
#include <SD.h>

File dir;
File file;

char* text;


void printDirectory(File dir);

void setup()
{
    Serial.begin(9600);
    SPI.setModule(0);

    Serial.print("Initializing SD card...");

    pinMode(10, OUTPUT);

    if(!SD.begin(32))
    {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");

    dir = SD.open("/");
    printDirectory(dir);
    dir.close();
}

void loop()
{
    if (Serial.available())
    {
        text = (char*)Serial.readStringUntil('\n').c_str();

        if (SD.exists(text))
        {
            Serial.println(text);

            file = SD.open(text);

            char data[file.size()];

            file.read(data, file.size()+1);
            
            file.close();

            Serial.println(data);
        }
        else
        {
            Serial.println("File not found.");
        }
    }
}


void printDirectory(File dir)
{
    Serial.println("SIZE\tNAME");
    while(true)
    {
        File entry = dir.openNextFile();

        if (!entry)
        {
            // no more files
            break;
        }

        // print directory name
        if (entry.isDirectory())
        {
            Serial.print("\t");
            Serial.print(entry.name());
            Serial.println("/");
        }
        else
        {
            // files have sizes, directories do not
            Serial.print(entry.size(), DEC);
            Serial.print("\t");
            Serial.println(entry.name());
        }

        entry.close();
    }
}
