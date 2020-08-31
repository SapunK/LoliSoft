#include "PopulateDatabase.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

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

static const char* CREATE_SHOES_QUERY = "CREATE TABLE public.shoes("
                                        "id serial NOT NULL, "
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
}

using namespace PopulateDatabase_NS;

PopulateDatabase::PopulateDatabase()
{

}

void PopulateDatabase::createDatabase()
{
    createColors();
    createMaterials();
    createModels();
    createShoes();
}

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
