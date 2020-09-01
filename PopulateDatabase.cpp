#include "PopulateDatabase.h"

#include <QSqlQuery>
#include <QSqlError>

#ifdef QT_DEBUG
namespace PopulateDatabase_NS {
static const char* CREATE_COLORS_QUERY = "CREATE TABLE public.colors "
                                         "(id serial NOT NULL,"
                                         "color varchar(50) NOT NULL,"
                                         "PRIMARY KEY (id));";

static const char* CREATE_MATERIALS_QUERY = "CREATE TABLE public.materials "
                                            "(id serial NOT NULL, "
                                            "material varchar(50) NOT NULL, "
                                            "PRIMARY KEY (id));";

static const char* CREATE_MODELS_QUERY = "CREATE TABLE public.models"
                                         "(id serial NOT NULL, "
                                         "model varchar(50) NOT NULL, "
                                         "PRIMARY KEY (id));";

//code integer, can be null (maybe varchar is needed for code?)
static const char* CREATE_SHOES_QUERY = "CREATE TABLE public.shoes("
                                        "id serial NOT NULL, "
                                        "code integer, "
                                        "color integer NOT NULL, "
                                        "model integer NOT NULL, "
                                        "material integer NOT NULL, "
                                        "size numeric NOT NULL, "
                                        "price numeric NOT NULL, "
                                        "PRIMARY KEY (id), "
                                        "CONSTRAINT color FOREIGN KEY (color) "
                                        "REFERENCES colors (id) MATCH SIMPLE "
                                        "ON UPDATE NO ACTION "
                                        "ON DELETE RESTRICT "
                                        "NOT VALID, "
                                        "CONSTRAINT material FOREIGN KEY (material) "
                                        "REFERENCES materials (id) MATCH SIMPLE "
                                        "ON UPDATE NO ACTION "
                                        "ON DELETE RESTRICT "
                                        "NOT VALID, "
                                        "CONSTRAINT model FOREIGN KEY (model) "
                                        "REFERENCES models (id) MATCH SIMPLE "
                                        "ON UPDATE NO ACTION "
                                        "ON DELETE RESTRICT "
                                        "NOT VALID);";

static const char* INSERT_COLORS = "INSERT INTO colors (color) VALUES ('red');"
                                   "INSERT INTO colors (color) VALUES ('blue');"
                                   "INSERT INTO colors (color) VALUES ('green');"
                                   "INSERT INTO colors (color) VALUES ('gray');";

static const char* INSERT_MATERIALS = "INSERT INTO materials (material) VALUES ('leather');"
                                      "INSERT INTO materials (material) VALUES ('artificial leather');"
                                      "INSERT INTO materials (material) VALUES ('rubber');"
                                      "INSERT INTO materials (material) VALUES ('textiles');";

static const char* INSERT_MODELS = "INSERT INTO models (model) VALUES ('ballet');"
                                   "INSERT INTO models (model) VALUES ('boat');"
                                   "INSERT INTO models (model) VALUES ('espadrille');"
                                   "INSERT INTO models (model) VALUES ('derby');";

static const char* INSERT_SHOES = "INSERT INTO shoes (code, color, model, material, size, price) VALUES (123, 1, 1, 1, 40, 1500);"
                                  "INSERT INTO shoes (code, color, model, material, size, price) VALUES (234, 1, 2, 3, 41, 1200);"
                                  "INSERT INTO shoes (code, color, model, material, size, price) VALUES (345, 2, 1, 2, 42, 1300);"
                                  "INSERT INTO shoes (code, color, model, material, size, price) VALUES (456, 3, 2, 1, 39, 1900);"
                                  "INSERT INTO shoes (code, color, model, material, size, price) VALUES (567, 1, 3, 3, 43, 2000);";
}

using namespace PopulateDatabase_NS;

PopulateDatabase::PopulateDatabase()
{

}

/*-----------------------------------------------------_PUBLIC FUNCTIONS_-------------------------------------------*/
void PopulateDatabase::createDatabase()
{
    createColors();
    createMaterials();
    createModels();
    createShoes();
}

//TODO test fill functions
void PopulateDatabase::fillDatabase()
{
    fillColors();
    fillModels();
    fillMaterials();
    fillShoes();
}
/*-----------------------------------------------------_PUBLIC FUNCTIONS_-------------------------------------------*/

/*-----------------------------------------------------_CREATE DATABASE_-------------------------------------------*/
void PopulateDatabase::createColors()
{
    QSqlQuery q;
    if(!q.exec(CREATE_COLORS_QUERY))
    {
        qWarning()<<"Unable to create table colors, error: "<<q.lastError()<<" last query: "<<q.lastQuery();
    }
}

void PopulateDatabase::createShoes()
{
    QSqlQuery q;
    if(!q.exec(CREATE_SHOES_QUERY))
    {
        qWarning()<<"Unable to create table shoes, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::createMaterials()
{
    QSqlQuery q;
    if(!q.exec(CREATE_MATERIALS_QUERY))
    {
        qWarning()<<"Unable to create table materials, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::createModels()
{
    QSqlQuery q;
    if(!q.exec(CREATE_MODELS_QUERY))
    {
        qWarning()<<"Unable to create table models, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}
/*-----------------------------------------------------_CREATE DATABASE_-------------------------------------------*/


/*-----------------------------------------------------_FILL DATABASE_-------------------------------------------*/
void PopulateDatabase::fillColors()
{
    QSqlQuery q;
    if(!q.exec(INSERT_COLORS))
    {
        qWarning()<<"Unable to insert into colors, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::fillModels()
{
    QSqlQuery q;
    if(!q.exec(INSERT_MODELS))
    {
        qWarning()<<"Unable to insert into models, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::fillMaterials()
{
    QSqlQuery q;
    if(!q.exec(INSERT_MATERIALS))
    {
        qWarning()<<"Unable to insert into materials, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::fillShoes()
{
    QSqlQuery q;
    if(!q.exec(INSERT_SHOES))
    {
        qWarning()<<"Unable to insert into shoes, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}
/*-----------------------------------------------------_FILL DATABASE_-------------------------------------------*/
#endif
